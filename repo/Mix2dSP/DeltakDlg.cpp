// DeltakDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "DeltakDlg.h"
#include "MixView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USERAPPLY WM_USER + 5

/////////////////////////////////////////////////////////////////////////////
// CDeltakDlg dialog


CDeltakDlg::CDeltakDlg(CWnd* pParent)
	: CDialog(CDeltakDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeltakDlg)
	m_dkInit = 0.0;
	m_dkFinal = 0.0;
	m_Ndk = 0;
	//}}AFX_DATA_INIT
}


void CDeltakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeltakDlg)
	DDX_Text(pDX, IDC_DKINIT, m_dkInit);
	DDX_Text(pDX, IDC_DKFINAL, m_dkFinal);
	DDX_Text(pDX, IDC_NDK, m_Ndk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeltakDlg, CDialog)
	//{{AFX_MSG_MAP(CDeltakDlg)
	ON_BN_CLICKED(IDOK, OnDeltakUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeltakDlg message handlers

void CDeltakDlg::OnDeltakUpdate() 
{
	UpdateData();
	g_pView->UpdateData();
	g_pView->m_Ndk			= m_Ndk;
	g_pView->m_dkFinal		= m_dkFinal;
	g_pView->m_dkInitial	= m_dkInit;
	g_pView->m_bDeltakDlgOn = TRUE;
	g_pView->SendMessage(WM_USERAPPLY);
	m_bDeltakOn = TRUE;
}
