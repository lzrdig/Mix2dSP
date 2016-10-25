/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: May 2002
//
/////////////////////////////////////////////////////////////////////////////
// ButtonEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "ButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFont CButtonEx::m_SymbolFont;
int CButtonEx::m_nInstanceCounter = 0;

/////////////////////////////////////////////////////////////////////////////
// CButtonEx

CButtonEx::CButtonEx()
{
	m_bDoReadOnly = FALSE;
	m_bIsReadOnly = FALSE;
	m_nStyle = 0;
	m_nCheck = 0;

	if (m_nInstanceCounter++ == 0)
	{
		InitializeSymbolFont();
	}
}

CButtonEx::~CButtonEx()
{
	if (--m_nInstanceCounter == 0)
	{
		m_SymbolFont.DeleteObject();
	}
	ASSERT(m_nInstanceCounter >= 0);
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_MESSAGE(BM_GETCHECK, OnBmGetcheck)
	ON_MESSAGE(BM_SETCHECK, OnBmSetcheck)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CButtonEx 

void CButtonEx::OnPaint() 
{
	if (m_bDoReadOnly != m_bIsReadOnly && AllowReadOnly())
	{
		// Remove/restore WS_TABSTOP for BS_AUTORADIOBUTTON buttons
		UINT nStyle = m_bIsReadOnly ? m_nStyle : (GetStyle() & 0xffff);
		BOOL bIsRadioButton = !((nStyle & 0x0f) ^ BS_AUTORADIOBUTTON);

		int nCheck = GetCheck();
		if (m_bDoReadOnly)
		{
			m_nStyle = (GetStyle() & 0xffff);
			if (IsWindowEnabled())
				ModifyStyle(m_nStyle & 0x0f, BS_OWNERDRAW);

			if (bIsRadioButton)
				ModifyStyle(WS_TABSTOP, 0);
		}
		else
		{
			ModifyStyle(BS_OWNERDRAW, m_nStyle & 0x0f);
			ASSERT(m_nStyle == (GetStyle() & 0xffff));

			if (bIsRadioButton && nCheck)
				ModifyStyle(0, WS_TABSTOP);
		}
		SetCheck(nCheck);

		m_bIsReadOnly = m_bDoReadOnly;
	}

	CButton::OnPaint();
}

void CButtonEx::OnEnable(BOOL bEnable) 
{
	ASSERT(IsWindowEnabled() == bEnable);

	if (m_bIsReadOnly && AllowReadOnly())
	{
		if (bEnable)
		{
			ModifyStyle(m_nStyle & 0x0f, BS_OWNERDRAW);
		}
		else
		{
			ModifyStyle(BS_OWNERDRAW, m_nStyle & 0x0f);
			ASSERT(m_nStyle == (GetStyle() & 0xffff));
		}
	}

	CButton::OnEnable(bEnable);
}

BOOL CButtonEx::SetReadOnly(BOOL bReadOnly)
{
	ASSERT(::IsWindow(m_hWnd));

	if (!AllowReadOnly())
		return FALSE;

	m_bDoReadOnly = !!bReadOnly;

	UpdateWindow();					// Force finishing "set read only".
	InvalidateRect(NULL, FALSE);	// The previous command is executed even if this
									// window is not visible, so we invalidate it again.

	return TRUE;
}

BOOL CButtonEx::AllowReadOnly()
{
	// Note: We support a "Read Only" property only for BS_AUTOCHECKBOX, BS_AUTO3STATE and BS_AUTORADIOBUTTON
	//	and not BS_PUSHLIKE, BS_ICON or BS_BITMAP
	UINT nStyle = m_bIsReadOnly ? m_nStyle : (GetStyle() & 0xffff);
	BOOL bIsCheckBox = !((nStyle & 0x0f) ^ BS_AUTOCHECKBOX);
	BOOL bIsCheckBox3State = !((nStyle & 0x0f) ^ BS_AUTO3STATE);
	BOOL bIsRadioButton = !((nStyle & 0x0f) ^ BS_AUTORADIOBUTTON);
	BOOL bIsPushlike= (nStyle & BS_PUSHLIKE);
	BOOL bIsIcon= (nStyle & BS_ICON);
	BOOL bIsBitmap= (nStyle & BS_BITMAP);

	return ((bIsCheckBox || bIsCheckBox3State || bIsRadioButton) && !bIsPushlike && !bIsIcon && !bIsBitmap);
}

#if _MSC_VER >= 1200
#pragma warning(push) 
#endif // _MSC_VER >= 1200
#pragma warning(disable : 4238)	//nonstandard extension used : class rvalue used as lvalue
void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CRect rcBounds, rcIcon, rcLabel, rcFocus;
	CSize sizeLabel, sizeIcon;
	int nIconOffset, nLabelOffset;
	ASSERT(GetButtonStyle() & BS_OWNERDRAW);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// Save dc state
	int nSavedDC = pDC->SaveDC();

	BOOL bIsFlat = m_nStyle & BS_FLAT;
	BOOL bIsMultiline = m_nStyle & BS_MULTILINE;
	BOOL bIsLeftText = m_nStyle & BS_LEFTTEXT;
	BOOL bIsCheckBox = !((m_nStyle & 0x0f) ^ BS_AUTOCHECKBOX);
	BOOL bIsCheckBox3State = !((m_nStyle & 0x0f) ^ BS_AUTO3STATE);
	BOOL bIsRadioButton = !((m_nStyle & 0x0f) ^ BS_AUTORADIOBUTTON);
	ASSERT(bIsCheckBox || bIsCheckBox3State || bIsRadioButton);

	UINT nIconFormat = DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;
	UINT nLabelFormat = DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER;
	if (bIsMultiline)
		nLabelFormat &= ~DT_SINGLELINE;

	// Get offsets
	nLabelOffset = pDC->GetTextExtent(_T(" "), 1).cx;
	CString sLabel;
	GetWindowText(sLabel);
	if (bIsMultiline)
	{
		CString sLine, sRest(sLabel);
		CSize sizeLine;
		int nDelimiter;

		sizeLabel.cx = sizeLabel.cy = 0;
		do
		{
			nDelimiter = sRest.FindOneOf(_T("\r\n"));
			if (nDelimiter >= 0)
			{
				sLine = sRest.Left(nDelimiter);
				sRest = sRest.Right(sRest.GetLength()-nDelimiter-1);
			}
			else
			{
				sLine = sRest;
			}

			sizeLine = pDC->GetTextExtent(sLine);

			sizeLabel.cx = max(sizeLabel.cx, sizeLine.cx);
			sizeLabel.cy += sizeLine.cy;
		}
		while (nDelimiter >= 0);
	}
	else
		sizeLabel = pDC->GetTextExtent(sLabel);

	CFont* pOldFont = pDC->SelectObject(&m_SymbolFont);
	if (bIsCheckBox || bIsCheckBox3State)
	{
		sizeIcon.cx = max(max(pDC->GetTextExtent(_T("c"), 1).cx,
				pDC->GetTextExtent(_T("d"), 1).cx),
			max(pDC->GetTextExtent(_T("e"), 1).cx,
				pDC->GetTextExtent(_T("f"), 1).cx));
		sizeIcon.cy = max(max(pDC->GetTextExtent(_T("c"), 1).cy,
				pDC->GetTextExtent(_T("d"), 1).cy),
			max(pDC->GetTextExtent(_T("e"), 1).cy,
				pDC->GetTextExtent(_T("f"), 1).cy));
	}
	else if (bIsRadioButton)
	{
		sizeIcon.cx = max(max(pDC->GetTextExtent(_T("k"), 1).cx,
				pDC->GetTextExtent(_T("l"), 1).cx),
			max(pDC->GetTextExtent(_T("m"), 1).cx,
				pDC->GetTextExtent(_T("n"), 1).cx));
		sizeIcon.cy = max(max(pDC->GetTextExtent(_T("k"), 1).cy,
				pDC->GetTextExtent(_T("l"), 1).cy),
			max(pDC->GetTextExtent(_T("m"), 1).cy,
				pDC->GetTextExtent(_T("n"), 1).cy));
	}
	nIconOffset = sizeIcon.cx;

	// Get rectangles for drawing
	GetWindowRect(&rcBounds);
	ScreenToClient(&rcBounds);

	rcLabel = rcIcon = rcBounds;
	if (bIsLeftText)
	{
		rcLabel.right = rcIcon.left = rcBounds.right - nIconOffset;
		rcLabel.left += nLabelOffset*2/3;
	}
	else
	{
		rcLabel.left = rcIcon.right = rcBounds.left + nIconOffset;
		rcLabel.left += nLabelOffset*5/3;
	}

	// Draw the background

	pDC->FillRect(rcBounds, &CBrush(GetSysColor(COLOR_3DFACE)));

	// Draw the icon
	int nOldBkMode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetSysColor(COLOR_3DFACE));
	}

	if (rcIcon.Height() - (sizeIcon.cy + nLabelOffset) > 0)
	{
		if (!((m_nStyle & 0x0C00) ^ BS_TOP))
		{
			rcIcon.bottom -= rcIcon.Height() - (sizeIcon.cy + nLabelOffset);
		}
		else if (!((m_nStyle & 0x0C00) ^ BS_BOTTOM))
		{
			rcIcon.top += rcIcon.Height() - (sizeIcon.cy + nLabelOffset);
		}
		else
		{
			int nHeight = rcIcon.Height();
			rcIcon.top += (nHeight - (sizeIcon.cy + nLabelOffset)) / 2;
			rcIcon.bottom -= (nHeight - (sizeIcon.cy + nLabelOffset)) / 2;
		}
	}

	if (bIsCheckBox || bIsCheckBox3State)
	{
		if (bIsCheckBox3State && m_nCheck >= 2)
		{
			pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
			pDC->DrawText("g", -1, rcIcon, nIconFormat);
			pDC->SetTextColor(GetSysColor(COLOR_3DSHADOW));
			pDC->DrawText("b", -1, rcIcon, nIconFormat);
		}
		else if (m_nCheck)
		{
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			pDC->DrawText("b", -1, rcIcon, nIconFormat);
		}

		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DHILIGHT));
		pDC->DrawText("d", -1, rcIcon, nIconFormat);
		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DDKSHADOW));
		pDC->DrawText("e", -1, rcIcon, nIconFormat);
		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DSHADOW));
		pDC->DrawText("c", -1, rcIcon, nIconFormat);
		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DLIGHT));
		pDC->DrawText("f", -1, rcIcon, nIconFormat);
	}
	else if (bIsRadioButton)
	{
		if (m_nCheck)
		{
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			pDC->DrawText("i", -1, rcIcon, nIconFormat);
		}

		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DHILIGHT));
		pDC->DrawText("k", -1, rcIcon, nIconFormat);
		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DDKSHADOW));
		pDC->DrawText("l", -1, rcIcon, nIconFormat);
		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DSHADOW));
		pDC->DrawText("j", -1, rcIcon, nIconFormat);
		pDC->SetTextColor(GetSysColor(bIsFlat ? COLOR_3DDKSHADOW : COLOR_3DLIGHT));
		pDC->DrawText("m", -1, rcIcon, nIconFormat);
	}

	pDC->SetBkMode(nOldBkMode);

	if (pOldFont)
		pDC->SelectObject(pOldFont);

	// Draw the label
	if (!((m_nStyle & 0x0300) ^ BS_CENTER))
	{
		int nWidth = rcLabel.Width();
		rcLabel.left += (nWidth - (sizeLabel.cx + nLabelOffset/2)) / 2;
		rcLabel.right -= (nWidth - (sizeLabel.cx + nLabelOffset/2)) / 2;
		rcLabel.OffsetRect(bIsLeftText ? -nLabelOffset*3/4 : -nLabelOffset/4, 0);
	}
	else if (!((m_nStyle & 0x0300) ^ BS_RIGHT))
	{
		rcLabel.left += rcLabel.Width() - (sizeLabel.cx + nLabelOffset/2);
		rcLabel.OffsetRect(bIsLeftText ? -nLabelOffset*3/2 : -nLabelOffset/2, 0);
	}
	else
	{
		rcLabel.right -= rcLabel.Width() - (sizeLabel.cx + nLabelOffset/2);
	}

	if (rcLabel.Height() - (sizeLabel.cy + nLabelOffset) > 0)
	{
		if (!((m_nStyle & 0x0C00) ^ BS_TOP))
		{
			rcLabel.bottom -= rcLabel.Height() - (sizeLabel.cy + nLabelOffset);
		}
		else if (!((m_nStyle & 0x0C00) ^ BS_BOTTOM))
		{
			rcLabel.top += rcLabel.Height() - (sizeLabel.cy + nLabelOffset);
		}
		else
		{
			int nHeight = rcLabel.Height();
			rcLabel.top += (nHeight - (sizeLabel.cy + nLabelOffset)) / 2;
			rcLabel.bottom -= (nHeight - (sizeLabel.cy + nLabelOffset)) / 2;
		}
	}

	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkColor(GetSysColor(COLOR_3DFACE));

	pDC->DrawText(sLabel, -1, rcLabel, nLabelFormat);

	// Draw focus rectangle if item has focus
	if (lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		rcLabel.left -= nLabelOffset/2;
		pDC->DrawFocusRect(rcLabel);
	}

	// Restore dc
	pDC->RestoreDC(nSavedDC);
}
#if _MSC_VER >= 1200
#pragma warning(pop) 
#else // _MSC_VER >= 1200
#pragma warning(default : 4238)	//nonstandard extension used : class rvalue used as lvalue
#endif // _MSC_VER >= 1200

UINT CButtonEx::OnGetDlgCode() 
{
	UINT nDlgCode = CButton::OnGetDlgCode();

	if (m_bIsReadOnly)
	{
		BOOL bIsRadioButton = !((m_nStyle & 0x0f) ^ BS_AUTORADIOBUTTON);
		if (bIsRadioButton)
			nDlgCode |= DLGC_RADIOBUTTON;
	}

	return nDlgCode;
}

LRESULT CButtonEx::OnBmGetcheck(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bIsReadOnly)
		return m_nCheck;
	else
		return Default();
}

LRESULT CButtonEx::OnBmSetcheck(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if (m_bIsReadOnly)
		InvalidateRect(NULL, FALSE);

	m_nCheck = wParam;
	return Default();
}

BOOL CButtonEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_bDoReadOnly = FALSE;
	m_bIsReadOnly = FALSE;

	return CButton::PreCreateWindow(cs);
}

void CButtonEx::PreSubclassWindow() 
{
	m_bDoReadOnly = FALSE;
	m_bIsReadOnly = FALSE;

	CButton::PreSubclassWindow();
}
