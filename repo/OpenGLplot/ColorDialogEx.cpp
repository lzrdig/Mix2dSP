/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2002
//
/////////////////////////////////////////////////////////////////////////////
// ColorDialogEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "ColorDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDialogEx

IMPLEMENT_DYNAMIC(CColorDialogEx, CColorDialog)

CColorDialogEx::CColorDialogEx(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
	m_bUseStrTitle = FALSE;
}

CColorDialogEx::CColorDialogEx(LPCTSTR szTitle, COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
	m_strTitle = szTitle;
	m_bUseStrTitle = TRUE;
}

CColorDialogEx::CColorDialogEx(int nID, COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
	if (m_strTitle.LoadString((UINT)nID))
		m_bUseStrTitle = TRUE;
	else
	{
		TRACE1("Warning: CColorDialogEx could not find a string for ID %d.\n", nID);
		m_bUseStrTitle = FALSE;
	}
}


BEGIN_MESSAGE_MAP(CColorDialogEx, CColorDialog)
	//{{AFX_MSG_MAP(CColorDialogEx)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CColorDialogEx::OnInitDialog() 
{
	BOOL bRet = CColorDialog::OnInitDialog();

	if (m_cc.lpTemplateName == MAKEINTRESOURCE(IDD_DLG_COLOR_BASIC))
		SetTimer(1,0,NULL);

	if (bRet && m_bUseStrTitle)
		SetWindowText(m_strTitle);

	return bRet;
}

void CColorDialogEx::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 1:
		GetDlgItem(65535)->SetFocus();
		KillTimer(1);
		break;
	default:
		{}
	}

	CColorDialog::OnTimer(nIDEvent);
}
