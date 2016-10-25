// LensDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "MixView.h"
#include "LensDlg.h"
#include "Mix_Prog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

# define WM_USERAPPLY WM_USER + 5

extern bool m_bLensScanOn;

/////////////////////////////////////////////////////////////////////////////
// CLensDlg dialog


CLensDlg::CLensDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLensDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLensDlg)
	m_fBeamSize = 0.0f;
	m_fDistance = 0.0f;
	m_fFocus = 0.0f;
	m_fDistance_f = 0.0f;
	m_nNdist = 0;
	//}}AFX_DATA_INIT
}


void CLensDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLensDlg)
	DDX_Text(pDX, IDC_BEAMSIZE, m_fBeamSize);
	DDX_Text(pDX, IDC_DISTANCE, m_fDistance);
	DDX_Text(pDX, IDC_FOCUS, m_fFocus);
	DDX_Text(pDX, IDC_DISTANCE_F, m_fDistance_f);
	DDX_Text(pDX, IDC_NDIST, m_nNdist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLensDlg, CDialog)
	//{{AFX_MSG_MAP(CLensDlg)
	ON_BN_CLICKED(IDOK, OnLensOpt)
	ON_BN_CLICKED(IDC_LENSSCAN, OnLensUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLensDlg message handlers

void CLensDlg::OnLensOpt() 
{
	// TODO: Add your control notification handler code here

	double index, wvl, focus, dis, beamsize;
	double beamdiam, radcurv;
	complex<double> Qd;
	complex<double> invQd(1,0);
	
	UpdateData(TRUE);
	g_pView->UpdateData();

	index	= 1;									// refraction index of the air
	wvl		= atof(g_pView->m_wavelength_s)*1.0e-9;	// pump wavelength
	focus	= m_fFocus*1.0e-3;						// focal length
	beamsize= m_fBeamSize*1.0e-3/sqrt(2*(double)log(double(2)));
	dis	= m_fDistance*1.0e-3;					//distance from lens to crystal

	UpdateData(FALSE);

	if((focus == 0) || (beamsize == 0) || (dis == 0))
	{
		SetDlgItemText(IDC_STATIC_CAP, "Set all the parameters to non-zero values");
	}
	else
	{
		GaussianAfterLens(index, wvl, dis, beamsize, focus, &beamdiam, &radcurv);
		
		CString str;

		str.Format("%5.3e",beamdiam*1.0e3);
		g_pView->m_beamdiam_s = str;
		g_pView->m_beamdiam_i = str;
		g_pView->m_beamdiam_p = str;
		
		str.Format("%5.3e",radcurv*1.0e3);
		g_pView->m_radcurv_s = str;		
		g_pView->m_radcurv_i = str;
		g_pView->m_radcurv_p = str;
		
		m_bLensOn = TRUE;
		g_pView->m_bLensDlgOn = TRUE;		
		if(m_bLensScanOn == TRUE)
		{
			double dzStep;
			double* pBeamDiamArray;
			double* pRadCurvArray;

			if(m_bLensScanOn == TRUE)
			{
				pBeamDiamArray = new double[m_nNdist];
				pRadCurvArray = new double[m_nNdist];

				int nScanIndex = 0;

				dzStep = (m_fDistance_f - m_fDistance)*1.0e-3/m_nNdist;
				if((pBeamDiamArray == NULL) || (pRadCurvArray == NULL))
				{
					TRACE("\nFailed allocation\n");
					g_pView->m_Ndist = 0;
				}
				else
				{
					g_pView->m_Ndist = m_nNdist;
					while(nScanIndex<=m_nNdist)
					{
						dis = m_fDistance*1.0e-3 + dzStep*nScanIndex;
						GaussianAfterLens(index, wvl, dis, beamsize, focus, 
							&pBeamDiamArray[nScanIndex], &pRadCurvArray[nScanIndex]);
						nScanIndex++;
					}
					g_pView->m_pBeamDiamArray = pBeamDiamArray;
					g_pView->m_pRadCurvArray = pRadCurvArray;
				}
			}
		}
		g_pView->SendMessage(WM_USERAPPLY);
	}
}


void CLensDlg::OnLensUpdate() 
{
	// TODO: Add your control notification handler code here
	int state = IsDlgButtonChecked(IDC_LENSSCAN);

#define ON  1
#define OFF 0

	CWnd* pDistLbl_i = GetDlgItem(IDC_STATIC_DIST_F);
	CWnd* pDist_f = GetDlgItem(IDC_DISTANCE_F);
	CWnd* pNDistLbl = GetDlgItem(IDC_STATIC_NDIST);
	CWnd* pNDist = GetDlgItem(IDC_NDIST);
	if(state == OFF)
	{
		m_bLensScanOn = FALSE;
		CheckDlgButton(IDC_LENSSCAN,ON);
		pDistLbl_i->ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_STATIC_DIST_I,"Initial value of d (mm)");
		pDist_f->ShowWindow(SW_SHOW);
		pNDistLbl->ShowWindow(SW_SHOW);
		pNDist->ShowWindow(SW_SHOW);
		m_bLensScanOn = TRUE;
	}
	else
	{
		m_bLensScanOn = TRUE;
		CheckDlgButton(IDC_LENSSCAN,OFF);
		pDistLbl_i->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC_DIST_I,"Distance d between lens and crystal (mm)");
		pDist_f->ShowWindow(SW_HIDE);
		pNDistLbl->ShowWindow(SW_HIDE);
		pNDist->ShowWindow(SW_HIDE);
		m_bLensScanOn = FALSE;
	}
}
