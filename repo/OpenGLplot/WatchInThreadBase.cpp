/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2000
//
/////////////////////////////////////////////////////////////////////////////
// WatchInThreadBase.cpp : Implementierung der Klasse CWatchInThreadBase
//

#include "StdAfx.h"
#include "GlWinApp.h"
#include "WatchInThreadBase.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWatchInThreadBase

CWatchInThreadBase::CWatchInThreadBase()
{
	m_hEventStartWatch = CreateEvent(NULL, FALSE, FALSE, NULL);			// auto reset, initially reset
	m_hEventWatchDone = CreateEvent(NULL, TRUE, TRUE, NULL);			// manual reset, initially set
	m_hEventKillWatchThread = CreateEvent(NULL, FALSE, FALSE, NULL);	// auto reset, initially reset
	m_hEventWatchThreadKilled = CreateEvent(NULL, FALSE, FALSE, NULL);	// auto reset, initially reset

	m_hwndNotifyFileChanged = NULL;

	m_pWatchWorkerThread = NULL;
	m_bFileLost = FALSE;
}

CWatchInThreadBase::~CWatchInThreadBase()
{
	ShutDownWatchThreadSafely();

	CloseHandle(m_hEventStartWatch);
	CloseHandle(m_hEventWatchDone);
	CloseHandle(m_hEventKillWatchThread);
	CloseHandle(m_hEventWatchThreadKilled);
}

void CWatchInThreadBase::ShutDownWatchThreadSafely()
{
	// It's a good idea to wait for the worker thread to notify via a
	// "thread killed" event that it has killed itself. Otherwise, in the case
	// where the app is terminating, is possible (even if unlikely) that it
	// will detect a memory leak of the CWinThread object before the
	// CWinThread object has had a chance to auto-delete itself.

	DWORD dwExitCode;
	HANDLE hThread = m_pWatchWorkerThread ? m_pWatchWorkerThread->m_hThread : NULL;
	if (m_pWatchWorkerThread != NULL &&
		GetExitCodeThread(hThread, &dwExitCode) &&
		dwExitCode == STILL_ACTIVE)
	{
		// Kill the worker thread by setting the "watch done" and "kill thread" events.
		SetEvent(m_hEventKillWatchThread);
		SetEvent(m_hEventWatchDone);
		SetEvent(m_hEventStartWatch);
		//WaitForSingleObject(m_hEventWatchThreadKilled, INFINITE);
		while (WaitForSingleObject(m_hEventWatchThreadKilled, 1) != WAIT_OBJECT_0)
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
			WaitForSingleObject(m_hEventWatchThreadKilled, 1);
		}

		// Reset it to be able to start a new worker thread by "WatchInThread"
		m_pWatchWorkerThread = NULL;
	}
}

BOOL CWatchInThreadBase::FileWatch(LPCTSTR szPathName)
{
	// Note: In order to get a correct behaviour in a SDI environment
	//	we start file watching in OnOpenDocument *BEFORE* the path name is set
	//	correctly. This is due to the fact that an overridden OnOpenDocument in
	//	a derived class may return FALSE.
	//	So when the path name has changed a short time after file watching has been started
	//	we stop this FileWatch procedure and restart a new one.
	DWORD dwResult = WAIT_TIMEOUT, dwLastError;

	// open the docfile
	CFile file;
	BOOL bRet = file.Open(szPathName, CFile::modeRead | CFile::shareDenyNone);

	if (bRet)
	{
		// save the last write time
		FILETIME ftLastWriteTime;
		BY_HANDLE_FILE_INFORMATION fileinfo;

		if (!GetFileInformationByHandle((HANDLE)file.m_hFile, &fileinfo))
		{
			file.Abort();
			return FALSE;
		}

		ftLastWriteTime = fileinfo.ftLastWriteTime;
		file.Close();

		// determine the path
		CString strPath = szPathName;
		strPath = strPath.Left(strPath.ReverseFind('\\'));
		ASSERT(!strPath.IsEmpty());

		// endless loop
		while (TRUE)
		{
			// get file notify event
			HANDLE hFileChanged = FindFirstChangeNotification(strPath, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);

			// wait for request to abort watching or for file change event
			HANDLE hWaitEvents[] = { m_hEventWatchDone, hFileChanged };
			//DWORD dwResult = WaitForMultipleObjects(2, hWaitEvents, FALSE, INFINITE);
			while (TRUE)
			{
				// some devices seem to NOT support FindFirstChangeNotification
				// so we take only the first handle into account then
				dwResult = WaitForMultipleObjects(hFileChanged == INVALID_HANDLE_VALUE ? 1 : 2, hWaitEvents, FALSE, 10);

				if (dwResult == WAIT_OBJECT_0 + 0 || dwResult == WAIT_OBJECT_0 + 1)
				{
					break;
				}
			}
			FindCloseChangeNotification(hFileChanged);

			// some file in the directory changed
			if (dwResult == WAIT_OBJECT_0 + 1)
			{
				// retrieve docfile's write time
				BOOL bSuccess = file.Open(szPathName, CFile::modeRead | CFile::shareDenyNone);
				dwLastError = GetLastError();
				if (!bSuccess && dwLastError == ERROR_FILE_NOT_FOUND)
				{
					// Try it a second time, some network devices report ERROR_FILE_NOT_FOUND
					// even if the file was just written.
					bSuccess = file.Open(szPathName, CFile::modeRead | CFile::shareDenyNone);
					dwLastError = GetLastError();
				}
				if (bSuccess)
				{
					if (!GetFileInformationByHandle((HANDLE)file.m_hFile, &fileinfo))
					{
						file.Abort();
					}
					else
					{
						// detect if changes made
						BOOL bAlarm = (ftLastWriteTime.dwHighDateTime != fileinfo.ftLastWriteTime.dwHighDateTime || 
										ftLastWriteTime.dwLowDateTime != fileinfo.ftLastWriteTime.dwLowDateTime ||
										IsFileLost());

						// resave last time
						ftLastWriteTime = fileinfo.ftLastWriteTime;
						file.Close();
						SetFileLostFlag(FALSE);

						if (bAlarm)
						{
							if (!DoFileAlarm(FA_FILE_WRITTEN, szPathName))
							{
								// leave the outer loop if watching was
								// aborted while being in DoFileAlarm.
								break;
							}
						}
					}
				}
				else if (!IsFileLost() && dwLastError == ERROR_FILE_NOT_FOUND)
				{
					SetFileLostFlag(TRUE);
					if (!DoFileAlarm(FA_FILE_LOST, szPathName))
					{
						// leave the outer loop if watching was
						// aborted while being in DoFileAlarm.
						break;
					}
				}
				else
				{
					TRACE1("FileWatch: An error occured (%d).\n", dwLastError);
				}
			}
			else
				break;
		}
	}
	return bRet;
}

BOOL CWatchInThreadBase::DoFileAlarm(int nCause, LPCTSTR szPathName)
{
	BOOL bInvert = FALSE;

	// alarm
	MessageBeep(MB_ICONEXCLAMATION);

	while (TRUE)
	{
		if (WaitForSingleObject(m_hEventWatchDone, 500) == WAIT_OBJECT_0)
		{
			if (bInvert)
				FlashWindow(m_hwndNotifyFileChanged, FALSE);

			return FALSE;
		}
		else if (!IsIconic(m_hwndNotifyFileChanged) && 
			IsWindowEnabled(m_hwndNotifyFileChanged))
		{
			if (bInvert)
				FlashWindow(m_hwndNotifyFileChanged, FALSE);

			if (GetForegroundWindow() == m_hwndNotifyFileChanged)
			{
				// Note: We don't call OnFileAlarm in the watching worker thread. In order to 
				//  reopen the document it will be necessary to update the views, which can only
				//	be done by the creating thread. So we have to post a message to the main thread.
				//  If the main thread is blocked by a MessageBox, then this message might get lost.
				//  To avoid that we make sure that the main window is enabled before posting the 
				//  message.
				m_strPathNameWatch = szPathName;
				PostFileAlarmMsg(WM_FILE_ALARM, nCause);
				break;
			}
		}
		else
		{
			bInvert = !bInvert;
			FlashWindow(m_hwndNotifyFileChanged, bInvert);
		}
	}
	return TRUE;
}
