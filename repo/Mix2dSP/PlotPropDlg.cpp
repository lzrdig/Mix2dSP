// PlotPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "PlotPropDlg.h"


// CPlotPropDlg dialog

IMPLEMENT_DYNAMIC(CPlotPropDlg, CDialog)

CPlotPropDlg::CPlotPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotPropDlg::IDD, pParent)
{

}

CPlotPropDlg::~CPlotPropDlg()
{
}

void CPlotPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTabExample1Dlg)
	DDX_Control(pDX, IDC_PLOTTABS, m_tbCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotPropDlg, CDialog)
END_MESSAGE_MAP()


// CPlotPropDlg message handlers

BOOL CPlotPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tbCtrl.InitDialogs();

	m_tbCtrl.InsertItem(0,"Frame");
	m_tbCtrl.InsertItem(1,"OpenGL");

	m_tbCtrl.ActivateTabDialogs();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
