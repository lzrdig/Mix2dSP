/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: march 2001
//
/////////////////////////////////////////////////////////////////////////////
// ListVwEx.h : Schnittstelle der Klasse CListBoxEx
//
// This class provedes a full row selection mode for the report
// mode list view control.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTBOXEX_H__B7C10A41_8B81_11D3_9395_0000C03C43E0__INCLUDED_)
#define AFX_LISTBOXEX_H__B7C10A41_8B81_11D3_9395_0000C03C43E0__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

class AFX_EXT_CLASS_GENERAL CListBoxEx : public CListBox
{
	DECLARE_DYNAMIC(CListBoxEx)

// Konstruktion
public:
	CListBoxEx();

// Attribute
protected:
	BOOL m_bReadOnly;
	BOOL m_bShowAlways;
	HBRUSH m_hBrush;

// Überschreibungen
protected:
	void RepaintSelectedItems();

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CListBoxEx)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CListBoxEx();
	void SetReadOnly(BOOL bReadOnly = TRUE);
	BOOL IsReadOnly() const;
	void SetShowAlways(BOOL bShowAlways = TRUE);

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CListBoxEx)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchange();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline BOOL CListBoxEx::IsReadOnly() const
	{ return m_bReadOnly; }

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_LISTBOXEX_H__B7C10A41_8B81_11D3_9395_0000C03C43E0__INCLUDED_)
