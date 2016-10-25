/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2000
//
/////////////////////////////////////////////////////////////////////////////
// WatchInThreadBase.h : Schnittstelle der Klasse CWatchInThreadBase
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATCHINTHREADBASE_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_)
#define AFX_WATCHINTHREADBASE_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


class AFX_EXT_CLASS_GENERAL CWatchInThreadBase
{
public:
	CWatchInThreadBase();

// Attribute
public:
	HANDLE m_hEventStartWatch;
	HANDLE m_hEventWatchDone;
	HANDLE m_hEventKillWatchThread;
	HANDLE m_hEventWatchThreadKilled;

	HWND m_hwndNotifyFileChanged;
	CString m_strPathNameWatch;

protected:
	CWinThread* m_pWatchWorkerThread;

	BOOL m_bFileLost;

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CWatchInThreadBase)
	public:
	virtual void SetFileLostFlag(BOOL bFileLost = TRUE);
	virtual BOOL IsFileLost();
	protected:
	virtual BOOL DoFileAlarm(int nCause, LPCTSTR szPathName);
	virtual BOOL PostFileAlarmMsg(UINT message, int nCause) = 0;
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CWatchInThreadBase();

	void ShutDownWatchThreadSafely();
protected:
	BOOL FileWatch(LPCTSTR szPathName);
};

inline void CWatchInThreadBase::SetFileLostFlag(BOOL bFileLost)
	{ ASSERT(this != NULL); m_bFileLost = bFileLost; }
inline BOOL CWatchInThreadBase::IsFileLost()
	{ ASSERT(this != NULL); return m_bFileLost; }
inline BOOL CWatchInThreadBase::PostFileAlarmMsg(UINT message, int nCause)
	{ ASSERT(FALSE); UNREFERENCED_PARAMETER(message); UNREFERENCED_PARAMETER(nCause); return FALSE; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_WATCHINTHREADBASE_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_)
