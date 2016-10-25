/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
// based on a sample of
// Oleg G. Galkin
//
/////////////////////////////////////////////////////////////////////////////
// SplitterWndEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx

IMPLEMENT_DYNAMIC(CSplitterWndEx, CSplitterWnd)

BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
//{{AFX_MSG_MAP(CSplitterWndEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx Konstruktion/Destruktion

CSplitterWndEx::CSplitterWndEx()
{
	m_bEnableResizing = TRUE;
	m_bEnableHighlighting = TRUE;
	m_bIsActive = FALSE;
}

CSplitterWndEx::~CSplitterWndEx()
{
}

BOOL CSplitterWndEx::Create(CWnd* pParentWnd,
	int nMaxRows, int nMaxCols, SIZE sizeMin,
	CCreateContext* pContext, DWORD dwStyle, UINT nID)
{
	// MFC source code
	ASSERT(pParentWnd != NULL);
	ASSERT(sizeMin.cx > 0 && sizeMin.cy > 0);   // minimum must be non-zero

	ASSERT(pContext != NULL);
	ASSERT(pContext->m_pNewViewClass != NULL);
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(dwStyle & SPLS_DYNAMIC_SPLIT);   // must have dynamic split behavior

	// Dynamic splitters are limited to 2x2
	ASSERT(nMaxRows >= 1 && nMaxRows <= 2);
	ASSERT(nMaxCols >= 1 && nMaxCols <= 2);
	ASSERT(nMaxCols > 1 || nMaxRows > 1);	   // 1x1 is not permitted

	m_nMaxRows = nMaxRows;
	m_nMaxCols = nMaxCols;
	ASSERT(m_nRows == 0 && m_nCols == 0);	   // none yet
	m_nRows = m_nCols = 1;	  // start off as 1x1
	if (!CreateCommon(pParentWnd, sizeMin, dwStyle, nID))
		return FALSE;
	ASSERT(m_nRows == 1 && m_nCols == 1);	   // still 1x1

	ASSERT(pContext->m_pNewViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	m_pDynamicViewClass = pContext->m_pNewViewClass;
	// save for later dynamic creations

	// add the first initial pane
	if (!CreateView(0, 0, m_pDynamicViewClass, sizeMin, pContext))
	{
		DestroyWindow(); // will clean up child windows
		return FALSE;
	}
	m_pColInfo[0].nIdealSize = sizeMin.cx;
	m_pRowInfo[0].nIdealSize = sizeMin.cy;

	// additional code
	int k;

	m_RowInfoEx.SetSize(m_nMaxRows);
	m_RowInfoEx[0].nRowColNumber	= 0;
	m_RowInfoEx[0].bIsHidden		= FALSE;
	for (k = 1; k < m_nMaxRows; k++)
	{
		m_RowInfoEx[k].nRowColNumber	= -1;
		m_RowInfoEx[k].bIsHidden		= FALSE;
	}

	m_ColInfoEx.SetSize(m_nMaxCols);
	m_ColInfoEx[0].nRowColNumber	= 0;
	m_ColInfoEx[0].bIsHidden		= FALSE;
	for (k = 1; k < m_nMaxCols; k++)
	{
		m_ColInfoEx[k].nRowColNumber	= -1;
		m_ColInfoEx[k].bIsHidden		= FALSE;
	}

	return TRUE;
}

// simple "wiper" splitter
BOOL CSplitterWndEx::CreateStatic(CWnd* pParentWnd,
	int nRows, int nCols, DWORD dwStyle, UINT nID)
{
	// MFC source code
	ASSERT(pParentWnd != NULL);
	ASSERT(nRows >= 1 && nRows <= 16);
	ASSERT(nCols >= 1 && nCols <= 16);
	ASSERT(nCols > 1 || nRows > 1);	 // 1x1 is not permitted
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(!(dwStyle & SPLS_DYNAMIC_SPLIT)); // can't have dynamic split

	ASSERT(m_nRows == 0 && m_nCols == 0);	   // none yet
	m_nRows = m_nMaxRows = nRows;
	m_nCols = m_nMaxCols = nCols;

	// create with zero minimum pane size
	if (!CreateCommon(pParentWnd, CSize(0, 0), dwStyle, nID))
		return FALSE;

	// additional code
	int k;

	m_RowInfoEx.SetSize(m_nMaxRows);
	for (k = 0; k < m_nMaxRows; k++)
	{
		m_RowInfoEx[k].nRowColNumber	= k;
		m_RowInfoEx[k].bIsHidden		= FALSE;
	}
	m_ColInfoEx.SetSize(m_nMaxCols);

	for (k = 0; k < m_nMaxCols; k++)
	{
		m_ColInfoEx[k].nRowColNumber	= k;
		m_ColInfoEx[k].bIsHidden		= FALSE;
	}

	// all panes must be created with explicit calls to CreateView
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx Kommandos

void CSplitterWndEx::ShowColumn(int colShow)
{
	int row, col, k, colOld = -1, colNew = -1;
	ASSERT_VALID(this);
	ASSERT(m_nCols < m_nMaxCols);

	// find index of colShow
	ASSERT(m_ColInfoEx.GetSize() == m_nMaxCols);
	for (k = 0; k < m_nMaxCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber == colShow)
		{
			colOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxCols);
	ASSERT(colOld >= m_nCols);
	ASSERT(m_ColInfoEx[colOld].bIsHidden);

	// find index, where to insert colShow
	for (k = 0; k <= m_nCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber > colShow || k == m_nCols)
		{
			colNew = k;
			break;
		}
	}

	int cxNew = m_pColInfo[colOld].nCurSize;
	m_nCols++;  // add a column
	ASSERT(m_nCols <= m_nMaxCols);

	// fill the hidden column
	for (row = 0; row < m_nMaxRows; row++)
	{
		CWnd* pPaneShow = GetDlgItem(IdFromRowColEx(row, colOld));
		if (row < m_nRows)				// visible rows
		{
			ASSERT(pPaneShow != NULL);
			pPaneShow->ShowWindow(SW_SHOWNA);
		}
		else							// hidden rows
		{
			if (pPaneShow == NULL)
				continue;
		}

		for (col = colOld - 1; col >= colNew; col--)
		{
			CWnd* pPane = GetDlgItem(IdFromRowColEx(row, col));
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowColEx(row, col+1));
		}

		pPaneShow->SetDlgCtrlID(IdFromRowColEx(row, colNew));
	}

	// update m_ColInfoEx
	m_ColInfoEx[colOld].bIsHidden = FALSE;
	RowColInfoEx tmpInfo = m_ColInfoEx[colOld];
	m_ColInfoEx.RemoveAt(colOld);
	m_ColInfoEx.InsertAt(colNew, tmpInfo);

	// new panes have been created -- recalculate layout
	for (col = colNew + 1; col <= colOld; col++)
		m_pColInfo[col].nIdealSize = m_pColInfo[col - 1].nCurSize;
	m_pColInfo[colNew].nIdealSize = cxNew;
	RecalcLayout();
}

void CSplitterWndEx::ShowRow(int rowShow)
{
	int row, col, k, rowOld = -1, rowNew = -1;
	ASSERT_VALID(this);
	ASSERT(m_nRows < m_nMaxRows);

	// find index of rowShow
	ASSERT(m_RowInfoEx.GetSize() == m_nMaxRows);
	for (k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == rowShow)
		{
			rowOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxRows);
	ASSERT(rowOld >= m_nRows);
	ASSERT(m_RowInfoEx[rowOld].bIsHidden);

	// find index, where to insert rowShow
	for (k = 0; k <= m_nRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber > rowShow || k == m_nRows)
		{
			rowNew = k;
			break;
		}
	}

	int cyNew = m_pRowInfo[rowOld].nCurSize;
	m_nRows++;  // add a row
	ASSERT(m_nRows <= m_nMaxRows);

	// fill the hidden row
	for (col = 0; col < m_nMaxCols; col++)
	{
		CWnd* pPaneShow = GetDlgItem(IdFromRowColEx(rowOld, col));
		if (col < m_nCols)				// visible columns
		{
			ASSERT(pPaneShow != NULL);
			pPaneShow->ShowWindow(SW_SHOWNA);
		}
		else							// hidden columns
		{
			if (pPaneShow == NULL)
				continue;
		}

		for (row = rowOld - 1; row >= rowNew; row--)
		{
			CWnd* pPane = GetDlgItem(IdFromRowColEx(row, col));
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowColEx(row+1, col));
		}

		pPaneShow->SetDlgCtrlID(IdFromRowColEx(rowNew, col));
	}

	// update m_RowInfoEx
	m_RowInfoEx[rowOld].bIsHidden = FALSE;
	RowColInfoEx tmpInfo = m_RowInfoEx[rowOld];
	m_RowInfoEx.RemoveAt(rowOld);
	m_RowInfoEx.InsertAt(rowNew, tmpInfo);

	// new panes have been created -- recalculate layout
	for (row = rowNew + 1; row <= rowOld; row++)
		m_pRowInfo[row].nIdealSize = m_pRowInfo[row - 1].nCurSize;
	m_pRowInfo[rowNew].nIdealSize = cyNew;
	RecalcLayout();
}

void CSplitterWndEx::HideColumn(int colHide)
{
	int rowActive, colActive, row, col, k, colOld = -1, colNew = m_nCols - 1;
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);

	// find index of colHide
	ASSERT(m_ColInfoEx.GetSize() == m_nMaxCols);
	for (k = 0; k < m_nMaxCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber == colHide)
		{
			colOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxCols);
	ASSERT(colOld < m_nCols);
	ASSERT(!m_ColInfoEx[colOld].bIsHidden);

	// if the column has an active window -- change it
	if (GetActivePane(&rowActive, &colActive) != NULL && colActive == colOld)
	{
		if (++colActive >= m_nCols)
			colActive = 0;
		SetActivePane(rowActive, colActive);
	}

	// hide all column panes
	for (row = 0; row < m_nMaxRows; row++)
	{
		CWnd* pPaneHide = GetDlgItem(IdFromRowColEx(row, colOld));
		if (row < m_nRows)				// visible rows
		{
			ASSERT(pPaneHide != NULL);
			pPaneHide->ShowWindow(SW_HIDE);
		}
		else							// hidden rows
		{
			if (pPaneHide == NULL)
				continue;
		}

		for (col = colOld + 1; col <= colNew; col++)
		{
			CWnd* pPane = GetDlgItem(IdFromRowColEx(row, col));
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowColEx(row, col-1));
		}

		pPaneHide->SetDlgCtrlID(IdFromRowColEx(row, colNew));
	}

	// update m_ColInfoEx
	m_ColInfoEx[colOld].bIsHidden = TRUE;
	RowColInfoEx tmpInfo = m_ColInfoEx[colOld];
	m_ColInfoEx.RemoveAt(colOld);
	m_ColInfoEx.InsertAt(colNew, tmpInfo);

	m_pColInfo[colNew].nCurSize = m_pColInfo[colOld].nCurSize;
	m_nCols--;
	RecalcLayout();
}

void CSplitterWndEx::HideRow(int rowHide)
{
	int rowActive, colActive, row, col, k, rowOld = -1, rowNew = m_nRows - 1;
	ASSERT_VALID(this);
	ASSERT(m_nRows > 1);

	// find index of rowHide
	ASSERT(m_RowInfoEx.GetSize() == m_nMaxRows);
	for (k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == rowHide)
		{
			rowOld = k;
			break;
		}
	}
	ASSERT(k < m_nMaxRows);
	ASSERT(rowOld < m_nRows);
	ASSERT(!m_RowInfoEx[rowOld].bIsHidden);

	// if the row has an active window -- change it
	if (GetActivePane(&rowActive, &colActive) != NULL && rowActive == rowOld)
	{
		if (++rowActive >= m_nRows)
			rowActive = 0;
		SetActivePane(rowActive, colActive);
	}

	// hide all row panes
	for (col = 0; col < m_nMaxCols; col++)
	{
		CWnd* pPaneHide = GetDlgItem(IdFromRowColEx(rowOld, col));
		if (col < m_nCols)				// visible columns
		{
			ASSERT(pPaneHide != NULL);
			pPaneHide->ShowWindow(SW_HIDE);
		}
		else							// hidden columns
		{
			if (pPaneHide == NULL)
				continue;
		}

		for (row = rowOld + 1; row <= rowNew; row++)
		{
			CWnd* pPane = GetDlgItem(IdFromRowColEx(row, col));
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowColEx(row-1, col));
		}

		pPaneHide->SetDlgCtrlID(IdFromRowColEx(rowNew, col));
	}

	// update m_RowInfoEx
	m_RowInfoEx[rowOld].bIsHidden = TRUE;
	RowColInfoEx tmpInfo = m_RowInfoEx[rowOld];
	m_RowInfoEx.RemoveAt(rowOld);
	m_RowInfoEx.InsertAt(rowNew, tmpInfo);

	m_pRowInfo[rowNew].nCurSize = m_pRowInfo[rowOld].nCurSize;
	m_nRows--;
	RecalcLayout();
}

BOOL CSplitterWndEx::IsColHidden(int col)
{
	ASSERT(m_ColInfoEx.GetSize() == m_nMaxCols);
	int k;
	for (k = 0; k < m_nMaxCols; k++)
	{
		if (m_ColInfoEx[k].nRowColNumber == col)
			return m_ColInfoEx[k].bIsHidden;
	}
	ASSERT(k < m_nMaxCols);
	return FALSE;
}

BOOL CSplitterWndEx::IsRowHidden(int row)
{
	ASSERT(m_RowInfoEx.GetSize() == m_nMaxRows);
	int k;
	for (k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == row)
			return m_RowInfoEx[k].bIsHidden;
	}
	ASSERT(k < m_nMaxRows);
	return FALSE;
}

int CSplitterWndEx::GetHiddenColumnCount()
{
	ASSERT(m_ColInfoEx.GetSize() == m_nMaxCols);
	int count = 0;
	for (int k = 0; k < m_nMaxCols; k++)
	{
		count += m_ColInfoEx[k].bIsHidden;
	}
	return count;
}

int CSplitterWndEx::GetHiddenRowCount()
{
	ASSERT(m_RowInfoEx.GetSize() == m_nMaxRows);
	int count = 0;
	for (int k = 0; k < m_nMaxRows; k++)
	{
		count += m_RowInfoEx[k].bIsHidden;
	}
	return count;
}

int CSplitterWndEx::GetSplitterBarWidth()
{
	return m_cxSplitter;
}

int CSplitterWndEx::GetSplitterBarHeight()
{
	return m_cySplitter;
}

void CSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bEnableResizing)
		CWnd::OnLButtonDown(nFlags, point);
	else
		CSplitterWnd::OnLButtonDown(nFlags, point);
}

BOOL CSplitterWndEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!m_bEnableResizing)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	else
		return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterWndEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bEnableResizing)
		CWnd::OnMouseMove(nFlags, point);
	else
		CSplitterWnd::OnMouseMove(nFlags, point);
}

BOOL CSplitterWndEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	BOOL bRetVal = FALSE;
	int row;
	int col;

	CRect mRect;
	CScrollBar* pBar;
	CWnd* pPane;
	CScrollView* pView;
	int nOldPos = -1;

	for (row = 0; row < m_nRows; row++)
	{
		for (col = 0; col < m_nCols; col++)
		{
			pPane = GetPane(row, col);
			if (!pPane->IsKindOf(RUNTIME_CLASS(CScrollView)))
				continue;

			pView = STATIC_DOWNCAST(CScrollView, pPane);
			if (pView != NULL)
			{
				pBar = pView->GetScrollBarCtrl(SB_VERT);
				if (pBar == NULL)
				{
					pBar = pView->GetScrollBarCtrl(SB_HORZ);
					/*if (pBar == NULL)
						continue;*/
				}
				pView->GetWindowRect(&mRect);

				if (pBar)
					nOldPos = pBar->GetScrollPos();

				if (pt.x >= mRect.left && pt.x <= mRect.right && pt.y >= mRect.top && pt.y <= mRect.bottom)
					if (pView->DoMouseWheel(nFlags, zDelta, pt))
					{
						bRetVal = TRUE;
					}

				if (pBar && col < m_nCols -1)
					pBar->SetScrollPos(nOldPos, FALSE);
			}
		}
	}

	return bRetVal;
}

int CSplitterWndEx::IdFromRowColEx(int row, int col) const
{
	// Same as CSplitterWnd::IdFromRowCol, except that we allow indices above m_nRows/m_nCols
	ASSERT_VALID(this);
	ASSERT(row >= 0);
	ASSERT(row < m_nMaxRows);
	ASSERT(col >= 0);
	ASSERT(col < m_nMaxCols);

	return AFX_IDW_PANE_FIRST + row * 16 + col;
}

CWnd* CSplitterWndEx::GetPaneEx(int row, int col) const
{
	// Same as CSplitterWnd::GetPane, except that we allow indices above m_nRows/m_nCols
	//	and that we take the *ORIGINAL* order instead of the CURRENT order
	ASSERT_VALID(this);
	ASSERT(row >= 0);
	ASSERT(row < m_nMaxRows);
	ASSERT(col >= 0);
	ASSERT(col < m_nMaxCols);

	ASSERT(m_RowInfoEx.GetSize() == m_nMaxRows);
	int k;
	for (k = 0; k < m_nMaxRows; k++)
	{
		if (m_RowInfoEx[k].nRowColNumber == row)
			break;
	}
	ASSERT(k < m_nMaxRows);

	ASSERT(m_ColInfoEx.GetSize() == m_nMaxCols);
	int l;
	for (l = 0; l < m_nMaxCols; l++)
	{
		if (m_ColInfoEx[l].nRowColNumber == col)
			break;
	}
	ASSERT(l < m_nMaxCols);

	CWnd* pView = GetDlgItem(IdFromRowColEx(k, l));
	ASSERT(pView != NULL);  // panes can be a CWnd, but are usually CViews
	return pView;
}

BOOL CSplitterWndEx::IsChildPaneEx(CWnd* pWnd, int* pRow, int* pCol)
{
	// Same as CSplitterWnd::IsChildPane, except that we allow indices above m_nRows/m_nCols
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	UINT nID = pWnd->GetDlgCtrlID();
	if (IsChild(pWnd) && nID >= AFX_IDW_PANE_FIRST && nID <= AFX_IDW_PANE_LAST)
	{
		if (pRow != NULL)
			*pRow = (nID - AFX_IDW_PANE_FIRST) / 16;
		if (pCol != NULL)
			*pCol = (nID - AFX_IDW_PANE_FIRST) % 16;
		ASSERT(pRow == NULL || *pRow < m_nMaxRows);
		ASSERT(pCol == NULL || *pCol < m_nMaxCols);
		return TRUE;
	}
	else
	{
		if (pRow != NULL)
			*pRow = -1;
		if (pCol != NULL)
			*pCol = -1;
		return FALSE;
	}
}

void CSplitterWndEx::SetActivePane(int row, int col, CWnd* pWnd)
{
	// set the focus to the pane
	CWnd* pPane = pWnd == NULL ? GetPane(row, col) : pWnd;
	if (pPane->IsKindOf(RUNTIME_CLASS(CView)))
	{
		CFrameWnd* pFrameWnd = GetParentFrame();
		ASSERT_VALID(pFrameWnd);
		pFrameWnd->SetActiveView((CView*)pPane);
	}
	else if (pPane->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
	{
		((CSplitterWndEx*)pPane)->SetActivePane(0,0);
	}
	else
	{
		TRACE0("Warning: Next pane is not a view - calling SetFocus.\n");
		pPane->SetFocus();
	}
}

CWnd* CSplitterWndEx::GetActivePane(int* pRow, int* pCol)
	// return active view, NULL when no active view
{
	ASSERT_VALID(this);

	// attempt to use active view of frame window
	CWnd* pView = NULL;
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	pView = pFrameWnd->GetActiveView();

	// failing that, use the current focus
	if (pView == NULL)
		pView = GetFocus();

	// Note: The original SplitterWnd::GetActivePane causes
	// problems with nested splitters - IsChildPane return 
	// nonsense in case pView is a pane of a nested splitter.
	// So we try to iterate to the correct splitter.
	while (pView)
	{
		if (pView->GetParent() == this)
			break;
		else
			pView = pView->GetParent();
	}

	// make sure the pane is a child pane of the splitter
	if (pView != NULL && !IsChildPane(pView, pRow, pCol))
		pView = NULL;

	return pView;
}

BOOL CSplitterWndEx::CanActivateNext(BOOL bPrev)
{
	ASSERT_VALID(this);

	if (m_nRows > 1 || m_nCols > 1)
		return TRUE;

	CWnd* pParent = GetParent();
	if (pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
	{
		// let the parent splitter decide it
		return ((CSplitterWndEx*)pParent)->CanActivateNext(bPrev);
	}

	return FALSE;
}

void CSplitterWndEx::ActivateNext(BOOL bPrev)
{
	ASSERT_VALID(this);

	// find the coordinate of the current pane
	int row, col;
	if (GetActivePane(&row, &col) == NULL)
	{
		if (bPrev)
		{
			row = m_nRows - 1;
			col = m_nCols - 1;
		}
		else
		{
			row = 0;
			col = 0;
		}
		TRACE("Warning: There is no current view - go to pane (%d,%d).\n", row, col);
	}
	else
	{
		ASSERT(row >= 0 && row < m_nRows);
		ASSERT(col >= 0 && col < m_nCols);

		// determine next pane
		if (bPrev)
		{
			// prev
			if (--col < 0)
			{
				col = m_nCols - 1;
				if (--row < 0)
				{
					// after first panes check for parent (nested) splitter
					CWnd* pParent = GetParent();
					if (pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
					{
						if (((CSplitterWndEx*)pParent)->CanActivateNext(bPrev))
						{
							// let the parent splitter do it
							((CSplitterWndEx*)pParent)->ActivateNext(bPrev);
							return;
						}
					}
					row = m_nRows - 1;
				}
			}
		}
		else
		{
			// next
			if (++col >= m_nCols)
			{
				col = 0;
				if (++row >= m_nRows)
				{
					// after last panes check for parent (nested) splitter
					CWnd* pParent = GetParent();
					if (pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
					{
						if (((CSplitterWndEx*)pParent)->CanActivateNext(bPrev))
						{
							// let the parent splitter do it
							((CSplitterWndEx*)pParent)->ActivateNext(bPrev);
							return;
						}
					}
					row = 0;
				}
			}
		}
	}

	// check for child (nested) splitter
	CWnd* pPane = GetPane(row, col);
	if (pPane->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
	{
		// let the child splitter do it
		((CSplitterWndEx*)pPane)->ActivateNext(bPrev);
	}
	else
	{
		// set newly active pane
		SetActivePane(row, col);
	}
}

void CSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	BOOL bMoreThanOnePane = FALSE;
	for (CWnd* pWnd = this; pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)); pWnd = pWnd->GetParent())
	{
		if (((CSplitterWnd*)pWnd)->GetRowCount() > 1 || ((CSplitterWnd*)pWnd)->GetColumnCount() > 1)
		{
			bMoreThanOnePane = TRUE;
			break;
		}
	}

	CWnd* pPane = GetActivePane();
	if (m_bEnableHighlighting && nType == splitBorder && bMoreThanOnePane && 
		pPane && !pPane->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	{
		CRect rect;
		pPane->GetWindowRect(&rect);
		ScreenToClient(&rect);

		if (rectArg.PtInRect(rect.CenterPoint()))
		{
			// if pDC == NULL, then just invalidate
			if (pDC == NULL)
			{
				RedrawWindow(rectArg, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
				return;
			}

			// otherwise, actually draw
			COLORREF colorHighlight = m_bIsActive ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_APPWORKSPACE);
			ASSERT_VALID(pDC);
			rect = rectArg;
			pDC->Draw3dRect(rect, colorHighlight, colorHighlight);
			rect.InflateRect(-GetSystemMetrics(SM_CXBORDER), -GetSystemMetrics(SM_CYBORDER));
			pDC->Draw3dRect(rect, colorHighlight, colorHighlight);
			return;
		}
	}

	CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
}

void CSplitterWndEx::RefreshSplitterBars(BOOL* pbActivate)
{
	// store activation status (to be used in the overridden OnDrawSplitter)
	if (pbActivate)
		m_bIsActive = *pbActivate;

	CRect rectInside;
	GetInsideRect(rectInside);
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}

int CSplitterWndEx::GetIdealRowInfo(int row) const
{
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nMaxRows);

	return m_pRowInfo[row].nIdealSize;
}

int CSplitterWndEx::GetIdealColumnInfo(int col) const
{
	ASSERT_VALID(this);
	ASSERT(col >= 0 && col < m_nMaxCols);

	return m_pColInfo[col].nIdealSize;
}

void CSplitterWndEx::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CSplitterWnd::OnShowWindow(bShow, nStatus);

	// Do not send WM_SHOWWINDOW to the panes immediately after CreateCommon.
	if (m_ColInfoEx.GetSize() == m_nMaxCols && m_RowInfoEx.GetSize() == m_nMaxRows)
	{
		for (int k = 0; k < m_nRows; k++)
		{
			for (int l = 0; l < m_nCols; l++)
			{
				CWnd* pView = GetPane(k, l);

				ASSERT(pView && IsWindow(pView->GetSafeHwnd()));
				pView->SendMessage(WM_SHOWWINDOW, bShow, nStatus);
			}
		}
	}
}
