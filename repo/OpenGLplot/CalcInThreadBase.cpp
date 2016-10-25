/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2000
//
/////////////////////////////////////////////////////////////////////////////
// CalcInThreadBase.cpp : Implementierung der Klasse CCalculateInThreadBase
//

#include "StdAfx.h"
#include "CalcInThreadBase.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalculateInThreadBase

CCalculateInThreadBase::CCalculateInThreadBase()
{
	m_hEventStartCalculate = CreateEvent(NULL, FALSE, FALSE, NULL);			// auto reset, initially reset
	m_hEventCalculateDone = CreateEvent(NULL, TRUE, TRUE, NULL);			// manual reset, initially set
	m_hEventKillCalculateThread = CreateEvent(NULL, FALSE, FALSE, NULL);	// auto reset, initially reset
	m_hEventCalculateThreadKilled = CreateEvent(NULL, FALSE, FALSE, NULL);	// auto reset, initially reset

	m_hwndNotifyCalculateDone = NULL;
	m_hwndNotifyProgress = NULL;

	m_nKindOfThreadProcedure = 0;

	m_pCalculateWorkerThread = NULL;
	m_percentStatusBar = 1.;
}

CCalculateInThreadBase::~CCalculateInThreadBase()
{
	ShutDownCalculateThreadSafely();

	CloseHandle(m_hEventStartCalculate);
	CloseHandle(m_hEventCalculateDone);
	CloseHandle(m_hEventKillCalculateThread);
	CloseHandle(m_hEventCalculateThreadKilled);
}

void CCalculateInThreadBase::ShutDownCalculateThreadSafely()
{
	// It's a good idea to wait for the worker thread to notify via a
	// "thread killed" event that it has killed itself. Otherwise, in the case
	// where the app is terminating, is possible (even if unlikely) that it
	// will detect a memory leak of the CWinThread object before the
	// CWinThread object has had a chance to auto-delete itself.

	DWORD dwExitCode;
	HANDLE hThread = m_pCalculateWorkerThread ? m_pCalculateWorkerThread->m_hThread : NULL;
	if (m_pCalculateWorkerThread != NULL &&
		GetExitCodeThread(hThread, &dwExitCode) &&
		dwExitCode == STILL_ACTIVE)
	{
		// Kill the worker thread by setting the "watch done" and "kill thread" events.
		SetEvent(m_hEventKillCalculateThread);
		SetEvent(m_hEventCalculateDone);
		SetEvent(m_hEventStartCalculate);
		//WaitForSingleObject(m_hEventCalculateThreadKilled, INFINITE);
		while (WaitForSingleObject(m_hEventCalculateThreadKilled, 1) != WAIT_OBJECT_0)
		{
			// Allow the calling thread to handle messages. This might be useful e.g. if 
			// the worker thread pops up a MessageBox, which must be closed before it can
			// finish itself.
			if (!RunExtraLoop())
				break;
		}

		// Next lines seems to be nessesary. 
		// When exiting the app it happens that the main thread is finshed
		// before the worker thread has ended itself. To avoid a memory
		// leak we give the worker thread some more time to end itself.
		while (GetExitCodeThread(hThread, &dwExitCode) &&
			dwExitCode == STILL_ACTIVE)
		{
			WaitForSingleObject(m_hEventCalculateThreadKilled, 1);
		}

		// Reset it to be able to start a new worker thread by "CalculateInThread"
		m_pCalculateWorkerThread = NULL;
	}
}
