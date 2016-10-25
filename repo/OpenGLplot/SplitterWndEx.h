/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
// based on a sample of
// Oleg G. Galkin
//
/////////////////////////////////////////////////////////////////////////////
// SplitterWndEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITTERWNDEX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_SPLITTERWNDEX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

class AFX_EXT_CLASS_GENERAL CSplitterWndEx : public CSplitterWnd
{
	DECLARE_DYNAMIC(CSplitterWndEx)
// Konstruktion
public:
	CSplitterWndEx();
	// Create a single view type splitter with multiple splits
	BOOL Create(CWnd* pParentWnd,
				int nMaxRows, int nMaxCols, SIZE sizeMin,
				CCreateContext* pContext,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE |
					WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT,
				UINT nID = AFX_IDW_PANE_FIRST);

	// Create a multiple view type splitter with static layout
	BOOL CreateStatic(CWnd* pParentWnd,
				int nRows, int nCols,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE,
				UINT nID = AFX_IDW_PANE_FIRST);

	typedef struct _RowColInfoEx
	{
		int nRowColNumber;	// number of row/column to be hidden/shown ( zerobased; -1 => nonexistant )
		BOOL bIsHidden;		// current state
	}
	RowColInfoEx;

	BOOL m_bEnableResizing;
	BOOL m_bEnableHighlighting;
	BOOL m_bIsActive;

// Attribute
protected:
	CArray<RowColInfoEx, RowColInfoEx&> m_ColInfoEx;
	CArray<RowColInfoEx, RowColInfoEx&> m_RowInfoEx;

// Operationen
public:
	int GetIdealRowInfo(int row) const;
	int GetIdealColumnInfo(int col) const;

	void ShowColumn(int colShow);
	void ShowRow(int rowShow);
	void HideColumn(int colHide);
	void HideRow(int rowHide);
	BOOL IsColHidden(int col);
	BOOL IsRowHidden(int row);
	int GetHiddenColumnCount();
	int GetHiddenRowCount();
	int GetSplitterBarWidth();
	int GetSplitterBarHeight();

	void RefreshSplitterBars(BOOL* pbActivate = NULL);
	int IdFromRowColEx(int row, int col) const;
	CWnd* GetPaneEx(int row, int col) const;
	BOOL IsChildPaneEx(CWnd* pWnd, int* pRow, int* pCol);

// Überschreibungen
	// determining active pane from focus or active view in frame
	virtual void SetActivePane(int row, int col, CWnd* pWnd = NULL);
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSplitterWndEx)
	public:
	virtual BOOL CanActivateNext(BOOL bPrev = FALSE);
	virtual void ActivateNext(BOOL bPrev = FALSE);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CSplitterWndEx();

// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CSplitterWndEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_SPLITTERWNDEX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
