/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: October 1999
//
/////////////////////////////////////////////////////////////////////////////
// ListVwEx.h : Schnittstelle der Klasse CListCtrlEx
//
// This class provedes a full row selection mode for the report
// mode list view control.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTCTRLEX_H__B7C10A41_8B81_11D3_9395_0000C03C43E0__INCLUDED_)
#define AFX_LISTCTRLEX_H__B7C10A41_8B81_11D3_9395_0000C03C43E0__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

typedef enum tagHIGHLIGHTTYPE
{
	HIGHLIGHT_NORMAL = 0, 
	HIGHLIGHT_ALLCOLUMNS = 1, 
	HIGHLIGHT_ROW = 2
}
HIGHLIGHTTYPE;

class AFX_EXT_CLASS_GENERAL CListCtrlEx : public CListCtrl
{
	friend class CListViewEx;
	DECLARE_DYNAMIC(CListCtrlEx)

// Konstruktion
public:
	CListCtrlEx();

// Attribute
protected:
	BOOL m_bReadOnly;
	HIGHLIGHTTYPE m_nHighlight;		// Indicate type of selection highlighting

#if _MSC_VER < 1200
public:
	CSize ApproximateViewRect(CSize sz = CSize(-1, -1), int iCount = -1) const;
#endif // _MSC_VER < 1200

// Überschreibungen
protected:
	void RepaintSelectedItems();

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CListCtrlEx();
	HIGHLIGHTTYPE SetHighlightType(HIGHLIGHTTYPE nHighlight);
	BOOL IsReadOnly() const;
	void SetReadOnly(BOOL bReadOnly = TRUE);

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline BOOL CListCtrlEx::IsReadOnly() const
	{ return m_bReadOnly; }

#if _MSC_VER < 1200
inline CSize CListCtrlEx::ApproximateViewRect(CSize sz, int iCount) const
	{ ASSERT(::IsWindow(m_hWnd)); return CSize((DWORD) ::SendMessage(m_hWnd, LVM_APPROXIMATEVIEWRECT, iCount, MAKELPARAM(sz.cx, sz.cy))); }
#endif // _MSC_VER < 1200

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_LISTCTRLEX_H__B7C10A41_8B81_11D3_9395_0000C03C43E0__INCLUDED_)
