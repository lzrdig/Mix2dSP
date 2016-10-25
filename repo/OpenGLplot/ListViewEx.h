/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: march 2001
//
/////////////////////////////////////////////////////////////////////////////
// ListViewEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTVIEWEX_H__8070C4E3_198C_11D5_8CCF_00001CD5E4D1__INCLUDED_)
#define AFX_LISTVIEWEX_H__8070C4E3_198C_11D5_8CCF_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "ListCtrlEx.h"

class AFX_EXT_CLASS_GENERAL CListViewEx : public CListView
{
	DECLARE_DYNAMIC(CListViewEx)

// Konstruktion
public:
	CListViewEx();

// Attribute
protected:
	HIGHLIGHTTYPE	m_nHighlight;              // Indicate type of selection highlighting
	BOOL m_bReadOnly;

// Operationen
public:
	CListCtrlEx& GetListCtrlEx() const;

// Überschreibungen

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CListViewEx)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CListViewEx();
	HIGHLIGHTTYPE SetHighlightType(HIGHLIGHTTYPE nHighlight);
	void SetReadOnly(BOOL bReadOnly = TRUE);

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CListViewEx)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline CListCtrlEx& CListViewEx::GetListCtrlEx() const
	{ return *(CListCtrlEx*)this; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_LISTVIEWEX_H__8070C4E3_198C_11D5_8CCF_00001CD5E4D1__INCLUDED_
