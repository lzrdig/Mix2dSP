/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: October 1999
//
/////////////////////////////////////////////////////////////////////////////
// ListCtrlEx.cpp : Implementierung der Klasse CListCtrlEx
//

#include "StdAfx.h"
#include "ListCtrlEx.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx Konstruktion/Destruktion

CListCtrlEx::CListCtrlEx()
{
	m_bReadOnly = FALSE;
	m_nHighlight = HIGHLIGHT_NORMAL;
}

CListCtrlEx::~CListCtrlEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// Diagnose CListCtrlEx

#ifdef _DEBUG
void CListCtrlEx::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CListCtrlEx::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);

	dc << "\nm_bReadOnly = " << m_bReadOnly;
	dc << "\nm_nHighlight = " << m_nHighlight;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx Zeichnen

#if _MSC_VER >= 1200
#pragma warning(push) 
#endif // _MSC_VER >= 1200
#pragma warning(disable : 4238)	//nonstandard extension used : class rvalue used as lvalue
void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nItem = lpDrawItemStruct->itemID;
	CImageList* pImageList;

	// Save dc state
	int nSavedDC = pDC->SaveDC();

	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;	 // get all state flags
	GetItem(&lvi);

	// Should the item be highlighted
	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) 
		|| ((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS))));

	// Get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol(rcBounds);

	CString sLabel = GetItemText(nItem, 0);

	// Labels are offset by a certain amount
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1).cx*2;

	CRect rcHighlight;
	CRect rcWnd;
	int nExt;
	switch (m_nHighlight)
	{
	case HIGHLIGHT_NORMAL: 
		nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
		rcHighlight = rcLabel;
		if (rcLabel.left + nExt < rcLabel.right)
			rcHighlight.right = rcLabel.left + nExt;
		break;
	case HIGHLIGHT_ALLCOLUMNS:
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		break;
	case HIGHLIGHT_ROW:
		GetClientRect(&rcWnd);
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		rcHighlight.right = rcWnd.right;
		break;
	default:
		rcHighlight = rcLabel;
	}

	// Draw the background color
	if (bHighlight)
	{
		if (GetFocus() == this)
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

			pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
		}
		else
		{
			if (IsWindowEnabled())
			{
				pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
			}
			else
			{
				if (m_bReadOnly)
				{
					BYTE redText, greenText, blueText;
					short hue, lumText, lumGrayText, lumBtnFace, sat;
					COLORREF colorText, colorBtnFace, colorGrayText;

					colorText = ::GetSysColor(COLOR_APPWORKSPACE);
					colorGrayText = ::GetSysColor(COLOR_GRAYTEXT);
					colorBtnFace = ::GetSysColor(COLOR_BTNFACE);

					RGB2HLS(GetRValue(colorBtnFace), GetGValue(colorBtnFace), GetBValue(colorBtnFace), hue, lumBtnFace, sat);
					RGB2HLS(GetRValue(colorGrayText), GetGValue(colorGrayText), GetBValue(colorGrayText), hue, lumGrayText, sat);
					RGB2HLS(GetRValue(colorText), GetGValue(colorText), GetBValue(colorText), hue, lumText, sat);
					lumText = (short)MulDiv(lumText, lumGrayText, lumBtnFace);
					HLS2RGB(hue, lumText, sat, redText, greenText, blueText);

					pDC->SetTextColor(RGB(redText, greenText, blueText));
				}
				else
				{
					pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
				}
			}

			if (m_bReadOnly)
			{
				pDC->SetBkColor(::GetSysColor(COLOR_APPWORKSPACE));
				pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_APPWORKSPACE)));
			}
			else
			{
				pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
				pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_BTNFACE)));
			}
		}
	}
	else
	{
		pDC->SetTextColor(IsWindowEnabled() ? ::GetSysColor(COLOR_BTNTEXT) : ::GetSysColor(COLOR_GRAYTEXT));

		if (m_bReadOnly)
		{
			pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_BTNFACE)));
		}
		else
		{
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));
		}
	}

	// Set clip region
	rcCol.right = rcCol.left + GetColumnWidth(0);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcCol);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// Draw state icon
	if (lvi.state & LVIS_STATEIMAGEMASK)
	{
		int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		pImageList = GetImageList(LVSIL_STATE);
		if (pImageList)
		{
			pImageList->Draw(pDC, nImage,
				CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
		}
	}

	// Draw normal and overlay icon
	pImageList = GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		UINT nOvlImageMask = lvi.state & LVIS_OVERLAYMASK;
		pImageList->Draw(pDC, lvi.iImage, 
			CPoint(rcIcon.left, rcIcon.top), 
			(bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask);
	}

	// Draw item label - Column 0
	rcLabel.left += offset*2/3;
	//rcLabel.right -= offset;

	pDC->DrawText(sLabel, -1, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	if (m_nHighlight == HIGHLIGHT_NORMAL)	 // Highlight only first column
	{
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}

	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;
	rgn.CreateRectRgnIndirect(&rcBounds);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	for (int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;

		// Draw the background if needed
		if (m_nHighlight == HIGHLIGHT_NORMAL)
			pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));

		sLabel = GetItemText(nItem, nColumn);
		if (sLabel.GetLength() == 0)
			continue;

		// Get the text justification
		UINT nJustify = DT_LEFT;
		switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}

		rcLabel = rcCol;
		rcLabel.left += offset;
		rcLabel.right -= offset;

		pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	}

	// Draw focus rectangle if item has focus
	if (lvi.state & LVIS_FOCUSED && (GetFocus() == this))
		pDC->DrawFocusRect(rcHighlight);

	// Restore dc
	pDC->RestoreDC(nSavedDC);
}
#if _MSC_VER >= 1200
#pragma warning(pop) 
#else // _MSC_VER >= 1200
#pragma warning(default : 4238)	//nonstandard extension used : class rvalue used as lvalue
#endif // _MSC_VER >= 1200

void CListCtrlEx::RepaintSelectedItems()
{
	CRect rcBounds, rcLabel;

	// Invalidate focused item so it can repaint 
	int nItem = GetNextItem(-1, LVNI_FOCUSED);

	if (nItem != -1)
	{
		GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
		GetItemRect(nItem, rcLabel, LVIR_LABEL);
		rcBounds.left = rcLabel.left;

		InvalidateRect(rcBounds, FALSE);
	}

	// Invalidate selected items depending on LVS_SHOWSELALWAYS
	if (!(GetStyle() & LVS_SHOWSELALWAYS))
	{
		for (nItem = GetNextItem(-1, LVNI_SELECTED);
		nItem != -1; nItem = GetNextItem(nItem, LVNI_SELECTED))
		{
			GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
			GetItemRect(nItem, rcLabel, LVIR_LABEL);
			rcBounds.left = rcLabel.left;

			InvalidateRect(rcBounds, FALSE);
		}
	}

	UpdateWindow();
}

HIGHLIGHTTYPE CListCtrlEx::SetHighlightType(HIGHLIGHTTYPE nHighlight)
{
	HIGHLIGHTTYPE nOldHighlight = m_nHighlight;
	if (nHighlight <= HIGHLIGHT_ROW)
	{
		m_nHighlight = nHighlight;
		if (::IsWindow(m_hWnd))
			Invalidate();
	}
	return nOldHighlight;
}

void CListCtrlEx::SetReadOnly(BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;

	if (::IsWindow(m_hWnd))
	{
		InvalidateRect(NULL, FALSE);
#ifdef _DEBUG
		if (!(GetStyle() & LVS_OWNERDRAWFIXED))
			TRACE0("Warning: The LVS_OWNERDRAWFIXED style is not set, so \"SetReadOnly\" will have no effect.\n");
#endif //_DEBUG
	}
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CListCtrlEx

void CListCtrlEx::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);

	// check if we are getting focus from label edit box
	if (pOldWnd != NULL && pOldWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}

void CListCtrlEx::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);

	// check if we are losing focus to label edit box
	if (pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}

void CListCtrlEx::OnPaint() 
{
	// in full row select mode, we need to extend the clipping region
	// so we can paint a selection all the way to the right
	if (m_nHighlight == HIGHLIGHT_ROW && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
	{
		CRect rcBounds;
		GetItemRect(0, rcBounds, LVIR_BOUNDS);

		CRect rcClient;
		GetClientRect(&rcClient);
		if (rcBounds.right < rcClient.right)
		{
			CPaintDC dc(this);

			CRect rcClip;
			dc.GetClipBox(rcClip);

			rcClip.left = min(rcBounds.right-1, rcClip.left);
			rcClip.right = rcClient.right;

			InvalidateRect(rcClip, FALSE);
		}
	}

	if (GetStyle() & LVS_OWNERDRAWFIXED)
	{
		CRect rcClient, rcBounds;
		GetClientRect(&rcClient);

		int nItem = -1;
		while ((nItem = GetNextItem(nItem, LVNI_FOCUSED)) != -1)
		{
			GetItemRect(nItem, rcBounds, LVIR_BOUNDS);

			if (rcBounds.left > rcClient.left)
				rcBounds.left = rcClient.left;
			if (rcBounds.right < rcClient.right)
				rcBounds.right = rcClient.right;

			InvalidateRect(rcBounds, FALSE);
		}
	}

	CListCtrl::OnPaint();
}

#if _MSC_VER >= 1200
#pragma warning(push) 
#endif // _MSC_VER >= 1200
#pragma warning(disable : 4238)	//nonstandard extension used : class rvalue used as lvalue
BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC) 
{
	if (GetStyle() & LVS_OWNERDRAWFIXED)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		pDC->FillRect(rcClient, &CBrush(m_bReadOnly ? ::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_WINDOW)));
		return TRUE;
	}

	return CListCtrl::OnEraseBkgnd(pDC);
}
#if _MSC_VER >= 1200
#pragma warning(pop) 
#else // _MSC_VER >= 1200
#pragma warning(default : 4238)	//nonstandard extension used : class rvalue used as lvalue
#endif // _MSC_VER >= 1200
