// ErgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "ErgDlg.h"
#include "MixView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USERAPPLY WM_USER + 5
/////////////////////////////////////////////////////////////////////////////
// CErgDlg dialog


CErgDlg::CErgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CErgDlg)
	m_dEidle_f = 0.0;
	m_dEidle_i = 0.0;
	m_dEpump_f = 0.0;
	m_dEpump_i = 0.0;
	m_dEsig_f = 0.0;
	m_dEsig_i = 0.0;
	m_nNerg = 0;
	//}}AFX_DATA_INIT
}


void CErgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErgDlg)
	DDX_Text(pDX, IDC_ERGIDL_FINAL, m_dEidle_f);
	DDX_Text(pDX, IDC_ERGIDL_INIT, m_dEidle_i);
	DDX_Text(pDX, IDC_ERGPUMP_FINAL, m_dEpump_f);
	DDX_Text(pDX, IDC_ERGPUMP_INIT, m_dEpump_i);
	DDX_Text(pDX, IDC_ERGSIG_FINAL, m_dEsig_f);
	DDX_Text(pDX, IDC_ERGSIG_INIT, m_dEsig_i);
	DDX_Text(pDX, IDC_NERG, m_nNerg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErgDlg, CDialog)
	//{{AFX_MSG_MAP(CErgDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnErgUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErgDlg message handlers

HBRUSH CErgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
//***************************************************************************
// Set the text colors for Signal,Idler, and Pump in the dialog box

	if(pWnd->GetDlgCtrlID() == IDC_ERGLBL_S)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	if(pWnd->GetDlgCtrlID() == IDC_ERGLBL_I)
	{
		pDC->SetTextColor(RGB(0,0,255));
	}
	if(pWnd->GetDlgCtrlID() == IDC_ERGLBL_P)
	{
		pDC->SetTextColor(RGB(0,100,0));
	}

	return hbr;
}

void CErgDlg::OnErgUpdate() 
{
	UpdateData();
	g_pView->UpdateData();
	g_pView->m_Nerg		= m_nNerg;
	g_pView->m_Esig_i	= m_dEsig_i;
	g_pView->m_Esig_f	= m_dEsig_f;
	g_pView->m_Eidle_i	= m_dEidle_i;
	g_pView->m_Eidle_f	= m_dEidle_f;
	g_pView->m_Epump_i	= m_dEpump_i;
	g_pView->m_Epump_f	= m_dEpump_f;

	(g_pView->m_energy_s).Format("%7.3e", m_dEsig_i);
	(g_pView->m_energy_i).Format("%7.3e", m_dEidle_i);
	(g_pView->m_energy_p).Format("%7.3e", m_dEpump_i);

	g_pView->m_bErgDlgOn = TRUE;
	m_bErgOn = TRUE;
	g_pView->SendMessage(WM_USERAPPLY);
}
