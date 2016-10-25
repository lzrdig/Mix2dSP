// PlotPropTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "PlotPropTabCtrl.h"
#include "PropSht1.h"
#include "PropSht2.h"


// CPlotPropTabCtrl dialog

IMPLEMENT_DYNAMIC(CPlotPropTabCtrl, CTabCtrl)

CPlotPropTabCtrl::CPlotPropTabCtrl()
{
	m_DialogIDs[0] =IDD_PLOTPROPSHT1;
	m_DialogIDs[1] =IDD_PLOTPROPSHT2;

	m_Dialogs[0] = new CPropSht1();
	m_Dialogs[1] = new CPropSht2();

	m_nPageCount = 2;
}

CPlotPropTabCtrl::~CPlotPropTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CPlotPropTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CPlotPropTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPlotPropTabCtrl message handlers
void CPlotPropTabCtrl::InitDialogs()
{
	CWnd* pParent;
	pParent = GetParent();
	m_Dialogs[0]->Create(m_DialogIDs[0],pParent);
	pParent = GetParent();
	m_Dialogs[1]->Create(m_DialogIDs[1],pParent);
}

void CPlotPropTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ActivateTabDialogs();
	*pResult = 0;
}

void CPlotPropTabCtrl::ActivateTabDialogs()
{

	int nSel = GetCurSel();
	if(m_Dialogs[nSel]->m_hWnd)
		m_Dialogs[nSel]->ShowWindow(SW_HIDE);

	CRect l_rectClient;
	CRect l_rectWnd;

	GetClientRect(l_rectClient);
	AdjustRect(FALSE,l_rectClient);
	GetWindowRect(l_rectWnd);
	GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
	for(int nCount=0; nCount < m_nPageCount; nCount++){
		m_Dialogs[nCount]->SetWindowPos(&wndTop, l_rectClient.left,l_rectClient.top,l_rectClient.Width(),l_rectClient.Height(),SWP_HIDEWINDOW);
	}
	m_Dialogs[nSel]->SetWindowPos(&wndTop,l_rectClient.left,l_rectClient.top,l_rectClient.Width(),l_rectClient.Height(),SWP_SHOWWINDOW);

	m_Dialogs[nSel]->ShowWindow(SW_SHOW);
}
