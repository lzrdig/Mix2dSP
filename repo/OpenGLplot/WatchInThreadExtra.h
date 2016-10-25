/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 2001
//
/////////////////////////////////////////////////////////////////////////////
// WatchInThreadExtra.h : Schnittstelle der Klasse CWatchInThreadExtra
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATCHINTHREADEXTRA_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_)
#define AFX_WATCHINTHREADEXTRA_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "WatchInThreadBase.h"

class CGlDocument;

class AFX_EXT_CLASS_GENERAL CWatchInThreadExtra : public CObject, public CWatchInThreadBase
{
	DECLARE_DYNAMIC(CWatchInThreadExtra)
public:
	CWatchInThreadExtra();

// Attribute
protected:
	CString m_strPathName;

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CWatchInThreadExtra)
	virtual BOOL PostFileAlarmMsg(UINT message, int nCause);
	//}}AFX_VIRTUAL

// Implementierung
public:
	void SetWatchPathName(LPCTSTR szPathName);
	void SetDocument(CDocument* pDoc);
	CGlDocument* GetDocument();
protected:
	CDocument* m_pDocument;

public:
	virtual ~CWatchInThreadExtra();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	static UINT WatchThreadProc(LPVOID pParam);
};

inline BOOL CWatchInThreadExtra::PostFileAlarmMsg(UINT message, int nCause)
	{ return AfxGetApp()->PostThreadMessage(message, (WPARAM)this, (LPARAM)nCause); }

#ifndef _DEBUG  // Testversion in PageMaschineZwei.cpp
inline CGlDocument* CWatchInThreadExtra::GetDocument()
	{ return (CGlDocument*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_WATCHINTHREADEXTRA_H__7C0A94EE_0665_11D5_8CB4_00001CD5E4D1__INCLUDED_)
