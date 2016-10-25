/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2000
//
/////////////////////////////////////////////////////////////////////////////
// CalcInThreadBase.h : Schnittstelle der Klasse CCalculateInThreadBase
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCINTHREADBASE_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_)
#define AFX_CALCINTHREADBASE_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


class AFX_EXT_CLASS_GENERAL CCalculateInThreadBase
{
public:
	CCalculateInThreadBase();

// Attribute
public:
	HANDLE m_hEventStartCalculate;
	HANDLE m_hEventCalculateDone;
	HANDLE m_hEventKillCalculateThread;
	HANDLE m_hEventCalculateThreadKilled;

	HWND m_hwndNotifyCalculateDone;
	HWND m_hwndNotifyProgress;

	int m_nKindOfThreadProcedure;

protected:
	CWinThread* m_pCalculateWorkerThread;

	double m_percentStatusBar;

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCalculateInThreadBase)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CCalculateInThreadBase();

	void ShutDownCalculateThreadSafely();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CALCINTHREADBASE_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_)
