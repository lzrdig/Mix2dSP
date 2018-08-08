/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// ClientFrame.cpp : Implementierungsdatei
//

#include "StdAfx.h"
#include "ClientFrm.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ID_TOOLTIP
#define ID_TOOLTIP 1
#endif

#pragma warning(disable: 4457)

/////////////////////////////////////////////////////////////////////////////
// CClientFrame

IMPLEMENT_DYNCREATE(CClientFrame, CWnd)

BEGIN_MESSAGE_MAP(CClientFrame, CWnd)
	//{{AFX_MSG_MAP(CClientFrame)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CClientFrame::CClientFrame()
{
	m_bTile = FALSE;
	m_pTopLeft = NULL;
	m_OldRect = CRect(0,0,0,0);
	m_bLButtonDownInBitmap = FALSE;
}

CClientFrame::~CClientFrame()
{
}

void CClientFrame::DrawTheBackground(CDC *pDC)
{
	if (pDC->IsPrinting())
		return;

	CRect rect;
	CPalette* pOldPalette = NULL;

	// Select and realize the palette
	if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE && m_Palette.m_hObject != NULL)
	{
		pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
		pDC->RealizePalette();
	}
	GetClientRect(&rect);
	pDC->DPtoLP(rect);

	CMemDC dc(pDC, rect);
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(pDC))
		return;

	BITMAP bm;
	m_Bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_Bitmap);
	if (m_bTile)
	{
		for (int i = ((int)floor((double)rect.left/bm.bmWidth))*bm.bmWidth; i <= rect.right; i += bm.bmWidth)
			for (int j = ((int)floor((double)rect.top/bm.bmHeight))*bm.bmHeight; j <= rect.bottom; j += bm.bmHeight)
				dc.BitBlt(i, j, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	}
	else
	{
		CRect& oldRect = m_OldRect;
		CBrush brush;
		brush.CreateSolidBrush(GetSysColor(COLOR_APPWORKSPACE));
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
		CRect cRect;
		int left, top;
		if (m_pTopLeft != NULL)
		{
			left = m_pTopLeft->x;
			top = m_pTopLeft->y;
		}
		else
		{
			left = (rect.Width()-bm.bmWidth)/2;
			top = (rect.Height()-bm.bmHeight)/2;
		}
		oldRect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		pDC->GetClipBox(&cRect);
		dc.PatBlt(cRect.left, cRect.top, cRect.Width(), cRect.Height(), PATCOPY);
		if (pOldBrush)
			dc.SelectObject(pOldBrush);
		dc.BitBlt(oldRect.left, oldRect.top, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	}
	if (pOldBitmap)
		dcImage.SelectObject(pOldBitmap);
	if (pOldPalette)
		pDC->SelectPalette(pOldPalette, FALSE);
	pDC->RealizePalette();
}

BOOL CClientFrame::GetBitmapAndPalette(UINT nIDResource)
{
	LPCTSTR lpszResourceName = (LPCTSTR)nIDResource;
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), 
		lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION);
	if (hBmp == NULL)
		return FALSE;

	m_Bitmap.Attach(hBmp);

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	m_Bitmap.GetObject(sizeof(ds), &ds);
	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL); // Desktop DC
	if (nColors > 256)
		m_Palette.CreateHalftonePalette(&dc);
	else
	{
		// Create the palette
		RGBQUAD *pRGB = new RGBQUAD[nColors];

		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(&m_Bitmap);
		::GetDIBColorTable(memDC, 0, nColors, pRGB);
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = (WORD)nColors;
		for (int i = 0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}
		m_Palette.CreatePalette(pLP);
		delete[] pLP;
		delete[] pRGB;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CClientFrame

BOOL CClientFrame::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);

	return TRUE;
}

void CClientFrame::OnPaint() 
{
	// Do not call CWnd::OnPaint() for painting messages
	InvalidateRect(&m_OldRect);
	CRect newRect;
	GetClientRect(&newRect);

	if (!m_bTile)
	{
		int left, top;
		BITMAP bm;
		m_Bitmap.GetBitmap(&bm);
		if (m_pTopLeft != NULL)
		{
			left = m_pTopLeft->x;
			top = m_pTopLeft->y;
		}
		else
		{
			left = (newRect.Width()-bm.bmWidth)/2;
			top = (newRect.Height()-bm.bmHeight)/2;
		}
		newRect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
	}
	InvalidateRect(&newRect);

	CPaintDC dc(this); // device context for painting
	DrawTheBackground(&dc);
}

BOOL CClientFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		CPoint point;
		CRect rect;
		GetClientRect(&rect);

		if (!m_bTile)
		{
			int left, top;
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			if (m_pTopLeft != NULL)
			{
				left = m_pTopLeft->x;
				top = m_pTopLeft->y;
			}
			else
			{
				left = (rect.Width()-bm.bmWidth)/2;
				top = (rect.Height()-bm.bmHeight)/2;
			}
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		GetCursorPos(&point);
		BOOL bIsOverlapped = (WindowFromPoint(point) != this);
		ScreenToClient(&point);

		if (rect.PtInRect(point) && !bIsOverlapped)
		{
			if (m_HyperLink.GetLinkCursor())
			{
				::SetCursor(m_HyperLink.GetLinkCursor());
				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CClientFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		m_HyperLink.GetToolTipCtrl().RelayEvent(pMsg);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CClientFrame::PreSubclassWindow() 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		m_HyperLink.SetDefaultCursor();		// Try and load up a "hand" cursor

		// Create the tooltip
		CRect rect;
		GetClientRect(&rect);
		if (!m_bTile && m_pTopLeft != NULL)
		{
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			long& left = m_pTopLeft->x;
			long& top = m_pTopLeft->y;
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		m_HyperLink.GetToolTipCtrl().Create(this);
		m_HyperLink.GetToolTipCtrl().AddTool(this, m_HyperLink.GetURL(), rect, ID_TOOLTIP);
	}

	CWnd::PreSubclassWindow();
}

void CClientFrame::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (!m_HyperLink.GetURL().IsEmpty())
	{

		CRect rect;
		GetClientRect(&rect);
		if (m_bTile)
		{
			m_HyperLink.GetToolTipCtrl().SetToolRect(this, ID_TOOLTIP, rect);
		}
		else if (m_pTopLeft == NULL)
		{
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			int left = (rect.Width()-bm.bmWidth)/2;
			int top = (rect.Height()-bm.bmHeight)/2;
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
			m_HyperLink.GetToolTipCtrl().SetToolRect(this, ID_TOOLTIP, rect);
		}
	}
}

void CClientFrame::SetURL(CString strURL)
{
	m_HyperLink.SetURL(strURL);
}

void CClientFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		CPoint point;
		CRect rect;
		GetClientRect(&rect);

		if (!m_bTile)
		{
			int left, top;
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			if (m_pTopLeft != NULL)
			{
				left = m_pTopLeft->x;
				top = m_pTopLeft->y;
			}
			else
			{
				left = (rect.Width()-bm.bmWidth)/2;
				top = (rect.Height()-bm.bmHeight)/2;
			}
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		GetCursorPos(&point);
		ScreenToClient(&point);

		if (rect.PtInRect(point))
		{
			m_bLButtonDownInBitmap = TRUE;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CClientFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		CPoint point;
		CRect rect;
		GetClientRect(&rect);

		if (!m_bTile)
		{
			int left, top;
			BITMAP bm;
			m_Bitmap.GetBitmap(&bm);
			if (m_pTopLeft != NULL)
			{
				left = m_pTopLeft->x;
				top = m_pTopLeft->y;
			}
			else
			{
				left = (rect.Width()-bm.bmWidth)/2;
				top = (rect.Height()-bm.bmHeight)/2;
			}
			rect = CRect(left, top, left + bm.bmWidth, top + bm.bmHeight);
		}
		GetCursorPos(&point);
		ScreenToClient(&point);

		if (rect.PtInRect(point) && m_bLButtonDownInBitmap)
		{
			m_HyperLink.Clicked();
		}
		m_bLButtonDownInBitmap = FALSE;
	}

	CWnd::OnLButtonUp(nFlags, point);
}
