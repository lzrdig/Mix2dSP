/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 2001
//
/////////////////////////////////////////////////////////////////////////////
// WatchInThreadExtra.cpp : Implementierung der Klasse CWatchInThreadExtra
//

#include "StdAfx.h"
#include "GlDocument.h"
#include "WatchInThreadExtra.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWatchInThreadExtra

IMPLEMENT_DYNAMIC(CWatchInThreadExtra, CObject)

/////////////////////////////////////////////////////////////////////////////
// CWatchInThreadExtra construction/destruction

CWatchInThreadExtra::CWatchInThreadExtra()
{
	m_pDocument = NULL;

	if (m_pWatchWorkerThread == NULL)
	{
		m_pWatchWorkerThread =
			AfxBeginThread(WatchThreadProc, this, THREAD_PRIORITY_BELOW_NORMAL);
	}

	m_hwndNotifyFileChanged = AfxGetSafeMainWnd()->GetSafeHwnd();
}

CWatchInThreadExtra::~CWatchInThreadExtra()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGlDocument Diagnose

#ifdef _DEBUG
void CWatchInThreadExtra::AssertValid() const
{
	CObject::AssertValid();
}

void CWatchInThreadExtra::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "\nm_bFileLost = " << m_bFileLost;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CWatchInThreadExtra::SetDocument(CDocument* pDoc) 
{
	ASSERT_VALID(pDoc);
	m_pDocument = pDoc;
}

#ifdef _DEBUG
CGlDocument* CWatchInThreadExtra::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument == NULL || m_pDocument->IsKindOf(RUNTIME_CLASS(CGlDocument)));
	return (CGlDocument*)m_pDocument;
}
#endif //_DEBUG

UINT CWatchInThreadExtra::WatchThreadProc(LPVOID pParam)
{
	CWatchInThreadExtra* pThreadExtra = (CWatchInThreadExtra*)pParam;
	ASSERT(pThreadExtra && pThreadExtra->IsKindOf(RUNTIME_CLASS(CWatchInThreadExtra)));

	// This is for better support of timed message boxes.
	ASSERT(AfxGetThread()->m_nThreadID != AfxGetApp()->m_nThreadID);
	VERIFY(AttachThreadInput(AfxGetThread()->m_nThreadID, AfxGetApp()->m_nThreadID, TRUE));

	// This watch thread runs in an infinite loop, waiting to watch the
	// file whenever the main application thread sets the "start watch" event.
	// The watch thread exits the loop only when the main application sets the
	// "kill watch" event.

	BOOL bSuccess;
	while (TRUE)
	{
		// Wait until the main application thread asks this thread to do
		// another act of watching.
		if (WaitForSingleObject(pThreadExtra->m_hEventStartWatch, INFINITE)
			!= WAIT_OBJECT_0)
			break;

		// Exit the thread if the main application sets the "kill watch"
		// event. The main application will set the "start watch" event
		// before setting the "kill watch" event.
		if (WaitForSingleObject(pThreadExtra->m_hEventKillWatchThread, 0)
			== WAIT_OBJECT_0)
			break; // Terminate this thread by existing the proc.

		// Reset event to indicate "not done", that is, act of watching is in progress.
		ResetEvent(pThreadExtra->m_hEventWatchDone);

		CString strPathName;
		while (WaitForSingleObject(pThreadExtra->m_hEventWatchDone, 1) != WAIT_OBJECT_0)
		{
			strPathName = pThreadExtra->m_strPathName;
			if (!strPathName.IsEmpty())
				bSuccess = pThreadExtra->FileWatch(strPathName);
			else
				bSuccess = FALSE;
		}

		// Set event to indicate that act of watching is done (i.e., no longer in progress),
		// even if perhaps interrupted by "kill watch" event detected in some watch function.
		SetEvent(pThreadExtra->m_hEventWatchDone);

		if (WaitForSingleObject(pThreadExtra->m_hEventKillWatchThread, 0)
			== WAIT_OBJECT_0)
			break; // terminate this thread by exiting the proc.
	}
	SetEvent(pThreadExtra->m_hEventWatchThreadKilled);

	return 0;
}

void CWatchInThreadExtra::SetWatchPathName(LPCTSTR szPathName)
{
	BOOL bIsWatching = (WaitForSingleObject(m_hEventWatchDone, 0) != WAIT_OBJECT_0);

	// stop file watching
	SetEvent(m_hEventWatchDone);
	SetFileLostFlag(FALSE);

	m_strPathName = szPathName;

	// restart file watching if it was active before
	if (bIsWatching)
		SetEvent(m_hEventStartWatch);
}
