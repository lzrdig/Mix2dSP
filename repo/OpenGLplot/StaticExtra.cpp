/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 2001
//
/////////////////////////////////////////////////////////////////////////////
// StaticExtra.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "StaticExtra.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticExtra

CStaticExtra::CStaticExtra()
{
	m_bFocusChangedFlag = FALSE;
	m_hBrush = NULL;
}

CStaticExtra::~CStaticExtra()
{
	if (m_hBrush)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}
}


BEGIN_MESSAGE_MAP(CStaticExtra, CStatic)
	//{{AFX_MSG_MAP(CStaticExtra)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CStaticExtra 

void CStaticExtra::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);

	CWnd* pParent = GetParent();
	if (pParent)
	{
		m_bFocusChangedFlag = TRUE;

		CRect rect;
		GetWindowRect(&rect);
		rect.InflateRect(2, 2, 2, 2);
		pParent->ScreenToClient(&rect);
		pParent->InvalidateRect(&rect, FALSE);
		pParent->UpdateWindow();
	}
}

void CStaticExtra::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);

	CWnd* pParent = GetParent();
	if (pParent)
	{
		m_bFocusChangedFlag = TRUE;

		CRect rect;
		GetWindowRect(&rect);
		rect.InflateRect(2, 2, 2, 2);
		pParent->ScreenToClient(&rect);
		pParent->InvalidateRect(&rect, FALSE);
		pParent->UpdateWindow();
	}
}

BOOL CStaticExtra::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		CWnd* pParent = GetParent();
		if (pParent)
		{
			CRect rect;
			GetWindowRect(&rect);
			pParent->ScreenToClient(&rect);
			CPoint point = rect.CenterPoint();
			pParent->SendMessage(WM_COMMAND, GetDlgCtrlID());

			return TRUE;
		}
	}
	return CStatic::PreTranslateMessage(pMsg);
}

HBRUSH CStaticExtra::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(nCtlColor);

	return m_hBrush;
}
