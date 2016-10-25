// PropSht2.cpp : implementation file
//

#include "stdafx.h"
#include "GlView.h"
#include "mix2dsp.h"
#include "PropSht2.h"
#include "DlgProperties.h"
#include "Plot3dView.h"
#include "GraphDlg.h"


// CPropSht2 dialog

IMPLEMENT_DYNAMIC(CPropSht2, CDialog)

CPropSht2::CPropSht2(CWnd* pParent /*=NULL*/)
	: CDialog(CPropSht2::IDD, pParent)
{

}

CPropSht2::~CPropSht2()
{
}

void CPropSht2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropSht2, CDialog)
	ON_BN_CLICKED(IDC_RADIO_MODEL2, &CPropSht2::OnRadioModel2)
	ON_BN_CLICKED(IDC_RADIO_MODEL1, &CPropSht2::OnRadioModel1)
	ON_BN_CLICKED(IDC_RADIO_MODEL0, &CPropSht2::OnRadioModel0)
END_MESSAGE_MAP()


// CPropSht2 message handlers

void CPropSht2::OnRadioModel2()
{
	// TODO: Add your control notification handler code here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CWnd *pParent = GetParent()->GetParent();
	CPlot3dView *pView = (CPlot3dView *)(((CGraphDlg*)pParent)->m_pPlot3DView);

	pApp->m_nPolygonMode = 2;
	pView->m_bBlackLinesOrRemoveHidden  = false;

	//RadioModel();
}

void CPropSht2::RadioModel() 
{
	//CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CWnd *pParent; 
	CPlot3dView *pView;

	pParent = GetParent()->GetParent();
	pView = ((CGraphDlg*)pParent)->m_pPlot3DView;

	//pView->MakeActive();

	pView->m_nKindOfView = KOV_SURF;
	pView->m_bBlackLinesOrRemoveHidden = false;
	pView->InvalidateRect(NULL, FALSE);
}

void CPropSht2::OnRadioModel1()
{
	// TODO: Add your control notification handler code here
}

void CPropSht2::OnRadioModel0()
{
	// TODO: Add your control notification handler code here
}

