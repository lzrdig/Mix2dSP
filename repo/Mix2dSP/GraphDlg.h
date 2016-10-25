//{{AFX_INCLUDES()
//#include "vsgraph.h"
//}}AFX_INCLUDES
#if !defined(AFX_GRAPHDLG_H__4D25CAA0_B5CB_11D4_A89B_00B0D0994500__INCLUDED_)
#define AFX_GRAPHDLG_H__4D25CAA0_B5CB_11D4_A89B_00B0D0994500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDlg.h : header file
//

#include "Inputs.h"
#include "Beams.h"
#include "afxwin.h"
#include "Mix_Prog.h"
#include "Gldocument.h"
//#include "Plot3dDocTemplate.h"
#include "Plot3dFrameWnd.h"
#include "Plot3dCtrl.h"

class CMixView;
class CPlot3dView;

// #defines that are necessary for printing the chart

// chart area view ratio (ie. height/width) for hardcopy
#define	CHART_VIEW_RATIO	(1.0/sqrt(2.0))

// plot area view ratio (ie. height/width) for hardcopy
#define	PLOT_VIEW_RATIO		0.6

#define	NEAREST_INT(a)		((int) a + ((fmod(a, 1.0) > 0.5) ? 1 : 0))

#define	STD_SYMBOL_SIZE		7
#define	STD_LINE_WIDTH		1


/////////////////////////////////////////////////////////////////////////////
// CGraphDlg dialog

class CGraphDlg : public CDialog
{
// Construction
public:
	CGraphDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGraphDlg)
	enum { IDD = IDD_GRAPHDLG };
	CProgressCtrl	m_ProgScan;
	CProgressCtrl	m_ProgT;
	CProgressCtrl	m_ProgS;
	int		m_type_sig;
	int		m_type_idl;
	int		m_type_pum;
	int		m_Beam_FT;
	//}}AFX_DATA

	// global variables
	CRect m_Rect;
	CInputs m_Inputs;

	double m_InFl_s, m_InFl_i, m_InFl_p;	// input peak fluence
	double m_InIr_s, m_InIr_i, m_InIr_p;	// input peak irradiance
	double m_InPow_s, m_InPow_i, m_InPow_p;	// input peak power
	double m_OutIr_s, m_OutIr_i, m_OutIr_p;	// output peak irradiance
	double m_OutE_s, m_OutE_i, m_OutE_p;	// output pulse energy

	long m_nx, m_ny, m_nt;					// grid sizes in x,y,t
	double m_dx, m_dy, m_dt;				// increment in x,y,t
	long m_XFactor;				// smoothing factor for FFT's; the same as used in 
								// Mix_Prog.cpp (CalculateOutput)
	long m_ngridx, m_ngridy, m_padval;
	double m_dthetax, m_dthetay;
	double m_maxgridx, m_maxgridy;
	double m_Xmin_fl,m_Xmax_fl,m_Ymin_fl,m_Ymax_fl;	// max and min grid values for fluence
	double m_Minx_ff,m_Maxx_ff;	// max and min grid values for farfield
	double m_Miny_ff,m_Maxy_ff;

	double m_Rot_X, m_Rot_Z;

	// Boolean variables for MixView
	bool m_brun, m_banalyze;

	// Spectra and Power variables
	double m_Smax, m_Smin;	// min and max x values
	double m_Pmax, m_Pmin;
	double m_Smaxval_s,m_Smaxval_i,m_Smaxval_p;	// max y values for spectra
	double m_Pmaxval_s,m_Pmaxval_i,m_Pmaxval_p;	// max y values for power
	double m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp;	// m-sq avg (wo) in time
	double m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp;	// m-sq avg (wo) in freq
	double m_mavgp_ts, m_mavgp_ti, m_mavgp_tp;		// m-sq avg (perp. to wo) in time
	double m_mavgp_ws, m_mavgp_wi, m_mavgp_wp;		// m-sq avg (perp. to wo) in freq

	// Printing variables
	double m_Maxprint, m_Minprint, m_Ntprint;
	double m_Xmax_print, m_Xmin_print, m_Ymax_print, m_Ymin_print;
	double m_deltax_print, m_deltay_print;

	long m_FFgridx, m_FFgridy;
	
	CString m_Type;	// for the printer button
	bool m_b2d;

	// Plot data
	Fluences *m_pFluenceData;

	// Pointer to the Main frame of input window
	CFrameWnd* m_pMixFrame;
	CMixView* m_pMixView;
	void GetFramePointers(CFrameWnd* pFrm,CMixView* pMixView);

	// Beam analysis variables
	double m_Time_min, m_Time_max, m_Freq_min, m_Freq_max;


	// Functions	
	void SetEnableBtns(BOOL bEnableBtns);

	// Progress bar indicators
	CProgressCtrl *pProgS, *pProgT;

	// Pointer to this class
	CWnd *m_pParent;

	// Plots window
	CPlot3dView* m_pPlot3DView;
	CPlot3dFrameWnd* m_pPlot3DFrame;
	CPlot3dDoc* m_pPlot3DDoc;
	CPlot3dCtrl m_Plot3dCtrl;
protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	afx_msg void OnRunbtn();
	virtual void OnCancel();

protected:

	// Generated message map functions
	//{{AFX_MSG(CGraphDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTypeSignal();
	afx_msg void OnTypeIdle();
	afx_msg void OnTypePump();
	afx_msg void OnFluence();
	afx_msg void OnFarfield();
	afx_msg void OnMovie();
	afx_msg void OnNormpower();
	afx_msg void OnPower();
	afx_msg void OnSpectra();
	afx_msg void OnAnalyze();
	afx_msg void OnFocusP();
	afx_msg void OnFocusW();
	afx_msg void OnMP();
	afx_msg void OnMW();
	afx_msg void OnTilt();
	afx_msg void OnBeamFt();
	afx_msg void OnBeamFt2();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLoadPlotData();
	afx_msg void CreatePlotWindow(CWnd *pParent);
	//}}AFX_MSG
	afx_msg LRESULT OnUserRundk(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_cPlotFrm;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDLG_H__4D25CAA0_B5CB_11D4_A89B_00B0D0994500__INCLUDED_)
