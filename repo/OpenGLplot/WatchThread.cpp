/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2001
//
/////////////////////////////////////////////////////////////////////////////
// Watchthread.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "GlDocument.h"

UINT CGlDocument::WatchThreadProc(LPVOID pParam)
{
	CGlDocument* pDoc = (CGlDocument*)pParam;
	ASSERT(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)));

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
		if (WaitForSingleObject(pDoc->m_hEventStartWatch, INFINITE)
			!= WAIT_OBJECT_0)
			break;

		// Exit the thread if the main application sets the "kill watch"
		// event. The main application will set the "start watch" event
		// before setting the "kill watch" event.
		if (WaitForSingleObject(pDoc->m_hEventKillWatchThread, 0)
			== WAIT_OBJECT_0)
			break; // Terminate this thread by existing the proc.

		// Reset event to indicate "not done", that is, act of watching is in progress.
		ResetEvent(pDoc->m_hEventWatchDone);

		CString strPathName;
		while (WaitForSingleObject(pDoc->m_hEventWatchDone, 1) != WAIT_OBJECT_0)
		{
			strPathName = pDoc->GetPathName();
			if (!strPathName.IsEmpty())
				bSuccess = pDoc->FileWatch(strPathName);
			else
				bSuccess = FALSE;
		}

		// Set event to indicate that act of watching is done (i.e., no longer in progress),
		// even if perhaps interrupted by "kill watch" event detected in some watch function.
		SetEvent(pDoc->m_hEventWatchDone);

		if (WaitForSingleObject(pDoc->m_hEventKillWatchThread, 0)
			== WAIT_OBJECT_0)
			break; // terminate this thread by exiting the proc.
	}
	SetEvent(pDoc->m_hEventWatchThreadKilled);

	return 0;
}
