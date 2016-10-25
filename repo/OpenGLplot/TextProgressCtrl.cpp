/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: March 1999
//
// based on a sample of
// Chris Maunder <chrismaunder@codeguru.com>
//
/////////////////////////////////////////////////////////////////////////////
// TextProgressCtrl.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "MemDC.h"
#include "TextProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

CTextProgressCtrl::CTextProgressCtrl()
{
	m_nPos				= 0;
	m_bStepDirForward	= TRUE;
	m_nStepSize			= 1;
	m_nMax				= 100;
	m_nMin				= 0;
	m_bShowText			= TRUE;
	m_bShowHand			= FALSE;
	m_dHandWidth		= 0.1;
	m_strText.Empty();
	HighlightColors();

	m_nBarWidth = -1;
}

CTextProgressCtrl::~CTextProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CTextProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f�r Nachrichten CTextProgressCtrl

LRESULT CTextProgressCtrl::OnSetText(WPARAM, LPARAM lParam)
{
	LPCTSTR szText = (LPCTSTR) lParam;
	LRESULT result = Default();

	if ((!szText && !m_strText.IsEmpty()) ||
		(szText && (m_strText != szText)))
	{
		m_strText = szText;
		Invalidate();
	}

	return result;
}

LRESULT CTextProgressCtrl::OnGetText(WPARAM wParam, LPARAM lParam)
{
	UINT cchTextMax = (UINT) wParam;
	LPTSTR szText = (LPTSTR) lParam;

	if (!_tcsncpy(szText, m_strText, cchTextMax))
		return 0;
	else
		return min(cchTextMax, (UINT) m_strText.GetLength());
}

BOOL CTextProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CTextProgressCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CProgressCtrl::OnSize(nType, cx, cy);

	m_nBarWidth	= -1;   // Force update if SetPos called
}

void CTextProgressCtrl::OnPaint() 
{
	if (m_nMin >= m_nMax)
		return;

	CRect leftRect, rightRect, clientRect;
	GetClientRect(&clientRect);

	double Fraction = ((double)(m_nPos - m_nMin)) / ((double)(m_nMax - m_nMin));

	CPaintDC PaintDC(this); // device context for painting
	CMemDC dc(&PaintDC);
	//CPaintDC dc(this);	// device context for painting (if not double buffering)
	CFont* pFont;
	CFont* pOldFont = NULL;
	CWnd* pParent = GetParent();
	if (pParent && (pFont = pParent->GetFont()) != NULL)
		pOldFont = dc.SelectObject(pFont);

	leftRect = rightRect = clientRect;

	if (m_bShowHand)
	{
		double tmpNum = (clientRect.right - clientRect.left)*(1. - 2.*m_dHandWidth)*Fraction;
		leftRect.left = (int)(round(tmpNum)) + clientRect.left;
		leftRect.right = leftRect.left + (int)ceil((clientRect.right - clientRect.left)*2.*m_dHandWidth);

		dc.FillSolidRect(rightRect, m_colBk);
		dc.FillSolidRect(leftRect, m_colFore);
	}
	else
	{
		leftRect.right = rightRect.left = clientRect.left + (int)((clientRect.right - clientRect.left)*Fraction);

		dc.FillSolidRect(rightRect, m_colBk);
		dc.FillSolidRect(leftRect, m_colFore);
	}

	if (m_bShowText)
	{
		CRgn rgn;
		CString str;
		if (!m_strText.IsEmpty())
			str = m_strText;
		else
			str.Format("%d%%", (int)(Fraction*100.0));

		dc.SetBkMode(TRANSPARENT);

		rgn.CreateRectRgn(rightRect.left, rightRect.top, rightRect.right, rightRect.bottom);
		dc.SelectClipRgn(&rgn);
		dc.SetTextColor(m_colTextFore);
		rgn.DeleteObject();

		dc.DrawText(str, clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		rgn.CreateRectRgn(leftRect.left, leftRect.top, leftRect.right, leftRect.bottom);
		dc.SelectClipRgn(&rgn);
		dc.SetTextColor(m_colTextBk);
		rgn.DeleteObject();

		dc.DrawText(str, clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (pOldFont)
		 dc.SelectObject(pOldFont);
}

void CTextProgressCtrl::OnSysColorChange() 
{
	if (m_nStatColFore == CR_HIGHLIGHT)
		m_colFore = GetSysColor(COLOR_HIGHLIGHT);
	else if (m_nStatColFore == CR_DEFAULT)
		m_colFore = GetSysColor(COLOR_3DFACE);

	if (m_nStatColBk == CR_HIGHLIGHT)
		m_colBk = GetSysColor(COLOR_WINDOW);
	else if (m_nStatColBk == CR_DEFAULT)
		m_colBk = GetSysColor(COLOR_3DFACE);

	if (m_nStatColTextFore == CR_HIGHLIGHT)
		m_colTextFore = GetSysColor(COLOR_HIGHLIGHT);

	if (m_nStatColTextBk == CR_HIGHLIGHT)
		m_colTextBk = GetSysColor(COLOR_WINDOW);

	CProgressCtrl::OnSysColorChange();
}

void CTextProgressCtrl::SetForeColor(COLORREF col)
{
	m_colFore = col;
	m_nStatColFore = CR_USERDEFINED;
}

void CTextProgressCtrl::SetBkColor(COLORREF col)
{
	m_colBk = col;
	m_nStatColBk = CR_USERDEFINED;
}

void CTextProgressCtrl::SetTextForeColor(COLORREF col)
{
	m_colTextFore = col;
	m_nStatColTextFore = CR_USERDEFINED;
}

void CTextProgressCtrl::SetTextBkColor(COLORREF col)
{
	m_colTextBk = col;
	m_nStatColTextBk = CR_USERDEFINED;
}

COLORREF CTextProgressCtrl::GetForeColor()
{
	return m_colFore;
}

COLORREF CTextProgressCtrl::GetBkColor()
{
	return m_colBk;
}

COLORREF CTextProgressCtrl::GetTextForeColor()
{
	return m_colTextFore;
}

COLORREF CTextProgressCtrl::GetTextBkColor()
{
	return m_colTextBk;
}

void CTextProgressCtrl::DefaultColors()
{
	m_colFore		= GetSysColor(COLOR_3DFACE);
	m_colBk			= GetSysColor(COLOR_3DFACE);
	m_colTextFore	= RGB(0, 0, 0);
	m_colTextBk		= RGB(0, 0, 0);

	m_nStatColFore = m_nStatColBk = m_nStatColTextFore = m_nStatColTextBk = CR_DEFAULT;
}

void CTextProgressCtrl::HighlightColors()
{
	m_colFore		= GetSysColor(COLOR_HIGHLIGHT);
	m_colBk			= GetSysColor(COLOR_WINDOW);
	m_colTextFore	= GetSysColor(COLOR_HIGHLIGHT);
	m_colTextBk		= GetSysColor(COLOR_WINDOW);

	m_nStatColFore = m_nStatColBk = m_nStatColTextFore = m_nStatColTextBk = CR_HIGHLIGHT;
}

BOOL CTextProgressCtrl::SetShowText(BOOL bShow)
{ 
	BOOL bOldShowText = m_bShowText;
	m_bShowText = bShow;

	if (::IsWindow(m_hWnd) && m_bShowText != bOldShowText)
		Invalidate();

	return bOldShowText;
}

BOOL CTextProgressCtrl::SetShowHand(BOOL bShow)
{
	BOOL bOldShowHand = m_bShowHand;
	m_bShowHand = bShow;

	if (::IsWindow(m_hWnd) && m_bShowHand != bOldShowHand)
		Invalidate();

	return bOldShowHand;
}

void CTextProgressCtrl::SetRange(int nLower, int nUpper)
{
	m_nMax = nUpper;
	m_nMin = nLower;
}

int CTextProgressCtrl::SetPos(int nPos) 
{
	if (!::IsWindow(m_hWnd))
		return -1;

	int nOldPos = m_nPos;
	m_nPos = nPos;

	CRect rect;
	GetClientRect(&rect);

	double Fraction = ((double)(m_nPos - m_nMin)) / ((double)(m_nMax - m_nMin));
	int nBarWidth = (int) (Fraction * rect.Width());

	if (nBarWidth != m_nBarWidth)
	{
		m_nBarWidth = nBarWidth;
		RedrawWindow();
	}

	return nOldPos;
}

int CTextProgressCtrl::StepIt() 
{
	if (m_bShowHand)
	{
		if ((m_bStepDirForward && m_nPos+m_nStepSize > m_nMax) || 
			(!m_bStepDirForward && m_nPos-m_nStepSize < m_nMin))
			m_bStepDirForward = !m_bStepDirForward;

		if (m_bStepDirForward)
			return SetPos(m_nPos + m_nStepSize);
		else
			return SetPos(m_nPos - m_nStepSize);
	}
	else
	{
		return SetPos(m_nPos + m_nStepSize);
	}
}

int CTextProgressCtrl::OffsetPos(int nPos)
{
	return SetPos(m_nPos + nPos);
}

int CTextProgressCtrl::SetStep(int nStep)
{
	int nOldStep = m_nStepSize;
	m_nStepSize = nStep;
	return nOldStep;
}

double CTextProgressCtrl::SetHandWidth(double dHand)
{
	double dOldHandWidth = m_dHandWidth;
	m_dHandWidth = dHand;
	return dOldHandWidth;
}
