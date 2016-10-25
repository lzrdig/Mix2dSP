// MixView.cpp : implementation file
//

#include "stdafx.h"
#include "Mix2dsp.h"
#include "MixView.h"

#include "Mix_Prog.h"
#include "GraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4244)

#define WM_USERAPPLY WM_USER + 5
#define WM_USERRUNDK WM_USER + 6

extern bool m_bStop;
extern bool m_bLensScanOn;

/////////////////////////////////////////////////////////////////////////////
// CMixView

IMPLEMENT_DYNCREATE(CMixView, CFormView)

CMixView::CMixView()
	: CFormView(CMixView::IDD)
{
	//{{AFX_DATA_INIT(CMixView)
	m_alpha_i = _T("");
	m_alpha_p = _T("");
	m_alpha_s = _T("");
	m_beamdiam_i = _T("");
	m_beamdiam_p = _T("");
	m_beamdiam_s = _T("");
	m_crystlength = _T("");
	m_deff = _T("");
	m_delay_i = _T("");
	m_delay_s = _T("");
	m_deltak = _T("");
	m_dist = _T("");
	m_duration_i = _T("");
	m_duration_p = _T("");
	m_duration_s = _T("");
	m_energy_i = _T("");
	m_energy_p = _T("");
	m_energy_s = _T("");
	m_gauss_i = _T("");
	m_gauss_p = _T("");
	m_gauss_s = _T("");
	m_gvd_i = _T("");
	m_gvd_p = _T("");
	m_gvd_s = _T("");
	m_gvi_i = _T("");
	m_gvi_p = _T("");
	m_gvi_s = _T("");
	m_index_i = _T("");
	m_index_p = _T("");
	m_index_s = _T("");
	m_offset_i = _T("");
	m_offset_s = _T("");
	m_phase_i = _T("");
	m_phase_p = _T("");
	m_phase_s = _T("");
	m_radcurv_i = _T("");
	m_radcurv_p = _T("");
	m_radcurv_s = _T("");
	m_rin_i = _T("");
	m_rin_p = _T("");
	m_rin_s = _T("");
	m_rout_i = _T("");
	m_rout_p = _T("");
	m_rout_s = _T("");
	m_tpoints = _T("");
	m_walkoff_i = _T("");
	m_walkoff_p = _T("");
	m_walkoff_s = _T("");
	m_wavelength_i = _T("");
	m_wavelength_p = _T("");
	m_wavelength_s = _T("");
	m_xgrid = _T("");
	m_xpoints = _T("");
	m_ygrid = _T("");
	m_ypoints = _T("");
	m_zstep = _T("");
	m_beta_i1 = _T("");
	m_beta_i2 = _T("");
	m_beta_i3 = _T("");
	m_beta_p1 = _T("");
	m_beta_p2 = _T("");
	m_beta_p3 = _T("");
	m_beta_s1 = _T("");
	m_beta_s2 = _T("");
	m_beta_s3 = _T("");
	m_n2_i1 = _T("");
	m_n2_i2 = _T("");
	m_n2_i3 = _T("");
	m_n2_p1 = _T("");
	m_n2_p2 = _T("");
	m_n2_p3 = _T("");
	m_n2_s1 = _T("");
	m_n2_s2 = _T("");
	m_n2_s3 = _T("");
	//}}AFX_DATA_INIT

	m_error = false;
	m_bFirstTime = true;
	m_bInputsChanged = false;
	g_pView = this;	
}

CMixView::~CMixView()
{
//	delete pGraphDlg;
}

void CMixView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMixView)
	DDX_Text(pDX, IDC_ALPHA_I, m_alpha_i);
	DDX_Text(pDX, IDC_ALPHA_P, m_alpha_p);
	DDX_Text(pDX, IDC_ALPHA_S, m_alpha_s);
	DDX_Text(pDX, IDC_BEAMDIAM_I, m_beamdiam_i);
	DDX_Text(pDX, IDC_BEAMDIAM_P, m_beamdiam_p);
	DDX_Text(pDX, IDC_BEAMDIAM_S, m_beamdiam_s);
	DDX_Text(pDX, IDC_CRYSTLENGTH, m_crystlength);
	DDX_Text(pDX, IDC_DEFF, m_deff);
	DDX_Text(pDX, IDC_DELAY_I, m_delay_i);
	DDX_Text(pDX, IDC_DELAY_S, m_delay_s);
	DDX_Text(pDX, IDC_DELTAK, m_deltak);
	DDX_Text(pDX, IDC_DIST, m_dist);
	DDX_Text(pDX, IDC_DURATION_I, m_duration_i);
	DDX_Text(pDX, IDC_DURATION_P, m_duration_p);
	DDX_Text(pDX, IDC_DURATION_S, m_duration_s);
	DDX_Text(pDX, IDC_ENERGY_I, m_energy_i);
	DDX_Text(pDX, IDC_ENERGY_P, m_energy_p);
	DDX_Text(pDX, IDC_ENERGY_S, m_energy_s);
	DDX_Text(pDX, IDC_GAUSS_I, m_gauss_i);
	DDX_Text(pDX, IDC_GAUSS_P, m_gauss_p);
	DDX_Text(pDX, IDC_GAUSS_S, m_gauss_s);
	DDX_Text(pDX, IDC_GVD_I, m_gvd_i);
	DDX_Text(pDX, IDC_GVD_P, m_gvd_p);
	DDX_Text(pDX, IDC_GVD_S, m_gvd_s);
	DDX_Text(pDX, IDC_GVI_I, m_gvi_i);
	DDX_Text(pDX, IDC_GVI_P, m_gvi_p);
	DDX_Text(pDX, IDC_GVI_S, m_gvi_s);
	DDX_Text(pDX, IDC_INDEX_I, m_index_i);
	DDX_Text(pDX, IDC_INDEX_P, m_index_p);
	DDX_Text(pDX, IDC_INDEX_S, m_index_s);
	DDX_Text(pDX, IDC_OFFSET_I, m_offset_i);
	DDX_Text(pDX, IDC_OFFSET_S, m_offset_s);
	DDX_Text(pDX, IDC_PHASE_I, m_phase_i);
	DDX_Text(pDX, IDC_PHASE_P, m_phase_p);
	DDX_Text(pDX, IDC_PHASE_S, m_phase_s);
	DDX_Text(pDX, IDC_RADCURV_I, m_radcurv_i);
	DDX_Text(pDX, IDC_RADCURV_P, m_radcurv_p);
	DDX_Text(pDX, IDC_RADCURV_S, m_radcurv_s);
	DDX_Text(pDX, IDC_RIN_I, m_rin_i);
	DDX_Text(pDX, IDC_RIN_P, m_rin_p);
	DDX_Text(pDX, IDC_RIN_S, m_rin_s);
	DDX_Text(pDX, IDC_ROUT_I, m_rout_i);
	DDX_Text(pDX, IDC_ROUT_P, m_rout_p);
	DDX_Text(pDX, IDC_ROUT_S, m_rout_s);
	DDX_Text(pDX, IDC_TPOINTS, m_tpoints);
	DDX_Text(pDX, IDC_WALKOFF_I, m_walkoff_i);
	DDX_Text(pDX, IDC_WALKOFF_P, m_walkoff_p);
	DDX_Text(pDX, IDC_WALKOFF_S, m_walkoff_s);
	DDX_Text(pDX, IDC_WAVELENGTH_I, m_wavelength_i);
	DDX_Text(pDX, IDC_WAVELENGTH_P, m_wavelength_p);
	DDX_Text(pDX, IDC_WAVELENGTH_S, m_wavelength_s);
	DDX_Text(pDX, IDC_XGRID, m_xgrid);
	DDX_Text(pDX, IDC_XPOINTS, m_xpoints);
	DDX_Text(pDX, IDC_YGRID, m_ygrid);
	DDX_Text(pDX, IDC_YPOINTS, m_ypoints);
	DDX_Text(pDX, IDC_ZSTEP, m_zstep);
	DDX_Text(pDX, IDC_BETA_I1, m_beta_i1);
	DDX_Text(pDX, IDC_BETA_I2, m_beta_i2);
	DDX_Text(pDX, IDC_BETA_I3, m_beta_i3);
	DDX_Text(pDX, IDC_BETA_P1, m_beta_p1);
	DDX_Text(pDX, IDC_BETA_P2, m_beta_p2);
	DDX_Text(pDX, IDC_BETA_P3, m_beta_p3);
	DDX_Text(pDX, IDC_BETA_S1, m_beta_s1);
	DDX_Text(pDX, IDC_BETA_S2, m_beta_s2);
	DDX_Text(pDX, IDC_BETA_S3, m_beta_s3);
	DDX_Text(pDX, IDC_N2_I1, m_n2_i1);
	DDX_Text(pDX, IDC_N2_I2, m_n2_i2);
	DDX_Text(pDX, IDC_N2_I3, m_n2_i3);
	DDX_Text(pDX, IDC_N2_P1, m_n2_p1);
	DDX_Text(pDX, IDC_N2_P2, m_n2_p2);
	DDX_Text(pDX, IDC_N2_P3, m_n2_p3);
	DDX_Text(pDX, IDC_N2_S1, m_n2_s1);
	DDX_Text(pDX, IDC_N2_S2, m_n2_s2);
	DDX_Text(pDX, IDC_N2_S3, m_n2_s3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMixView, CFormView)
	//{{AFX_MSG_MAP(CMixView)
	ON_BN_CLICKED(IDC_ACCEPT, OnAccept)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_ALPHA_I, OnSetfocusAlphaI)
	ON_EN_SETFOCUS(IDC_ALPHA_P, OnSetfocusAlphaP)
	ON_EN_SETFOCUS(IDC_ALPHA_S, OnSetfocusAlphaS)
	ON_EN_SETFOCUS(IDC_BEAMDIAM_I, OnSetfocusBeamdiamI)
	ON_EN_SETFOCUS(IDC_BEAMDIAM_P, OnSetfocusBeamdiamP)
	ON_EN_SETFOCUS(IDC_BEAMDIAM_S, OnSetfocusBeamdiamS)
	ON_EN_SETFOCUS(IDC_CRYSTLENGTH, OnSetfocusCrystlength)
	ON_EN_SETFOCUS(IDC_DEFF, OnSetfocusDeff)
	ON_EN_SETFOCUS(IDC_DELAY_I, OnSetfocusDelayI)
	ON_EN_SETFOCUS(IDC_DELAY_S, OnSetfocusDelayS)
	ON_EN_SETFOCUS(IDC_DELTAK, OnSetfocusDeltak)
	ON_EN_SETFOCUS(IDC_DIST, OnSetfocusDist)
	ON_EN_SETFOCUS(IDC_DURATION_I, OnSetfocusDurationI)
	ON_EN_SETFOCUS(IDC_DURATION_P, OnSetfocusDurationP)
	ON_EN_SETFOCUS(IDC_DURATION_S, OnSetfocusDurationS)
	ON_EN_SETFOCUS(IDC_ENERGY_I, OnSetfocusEnergyI)
	ON_EN_SETFOCUS(IDC_ENERGY_P, OnSetfocusEnergyP)
	ON_EN_SETFOCUS(IDC_ENERGY_S, OnSetfocusEnergyS)
	ON_EN_SETFOCUS(IDC_GAUSS_I, OnSetfocusGaussI)
	ON_EN_SETFOCUS(IDC_GAUSS_P, OnSetfocusGaussP)
	ON_EN_SETFOCUS(IDC_GAUSS_S, OnSetfocusGaussS)
	ON_EN_SETFOCUS(IDC_GVD_I, OnSetfocusGvdI)
	ON_EN_SETFOCUS(IDC_GVD_P, OnSetfocusGvdP)
	ON_EN_SETFOCUS(IDC_GVD_S, OnSetfocusGvdS)
	ON_EN_SETFOCUS(IDC_GVI_I, OnSetfocusGviI)
	ON_EN_SETFOCUS(IDC_GVI_P, OnSetfocusGviP)
	ON_EN_SETFOCUS(IDC_GVI_S, OnSetfocusGviS)
	ON_EN_SETFOCUS(IDC_INDEX_I, OnSetfocusIndexI)
	ON_EN_SETFOCUS(IDC_INDEX_P, OnSetfocusIndexP)
	ON_EN_SETFOCUS(IDC_INDEX_S, OnSetfocusIndexS)
	ON_EN_SETFOCUS(IDC_OFFSET_I, OnSetfocusOffsetI)
	ON_EN_SETFOCUS(IDC_OFFSET_S, OnSetfocusOffsetS)
	ON_EN_SETFOCUS(IDC_PHASE_I, OnSetfocusPhaseI)
	ON_EN_SETFOCUS(IDC_PHASE_P, OnSetfocusPhaseP)
	ON_EN_SETFOCUS(IDC_PHASE_S, OnSetfocusPhaseS)
	ON_EN_SETFOCUS(IDC_RADCURV_I, OnSetfocusRadcurvI)
	ON_EN_SETFOCUS(IDC_RADCURV_P, OnSetfocusRadcurvP)
	ON_EN_SETFOCUS(IDC_RADCURV_S, OnSetfocusRadcurvS)
	ON_EN_SETFOCUS(IDC_RIN_I, OnSetfocusRinI)
	ON_EN_SETFOCUS(IDC_RIN_P, OnSetfocusRinP)
	ON_EN_SETFOCUS(IDC_RIN_S, OnSetfocusRinS)
	ON_EN_SETFOCUS(IDC_ROUT_I, OnSetfocusRoutI)
	ON_EN_SETFOCUS(IDC_ROUT_P, OnSetfocusRoutP)
	ON_EN_SETFOCUS(IDC_ROUT_S, OnSetfocusRoutS)
	ON_EN_SETFOCUS(IDC_TPOINTS, OnSetfocusTpoints)
	ON_EN_SETFOCUS(IDC_WALKOFF_I, OnSetfocusWalkoffI)
	ON_EN_SETFOCUS(IDC_WALKOFF_P, OnSetfocusWalkoffP)
	ON_EN_SETFOCUS(IDC_WALKOFF_S, OnSetfocusWalkoffS)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_I, OnSetfocusWavelengthI)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_P, OnSetfocusWavelengthP)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_S, OnSetfocusWavelengthS)
	ON_EN_SETFOCUS(IDC_XGRID, OnSetfocusXgrid)
	ON_EN_SETFOCUS(IDC_XPOINTS, OnSetfocusXpoints)
	ON_EN_SETFOCUS(IDC_YGRID, OnSetfocusYgrid)
	ON_EN_SETFOCUS(IDC_YPOINTS, OnSetfocusYpoints)
	ON_EN_SETFOCUS(IDC_ZSTEP, OnSetfocusZstep)
	ON_EN_SETFOCUS(IDC_BETA_I1, OnSetfocusBetaI1)
	ON_EN_SETFOCUS(IDC_BETA_I2, OnSetfocusBetaI2)
	ON_EN_SETFOCUS(IDC_BETA_I3, OnSetfocusBetaI3)
	ON_EN_SETFOCUS(IDC_BETA_P1, OnSetfocusBetaP1)
	ON_EN_SETFOCUS(IDC_BETA_P2, OnSetfocusBetaP2)
	ON_EN_SETFOCUS(IDC_BETA_P3, OnSetfocusBetaP3)
	ON_EN_SETFOCUS(IDC_BETA_S1, OnSetfocusBetaS1)
	ON_EN_SETFOCUS(IDC_BETA_S2, OnSetfocusBetaS2)
	ON_EN_SETFOCUS(IDC_BETA_S3, OnSetfocusBetaS3)
	ON_EN_SETFOCUS(IDC_N2_I1, OnSetfocusN2I1)
	ON_EN_SETFOCUS(IDC_N2_I2, OnSetfocusN2I2)
	ON_EN_SETFOCUS(IDC_N2_I3, OnSetfocusN2I3)
	ON_EN_SETFOCUS(IDC_N2_P1, OnSetfocusN2P1)
	ON_EN_SETFOCUS(IDC_N2_P2, OnSetfocusN2P2)
	ON_EN_SETFOCUS(IDC_N2_P3, OnSetfocusN2P3)
	ON_EN_SETFOCUS(IDC_N2_S1, OnSetfocusN2S1)
	ON_EN_SETFOCUS(IDC_N2_S2, OnSetfocusN2S2)
	ON_EN_SETFOCUS(IDC_N2_S3, OnSetfocusN2S3)
	ON_EN_CHANGE(IDC_ALPHA_I, OnChangeAlphaI)
	ON_EN_CHANGE(IDC_ALPHA_P, OnChangeAlphaP)
	ON_EN_CHANGE(IDC_ALPHA_S, OnChangeAlphaS)
	ON_EN_CHANGE(IDC_BEAMDIAM_I, OnChangeBeamdiamI)
	ON_EN_CHANGE(IDC_BEAMDIAM_P, OnChangeBeamdiamP)
	ON_EN_CHANGE(IDC_BEAMDIAM_S, OnChangeBeamdiamS)
	ON_EN_CHANGE(IDC_BETA_I1, OnChangeBetaI1)
	ON_EN_CHANGE(IDC_BETA_I2, OnChangeBetaI2)
	ON_EN_CHANGE(IDC_BETA_I3, OnChangeBetaI3)
	ON_EN_CHANGE(IDC_BETA_P1, OnChangeBetaP1)
	ON_EN_CHANGE(IDC_BETA_P2, OnChangeBetaP2)
	ON_EN_CHANGE(IDC_BETA_P3, OnChangeBetaP3)
	ON_EN_CHANGE(IDC_BETA_S1, OnChangeBetaS1)
	ON_EN_CHANGE(IDC_BETA_S2, OnChangeBetaS2)
	ON_EN_CHANGE(IDC_BETA_S3, OnChangeBetaS3)
	ON_EN_CHANGE(IDC_CRYSTLENGTH, OnChangeCrystlength)
	ON_EN_CHANGE(IDC_DEFF, OnChangeDeff)
	ON_EN_CHANGE(IDC_DELAY_I, OnChangeDelayI)
	ON_EN_CHANGE(IDC_DELAY_S, OnChangeDelayS)
	ON_EN_CHANGE(IDC_DELTAK, OnChangeDeltak)
	ON_EN_CHANGE(IDC_DIST, OnChangeDist)
	ON_EN_CHANGE(IDC_DURATION_I, OnChangeDurationI)
	ON_EN_CHANGE(IDC_DURATION_P, OnChangeDurationP)
	ON_EN_CHANGE(IDC_DURATION_S, OnChangeDurationS)
	ON_EN_CHANGE(IDC_ENERGY_I, OnChangeEnergyI)
	ON_EN_CHANGE(IDC_ENERGY_P, OnChangeEnergyP)
	ON_EN_CHANGE(IDC_ENERGY_S, OnChangeEnergyS)
	ON_EN_CHANGE(IDC_GAUSS_I, OnChangeGaussI)
	ON_EN_CHANGE(IDC_GAUSS_P, OnChangeGaussP)
	ON_EN_CHANGE(IDC_GAUSS_S, OnChangeGaussS)
	ON_EN_CHANGE(IDC_GVD_I, OnChangeGvdI)
	ON_EN_CHANGE(IDC_GVD_P, OnChangeGvdP)
	ON_EN_CHANGE(IDC_GVD_S, OnChangeGvdS)
	ON_EN_CHANGE(IDC_GVI_I, OnChangeGviI)
	ON_EN_CHANGE(IDC_GVI_P, OnChangeGviP)
	ON_EN_CHANGE(IDC_GVI_S, OnChangeGviS)
	ON_EN_CHANGE(IDC_INDEX_I, OnChangeIndexI)
	ON_EN_CHANGE(IDC_INDEX_P, OnChangeIndexP)
	ON_EN_CHANGE(IDC_INDEX_S, OnChangeIndexS)
	ON_EN_CHANGE(IDC_N2_I1, OnChangeN2I1)
	ON_EN_CHANGE(IDC_N2_I2, OnChangeN2I2)
	ON_EN_CHANGE(IDC_N2_I3, OnChangeN2I3)
	ON_EN_CHANGE(IDC_N2_P1, OnChangeN2P1)
	ON_EN_CHANGE(IDC_N2_P2, OnChangeN2P2)
	ON_EN_CHANGE(IDC_N2_P3, OnChangeN2P3)
	ON_EN_CHANGE(IDC_N2_S1, OnChangeN2S1)
	ON_EN_CHANGE(IDC_N2_S2, OnChangeN2S2)
	ON_EN_CHANGE(IDC_N2_S3, OnChangeN2S3)
	ON_EN_CHANGE(IDC_OFFSET_I, OnChangeOffsetI)
	ON_EN_CHANGE(IDC_OFFSET_S, OnChangeOffsetS)
	ON_EN_CHANGE(IDC_PHASE_I, OnChangePhaseI)
	ON_EN_CHANGE(IDC_PHASE_P, OnChangePhaseP)
	ON_EN_CHANGE(IDC_PHASE_S, OnChangePhaseS)
	ON_EN_CHANGE(IDC_RADCURV_I, OnChangeRadcurvI)
	ON_EN_CHANGE(IDC_RADCURV_P, OnChangeRadcurvP)
	ON_EN_CHANGE(IDC_RADCURV_S, OnChangeRadcurvS)
	ON_EN_CHANGE(IDC_RIN_I, OnChangeRinI)
	ON_EN_CHANGE(IDC_RIN_P, OnChangeRinP)
	ON_EN_CHANGE(IDC_RIN_S, OnChangeRinS)
	ON_EN_CHANGE(IDC_ROUT_I, OnChangeRoutI)
	ON_EN_CHANGE(IDC_ROUT_P, OnChangeRoutP)
	ON_EN_CHANGE(IDC_ROUT_S, OnChangeRoutS)
	ON_EN_CHANGE(IDC_TPOINTS, OnChangeTpoints)
	ON_EN_CHANGE(IDC_WALKOFF_I, OnChangeWalkoffI)
	ON_EN_CHANGE(IDC_WALKOFF_P, OnChangeWalkoffP)
	ON_EN_CHANGE(IDC_WALKOFF_S, OnChangeWalkoffS)
	ON_EN_CHANGE(IDC_WAVELENGTH_I, OnChangeWavelengthI)
	ON_EN_CHANGE(IDC_WAVELENGTH_P, OnChangeWavelengthP)
	ON_EN_CHANGE(IDC_WAVELENGTH_S, OnChangeWavelengthS)
	ON_EN_CHANGE(IDC_XGRID, OnChangeXgrid)
	ON_EN_CHANGE(IDC_XPOINTS, OnChangeXpoints)
	ON_EN_CHANGE(IDC_YGRID, OnChangeYgrid)
	ON_EN_CHANGE(IDC_YPOINTS, OnChangeYpoints)
	ON_EN_CHANGE(IDC_ZSTEP, OnChangeZstep)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RUNERG, OnRunErgScan)
	ON_BN_CLICKED(IDC_RUNDK, OnRunDeltakScan)
	ON_COMMAND(ID_EXTRAS_KVEC, OnExtrasKvec)
	ON_UPDATE_COMMAND_UI(ID_EXTRAS_KVEC, OnUpdateExtrasKvec)
	ON_COMMAND(ID_EXTRAS_ERG, OnExtrasErg)
	ON_UPDATE_COMMAND_UI(ID_EXTRAS_ERG, OnUpdateExtrasErg)
	ON_COMMAND(ID_EXTRAS_LENS, OnExtrasLens)
	ON_UPDATE_COMMAND_UI(ID_EXTRAS_LENS, OnUpdateExtrasLens)
	ON_BN_CLICKED(IDC_RUNLENS, OnRunLensScan)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USERAPPLY, OnUserApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMixView diagnostics

#ifdef _DEBUG
void CMixView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMixView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMixView message handlers


void CMixView::OnInitialUpdate() 
{
	
//***************************************************************************
// Resize the main frame to fit the size of the dialog area

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(false);

	//if(m_bFirstTime)
	//{
	//	// Create the graph box
	//	pGraphDlg = new CGraphDlg();
	//	if(pGraphDlg != NULL)
	//	{
	//		pGraphDlg->Create(IDD_GRAPHDLG,NULL);
	//	}
	//
	//	// Create pointers to this view and its main frame
	//	pView = this;
	//	pMF = pView->GetParentFrame(); 

	//}
				
//***************************************************************************
// Create an object of class CInputs and read the input file 'Inputs.in'
	
	CInputs* pInputs;
	pInputs = new CInputs;

	Reader(pInputs);


//***************************************************************************
// Set dialog boxes to their corresponding values

	CString str;

	str.Format("%5.3f",pInputs->GetVar(DEFF));
	m_deff = str;

	str.Format("%5.3f",pInputs->GetVar(DELTAK));
	m_deltak = str;

	str.Format("%5.2e",pInputs->GetVar(ENERGY_P));
	m_energy_p = str;

	str.Format("%5.2e",pInputs->GetVar(ENERGY_S));
	m_energy_s = str;

	str.Format("%5.2e",pInputs->GetVar(ENERGY_I));
	m_energy_i = str;

	str.Format("%5.2e",pInputs->GetVar(BEAMDIAM_S));
	m_beamdiam_s = str;

	str.Format("%5.2e",pInputs->GetVar(BEAMDIAM_I));
	m_beamdiam_i = str;

	str.Format("%5.2e",pInputs->GetVar(BEAMDIAM_P));
	m_beamdiam_p = str;

	str.Format("%5.3f",pInputs->GetVar(DURATION_S));
	m_duration_s = str;

	str.Format("%5.3f",pInputs->GetVar(DURATION_I));
	m_duration_i = str;

	str.Format("%5.3f",pInputs->GetVar(DURATION_P));
	m_duration_p = str;

	str.Format("%5.3f",pInputs->GetVar(LAMBDA_S));
	m_wavelength_s = str;
	
	str.Format("%5.3f",pInputs->GetVar(LAMBDA_I));
	m_wavelength_i = str;

	str.Format("%5.3f",pInputs->GetVar(LAMBDA_P));
	m_wavelength_p = str;
	
	str.Format("%5.3f",pInputs->GetVar(INDEX_S));
	m_index_s = str;

	str.Format("%5.3f",pInputs->GetVar(INDEX_I));
	m_index_i = str;

	str.Format("%5.3f",pInputs->GetVar(INDEX_P));
	m_index_p = str;

	str.Format("%5.3f",pInputs->GetVar(GVI_S));
	m_gvi_s = str;

	str.Format("%5.3f",pInputs->GetVar(GVI_I));
	m_gvi_i = str;

	str.Format("%5.3f",pInputs->GetVar(GVI_P));
	m_gvi_p = str;

	str.Format("%5.2e",pInputs->GetVar(GVD_S));
	m_gvd_s = str;

	str.Format("%5.2e",pInputs->GetVar(GVD_I));
	m_gvd_i = str;

	str.Format("%5.2e",pInputs->GetVar(GVD_P));
	m_gvd_p = str;

	str.Format("%5.3f",pInputs->GetVar(ALPHA_S));
	m_alpha_s = str;

	str.Format("%5.3f",pInputs->GetVar(ALPHA_I));
	m_alpha_i = str;

	str.Format("%5.3f",pInputs->GetVar(ALPHA_P));
	m_alpha_p = str;

	str.Format("%5.3f",pInputs->GetVar(RC_IN_S));
	m_rin_s = str;

	str.Format("%5.3f",pInputs->GetVar(RC_IN_I));
	m_rin_i = str;

	str.Format("%5.3f",pInputs->GetVar(RC_IN_P));
	m_rin_p = str;

	str.Format("%5.3f",pInputs->GetVar(RC_OUT_S));
	m_rout_s = str;

	str.Format("%5.3f",pInputs->GetVar(RC_OUT_I));
	m_rout_i = str;

	str.Format("%5.3f",pInputs->GetVar(RC_OUT_P));
	m_rout_p = str;

	str.Format("%5.3f",pInputs->GetVar(CRYSTLENGTH));
	m_crystlength = str;

	str.Format("%5.2e",pInputs->GetVar(N2_S1));
	m_n2_s1 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_S2));
	m_n2_s2 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_S3));
	m_n2_s3 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_I1));
	m_n2_i1 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_I2));
	m_n2_i2 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_I3));
	m_n2_i3 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_P1));
	m_n2_p1 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_P2));
	m_n2_p2 = str;

	str.Format("%5.2e",pInputs->GetVar(N2_P3));
	m_n2_p3 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_S1));
	m_beta_s1 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_S2));
	m_beta_s2 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_S3));
	m_beta_s3 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_I1));
	m_beta_i1 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_I2));
	m_beta_i2 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_I3));
	m_beta_i3 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_P1));
	m_beta_p1 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_P2));
	m_beta_p2 = str;

	str.Format("%5.2e",pInputs->GetVar(BETA_P3));
	m_beta_p3 = str;
	
	str.Format("%5.3f",pInputs->GetVar(RHO_S));
	m_walkoff_s = str;

	str.Format("%5.3f",pInputs->GetVar(RHO_I));
	m_walkoff_i = str;

	str.Format("%5.3f",pInputs->GetVar(RHO_P));
	m_walkoff_p = str;

	str.Format("%5.3f",pInputs->GetVar(OFFSET_S));
	m_offset_s = str;

	str.Format("%5.3f",pInputs->GetVar(OFFSET_I));
	m_offset_i = str;

	str.Format("%5.2e",pInputs->GetVar(RADCURV_S));
	m_radcurv_s = str;

	str.Format("%5.2e",pInputs->GetVar(RADCURV_I));
	m_radcurv_i = str;

	str.Format("%5.2e",pInputs->GetVar(RADCURV_P));
	m_radcurv_p = str;

	str.Format("%d",long(pInputs->GetVar(GAUSSINDEX_S)));
	m_gauss_s = str;

	str.Format("%d",long(pInputs->GetVar(GAUSSINDEX_I)));
	m_gauss_i = str;

	str.Format("%d",long(pInputs->GetVar(GAUSSINDEX_P)));
	m_gauss_p = str;

	str.Format("%5.3f",pInputs->GetVar(PHI_S));
	m_phase_s = str;

	str.Format("%5.3f",pInputs->GetVar(PHI_I));
	m_phase_i = str;

	str.Format("%5.3f",pInputs->GetVar(PHI_P));
	m_phase_p = str;

	str.Format("%5.3f",pInputs->GetVar(DELAY_S));
	m_delay_s = str;

	str.Format("%5.3f",pInputs->GetVar(DELAY_I));
	m_delay_i = str;

	str.Format("%d",long(pInputs->GetVar(NX)));
	m_xpoints = str;

	str.Format("%d",long(pInputs->GetVar(NY)));
	m_ypoints = str;

	str.Format("%d",long(pInputs->GetVar(NT)));
	m_tpoints = str;

	str.Format("%d",long(pInputs->GetVar(NZ)));
	m_zstep = str;

	str.Format("%5.3f",pInputs->GetVar(DIST));
	m_dist = str;

	str.Format("%5.2e",pInputs->GetVar(MAXGRID_X));
	m_xgrid = str;

	str.Format("%5.2e",pInputs->GetVar(MAXGRID_Y));
	m_ygrid = str;

	delete pInputs;
	
	// Reset InputsChanged to false
	m_bInputsChanged = false;

	CFormView::OnInitialUpdate();
}



void CMixView::OnAccept() 
{

	UpdateData();


//***************************************************************************
// Test for errors on the input form

	bool Error = TestForErrors();
	if(Error)
	{
		// Reset m_error
		m_error = false;

		return;
	}


//***************************************************************************
// Print the current information to the inputs.in file

	CString filename = "Inputs.in";
	FILE * stream;
	stream = fopen(filename, "w");

	fprintf(stream,"%5.2f %s", atof(m_deff), 
		"                   !deff (pm/V)\n");
	fprintf(stream,"%5.2f %s", atof(m_deltak),
		"                   !deltak (1/mm) kp-ks-ki\n");
	fprintf(stream,"%5.2e %s", atof(m_energy_p),
		"                   !pump energy (J)\n");
	fprintf(stream,"%5.2e %s %5.2e %s", atof(m_energy_s),",",atof(m_energy_i),
		"              !signal, idler energy (J)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_beamdiam_s),",",  
		atof(m_beamdiam_i),",", atof(m_beamdiam_p),
		"          !beam diameters (mm) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_duration_s),",", 
		atof(m_duration_i),",", atof(m_duration_p),
		"          !pulse duration (ps) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_wavelength_s),",", 
		atof(m_wavelength_i),",", atof(m_wavelength_p),
		"          !wavelengths (nm) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_index_s),",",		
		atof(m_index_i),",", atof(m_index_p),
		"          !index of refraction (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_gvi_s),",", 
		atof(m_gvi_i),",", atof(m_gvi_p),
		"          !group velocity index (s,i,p)\n");
	fprintf(stream,"%5.3e %s %5.3e %s %5.3e %s", atof(m_gvd_s),",", 
		atof(m_gvd_i),",", atof(m_gvd_p),
		"          !group velocity dispersion (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_alpha_s),",", 
		atof(m_alpha_i),",", atof(m_alpha_p),
		"          !crystal absorption (1/mm) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_rin_s),",", 
		atof(m_rin_i),",", atof(m_rin_p),
		"          !crystal input reflectivity (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_rout_s),",",
		atof(m_rout_i),",", atof(m_rout_p),
		"          !crystal output reflectivity (s,i,p)\n");
	fprintf(stream,"%5.3f %s", atof(m_crystlength),
		"                    !crystal length (mm)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_n2_s1),",",
		atof(m_n2_s2),",", atof(m_n2_s3),
		"          !n-squared signal (sq cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_n2_i1),",",
		atof(m_n2_i2),",", atof(m_n2_i3),
		"          !n-squared idler (sq cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_n2_p1),",",
		atof(m_n2_p2),",", atof(m_n2_p3),
		"          !n-squared pump (sq cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_beta_s1),",",
		atof(m_beta_s2),",", atof(m_beta_s3),
		"          !beta signal (cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_beta_i1),",",
		atof(m_beta_i2),",", atof(m_beta_i3),
		"          !beta idler (cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", atof(m_beta_p1),",",
		atof(m_beta_p2),",", atof(m_beta_p3),
		"          !beta pump (cm/W)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_walkoff_s),",", 
		atof(m_walkoff_i),",", atof(m_walkoff_p),
		"          !walkoff angles (mrad) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_radcurv_s),",", 
		atof(m_radcurv_i),",", atof(m_radcurv_p),
		"          !radius of curvature (mm in air) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s", atof(m_offset_s),",", 
		atof(m_offset_i),"          !offset in wo direction (mm) (s,i,p)\n");
	fprintf(stream,"%d %s %d %s %d %s", atol(m_gauss_s),",", 
		atol(m_gauss_i),",", atol(m_gauss_p),
		"          !Gaussian index order (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", atof(m_phase_s),",", 
		atof(m_phase_i),",", atof(m_phase_p),
		"          !phase of s,i,p (rad)\n");
	fprintf(stream,"%5.3f %s %5.3f %s", atof(m_delay_s),",",atof(m_delay_i),
		"          !time delay (s,i) rel. to pump (ps)\n");
	fprintf(stream,"%d %s %d %s %d %s %d %s", atol(m_xpoints),",", 
		atol(m_ypoints),",", atol(m_tpoints),",", atol(m_zstep),
		"          !number of grid points (x,y,t,z)\n");
	fprintf(stream,"%5.2e %s %5.2e %s", atof(m_xgrid),",",atof(m_ygrid),
		"          !maximum grid extent (mm) (x,y)\n");
	fprintf(stream,"%6.4f %s", atof(m_dist),
		"                   ! distance to detector (mm)\n");
	
	fclose(stream);

	// Create the graphDlg here
	if(m_bFirstTime)
	{
		// Create the graph box
		pGraphDlg = new CGraphDlg();
		if(pGraphDlg != NULL)
		{
			pGraphDlg->Create(IDD_GRAPHDLG, NULL);
		}
	
		// Create pointers to this view and its main frame
		pView = this;
		pMF = pView->GetParentFrame(); 

	}

	// Hide this window and send pGraphDlg pointers to this view 
	pMF->ShowWindow(SW_HIDE);
	pGraphDlg->GetFramePointers(pMF,pView);

	// Show the dialog box
	if(m_bFirstTime)
	{
		pGraphDlg->m_brun = false;
		pGraphDlg->m_banalyze = false;
	
		m_bFirstTime = false;
	}
	else if(m_bInputsChanged)
	{
		pGraphDlg->SetEnableBtns(false);
		pGraphDlg->m_brun = false;
		pGraphDlg->m_banalyze = false;
	}
	else if(pGraphDlg->m_brun)
	{
		if(pGraphDlg->m_banalyze)
		{
			pGraphDlg->GetDlgItem(IDC_ANALYZE)->EnableWindow(false);
		}
		else
		{
			pGraphDlg->GetDlgItem(IDC_BEAM_FT)->EnableWindow(false);
			pGraphDlg->GetDlgItem(IDC_BEAM_FT2)->EnableWindow(false);
			pGraphDlg->GetDlgItem(IDC_M_W)->EnableWindow(false);
			pGraphDlg->GetDlgItem(IDC_M_P)->EnableWindow(false);
			pGraphDlg->GetDlgItem(IDC_FOCUS_W)->EnableWindow(false);
			pGraphDlg->GetDlgItem(IDC_FOCUS_P)->EnableWindow(false);
			pGraphDlg->GetDlgItem(IDC_TILT)->EnableWindow(false);
		}
	}
	
	// Show the graph box
	//pGraphDlg->ShowWindow(SW_SHOW);
	pGraphDlg->ShowWindow(SW_NORMAL);
}


HBRUSH CMixView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

//***************************************************************************
// Set the text colors for Signal,Idler, and Pump in the dialog box

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_SIGNAL)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_IDLER)
	{
		pDC->SetTextColor(RGB(0,0,255));
	}
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_PUMP)
	{
		pDC->SetTextColor(RGB(0,100,0));
	}

	// If there is an error, highlight the text in the help box in red
	if(m_error)
	{
		if(pWnd->GetDlgCtrlID() == IDC_HELPBOX)
		{
			pDC->SetTextColor(RGB(255,0,0));
		}
	}
	else
	{
		if(pWnd->GetDlgCtrlID() == IDC_HELPBOX)
		{
			pDC->SetTextColor(RGB(0,0,0));
		}
	}

	
	return hbr;
}


//********************** OnSetFocus Functions **************************
//
// These functions set the text in the help dialog box when a
// particular edit box is selected.
//
//**********************************************************************

void CMixView::OnSetfocusAlphaI() 
{
	CString str = "Linear power loss per mm.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusAlphaP() 
{
	CString str = "Linear power loss per mm.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusAlphaS() 
{
	CString str = "Linear power loss per mm.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBeamdiamI() 
{
	CString str;
	str.Format("%s %s %s",
		"Beam diameter FWHM power.",
		"\nTo convert from the radius of (1/e)^2 irradiance,",
		"\nmultiply by 1.18.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBeamdiamP() 
{
	CString str;
	str.Format("%s %s %s",
		"Beam diameter FWHM power.",
		"\nTo convert from the radius of (1/e)^2 irradiance,",
		"\nmultiply by 1.18.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBeamdiamS() 
{
	CString str;
	str.Format("%s %s %s",
		"Beam diameter FWHM power.",
		"\nTo convert from the radius of (1/e)^2 irradiance,",
		"\nmultiply by 1.18.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusCrystlength() 
{
	CString str = "Length of the crystal in mm.";
	SetDlgItemText(IDC_HELPBOX,str);	
}

void CMixView::OnSetfocusDeff() 
{
	CString str;
	str.Format("%s %s",
		"Nonlinear coefficient in pm/V.",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);	
}

void CMixView::OnSetfocusDelayI() 
{
	CString str;
	str.Format("%s %s",
		"Pulse delay relative to the pump pulse.",
		"\nPulse trails pump for positive values.");
	SetDlgItemText(IDC_HELPBOX,str);	
}

void CMixView::OnSetfocusDelayS() 
{
	CString str;
	str.Format("%s %s",
		"Pulse delay relative to the pump pulse.",
		"\nPulse trails pump for positive values.");
	SetDlgItemText(IDC_HELPBOX,str);	
}

void CMixView::OnSetfocusDeltak() 
{
	CString str;
	str.Format("%s %s",
		"Phase mismatch of carrier waves (central frequencies).",
		"\nDeltak = k[p] - k[s] - k[i].");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusDist() 
{
	CString str = "Distance to the detector in mm.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusDurationI() 
{
	CString str = "Pulse duration FWHM power.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusDurationP() 
{
	CString str = "Pulse duration FWHM power.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusDurationS() 
{
	CString str = "Pulse duration FWHM power.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusEnergyI() 
{
	CString str;
	str.Format("%s %s",
		"For type 1 second harmonic generation and its inverse,",
		"\ndivide the input energy equally between signal and idler.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusEnergyP() 
{
	CString str;
	str.Format("%s %s",
		"For type 1 second harmonic generation and its inverse,",
		"\ndivide the input energy equally between signal and idler.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusEnergyS() 
{
	CString str;
	str.Format("%s %s",
		"For type 1 second harmonic generation and its inverse,",
		"\ndivide the input energy equally between signal and idler.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGaussI() 
{
	CString str;
	str.Format("%s",
		"Must be integer 1,2,3,4, or 5.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGaussP() 
{
	CString str;
	str.Format("%s",
		"Must be integer 1,2,3,4, or 5.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGaussS() 
{
	CString str;
	str.Format("%s",
		"Must be integer 1,2,3,4, or 5.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGvdI() 
{
	CString str;
	str.Format("%s %s",
		"Group velocity dispersion.",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGvdP() 
{
	CString str;
	str.Format("%s %s",
		"Group velocity dispersion.",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGvdS() 
{
	CString str;
	str.Format("%s %s",
		"Group velocity dispersion.",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGviI() 
{
	CString str;
	str.Format("%s %s",
		"Group velocity = (c/group velocity index)",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGviP() 
{
	CString str;
	str.Format("%s %s",
		"Group velocity = (c/group velocity index)",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusGviS() 
{
	CString str;
	str.Format("%s %s",
		"Group velocity = (c/group velocity index)",
		"\nFind by running SNLO's QMix.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusIndexI() 
{
	CString str = "Find by running SNLO's QMix.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusIndexP() 
{
	CString str = "Find by running SNLO's QMix.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusIndexS() 
{
	CString str = "Find by running SNLO's QMix.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusOffsetI() 
{
	CString str;
	str.Format("%s %s",
		"Beam position relative to pump beam.  Positive direction",
		"\nis in the direction of positive walkoff.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusOffsetS() 
{
	CString str;
	str.Format("%s %s",
		"Beam position relative to pump beam.  Positive direction",
		"\nis in the direction of positive walkoff.");
	SetDlgItemText(IDC_HELPBOX,str);	
}

void CMixView::OnSetfocusPhaseI() 
{
	CString str = "Phases only matter if all three waves start with nonzero irradiance.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusPhaseP() 
{
	CString str = "Phases only matter if all three waves start with nonzero irradiance.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusPhaseS() 
{
	CString str = "Phases only matter if all three waves start with nonzero irradiance.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRadcurvI() 
{
	CString str;
	str.Format("%s %s",
		"Radius of curv. of wavefront in air at input face of crystal.",
		"\nPositive values => focusing. Use SNLO's Focus to help decide curvature.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRadcurvP() 
{
	CString str;
	str.Format("%s %s",
		"Radius of curv. of wavefront in air at input face of crystal.",
		"\nPositive values => focusing. Use SNLO's Focus to help decide curvature.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRadcurvS() 
{
	CString str;
	str.Format("%s %s",
		"Radius of curv. of wavefront in air at input face of crystal.",
		"\nPositive values => focusing. Use SNLO's Focus to help decide curvature.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRinI() 
{
	CString str = "Power reflectivity (0-1).";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRinP() 
{
	CString str = "Power reflectivity (0-1).";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRinS() 
{
	CString str = "Power reflectivity (0-1).";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRoutI() 
{
	CString str = "Power reflectivity (0-1).";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRoutP() 
{
	CString str = "Power reflectivity (0-1).";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusRoutS() 
{
	CString str = "Power reflectivity (0-1).";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusTpoints() 
{
	CString str;
	str.Format("%s %s",
		"Number of time grid points.",
		"\nMust be power of 2, suggested value is 128.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusWalkoffI() 
{
	CString str;
	str.Format("%s %s",
		"Birefringent walkoff angle.",
		"\nFind by running SNLO's QMIX.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusWalkoffP() 
{
	CString str;
	str.Format("%s %s",
		"Birefringent walkoff angle.",
		"\nFind by running SNLO's QMIX.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusWalkoffS() 
{
	CString str;
	str.Format("%s %s",
		"Birefringent walkoff angle.",
		"\nFind by running SNLO's QMIX.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusWavelengthI() 
{
	CString str = "Pump must be bluest wave.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusWavelengthP() 
{
	CString str = "Pump must be bluest wave.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusWavelengthS() 
{
	CString str = "Pump must be bluest wave.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusXgrid() 
{
	CString str;
	str.Format("%s %s",
		"Grid size in walkoff direction.",
		"\nSet to zero for auto scaling.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusXpoints() 
{
	CString str;
	str.Format("%s %s",
		"Number grid points in walkoff direction.",
		"\nMust be power of 2, suggested value is 32 or 64.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusYgrid() 
{
	CString str;
	str.Format("%s %s",
		"Grid size perpendicular to walkoff direction.",
		"\nSet to zero for auto scaling.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusYpoints() 
{
	CString str;
	str.Format("%s %s",
		"Number grid points perpendicular to walkoff direction.",
		"\nMust be power of 2, suggested value is 32 or 64.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusZstep() 
{
	CString str = "Suggested values 20-50.";
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaI1() 
{
	CString str;
	str.Format("%s %s %s","Two photon cross absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.",
	"\nFor type I SHG and inverse use self modulation value.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaI2() 
{
	CString str;
	str.Format("%s %s","Two photon absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaI3() 
{
	CString str;
	str.Format("%s %s","Two photon cross absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaP1() 
{
	CString str;
	str.Format("%s %s","Two photon cross absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaP2() 
{
	CString str;
	str.Format("%s %s","Two photon cross absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaP3() 
{
	CString str;
	str.Format("%s %s","Two photon absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaS1() 
{
	CString str;
	str.Format("%s %s","Two photon absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaS2() 
{
	CString str;
	str.Format("%s %s %s","Two photon cross absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.",
	"\nFor type I SHG and inverse use self modulation value.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusBetaS3() 
{
	CString str;
	str.Format("%s %s","Two photon cross absorption coefficient.",
	"\nNote: beta is wavelength and polarization dependent.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2I1() 
{
	CString str;
	str.Format("%s %s %s %s","Nonlinear refractive index - cross modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 2x self modulation term.",
	"\nFor type I SHG and inverse use self modulation value.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2I2() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - self modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 1E-15 - 1E-16 (see beta_n2.pdf).");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2I3() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - cross modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 2x self modulation term.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2P1() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - cross modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 2x self modulation term.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2P2() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - cross modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 2x self modulation term.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2P3() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - self modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 1E-15 - 1E-16 (see beta_n2.pdf).");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2S1() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - self modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 1E-15 - 1E-16 (see beta_n2.pdf).");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2S2() 
{
	CString str;
	str.Format("%s %s %s %s","Nonlinear refractive index - cross modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 2x self modulation term.",
	"\nFor type I SHG and inverse use self modulation value.");
	SetDlgItemText(IDC_HELPBOX,str);
}

void CMixView::OnSetfocusN2S3() 
{
	CString str;
	str.Format("%s %s %s","Nonlinear refractive index - cross modulation term.",
	"\nNote: n2 is wavelength and polarization dependent.",
	"\nTypical values 2x self modulation term.");
	SetDlgItemText(IDC_HELPBOX,str);
}

/*

// Tried to get the program to open the readme file, but couldn't 
// figure out how to get the current directory location if it was
// different from the default, so I removed it from the program

void CMixView::OnHelpReadme() 
{
	STARTUPINFO StartupInfo;
	memset (&StartupInfo, 0, sizeof (STARTUPINFO)); // use default window features
	StartupInfo.cb = sizeof (STARTUPINFO);     // this field must be filled in

	PROCESS_INFORMATION ProcessInfo;

	::CreateProcess
		("C:\\Windows\\Notepad.exe", // run "Notepad.exe" executable file
		"Notepad.exe C:\\SNLO\\Mix2dsp\\Readme.txt",  // open Readme.txt file
//		"Notepad.exe C:\\ChrisFiles\\GUI_Backups\\Mix2dsp\\Readme.txt",  // open Readme.txt file
		NULL,         // default process security attributes
		NULL,         // default thread security attributes
		FALSE,        // process does not inherit handles
		0,            // no creation flags (use defaults)
		NULL,          // use environment of calling process
		"C:\\Windows", // directory for Mix2dsp  
		&StartupInfo, // specifies features of process window
		&ProcessInfo); // receives information on process

}
*/

//***************************** OnChange functions ************************
//
// These functions will let pGraphDlg know that inputs have changed.
//
//*************************************************************************

void CMixView::OnChangeAlphaI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeAlphaP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeAlphaS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBeamdiamI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBeamdiamP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBeamdiamS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaI1() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaI2() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaI3() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaP1() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaP2() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaP3() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaS1() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaS2() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeBetaS3() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeCrystlength() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDeff() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDelayI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDelayS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDeltak() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDist() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDurationI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDurationP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeDurationS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeEnergyI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeEnergyP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeEnergyS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGaussI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGaussP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGaussS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGvdI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGvdP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGvdS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGviI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGviP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeGviS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeIndexI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeIndexP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeIndexS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2I1() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2I2() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2I3() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2P1() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2P2() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2P3() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2S1() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2S2() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeN2S3() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeOffsetI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeOffsetS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangePhaseI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangePhaseP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangePhaseS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRadcurvI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRadcurvP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRadcurvS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRinI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRinP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRinS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRoutI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRoutP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeRoutS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeTpoints() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeWalkoffI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeWalkoffP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeWalkoffS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeWavelengthI() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeWavelengthP() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeWavelengthS() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeXgrid() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeXpoints() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeYgrid() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeYpoints() 
{
	m_bInputsChanged = true;
}

void CMixView::OnChangeZstep() 
{
	m_bInputsChanged = true;
}


BOOL CMixView::DestroyWindow() 
{
	delete pGraphDlg;
	delete pView;
	delete pMF;
	
	return CFormView::DestroyWindow();
}


//***************************************************************************
//
// TestForErrors will verify that the input parameters are valid
// for running the program
// 
//***************************************************************************

bool CMixView::TestForErrors()
{
	

//***************************************************************************
// Test for negative input values

	if((atof(m_beamdiam_s) <= 0) || (atof(m_beamdiam_i) <= 0) || (atof(m_beamdiam_p) <= 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Beam diameters must be greater than zero.");
		return true;
	}
	else if(atof(m_crystlength) <= 0)
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Crystal length must be greater than zero.");
		return true;
	}
	else if(atof(m_dist) < 0)
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Distance to detector must be zero or greater.");
		return true;
	}
	else if((atof(m_duration_s) < 0) || (atof(m_duration_i) < 0) || (atof(m_duration_p) < 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Pulse duration must be zero or greater.");
		return true;
	}
	else if((atof(m_energy_s) < 0) || (atof(m_energy_i) < 0) || (atof(m_energy_p) < 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Energy must be zero or greater.");
		return true;
	}
	else if((atol(m_gauss_s) < 1) || (atof(m_gauss_s) > 5) || (atof(m_gauss_i) < 1) || (atof(m_gauss_i) > 5) || (atof(m_gauss_p) < 1) || (atof(m_gauss_p) > 5)) 
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Gaussian index range is 1 to 5.");
		return true;
	}	
	else if((atof(m_gvi_s) <= 0) || (atof(m_gvi_i) <= 0) || (atof(m_gvi_p) <= 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Group velocity index must be greater than zero.");
		return true;
	}
	else if((atof(m_index_s) <= 0) || (atof(m_index_i) <= 0) || (atof(m_index_p) <= 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Index of refraction must be greater than zero.");
		return true;
	}
	else if((atof(m_radcurv_s) == 0) || (atof(m_radcurv_i) == 0) || (atof(m_radcurv_p) == 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Radius of curvature must be nonzero.");
		return true;
	}
	else if((atof(m_wavelength_s) < 0) || (atof(m_wavelength_i) < 0) || (atof(m_wavelength_p) < 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Wavelengths must be greater than zero.");
		return true;
	}
	else if((atof(m_xgrid) < 0) || (atof(m_ygrid) < 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Maximum grid extents must be zero or greater.");
		return true;
	}
	else if(atof(m_zstep) <= 0)
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Number of z-steps must be nonzero.");
		return true;
	}
	

//***************************************************************************
// Verify that one of the waves is pulsed

	if((atof(m_duration_s) == 0) && (atof(m_duration_i) == 0) && (atof(m_duration_p) == 0))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: At least one wave must be pulsed.");
		return true;
	}


//***************************************************************************
// Verify that (1/Wavelength_s) + (1/Wavelength_i) = (1/Wavelength_p)

	double Wav_sum = (1/atof(m_wavelength_s)) + (1/atof(m_wavelength_i));
	
	if((Wav_sum + 1.e4 < (1/atof(m_wavelength_p))) || (Wav_sum - 1.e4 > (1/atof(m_wavelength_p))))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: The sum of [1/lambda(s) + 1/lambda(i)] must be almost equal to 1/lambda(p).");
		return true;
	}


//***************************************************************************
// Verify that the crystal input/output reflectivities and
// the input mirror reflectivity are between zero and one

	if((atof(m_rin_s) < 0) || (atof(m_rin_s) > 1) || (atof(m_rin_i) < 0) || (atof(m_rin_i) > 1) || (atof(m_rin_p) < 0) || (atof(m_rin_p) > 1))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Crystal input reflectivity range is 0 to 1.");
		return true;
	}
	else if((atof(m_rout_s) < 0) || (atof(m_rout_s) > 1) || (atof(m_rout_i) < 0) || (atof(m_rout_i) > 1) || (atof(m_rout_p) < 0) || (atof(m_rout_p) > 1))
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Crystal output reflectivity range is 0 to 1.");
		return true;
	}

//**********************************************
// Verify that the input grid points (x,y,t) are
// a factor of two

	double test_t = double(((double)log10((double)atoi(m_tpoints)))/(double)log10((double)2.0));
	test_t = fmod(test_t,1);
	double test_x = double(((double)log10((double)atoi(m_xpoints)))/(double)log10((double)2.0));
	test_x = fmod(test_x,1);
	double test_y = double(((double)log10((double)atoi(m_ypoints)))/(double)log10((double)2.0));
	test_y = fmod(test_y,1);

	if( (test_t != 0) || (test_x != 0) || (test_y != 0) )
	{
		m_error = true;
		SetDlgItemText(IDC_HELPBOX,"ERROR: Number of grid points in x,y,t must be a power of 2.");
		return true;
	}
		

//************************************************
//Test system memory
	
//	SYSTEM_INFO Info;
//	LPSYSTEM_INFO lpInfo=&Info;
//	::GetSystemInfo(lpInfo);
//	CString* dummy=lpInfo->lpMaximumApplicationAddress;
//	CString str;

// Default return value
	m_error = false;
	return false;
}


void CMixView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CFormView::OnLButtonUp(nFlags, point);
}

void CMixView::OnExtrasKvec() 
{
	deltakDlg.DoModal();
}

void CMixView::OnUpdateExtrasKvec(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

LRESULT CMixView::OnUserApply(WPARAM wParam, LPARAM lParam)
{
	char str[3000];
	int nResult = 0;

	UpdateData(FALSE);

	if(m_bErgDlgOn == TRUE)
	{
		ergDlg.EndDialog(nResult);
		m_bErgDlgOn = FALSE;
		sprintf(str,"%s %i\n%s %5.3e  %5.3e  %5.3e\n%s %5.3e  %5.3e  %5.3e",
			"Number of energy values = ",m_Nerg,
			"Initial pulse energies = ",m_Esig_i, m_Eidle_i, m_Epump_i,
			"Final pulse energies = ",m_Esig_f, m_Eidle_f, m_Epump_f);
		SetDlgItemText(IDC_HELPBOX, str);
	}
	if(m_bDeltakDlgOn == TRUE)
	{
		deltakDlg.EndDialog(nResult);
		m_bDeltakDlgOn = FALSE;
		sprintf(str,"%s %i\n%s %5.3e\n%s %5.3e",
			"Number of dk values = ",m_Ndk,
			"Initial value of dk = ",m_dkInitial,
			"Final value of dk = ",m_dkFinal);
		SetDlgItemText(IDC_HELPBOX, str);
	}
	if(m_bLensDlgOn == TRUE)
	{
		lensDlg.EndDialog(nResult);
		UpdateData(FALSE);
		m_bLensDlgOn = FALSE;
	}

	return 0;
}

void CMixView::OnRunErgScan() 
{
	double m_dStep_s, m_dStep_i, m_dStep_p;
	
	m_nScanIndex = 0;

	if(m_bErgOn == TRUE)
	{
		m_dStep_s = (m_Esig_f - m_Esig_i)/m_Nerg;
		m_dStep_i = (m_Eidle_f - m_Eidle_i)/m_Nerg;
		m_dStep_p = (m_Epump_f - m_Epump_i)/m_Nerg;
		while(m_nScanIndex<=m_Nerg)
		{
			UpdateData(TRUE);
			m_energy_s.Format("%5.2e", m_Esig_i  + m_dStep_s*m_nScanIndex);
			m_energy_i.Format("%5.2e", m_Eidle_i + m_dStep_i*m_nScanIndex);
			m_energy_p.Format("%5.2e", m_Epump_i + m_dStep_p*m_nScanIndex);
			UpdateData(FALSE);
			OnAccept();
			if(m_nScanIndex == 0) (pGraphDlg->m_ProgScan).SetRange(0,m_Nerg + 1);
			pGraphDlg->OnRunbtn();
			pGraphDlg->OnCancel();
			(pGraphDlg->m_ProgScan).SetPos(m_nScanIndex + 1);
			m_nScanIndex++;
		}
		m_bErgOn = FALSE;
	}
}

void CMixView::OnRunDeltakScan() 
{
	// TODO: Add your control notification handler code here
	double m_dkStep;
	
	m_nScanIndex = 0;

	if(m_bDeltakOn == TRUE)
	{
		m_dkStep = (m_dkFinal - m_dkInitial)/m_Ndk;
		while(m_nScanIndex<=m_Ndk)
		{
			UpdateData(TRUE);
			m_deltak.Format("%5.3e", m_dkInitial + m_dkStep*m_nScanIndex);
			UpdateData(FALSE);
			OnChangeDeltak();
			OnAccept();
			pGraphDlg->OnRunbtn();
			pGraphDlg->OnCancel();
			m_nScanIndex++;
		}
		m_bDeltakOn = FALSE;
	}
}

void CMixView::OnExtrasErg() 
{
	// TODO: Add your command handler code here
	ergDlg.DoModal();	
}

void CMixView::OnUpdateExtrasErg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMixView::OnExtrasLens() 
{
	// TODO: Add your command handler code here
	lensDlg.DoModal();	
}

void CMixView::OnUpdateExtrasLens(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMixView::OnRunLensScan() 
{
	m_nScanIndex = 0;

	if(m_bLensScanOn == TRUE)
	{		
		while((m_nScanIndex<=m_Ndist) && (m_pBeamDiamArray != NULL) && (m_pRadCurvArray != NULL))
		{
			UpdateData(TRUE);
			m_beamdiam_s.Format("%5.3e", m_pBeamDiamArray[m_nScanIndex]*1.0e3);
			m_beamdiam_i.Format("%5.3e", m_pBeamDiamArray[m_nScanIndex]*1.0e3);
			m_beamdiam_p.Format("%5.3e", m_pBeamDiamArray[m_nScanIndex]*1.0e3);
			m_radcurv_s.Format("%5.3e", m_pRadCurvArray[m_nScanIndex]*1.0e3);
			m_radcurv_i.Format("%5.3e", m_pRadCurvArray[m_nScanIndex]*1.0e3);
			m_radcurv_p.Format("%5.3e", m_pRadCurvArray[m_nScanIndex]*1.0e3);
			m_xgrid.Format("0.0"); m_ygrid.Format("0.0");
			UpdateData(FALSE);
			OnAccept();
			if(m_nScanIndex == 0) (pGraphDlg->m_ProgScan).SetRange(0,m_Ndist + 1);
			pGraphDlg->OnRunbtn();
			pGraphDlg->OnCancel();
			(pGraphDlg->m_ProgScan).SetPos(m_nScanIndex + 1);
			m_nScanIndex++;
		}
		m_bLensScanOn = FALSE;
	}
}
