/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 2001
//
/////////////////////////////////////////////////////////////////////////////
// StaticExtra.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICEXTRA_H__DED0E2A3_8655_11D5_9E70_00001CD5E4D1__INCLUDED_)
#define AFX_STATICEXTRA_H__DED0E2A3_8655_11D5_9E70_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Fenster CStaticExtra 

class AFX_EXT_CLASS_GENERAL CStaticExtra : public CStatic
{
// Konstruktion
public:
	CStaticExtra();

// Attribute
private:
	BOOL m_bFocusChangedFlag;
	HBRUSH m_hBrush;

// Operationen
public:
	BOOL HasFocusChanged();
	void ResetFocusChangedFlag();
	void SetBkColor(COLORREF col);
	void UnsetBkColor();

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CStaticExtra)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CStaticExtra();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CStaticExtra)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

inline BOOL CStaticExtra::HasFocusChanged()
	{ return m_bFocusChangedFlag; };
inline void CStaticExtra::ResetFocusChangedFlag()
	{ m_bFocusChangedFlag = FALSE; };
inline void CStaticExtra::SetBkColor(COLORREF col)
	{ if (m_hBrush) DeleteObject(m_hBrush); m_hBrush = CreateSolidBrush(col); }
inline void CStaticExtra::UnsetBkColor()
	{ if (m_hBrush) DeleteObject(m_hBrush); m_hBrush = NULL; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_STATICEXTRA_H__DED0E2A3_8655_11D5_9E70_00001CD5E4D1__INCLUDED_
