#if !defined(AFX_MIXVIEW_H__9D4D88A1_3D4C_11D4_9234_00A0240EAC17__INCLUDED_)
#define AFX_MIXVIEW_H__9D4D88A1_3D4C_11D4_9234_00A0240EAC17__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MixView.h : header file
//
#include "DeltakDlg.h"
#include "ErgDlg.h"
#include "LensDlg.h"

class CGraphDlg;

extern CGraphDlg* g_pGraphDlg;
extern bool m_bDeltakOn;
extern bool m_bErgOn;
extern bool m_bLensOn;
extern int m_nScanIndex;

/////////////////////////////////////////////////////////////////////////////
// CMixView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMixView : public CFormView
{
protected:
	CMixView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMixView)

// Form Data
public:
	//{{AFX_DATA(CMixView)
	enum { IDD = IDD_INPUT_FORM };
	CString	m_alpha_i;
	CString	m_alpha_p;
	CString	m_alpha_s;
	CString	m_beamdiam_i;
	CString	m_beamdiam_p;
	CString	m_beamdiam_s;
	CString	m_crystlength;
	CString	m_deff;
	CString	m_delay_i;
	CString	m_delay_s;
	CString	m_deltak;
	CString	m_dist;
	CString	m_duration_i;
	CString	m_duration_p;
	CString	m_duration_s;
	CString	m_energy_i;
	CString	m_energy_p;
	CString	m_energy_s;
	CString	m_gauss_i;
	CString	m_gauss_p;
	CString	m_gauss_s;
	CString	m_gvd_i;
	CString	m_gvd_p;
	CString	m_gvd_s;
	CString	m_gvi_i;
	CString	m_gvi_p;
	CString	m_gvi_s;
	CString	m_index_i;
	CString	m_index_p;
	CString	m_index_s;
	CString	m_offset_i;
	CString	m_offset_s;
	CString	m_phase_i;
	CString	m_phase_p;
	CString	m_phase_s;
	CString	m_radcurv_i;
	CString	m_radcurv_p;
	CString	m_radcurv_s;
	CString	m_rin_i;
	CString	m_rin_p;
	CString	m_rin_s;
	CString	m_rout_i;
	CString	m_rout_p;
	CString	m_rout_s;
	CString	m_tpoints;
	CString	m_walkoff_i;
	CString	m_walkoff_p;
	CString	m_walkoff_s;
	CString	m_wavelength_i;
	CString	m_wavelength_p;
	CString	m_wavelength_s;
	CString	m_xgrid;
	CString	m_xpoints;
	CString	m_ygrid;
	CString	m_ypoints;
	CString	m_zstep;
	CString	m_beta_i1;
	CString	m_beta_i2;
	CString	m_beta_i3;
	CString	m_beta_p1;
	CString	m_beta_p2;
	CString	m_beta_p3;
	CString	m_beta_s1;
	CString	m_beta_s2;
	CString	m_beta_s3;
	CString	m_n2_i1;
	CString	m_n2_i2;
	CString	m_n2_i3;
	CString	m_n2_p1;
	CString	m_n2_p2;
	CString	m_n2_p3;
	CString	m_n2_s1;
	CString	m_n2_s2;
	CString	m_n2_s3;
	//}}AFX_DATA

	CString m_Run, m_Analyze;

	CGraphDlg* pGraphDlg;
	CFrameWnd* pMF;
	CMixView* pView;

	CDeltakDlg	deltakDlg;
	CErgDlg		ergDlg;
	CLensDlg	lensDlg;

	CString m_Location;

	bool m_error, m_bFirstTime, m_bInputsChanged;

	bool TestForErrors();

private:

// Attributes
public:
		
// Operations
public:
	bool m_bDeltakDlgOn;
	bool m_bErgDlgOn;
	bool m_bLensDlgOn;
	int		m_nScanIndex;
	double	m_dkInitial;
	double	m_dkFinal;
	int		m_Ndk;
	double	m_DistInitial;
	double	m_DistFinal;
	int		m_Ndist;
	double	m_Eidle_f;
	double	m_Eidle_i;
	double	m_Epump_f;
	double	m_Epump_i;
	double	m_Esig_f;
	double	m_Esig_i;
	int		m_Nerg;
	double*	m_pBeamDiamArray;
	double*	m_pRadCurvArray;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMixView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMixView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMixView)
	afx_msg void OnAccept();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetfocusAlphaI();
	afx_msg void OnSetfocusAlphaP();
	afx_msg void OnSetfocusAlphaS();
	afx_msg void OnSetfocusBeamdiamI();
	afx_msg void OnSetfocusBeamdiamP();
	afx_msg void OnSetfocusBeamdiamS();
	afx_msg void OnSetfocusCrystlength();
	afx_msg void OnSetfocusDeff();
	afx_msg void OnSetfocusDelayI();
	afx_msg void OnSetfocusDelayS();
	afx_msg void OnSetfocusDeltak();
	afx_msg void OnSetfocusDist();
	afx_msg void OnSetfocusDurationI();
	afx_msg void OnSetfocusDurationP();
	afx_msg void OnSetfocusDurationS();
	afx_msg void OnSetfocusEnergyI();
	afx_msg void OnSetfocusEnergyP();
	afx_msg void OnSetfocusEnergyS();
	afx_msg void OnSetfocusGaussI();
	afx_msg void OnSetfocusGaussP();
	afx_msg void OnSetfocusGaussS();
	afx_msg void OnSetfocusGvdI();
	afx_msg void OnSetfocusGvdP();
	afx_msg void OnSetfocusGvdS();
	afx_msg void OnSetfocusGviI();
	afx_msg void OnSetfocusGviP();
	afx_msg void OnSetfocusGviS();
	afx_msg void OnSetfocusIndexI();
	afx_msg void OnSetfocusIndexP();
	afx_msg void OnSetfocusIndexS();
	afx_msg void OnSetfocusOffsetI();
	afx_msg void OnSetfocusOffsetS();
	afx_msg void OnSetfocusPhaseI();
	afx_msg void OnSetfocusPhaseP();
	afx_msg void OnSetfocusPhaseS();
	afx_msg void OnSetfocusRadcurvI();
	afx_msg void OnSetfocusRadcurvP();
	afx_msg void OnSetfocusRadcurvS();
	afx_msg void OnSetfocusRinI();
	afx_msg void OnSetfocusRinP();
	afx_msg void OnSetfocusRinS();
	afx_msg void OnSetfocusRoutI();
	afx_msg void OnSetfocusRoutP();
	afx_msg void OnSetfocusRoutS();
	afx_msg void OnSetfocusTpoints();
	afx_msg void OnSetfocusWalkoffI();
	afx_msg void OnSetfocusWalkoffP();
	afx_msg void OnSetfocusWalkoffS();
	afx_msg void OnSetfocusWavelengthI();
	afx_msg void OnSetfocusWavelengthP();
	afx_msg void OnSetfocusWavelengthS();
	afx_msg void OnSetfocusXgrid();
	afx_msg void OnSetfocusXpoints();
	afx_msg void OnSetfocusYgrid();
	afx_msg void OnSetfocusYpoints();
	afx_msg void OnSetfocusZstep();
	afx_msg void OnSetfocusBetaI1();
	afx_msg void OnSetfocusBetaI2();
	afx_msg void OnSetfocusBetaI3();
	afx_msg void OnSetfocusBetaP1();
	afx_msg void OnSetfocusBetaP2();
	afx_msg void OnSetfocusBetaP3();
	afx_msg void OnSetfocusBetaS1();
	afx_msg void OnSetfocusBetaS2();
	afx_msg void OnSetfocusBetaS3();
	afx_msg void OnSetfocusN2I1();
	afx_msg void OnSetfocusN2I2();
	afx_msg void OnSetfocusN2I3();
	afx_msg void OnSetfocusN2P1();
	afx_msg void OnSetfocusN2P2();
	afx_msg void OnSetfocusN2P3();
	afx_msg void OnSetfocusN2S1();
	afx_msg void OnSetfocusN2S2();
	afx_msg void OnSetfocusN2S3();
	afx_msg void OnChangeAlphaI();
	afx_msg void OnChangeAlphaP();
	afx_msg void OnChangeAlphaS();
	afx_msg void OnChangeBeamdiamI();
	afx_msg void OnChangeBeamdiamP();
	afx_msg void OnChangeBeamdiamS();
	afx_msg void OnChangeBetaI1();
	afx_msg void OnChangeBetaI2();
	afx_msg void OnChangeBetaI3();
	afx_msg void OnChangeBetaP1();
	afx_msg void OnChangeBetaP2();
	afx_msg void OnChangeBetaP3();
	afx_msg void OnChangeBetaS1();
	afx_msg void OnChangeBetaS2();
	afx_msg void OnChangeBetaS3();
	afx_msg void OnChangeCrystlength();
	afx_msg void OnChangeDeff();
	afx_msg void OnChangeDelayI();
	afx_msg void OnChangeDelayS();
	afx_msg void OnChangeDeltak();
	afx_msg void OnChangeDist();
	afx_msg void OnChangeDurationI();
	afx_msg void OnChangeDurationP();
	afx_msg void OnChangeDurationS();
	afx_msg void OnChangeEnergyI();
	afx_msg void OnChangeEnergyP();
	afx_msg void OnChangeEnergyS();
	afx_msg void OnChangeGaussI();
	afx_msg void OnChangeGaussP();
	afx_msg void OnChangeGaussS();
	afx_msg void OnChangeGvdI();
	afx_msg void OnChangeGvdP();
	afx_msg void OnChangeGvdS();
	afx_msg void OnChangeGviI();
	afx_msg void OnChangeGviP();
	afx_msg void OnChangeGviS();
	afx_msg void OnChangeIndexI();
	afx_msg void OnChangeIndexP();
	afx_msg void OnChangeIndexS();
	afx_msg void OnChangeN2I1();
	afx_msg void OnChangeN2I2();
	afx_msg void OnChangeN2I3();
	afx_msg void OnChangeN2P1();
	afx_msg void OnChangeN2P2();
	afx_msg void OnChangeN2P3();
	afx_msg void OnChangeN2S1();
	afx_msg void OnChangeN2S2();
	afx_msg void OnChangeN2S3();
	afx_msg void OnChangeOffsetI();
	afx_msg void OnChangeOffsetS();
	afx_msg void OnChangePhaseI();
	afx_msg void OnChangePhaseP();
	afx_msg void OnChangePhaseS();
	afx_msg void OnChangeRadcurvI();
	afx_msg void OnChangeRadcurvP();
	afx_msg void OnChangeRadcurvS();
	afx_msg void OnChangeRinI();
	afx_msg void OnChangeRinP();
	afx_msg void OnChangeRinS();
	afx_msg void OnChangeRoutI();
	afx_msg void OnChangeRoutP();
	afx_msg void OnChangeRoutS();
	afx_msg void OnChangeTpoints();
	afx_msg void OnChangeWalkoffI();
	afx_msg void OnChangeWalkoffP();
	afx_msg void OnChangeWalkoffS();
	afx_msg void OnChangeWavelengthI();
	afx_msg void OnChangeWavelengthP();
	afx_msg void OnChangeWavelengthS();
	afx_msg void OnChangeXgrid();
	afx_msg void OnChangeXpoints();
	afx_msg void OnChangeYgrid();
	afx_msg void OnChangeYpoints();
	afx_msg void OnChangeZstep();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExtrasKvec();
	afx_msg void OnUpdateExtrasKvec(CCmdUI* pCmdUI);
	afx_msg void OnRunErgScan();
	afx_msg void OnRunDeltakScan();
	afx_msg void OnExtrasErg();
	afx_msg void OnUpdateExtrasErg(CCmdUI* pCmdUI);
	afx_msg void OnExtrasLens();
	afx_msg void OnUpdateExtrasLens(CCmdUI* pCmdUI);
	afx_msg void OnRunLensScan();
	//}}AFX_MSG
	afx_msg LRESULT OnUserApply(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIXVIEW_H__9D4D88A1_3D4C_11D4_9234_00A0240EAC17__INCLUDED_)
