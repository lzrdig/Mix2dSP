// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "GraphDlg.h"
#include "MixView.h"
#include "Plot3dView.h"
#include "Plot3dDoc.h"
#include "Plot3dCtrl.h"
#include "PlotPropDlg.h"

#include "Inputs.h"
#include "Fields.h"
#include "GlobVar.h"
#include "DecIncl.h"
#include "Mix_Prog.h"

#include "MainFrm.h"

#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4189)
#pragma warning(disable:4100)

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USERRUNDK WM_USER + 6

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg dialog


CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraphDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_type_sig = -1;
	m_type_idl = -1;
	m_type_pum = -1;
	m_Beam_FT = -1;
	//}}AFX_DATA_INIT
	// Initialize global variables
	m_InFl_s = 0.0;
	m_InFl_i = 0.0;
	m_InFl_p = 0.0;
	m_InIr_s = 0.0;
	m_InIr_i = 0.0;
	m_InIr_p = 0.0;
	m_InPow_s = 0.0;
	m_InPow_i = 0.0;
	m_InPow_p = 0.0;
	m_OutIr_s = 0.0;
	m_OutIr_i = 0.0;
	m_OutIr_p = 0.0;
	m_OutE_s = 0.0;
	m_OutE_i = 0.0;
	m_OutE_p = 0.0;

	m_nx = 0;
	m_ny = 0;
	m_nt = 0;
	m_dx = 0.0;
	m_dy = 0.0;
	m_dt = 0.0;
	m_XFactor = 0;

	m_Xmin_fl = 0.0;
	m_Xmax_fl = 0.0;
	m_Ymin_fl = 0.0;
	m_Ymax_fl = 0.0;

	m_Minx_ff = 0.0;
	m_Maxx_ff = 0.0;
	m_Miny_ff = 0.0;
	m_Maxy_ff = 0.0;

	m_Rot_X = 15;
	m_Rot_Z = 225;

	m_Smax = 0;
	m_Smin = 0;

	m_Pmax = 0;
	m_Pmin = 0;

	m_Maxprint = 0;
	m_Minprint = 0;
	m_Ntprint = 0;

	m_Xmax_print = 0;
	m_Xmin_print = 0;
	m_Ymax_print = 0;
	m_Ymin_print = 0;

	m_Smaxval_s = 0.0;
	m_Smaxval_i = 0.0;
	m_Smaxval_p = 0.0;

	m_Pmaxval_s = 0.0;
	m_Pmaxval_i = 0.0;
	m_Pmaxval_p = 0.0;

	m_Time_min = 0.0;
	m_Time_max = 0.0;
	m_Freq_min = 0.0;
	m_Freq_max = 0.0;

	m_FFgridx = 0;
	m_FFgridy = 0;

	m_mavgwo_ts = 0.0;
	m_mavgwo_ti = 0.0;
	m_mavgwo_tp = 0.0;

	m_mavgp_ts = 0.0;
	m_mavgp_ti = 0.0;
	m_mavgp_tp = 0.0;

	m_mavgwo_ws = 0.0;
	m_mavgwo_wi = 0.0;
	m_mavgwo_wp = 0.0;

	m_mavgp_ws = 0.0;
	m_mavgp_wi = 0.0;
	m_mavgp_wp = 0.0;
	
	m_b2d = false;

	m_pPlot3DView = NULL;
	m_pPlot3DFrame = NULL;
	m_pPlot3DDoc = NULL;

	m_pFluenceData = NULL;
}


void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphDlg)
	DDX_Control(pDX, IDC_PROGRESS_SCAN, m_ProgScan);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_ProgT);
	DDX_Control(pDX, IDC_PROGRESS_SLICE, m_ProgS);
	DDX_Radio(pDX, IDC_TYPE_SIG, m_type_sig);
	DDX_Radio(pDX, IDC_TYPE_IDL, m_type_idl);
	DDX_Radio(pDX, IDC_TYPE_PUM, m_type_pum);
	DDX_Radio(pDX, IDC_BEAM_FT, m_Beam_FT);
	DDX_Control(pDX, IDC_PLOTFRM, m_cPlotFrm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	//{{AFX_MSG_MAP(CGraphDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TYPE_SIG, OnTypeSignal)
	ON_BN_CLICKED(IDC_TYPE_IDL, OnTypeIdle)
	ON_BN_CLICKED(IDC_TYPE_PUM, OnTypePump)
	ON_BN_CLICKED(IDC_FLUENCE, OnFluence)
	ON_BN_CLICKED(IDC_FARFIELD, OnFarfield)
	ON_BN_CLICKED(IDC_MOVIE, OnMovie)
	ON_BN_CLICKED(IDC_NORMPOWER, OnNormpower)
	ON_BN_CLICKED(IDC_POWER, OnPower)
	ON_BN_CLICKED(IDC_SPECTRA, OnSpectra)
	ON_BN_CLICKED(IDC_ANALYZE, OnAnalyze)
	ON_BN_CLICKED(IDC_FOCUS_P, OnFocusP)
	ON_BN_CLICKED(IDC_FOCUS_W, OnFocusW)
	ON_BN_CLICKED(IDC_M_P, OnMP)
	ON_BN_CLICKED(IDC_M_W, OnMW)
	ON_BN_CLICKED(IDC_TILT, OnTilt)
	ON_BN_CLICKED(IDC_BEAM_FT, OnBeamFt)
	ON_BN_CLICKED(IDC_BEAM_FT2, OnBeamFt2)
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_RUNBTN, OnRunbtn)
	ON_BN_CLICKED(IDC_LOADPLOT, OnLoadPlotData)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USERRUNDK, OnUserRundk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg message handlers

HBRUSH CGraphDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	
	//*****************************************************************
	// Set the text colors for Signal,Idler, and Pump in the dialog box

	if(pWnd->GetDlgCtrlID() == IDC_TYPE_SIG)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	if(pWnd->GetDlgCtrlID() == IDC_TYPE_IDL)
	{
		pDC->SetTextColor(RGB(0,0,255));
	}
	if(pWnd->GetDlgCtrlID() == IDC_TYPE_PUM)
	{
		pDC->SetTextColor(RGB(0,100,0));
	}
	
	return hbr;
}


BOOL CGraphDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Create the plot window
	CreatePlotWindow((CWnd*)this);

	// Initialize the pump button for the display box
	CheckRadioButton(IDC_TYPE_SIG,IDC_TYPE_IDL,IDC_TYPE_PUM);

	// Initialize the frequency button for the beam analysis box
	CheckRadioButton(IDC_BEAM_FT,IDC_BEAM_FT2,IDC_BEAM_FT);	

	// Disable all buttons except for the 'Run' button
	SetEnableBtns(false);
		
	// Set up the progress bars
	pProgS = &m_ProgS;
	pProgT = &m_ProgT;
	
	pProgS->SetRange(0,100);
	pProgT->SetRange(0,100);

	pProgS->SetPos(0);
	pProgT->SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//************************** Run Button ********************************
//
//
//
//**********************************************************************

void CGraphDlg::OnRunbtn() 
{
	
//**********************************************************************
// Check to see if button is clicked as Run or Stop

	CString str; 
	GetDlgItemText(IDC_RUNBTN, str);
	if(str == "Run")
	{
		CString stop = "Stop";
		SetDlgItemText(IDC_RUNBTN,stop);
		SetStop(false);
	}
	else if((str == "Stop"))
	{
		CString run = "Run";
		SetDlgItemText(IDC_RUNBTN,run);
		SetStop(true);
		SetDlgItemText(IDC_GRAPH_HELP," ");

		// Let MixView know that the run was not completed
		m_brun = false;

		// Update the progress bars
		pProgS->SetPos(0);
		pProgT->SetPos(0);

		return;
	}

//**********************************************************************
// Reset the global variables and define local variables

	m_nx = 0;
	m_ny = 0;
	m_nt = 0;
	m_dx = 0.0;
	m_dy = 0.0;
	m_dt = 0.0;
	m_XFactor = 0;

	m_Xmin_fl = 0.0;
	m_Xmax_fl = 0.0;
	m_Ymin_fl = 0.0;
	m_Ymax_fl = 0.0;

	m_Minx_ff = 0.0;
	m_Maxx_ff = 0.0;
	m_Miny_ff = 0.0;
	m_Maxy_ff = 0.0;

	m_Rot_X = 15;
	m_Rot_Z = 225;

	m_Smax = 0;
	m_Smin = 0;

	m_Pmax = 0;
	m_Pmin = 0;

	m_Maxprint = 0;
	m_Minprint = 0;
	m_Ntprint = 0;

	m_Xmax_print = 0;
	m_Xmin_print = 0;
	m_Ymax_print = 0;
	m_Ymin_print = 0;

	m_Smaxval_s = 0.0;
	m_Smaxval_i = 0.0;
	m_Smaxval_p = 0.0;

	m_Pmaxval_s = 0.0;
	m_Pmaxval_i = 0.0;
	m_Pmaxval_p = 0.0;

	m_FFgridx = 0;
	m_FFgridy = 0;
	
	m_b2d = false;
	m_Type = "";
		
	SetRotation(m_Rot_X,m_Rot_Z);

	SetEnableBtns(false);

	// Set the progress bars to zero
	pProgS->SetPos(0);
	pProgT->SetPos(0);

	// Enable the rotation buttons for the graph
	GetDlgItem(IDC_UPROT)->EnableWindow(true);
	GetDlgItem(IDC_DOWNROT)->EnableWindow(true);
	GetDlgItem(IDC_LEFTROT)->EnableWindow(true);
	GetDlgItem(IDC_RIGHTROT)->EnableWindow(true);
	GetDlgItem(IDC_RESETROT)->EnableWindow(true);


//**********************************************************************
// Read data from the input file and convert the variables to SI units

	if(!GetStop())
	{
		SetDlgItemText(IDC_GRAPH_HELP,"Initializing...");
	}

	// Create objects for the input values and global variables
	CInputs* pInputs;
	pInputs = new CInputs;

	CGlobalVar* pGV;
	pGV = new CGlobalVar;

	// Begin the program
	Reader(pInputs);

	WriteOut(pInputs);

	ConvertToSI(pInputs);

	m_Inputs = *pInputs;	// Set the global inputs object to new values
	
//**********************************************************************
// Create a CFields object

	// Get the values for the array sizes
	long nx = long(pInputs->GetVar(NX));
	long ny = long(pInputs->GetVar(NY));
	long nt = long(pInputs->GetVar(NT));

	// Set global variables to new values
	m_nx = nx;
	m_ny = ny;
	m_nt = nt;
	m_XFactor = 4;	// see 'Spectra' & 'CalculateOutput' in Mix_Prog.cpp

	// Create a CFields object
	CFields* pField;
	pField = new CFields(nx*ny*nt, nx, ny, nt);

	SetGrids(pInputs,pGV,pField);

	// Set global grid variables to current values
	m_dx = pGV->dGetGlobal(DX);
	m_dy = pGV->dGetGlobal(DY);
	m_dt = pGV->dGetGlobal(DT);

	m_Xmin_fl = pGV->dGetGlobal(ORIGINX_FL);
	m_Xmax_fl = -(pGV->dGetGlobal(ORIGINX_FL));
	m_Ymin_fl = pGV->dGetGlobal(ORIGINY_FL);
	m_Ymax_fl = -(pGV->dGetGlobal(ORIGINY_FL));

	m_maxgridx = pInputs->GetVar(MAXGRID_X);
	m_maxgridy = pInputs->GetVar(MAXGRID_Y);
	m_ngridx = pGV->lGetGlobal(NGRIDX);
	m_ngridy = pGV->lGetGlobal(NGRIDY);
	m_padval = pGV->lGetGlobal(PADVAL);

	m_dthetax = pGV->dGetGlobal(DTHETAX);
	m_dthetay = pGV->dGetGlobal(DTHETAY);

	m_Pmin = pField->pTgrid[1];
	m_Pmax = pField->pTgrid[nt];

	pProgS->SetPos(5);


	if(m_bLensOn == TRUE)
	{
		FILE* stream;
		stream=fopen("Mix_outLENS.dat","a+");
		
		fprintf(stream,"%s %5.3e %5.3e",
			"\n Xgrid / Ygrid size (mm)               = ", 
			m_maxgridx, m_maxgridy);

		fclose(stream);
	}
		
	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_TYPE_SIG,IDC_TYPE_PUM);

	if(num == IDC_TYPE_SIG)
	{
		SetRadio(1);
	}
	else if(num == IDC_TYPE_IDL)
	{
		SetRadio(2);
	}
	else if(num == IDC_TYPE_PUM)
	{
		SetRadio(3);
	}


//**********************************************************************
// Begin the calculations

	if(!GetStop())
	{
		SetDlgItemText(IDC_GRAPH_HELP,"Initializing fields and phases...");

		InitializeFields(pInputs,pGV,pField);

		InitializePhases(pInputs,pGV,pField);

		pProgS->SetPos(7);
	}

	if(!GetStop())
	{
		SetDlgItemText(IDC_GRAPH_HELP,"Propagating through the crystal...");

		Integrate(pInputs,pGV,pField,pProgS,pProgT);
	}
	
	if(!GetStop())
	{
		SetDlgItemText(IDC_GRAPH_HELP,"Creating output data files...");

		CalculateOutput(pInputs,pGV,pField,&m_Minx_ff,&m_Maxx_ff,
			&m_Smax,&m_Smin,&m_Smaxval_s,&m_Smaxval_i,&m_Smaxval_p,
			&m_Pmaxval_s,&m_Pmaxval_i,&m_Pmaxval_p,pProgS);
	}
	
	
//**********************************************************************
// Set the global variables for input and output info.

	if(!GetStop())
	{
		m_InFl_s = pGV->dGetGlobal(PEAKFL_S);
		m_InFl_i = pGV->dGetGlobal(PEAKFL_I);
		m_InFl_p = pGV->dGetGlobal(PEAKFL_P);
		m_InIr_s = pGV->dGetGlobal(PEAKIR_S);
		m_InIr_i = pGV->dGetGlobal(PEAKIR_I);
		m_InIr_p = pGV->dGetGlobal(PEAKIR_P);
		m_InPow_s = pGV->dGetGlobal(PEAKPOW_S);
		m_InPow_i = pGV->dGetGlobal(PEAKPOW_I);
		m_InPow_p = pGV->dGetGlobal(PEAKPOW_P);
		m_OutIr_s = pGV->dGetGlobal(IROUT_S);
		m_OutIr_i = pGV->dGetGlobal(IROUT_I);
		m_OutIr_p = pGV->dGetGlobal(IROUT_P);
		m_OutE_s = pGV->dGetGlobal(EPULSE_S);
		m_OutE_i = pGV->dGetGlobal(EPULSE_I);
		m_OutE_p = pGV->dGetGlobal(EPULSE_P);
	}


//**********************************************************************
// Set the help dialog text to the input and output info.

	if(!GetStop())
	{
		CString strings;
		strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
			"Input Peak Irradiance [W/m^2]:   ",
			m_InIr_s,m_InIr_i,m_InIr_p,
			"\nInput Peak Fluence [J/m^2]:    ",
			m_InFl_s,m_InFl_i,m_InFl_p,
			"\nInput Peak Power [W]:               ",
			m_InPow_s,m_InPow_i,m_InPow_p,
			"\nOutput Peak Irradiance [W/m^2]:",
			m_OutIr_s,m_OutIr_i,m_OutIr_p,
			"\nOutput Pulse Energy [J]:             ",
			m_OutE_s,m_OutE_i,m_OutE_p);

		SetDlgItemText(IDC_GRAPH_HELP,strings);

		pProgS->SetPos(95);

		OnFluence();

		pProgS->SetPos(99);
		
	}


//**********************************************************************
// Check to see if the 'Stop' button was pressed

	if( GetStop() )
	{
		CString run = "Run";
		SetDlgItemText(IDC_RUNBTN,run);
		delete pInputs;
		delete pGV;
		delete pField;

		SetDlgItemText(IDC_GRAPH_HELP,"");

		// Let MixView know that the run was not completed
		m_brun = false;

		// Update the progress bars
		pProgS->SetPos(0);
		pProgT->SetPos(0);

		return;
	}
	

//**********************************************************************
// Reset the run button, enable the other buttons, and delete pointers

	CString run = "Run";
	SetDlgItemText(IDC_RUNBTN,run);

	SetEnableBtns(true);

	delete pInputs;
	delete pGV;
	delete pField;

	// Let MixView know that the run completed successfully
	m_brun = true;

	// Update the progress bars
	pProgS->SetPos(0);
	pProgT->SetPos(0);

		
}	// End OnRunBtn


//************************** Fluence Button ****************************
// 
//
//
//**********************************************************************

void CGraphDlg::OnFluence() 
{
//***************************************************************************
// Open the 'Fluence.dat' file for plotting

	CString filename = "Fluence.dat";
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Fluence.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	// Define local variables
	int ii,jj,k2;

	// variables for reading the fluence data from the file
	double xval,yval,fluences,fluencei,fluencep;
	int Radval;

	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_TYPE_SIG,IDC_TYPE_PUM);



//**********************************************************************
// Read the fluence data

	double *Temp;
	Temp = dvector(1,m_nx*m_ny);

	double Maxgridx = m_Inputs.GetVar(MAXGRID_X);
	double Maxgridy = m_Inputs.GetVar(MAXGRID_Y);

	// Transfer the fluence data into the chart
	Radval = GetRadio();

	for(ii=0;ii<m_ny;++ii)
	{
		for(jj=0;jj<m_nx;++jj)
		{
			k2 = ii+1 + jj*m_ny;

			fin >> xval;
			fin >> yval;
			fin >> fluences;
			fin >> fluencei;
			fin >> fluencep;

			if(Radval == 1)
			{
				Temp[k2] = fluences;
			}
			else if (Radval == 2)
			{
				Temp[k2] = fluencei;
			}
			else if (Radval == 3)
			{
				Temp[k2] = fluencep;
			}
		}
	}

	// Close fluence.dat
	fin.close();
	

	// Free up memory
	free_dvector(Temp,1,m_nx*m_ny);

	// Rewrite the input and output globals to the help box
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Input Peak Irradiance [W/m^2]:   ",
		m_InIr_s,m_InIr_i,m_InIr_p,
		"\nInput Peak Fluence [J/m^2]:    ",
		m_InFl_s,m_InFl_i,m_InFl_p,
		"\nInput Peak Power [W]:               ",
		m_InPow_s,m_InPow_i,m_InPow_p,
		"\nOutput Peak Irradiance [W/m^2]:",
		m_OutIr_s,m_OutIr_i,m_OutIr_p,
		"\nOutput Pulse Energy [J]:             ",
		m_OutE_s,m_OutE_i,m_OutE_p);

	SetDlgItemText(IDC_GRAPH_HELP,strings);

	m_b2d = false;		// for printing 3d chart
	m_Type = "FLUENCE";
	
}	// End OnFluence


//************************* FarField Button ****************************
//
//
//
//**********************************************************************

void CGraphDlg::OnFarfield() 
{

	// Define local variables
	double Lambda;

	
//***************************************************************************
// Open the 'Farfield.dat' file for plotting

	CString filename = "Farfield.dat";
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Farfield.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

// Define local variables
	int ii,jj,j2;

// Variables for reading the farfield data from the file
	double xval,yval,farfields,farfieldi,farfieldp;
	int Radval;

	double Maxffx, Maxffy, FFpad;
	double Originx, Originy, Maxx, Maxy;

	Maxffx = (m_nx - 1)*m_dthetax;
	Maxffy = (m_ny - 1)*m_dthetay;

	Originx = -(0.5*m_nx)*m_dthetax;
	Originy = -(0.5*m_ny)*m_dthetay;

	Maxx = (0.5*m_nx - 1)*m_dthetax;
	Maxy = (0.5*m_ny - 1)*m_dthetay;

// If nx doesn't equal ny, choose the larger dimension
	if(Maxffx > Maxffy)
	{
		m_FFgridx = m_nx;
		m_FFgridy = long(m_ny*(Maxffx/Maxffy));	// long() rounds down

		FFpad = long((m_FFgridy - m_ny)/2);

		Originy = Originy - FFpad*m_dthetay;	
		Maxy = Maxy + FFpad*m_dthetay;
	}
	else if(Maxffx <= Maxffy)
	{
		m_FFgridx = long(m_nx*(Maxffy/Maxffx));	// long() rounds down
		m_FFgridy = m_ny;

		FFpad = long((m_FFgridx - m_nx)/2);

		Originx = Originx - FFpad*m_dthetax;
		Maxx = Maxx + FFpad*m_dthetax;
	}
	
		

//**********************************************************************
// Calculate the new grids 


	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_TYPE_SIG,IDC_TYPE_PUM);

	if(num == IDC_TYPE_SIG)
	{
		Lambda = m_Inputs.GetVar(LAMBDA_S);
		SetRadio(1);
	}
	else if(num == IDC_TYPE_IDL)
	{
		Lambda = m_Inputs.GetVar(LAMBDA_I);
		SetRadio(2);
	}
	else if(num == IDC_TYPE_PUM)
	{
		Lambda = m_Inputs.GetVar(LAMBDA_P);
		SetRadio(3);
	}



	int num2 = GetRadio();

	
//**********************************************************************
// Read the farfield data

	double *Temp;
	Temp = dvector(1,m_nx*m_ny);

	double FFmax;
	FFmax = 0.0;
	// Transfer the farfield data into the chart
	Radval = GetRadio();

	for(ii=0;ii<m_ny;++ii)
	{
		for(jj=0;jj<m_nx;++jj)
		{
			j2 = ii+1 + jj*m_ny;

			fin >> xval;
			fin >> yval;
			fin >> farfields;
			fin >> farfieldi;
			fin >> farfieldp;

			if(Radval == 1)
			{
				Temp[j2] = farfields;

				if(FFmax < Temp[j2])
				{
					FFmax = Temp[j2];
				}

			}
			else if (Radval == 2)
			{
				Temp[j2] = farfieldi;	

					if(FFmax < Temp[j2])
				{
					FFmax = Temp[j2];
				}

			}
			else if (Radval == 3)
			{
				Temp[j2] = farfieldp;

					if(FFmax < Temp[j2])
				{
					FFmax = Temp[j2];
				}

			}
		}
	}
	
	// Close farfield.dat
	fin.close();

	
	// Free up memory
	free_dvector(Temp,1,m_nx*m_ny);

	// Write the input and output globals to the help box
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Input Peak Irradiance [W/m^2]:   ",
		m_InIr_s,m_InIr_i,m_InIr_p,
		"\nInput Peak Fluence [J/m^2]:    ",
		m_InFl_s,m_InFl_i,m_InFl_p,
		"\nInput Peak Power [W]:               ",
		m_InPow_s,m_InPow_i,m_InPow_p,
		"\nOutput Peak Irradiance [W/m^2]:",
		m_OutIr_s,m_OutIr_i,m_OutIr_p,
		"\nOutput Pulse Energy [J]:             ",
		m_OutE_s,m_OutE_i,m_OutE_p);

	SetDlgItemText(IDC_GRAPH_HELP,strings);

	m_b2d = false;		// for printing 3d chart
	m_Type = "FARFIELD";

}	// End OnFarfield


//************************** Movie Button ******************************
//
// 
//
//**********************************************************************

void CGraphDlg::OnMovie() 
{

//**********************************************************************
// Check to see if button is clicked as Run or Stop
	
	CString str; 
	GetDlgItemText(IDC_MOVIE, str);
	if(str == "Movie")
	{
		CString stop = "Stop";
		SetDlgItemText(IDC_MOVIE,stop);
		SetStop(false);
	}
	else if((str == "Stop"))
	{
		CString run = "Movie";
		SetDlgItemText(IDC_MOVIE,run);
		SetStop(true);
		SetDlgItemText(IDC_GRAPH_HELP," ");

		return;
	}

// Define local variables
	CString filename;
	int ii,i2,jj,j2,kk,k2,ll,l2,nn,n2;
	double Maxval, Tempval, rval, ival;
	double *Temp, *TempF, *TempF1, *Irradiance, *Power;
	double *Tempdat, *TField;

	Maxval = 0.0;



//**********************************************************************
// Check the Radio Buttons to see which one is selected

	int num = GetCheckedRadioButton(IDC_TYPE_SIG,IDC_TYPE_PUM);

	if(num == IDC_TYPE_SIG)
	{
		filename = "EField_s.dat";
		
		SetRadio(1);
	}
	else if(num == IDC_TYPE_IDL)
	{
		filename = "EField_i.dat";
				
		SetRadio(2);
	}
	else if(num == IDC_TYPE_PUM)
	{
		filename = "EField_p.dat";
				
		SetRadio(3);
	}


//**********************************************************************
// Read the data from the EField file
	
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Electric fields do not exist.  Press 'Run' to create the files.");
		return;
	}

	// Initialize Temp
	Temp = dvector(1,2*m_nx*(m_ny/2)*m_nt);

	// Transfer the Efield data from the file to Temp
	for(ii=0;ii<m_nx*(m_ny/2)*m_nt;++ii)
	{
		i2=2*(ii+1);

		fin >> rval;
		fin >> ival;

		Temp[i2-1] = rval;
		Temp[i2] = ival;
	}

	fin.close();


//**********************************************************************
// Reset the field for total ny values

	// Initialize Tempdat
	Tempdat = dvector(1,2*m_nx*(m_ny/2)*m_nt);
	
	// Mirror the Tempdat array with Temp values
	for(ii=0;ii<m_nt;++ii)
	{
		for(jj=0;jj<m_nx;++jj)
		{
			for(kk=0;kk<(m_ny/2);++kk)
			{
				k2 = ii + jj*m_nt*(m_ny/2) + kk*m_nt;
				i2 = 2*(k2+1);

				ll = ii + jj*m_nt*(m_ny/2) + ((m_ny/2)-(kk+1))*m_nt;
				l2 = 2*(ll+1);

				Tempdat[i2-1] = Temp[l2-1];
				Tempdat[i2] = Temp[l2];
			}
		}
	}

	// Initialize TField
	TField = dvector(1,2*m_nx*m_ny*m_nt);

	// Fill TField with the values from Temp
	for(ii=0;ii<m_nt;++ii)
	{
		for(jj=0;jj<m_nx;++jj)
		{
			for(kk=0;kk<m_ny;++kk)
			{
				k2 = ii + jj*m_nt*m_ny + kk*m_nt;
				i2 = 2*(k2+1);
	
				ll = ii + jj*m_nt*(m_ny/2) + (kk-(m_ny/2))*m_nt;
				l2 = 2*(ll+1);

				nn = ii + jj*m_nt*(m_ny/2) + kk*m_nt;
				n2 = 2*(nn+1);

				if(kk<(m_ny/2))
				{
					TField[i2-1] = Temp[n2-1];
					TField[i2] = Temp[n2];
				}
				else
				{
					TField[i2-1] = Tempdat[l2-1];
					TField[i2] = Tempdat[l2];
				}
				
			}
		}
	}
				
	// Free up memory
	free_dvector(Temp,1,2*m_nx*(m_ny/2)*m_nt);
	free_dvector(Tempdat,1,2*m_nx*(m_ny/2)*m_nt);
	

//**********************************************************************
// Setup the irradiance array

	// Initialize Irradiance
	Irradiance = dvector(1,m_nx*m_ny*m_nt);

	// Setup the irradiance data
	for(ii=0;ii<m_nx;++ii)
	{
		for(jj=0;jj<m_ny;++jj)
		{
			for(kk=0;kk<m_nt;++kk)
			{
				k2 = kk+1 + jj*m_nt + ii*m_nt*m_ny;
				i2 = 2*k2;

				Tempval = pow(TField[i2-1],2) + pow(TField[i2],2);
				Irradiance[k2] = Tempval;

				if(Tempval > Maxval)
				{
					Maxval = Tempval;
				}
			}
		}
	}

	// Free up memory
	free_dvector(TField,1,2*m_nx*m_ny*m_nt);

	// Normalize the irradiance
	for(ii=0;ii<m_nx*m_ny*m_nt;++ii)
	{
		Irradiance[ii+1] = Irradiance[ii+1]/Maxval;
	}


//**********************************************************************
// Setup the power array

	Maxval = 0.0;

	// Initialize TempF and TempF1
	TempF = dvector(1,m_nt);
	TempF1 = dvector(1,m_ny*m_nt);

	// Set TempF to zero
	for(ii=0;ii<m_nt;++ii)
	{
		TempF[ii+1] = 0.0;
	}

	// Set TempF1 to zero
	for(ii=0;ii<m_ny*m_nt;++ii)
	{
		TempF1[ii+1] = 0.0;
	}

	// Sum irradiance in x-direction
	for(ii=0;ii<m_nt;++ii)
	{
		for(jj=0;jj<m_ny;++jj)
		{
			for(kk=0;kk<m_nx;++kk)
			{
				j2=(ii+1) + jj*m_nt + kk*m_ny*m_nt;
				i2=(jj+1) + ii*m_ny;

				TempF1[i2]=TempF1[i2] + Irradiance[j2]; 
			}
		}
	}

	// Sum irradiance in y-direction
	for(ii=0;ii<m_nt;++ii)
	{
		for(jj=0;jj<m_ny;++jj)
		{
			j2=(jj+1) + ii*m_ny;
			TempF[ii+1] = TempF[ii+1] + TempF1[j2];
		}

		Tempval = TempF[ii+1];

		if(Tempval > Maxval)
		{
			Maxval = Tempval;
		}
	}

	// Free up memory
	free_dvector(TempF1,1,m_ny*m_nt);

	// Initialize Power
	Power = dvector(1,m_nt);

	// Fill the power array
	for(ii=0; ii<m_nt; ++ii)
	{
		Power[ii+1] = TempF[ii+1];
	}

	// Free up memory
	free_dvector(TempF,1,m_nt);



//**********************************************************************
// Plot the irradiance data

	// Transfer the irradiance data into the chart
	for(ii=0;ii<m_nt;++ii)
	{
		CheckWinMessages(1000);
		if( GetStop())
		{
			break;
		}

		if(Power[ii+1] > Maxval*0.001)
		{
			//pChart->SetIsBatched(true);
			//pData->SetIsBatched(true);

			for(jj=0;jj<m_ny;++jj)
			{
				for(kk=0;kk<m_nx;++kk)
				{
					k2 = ii+1 + jj*m_nt + kk*m_nt*m_ny;
	
					//pData->SetValue(kk+1,jj+1,Irradiance[k2]);
				}
			}

			//pChart->SetIsBatched(false);
			//pData->SetIsBatched(false);
			
		}
	}
			
	// Free up memory
	free_dvector(Power,1,m_nt);
	free_dvector(Irradiance,1,2*m_nx*m_ny*m_nt);


//**********************************************************************
// Check to see if the 'Stop' button was pressed 

	if( GetStop() )
	{
		CString run = "Movie";
		SetDlgItemText(IDC_MOVIE,run);

		SetDlgItemText(IDC_GRAPH_HELP,"");

		return;
	}
	

//**********************************************************************
// Reset the movie button 

	CString run = "Movie";
	SetDlgItemText(IDC_MOVIE,run);
			
}	// End OnMovie


//************************* 3d Graphing Buttons ***********************
//
//
//
//*********************************************************************


		//************** OnNormPower Button *****************

void CGraphDlg::OnNormpower() 
{

//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);


	// Open the 'Spectrum.dat' file for plotting
	CString filename = "Power.dat";
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	fin.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "NORMPOWER";

	// Rewrite the input and output globals to the help box
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Input Peak Irradiance [W/m^2]:   ",
		m_InIr_s,m_InIr_i,m_InIr_p,
		"\nInput Peak Fluence [J/m^2]:    ",
		m_InFl_s,m_InFl_i,m_InFl_p,
		"\nInput Peak Power [W]:               ",
		m_InPow_s,m_InPow_i,m_InPow_p,
		"\nOutput Peak Irradiance [W/m^2]:",
		m_OutIr_s,m_OutIr_i,m_OutIr_p,
		"\nOutput Pulse Energy [J]:             ",
		m_OutE_s,m_OutE_i,m_OutE_p);

	SetDlgItemText(IDC_GRAPH_HELP,strings);
	
}	// End OnNormPower


		//************** OnPower Button *****************

void CGraphDlg::OnPower() 
{
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);


	// Open the 'Spectrum.dat' file for plotting
	CString filename = "Power.dat";
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	fin.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "POWER";

	// Rewrite the input and output globals to the help box
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Input Peak Irradiance [W/m^2]:   ",
		m_InIr_s,m_InIr_i,m_InIr_p,
		"\nInput Peak Fluence [J/m^2]:    ",
		m_InFl_s,m_InFl_i,m_InFl_p,
		"\nInput Peak Power [W]:               ",
		m_InPow_s,m_InPow_i,m_InPow_p,
		"\nOutput Peak Irradiance [W/m^2]:",
		m_OutIr_s,m_OutIr_i,m_OutIr_p,
		"\nOutput Pulse Energy [J]:             ",
		m_OutE_s,m_OutE_i,m_OutE_p);

	SetDlgItemText(IDC_GRAPH_HELP,strings);
	
}	// End OnPower


		//*********** OnSpectra Button **********

void CGraphDlg::OnSpectra() 
{
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);

	// Open the 'Spectrum.dat' file for plotting
	CString filename = "Spectra.dat";
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Spectra.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	fin.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "SPECTRA";

	// Rewrite the input and output globals to the help box
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Input Peak Irradiance [W/m^2]:   ",
		m_InIr_s,m_InIr_i,m_InIr_p,
		"\nInput Peak Fluence [J/m^2]:    ",
		m_InFl_s,m_InFl_i,m_InFl_p,
		"\nInput Peak Power [W]:               ",
		m_InPow_s,m_InPow_i,m_InPow_p,
		"\nOutput Peak Irradiance [W/m^2]:",
		m_OutIr_s,m_OutIr_i,m_OutIr_p,
		"\nOutput Pulse Energy [J]:             ",
		m_OutE_s,m_OutE_i,m_OutE_p);

	SetDlgItemText(IDC_GRAPH_HELP,strings);

}	// End OnSpectra


//********************** Beam Analysis Functions **********************
//
//
//
//*********************************************************************
		//***************** OnAnalyze Button *************************
void CGraphDlg::OnAnalyze() 
{

//**********************************************************************
// Check to see if button is clicked as Analyze or Stop

	CString str; 
	GetDlgItemText(IDC_ANALYZE, str);
	if(str == "Analyze")
	{
		CString stop = "Stop";
		SetDlgItemText(IDC_ANALYZE,stop);
		SetStop(false);
	}
	else if((str == "Stop"))
	{
		CString run = "Analyze";
		SetDlgItemText(IDC_ANALYZE,run);
		SetStop(true);
		SetDlgItemText(IDC_GRAPH_HELP," ");

		// Let MixView know that analyze was not completed
		m_banalyze = false;

		// Update the progress bars
		pProgS->SetPos(0);
		pProgT->SetPos(0);

		return;
	}

	// Update the progress bars
	pProgS->SetPos(0);
	pProgT->SetPos(0);

	// Reset the m-sqr global variables to zero
	m_mavgwo_ts = 0.0;
	m_mavgwo_ti = 0.0;
	m_mavgwo_tp = 0.0;

	m_mavgp_ts = 0.0;
	m_mavgp_ti = 0.0;
	m_mavgp_tp = 0.0;

	m_mavgwo_ws = 0.0;
	m_mavgwo_wi = 0.0;
	m_mavgwo_wp = 0.0;

	m_mavgp_ws = 0.0;
	m_mavgp_wi = 0.0;
	m_mavgp_wp = 0.0;

	// Initialize local variables
	float *Power_s, *Power_i, *Power_p;
	float *Spectrum_s, *Spectrum_i, *Spectrum_p;
	float *EField;
	double *Msavgt, *Miavgt, *Mpavgt;
	double *Msavgw, *Miavgw, *Mpavgw;

	double Pmax_s, Pmax_i, Pmax_p;
	double Smax_s, Smax_i, Smax_p;

	CBeams *pBeam, *pSBeam;
	
	double rval,ival;
	double sgrid;
	
	FILE* stream;

	double Lambda_s = m_Inputs.GetVar(LAMBDA_S);
	double Lambda_i = m_Inputs.GetVar(LAMBDA_I);
	double Lambda_p = m_Inputs.GetVar(LAMBDA_P);
	
	long nx = m_nx;
	long ny = m_ny;
	long nt = m_nt;
	double dx = m_dx;
	double dy = m_dy;
	double dt = m_dt;

	long XFactor = 4;	

	Msavgt = dvector(1,2);
	Miavgt = dvector(1,2);
	Mpavgt = dvector(1,2);
	Msavgw = dvector(1,2);
	Miavgw = dvector(1,2);
	Mpavgw = dvector(1,2);

	int ii,i2;

	CString filename;

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Performing beam analysis...");


//******************************************************************
// Setup the power and spectrum arrays
	
	// Initialize power arrays
	Power_s = vector(1,nt);
	Power_i = vector(1,nt);
	Power_p = vector(1,nt);

	Pmax_s = 0.0;
	Pmax_i = 0.0;
	Pmax_p = 0.0;

	// Initialize spectrum arrays
	Spectrum_s = vector(1,XFactor*nt);
	Spectrum_i = vector(1,XFactor*nt);
	Spectrum_p = vector(1,XFactor*nt);

	Smax_s = 0.0;
	Smax_i = 0.0;
	Smax_p = 0.0;

	// Initialize pBeam and pSBeam (and the global CBeams)
	pBeam = new CBeams(nt);
	pSBeam = new CBeams(XFactor*nt);

	
//**********************************************************************
// Do beam analysis for the signal

	// Initialize the EField
	EField = vector(1,2*nx*(ny/2)*nt);

	if(!GetStop())
	{

	// Setup normalized power data
	filename = "EField_s.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("EField_s.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	// Find the leftmost array value greater than 0.001 norm power
	for(ii=0;ii<nx*(ny/2)*nt;++ii)
	{
		i2 = 2*(ii+1);

		fins >> rval;
		fins >> ival;

		EField[i2-1] = rval;
		EField[i2] = ival;
	}
	fins.close();	// EField_s.dat

	}	// End if(!GetStop())

	//******************************************************************
	// Do time and frequency processing, and save values to the 
	// output files 'Beams_ts.dat' and 'Beams_ws.dat'

	if(!GetStop())
	{

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Time processing for the signal...");

	
	// Call AnalyzeBeams_t for time processing
	AnalyzeBeams_t(Power_s,EField,pBeam,Lambda_s,Pmax_s,
				   nx,ny,nt,dx,dy,dt,pProgS,pProgT,0,Msavgt);

	// 'Beams_ts.dat'
	stream=fopen("Beams_ts.dat","w");
	for(ii=0;ii<nt;++ii)
	{
		fprintf(stream,"%e %e %e %e %e %e %e %e %e %s",
			dt*(ii+1),Power_s[ii+1],pBeam->GetMsqrx(ii+1),Msavgt[1],
			pBeam->GetMsqry(ii+1),Msavgt[2],
			pBeam->GetWavofCurvx(ii+1),pBeam->GetWavofCurvy(ii+1),
			pBeam->GetTiltx(ii+1),"\n");
	}
	fclose(stream);

	// Update the progress bars
	pProgT->SetPos(15);

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Frequency processing for the signal...");

	// Call AnalyzeBeams_w for frequency processing
	AnalyzeBeams_w(Spectrum_s,EField,pSBeam,Lambda_s,Smax_s,
				   nx,ny,nt,dx,dy,dt,pProgS,pProgT,15,Msavgw);

	// 'Beams_ws.dat'
	stream=fopen("Beams_ws.dat","w");
	for(ii=0;ii<XFactor*nt;++ii)
	{
		sgrid=(1/(1.e12*XFactor*nt*dt))*((ii+1)-(1+0.5*XFactor*nt));

		if(ii==0)
		{
			m_Freq_min = sgrid;
		}

		fprintf(stream,"%e %e %e %e %e %e %e %e %e %s",
			sgrid,Spectrum_s[ii+1],pSBeam->GetMsqrx(ii+1),Msavgw[1],
			pSBeam->GetMsqry(ii+1),Msavgw[2],
			pSBeam->GetWavofCurvx(ii+1),pSBeam->GetWavofCurvy(ii+1),
			pSBeam->GetTiltx(ii+1),"\n");
	}
	fclose(stream);

	// Update the progress bars
	pProgT->SetPos(30);

	// Set global variables for the time and frequency axes
	m_Time_min = dt;
	m_Time_max = dt*nt;
	m_Freq_max = sgrid;

	}	// End if(!GetStop())
	
	// Free up memory
	free_vector(EField,1,2*nx*(ny/2)*nt);
	free_vector(Power_s,1,nt);
	free_vector(Spectrum_s,1,XFactor*nt);


//**********************************************************************
// Do beam analysis for the idler

	// Initialize the EField
	EField = vector(1,2*nx*(ny/2)*nt);

	if(!GetStop())
	{

	// Setup normalized power data
	filename = "EField_i.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("EField_i.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	// Find the leftmost array value greater than 0.001 norm power
	for(ii=0;ii<nx*(ny/2)*nt;++ii)
	{
		i2 = 2*(ii+1);

		fini >> rval;
		fini >> ival;

		EField[i2-1] = rval;
		EField[i2] = ival;
	}
	fini.close();	// EField_i.dat

	}	// End if(!GetStop())

	//******************************************************************
	// Do time and frequency processing, and save values to the 
	// output files 'Beams_ti.dat' and 'Beams_wi.dat'

	if(!GetStop())
	{

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Time processing for the idler...");

	// Call AnalyzeBeams_t for time processing
	AnalyzeBeams_t(Power_i,EField,pBeam,Lambda_i,Pmax_i,
				   nx,ny,nt,dx,dy,dt,pProgS,pProgT,30,Miavgt);

	// 'Beams_ti.dat'
	stream=fopen("Beams_ti.dat","w");
	for(ii=0;ii<nt;++ii)
	{
		fprintf(stream,"%e %e %e %e %e %e %e %e %e %s",
			dt*(ii+1),Power_i[ii+1],pBeam->GetMsqrx(ii+1),Miavgt[1],
			pBeam->GetMsqry(ii+1),Miavgt[2],
			pBeam->GetWavofCurvx(ii+1),pBeam->GetWavofCurvy(ii+1),
			pBeam->GetTiltx(ii+1),"\n");
	}
	fclose(stream);

	// Update the progress bars
	pProgT->SetPos(45);

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Frequency processing for the idler...");

	// Call AnalyzeBeams_w for frequency processing
	AnalyzeBeams_w(Spectrum_i,EField,pSBeam,Lambda_i,Smax_i,
				   nx,ny,nt,dx,dy,dt,pProgS,pProgT,45,Miavgw);

	// 'Beams_wi.dat'
	stream=fopen("Beams_wi.dat","w");
	for(ii=0;ii<XFactor*nt;++ii)
	{
		sgrid=(1/(1.e12*XFactor*nt*dt))*((ii+1)-(1+0.5*XFactor*nt));

		fprintf(stream,"%e %e %e %e %e %e %e %e %e %s",
			sgrid,Spectrum_i[ii+1],pSBeam->GetMsqrx(ii+1),Miavgw[1],
			pSBeam->GetMsqry(ii+1),Miavgw[2],
			pSBeam->GetWavofCurvx(ii+1),pSBeam->GetWavofCurvy(ii+1),
			pSBeam->GetTiltx(ii+1),"\n");
	}
	fclose(stream);

	// Update the progress bars
	pProgT->SetPos(65);

	}// End if(!GetStop())

	// Free up memory
	free_vector(EField,1,2*nx*(ny/2)*nt);
	free_vector(Power_i,1,nt);
	free_vector(Spectrum_i,1,XFactor*nt);


//**********************************************************************
// Do beam analysis for the pump

	// Initialize the EField
	EField = vector(1,2*nx*(ny/2)*nt);

	if(!GetStop())
	{

	// Setup normalized power data
	filename = "EField_p.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("EField_p.dat does not exist.  Press 'Run' to create the file.");
		return;
	}

	for(ii=0;ii<nx*(ny/2)*nt;++ii)
	{
		i2 = 2*(ii+1);

		finp >> rval;
		finp >> ival;

		EField[i2-1] = rval;
		EField[i2] = ival;
	}
	finp.close();	// EField_p.dat

	}	// End if(!GetStop())
	
	//******************************************************************
	// Do time and frequency processing, and save values to the 
	// output files 'Beams_tp.dat' and 'Beams_wp.dat'

	if(!GetStop())
	{

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Time processing for the pump...");

	// Call AnalyzeBeams_t for time processing
	AnalyzeBeams_t(Power_p,EField,pBeam,Lambda_p,Pmax_p,
				   nx,ny,nt,dx,dy,dt,pProgS,pProgT,65,Mpavgt);

	// 'Beams_tp.dat'
	stream=fopen("Beams_tp.dat","w");
	for(ii=0;ii<nt;++ii)
	{
		fprintf(stream,"%e %e %e %e %e %e %e %e %e %s",
			dt*(ii+1),Power_p[ii+1],pBeam->GetMsqrx(ii+1),Mpavgt[1],
			pBeam->GetMsqry(ii+1),Mpavgt[2],
			pBeam->GetWavofCurvx(ii+1),pBeam->GetWavofCurvy(ii+1),
			pBeam->GetTiltx(ii+1),"\n");
	}
	fclose(stream);

	// Update the progress bars
	pProgT->SetPos(85);

	// Tell the user what's happening
	SetDlgItemText(IDC_GRAPH_HELP,"Frequency processing for the pump...");

	// Call AnalyzeBeams_w for frequency processing
	AnalyzeBeams_w(Spectrum_p,EField,pSBeam,Lambda_p,Smax_p,
				   nx,ny,nt,dx,dy,dt,pProgS,pProgT,85,Mpavgw);

	// 'Beams_wp.dat'
	stream=fopen("Beams_wp.dat","w");
	for(ii=0;ii<XFactor*nt;++ii)
	{
		sgrid=(1/(1.e12*XFactor*nt*dt))*((ii+1)-(1+0.5*XFactor*nt));

		fprintf(stream,"%e %e %e %e %e %e %e %e %e %s",
			sgrid,Spectrum_p[ii+1],pSBeam->GetMsqrx(ii+1),Mpavgw[1],
			pSBeam->GetMsqry(ii+1),Mpavgw[1],
			pSBeam->GetWavofCurvx(ii+1),pSBeam->GetWavofCurvy(ii+1),
			pSBeam->GetTiltx(ii+1),"\n");
	}
	fclose(stream);

	// Update the progress bars
	pProgT->SetPos(99);

	}	// End if(!GetStop())

	// Free up memory
	free_vector(EField,1,2*nx*(ny/2)*nt);
	free_vector(Power_p,1,nt);
	free_vector(Spectrum_p,1,XFactor*nt);
	

//**********************************************************************
// Write the input and output globals to the help box

	if(!GetStop())
	{

	// Set the global variables to the current m-squared values
	m_mavgwo_ts = Msavgt[1];
	m_mavgwo_ti = Miavgt[1];
	m_mavgwo_tp = Mpavgt[1];

	m_mavgp_ts = Msavgt[2];
	m_mavgp_ti = Miavgt[2];
	m_mavgp_tp = Mpavgt[2];

	m_mavgwo_ws = Msavgw[1];
	m_mavgwo_wi = Miavgw[1];
	m_mavgwo_wp = Mpavgw[1];

	m_mavgp_ws = Msavgw[2];
	m_mavgp_wi = Miavgw[2];
	m_mavgp_wp = Mpavgw[2];

	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Avg M^2 (Time) in walkoff direction:   ",
		m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp,
		"\nAvg M^2 (Time) perpendicular to wo dir:",
		m_mavgp_ts, m_mavgp_ti, m_mavgp_tp,
		"\nAvg M^2 (Freq) in walkoff direction:   ",
		m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp,
		"\nAvg M^2 (Freq) perpendicular to wo dir:",
		m_mavgp_ws, m_mavgp_wi, m_mavgp_wp);

	SetDlgItemText(IDC_GRAPH_HELP,strings);

	}

//**********************************************************************
// Check to see if the 'Stop' button was pressed

	if( GetStop() )
	{
		CString run = "Analyze";
		SetDlgItemText(IDC_ANALYZE,run);
		
		// Free up memory
		free_dvector(Msavgt,1,2);
		free_dvector(Miavgt,1,2);
		free_dvector(Mpavgt,1,2);
		free_dvector(Msavgw,1,2);
		free_dvector(Miavgw,1,2);
		free_dvector(Mpavgw,1,2);

		delete pBeam;
		delete pSBeam;
		
		SetDlgItemText(IDC_GRAPH_HELP,"");

		// Let MixView know that analyze was not completed
		m_banalyze = false;

		// Update the progress bars
		pProgS->SetPos(0);
		pProgT->SetPos(0);

		return;
	}
	

//**********************************************************************
// Reset the 'Analyze' button, enable other buttons, and delete pointers

	CString run = "Analyze";
	SetDlgItemText(IDC_ANALYZE,run);

	// Update the progress bars
	pProgS->SetPos(0);
	pProgT->SetPos(0);

	// Enable the chart buttons for beam analysis
	GetDlgItem(IDC_BEAM_FT)->EnableWindow(true);
	GetDlgItem(IDC_BEAM_FT2)->EnableWindow(true);
	GetDlgItem(IDC_M_W)->EnableWindow(true);
	GetDlgItem(IDC_M_P)->EnableWindow(true);
	GetDlgItem(IDC_FOCUS_W)->EnableWindow(true);
	GetDlgItem(IDC_FOCUS_P)->EnableWindow(true);
	GetDlgItem(IDC_TILT)->EnableWindow(true);

	// Disable the 'Analyze' button
	GetDlgItem(IDC_ANALYZE)->EnableWindow(false);

	// Update the progress bars
	pProgS->SetPos(0);
	pProgT->SetPos(0);

	// Free up memory
	free_dvector(Msavgt,1,2);
	free_dvector(Miavgt,1,2);
	free_dvector(Mpavgt,1,2);
	free_dvector(Msavgw,1,2);
	free_dvector(Miavgw,1,2);
	free_dvector(Mpavgw,1,2);

	delete pBeam;
	delete pSBeam;

	// Let MixView know that analyze completed successfully
	m_banalyze = true;
		
}	// End OnAnalyze


		//***************** OnFocusP Button *************************
void CGraphDlg::OnFocusP() 
{
	// Declare local variables
	CString filename;

		
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);


	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_BEAM_FT,IDC_BEAM_FT2);

	if(num == IDC_BEAM_FT)	// Frequency button
	{

	// Define local variables
	int ii;
	long XFactor = 4;	// factor for spectral smoothing

	//*********************************************************************
	// Open 'Spectra.dat' to find the ranges of points > 0.001*norm spectra

	bool done_s,done_i,done_p;
	double fval,sps,spi,spp;
	double Smax_s, Smax_i, Smax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right values to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Smax_s = Smax_i = Smax_p = 0.0;

	long spnt;

	double *sp_s, *sp_i, *sp_p;

	sp_s = dvector(1,XFactor*m_nt);
	sp_i = dvector(1,XFactor*m_nt);
	sp_p = dvector(1,XFactor*m_nt);

	filename = "Spectra.dat";
	ifstream finsp(filename);

	if(!finsp)
	{
		AfxMessageBox("Spectra.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the spectra data
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		finsp >> fval;
		finsp >> sps;
		finsp >> spi;
		finsp >> spp;

		sp_s[ii+1] = sps;
		sp_i[ii+1] = spi;
		sp_p[ii+1] = spp;

		if(Smax_s < sp_s[ii+1])
		{
			Smax_s = sp_s[ii+1];
		}
		else if(Smax_i < sp_i[ii+1])
		{
			Smax_i = sp_i[ii+1];
		}
		else if(Smax_p < sp_p[ii+1])
		{
			Smax_p = sp_p[ii+1];
		}
	}
	finsp.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[ii+1]/Smax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[ii+1]/Smax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[ii+1]/Smax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[XFactor*m_nt - ii]/Smax_s > 0.001)
			{
				right_s = XFactor*m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[XFactor*m_nt - ii]/Smax_i > 0.001)
			{
				right_i = XFactor*m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[XFactor*m_nt - ii]/Smax_p > 0.001)
			{
				right_p = XFactor*m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	spnt = XFactor*m_nt - ((XFactor*m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (1/(1.e12*XFactor*m_nt*m_dt))*(right-(1+0.5*XFactor*m_nt));
	m_Minprint = (1/(1.e12*XFactor*m_nt*m_dt))*((left+1)-(1+0.5*XFactor*m_nt));
	m_Ntprint = spnt;

	// Free up memory
	free_dvector(sp_s,1,XFactor*m_nt);
	free_dvector(sp_i,1,XFactor*m_nt);
	free_dvector(sp_p,1,XFactor*m_nt);


	//*****************************************************************
	// Open the 'Beam_w' files for plotting

	// Set the min and max values for printing
//	m_Maxprint = m_Freq_max;
//	m_Minprint = m_Freq_min;

	filename = "Beams_ws.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ws.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wi.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_wi.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_wp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}


	
//*********************************************************************
// Fill the chart with data

	double tvals,spects,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,specti,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,spectp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the data into the chart
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		fins >> tvals;
		fins >> spects;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> specti;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> spectp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
		
/*		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals);
			pData->SetY(1,(ii-left)+1,wavys);
			pData->SetY(2,(ii-left)+1,wavyi);
			pData->SetY(3,(ii-left)+1,wavyp);
		}*/
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "FOCUSW_P";
		
	}
	else if(num == IDC_BEAM_FT2)	// Time button
	{

	// Define local variables
	int ii;

	//*********************************************************************
	// Open 'Power.dat' to find the ranges of points > 0.001*norm pow

	bool done_s,done_i,done_p;
	double xval,pows,powi,powp;
	double Pmax_s, Pmax_i, Pmax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right variables to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Pmax_s = Pmax_i = Pmax_p = 0.0;

	long pownt;

	double *pow_s, *pow_i, *pow_p;

	pow_s = dvector(1,m_nt);
	pow_i = dvector(1,m_nt);
	pow_p = dvector(1,m_nt);

	filename = "Power.dat";
	ifstream finpow(filename);

	if(!finpow)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the power data
	for(ii=0;ii<m_nt;++ii)
	{
		finpow >> xval;
		finpow >> pows;
		finpow >> powi;
		finpow >> powp;

		pow_s[ii+1] = pows;
		pow_i[ii+1] = powi;
		pow_p[ii+1] = powp;

		if(Pmax_s < pow_s[ii+1])
		{
			Pmax_s = pow_s[ii+1];
		}
		else if(Pmax_i < pow_i[ii+1])
		{
			Pmax_i = pow_i[ii+1];
		}
		else if(Pmax_p < pow_p[ii+1])
		{
			Pmax_p = pow_p[ii+1];
		}
	}
	finpow.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[ii+1]/Pmax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[ii+1]/Pmax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[ii+1]/Pmax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[m_nt - ii]/Pmax_s > 0.001)
			{
				right_s = m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[m_nt - ii]/Pmax_i > 0.001)
			{
				right_i = m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[m_nt - ii]/Pmax_p > 0.001)
			{
				right_p = m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	pownt = m_nt - ((m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (m_dt*right)*1.e12;
	m_Minprint = (m_dt*(left+1))*1.e12;
	m_Ntprint = pownt;

	// Free up memory
	free_dvector(pow_s,1,m_nt);
	free_dvector(pow_i,1,m_nt);
	free_dvector(pow_p,1,m_nt);
	
	//*****************************************************************
	// Open the 'Beam_t' files for plotting

	filename = "Beams_ts.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ts.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_ti.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_ti.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_tp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_tp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	
//*********************************************************************
// Fill the chart with data

	double tvals,powers,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,poweri,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,powerp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the data into the chart
	for(ii=0;ii<m_nt;++ii)
	{
		fins >> tvals;
		fins >> powers;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> poweri;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> powerp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
		
/*		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals*1.e12);
			pData->SetY(1,(ii-left)+1,wavys);
			pData->SetY(2,(ii-left)+1,wavyi);
			pData->SetY(3,(ii-left)+1,wavyp);
		}
*/			
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "FOCUST_P";

	}// End if(IDC_BEAM_FT2)

	// Set the help box to the m-squared values
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Avg M^2 (Time) in walkoff direction:   ",
		m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp,
		"\nAvg M^2 (Time) perpendicular to wo dir:",
		m_mavgp_ts, m_mavgp_ti, m_mavgp_tp,
		"\nAvg M^2 (Freq) in walkoff direction:   ",
		m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp,
		"\nAvg M^2 (Freq) perpendicular to wo dir:",
		m_mavgp_ws, m_mavgp_wi, m_mavgp_wp);

	SetDlgItemText(IDC_GRAPH_HELP,strings);
	
}	// End OnFocusP


		//**************************** OnFocusW Button ************************
void CGraphDlg::OnFocusW() 
{
	// Declare local variables
	CString filename;

		
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);

	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_BEAM_FT,IDC_BEAM_FT2);

	if(num == IDC_BEAM_FT)	// Frequency button
	{

	// Define local variables
	int ii;
	long XFactor = 4;	// factor for spectral smoothing

	//*********************************************************************
	// Open 'Spectra.dat' to find the ranges of points > 0.001*norm spectra

	bool done_s,done_i,done_p;
	double fval,sps,spi,spp;
	double Smax_s, Smax_i, Smax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right values to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Smax_s = Smax_i = Smax_p = 0.0;

	long spnt;

	double *sp_s, *sp_i, *sp_p;

	sp_s = dvector(1,XFactor*m_nt);
	sp_i = dvector(1,XFactor*m_nt);
	sp_p = dvector(1,XFactor*m_nt);

	filename = "Spectra.dat";
	ifstream finsp(filename);

	if(!finsp)
	{
		AfxMessageBox("Spectra.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the spectra data
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		finsp >> fval;
		finsp >> sps;
		finsp >> spi;
		finsp >> spp;

		sp_s[ii+1] = sps;
		sp_i[ii+1] = spi;
		sp_p[ii+1] = spp;

		if(Smax_s < sp_s[ii+1])
		{
			Smax_s = sp_s[ii+1];
		}
		else if(Smax_i < sp_i[ii+1])
		{
			Smax_i = sp_i[ii+1];
		}
		else if(Smax_p < sp_p[ii+1])
		{
			Smax_p = sp_p[ii+1];
		}
	}
	finsp.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[ii+1]/Smax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[ii+1]/Smax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[ii+1]/Smax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[XFactor*m_nt - ii]/Smax_s > 0.001)
			{
				right_s = XFactor*m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[XFactor*m_nt - ii]/Smax_i > 0.001)
			{
				right_i = XFactor*m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[XFactor*m_nt - ii]/Smax_p > 0.001)
			{
				right_p = XFactor*m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	spnt = XFactor*m_nt - ((XFactor*m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (1/(1.e12*XFactor*m_nt*m_dt))*(right-(1+0.5*XFactor*m_nt));
	m_Minprint = (1/(1.e12*XFactor*m_nt*m_dt))*((left+1)-(1+0.5*XFactor*m_nt));
	m_Ntprint = spnt;

	// Free up memory
	free_dvector(sp_s,1,XFactor*m_nt);
	free_dvector(sp_i,1,XFactor*m_nt);
	free_dvector(sp_p,1,XFactor*m_nt);


	//*****************************************************************
	// Open the 'Beam_w' files for plotting

	filename = "Beams_ws.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ws.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wi.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_wi.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_wp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	
//*********************************************************************
// Fill the chart with data

	double tvals,spects,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,specti,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,spectp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the data into the chart
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		fins >> tvals;
		fins >> spects;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> specti;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> spectp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
/*
		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals);
			pData->SetY(1,(ii-left)+1,wavxs);
			pData->SetY(2,(ii-left)+1,wavxi);
			pData->SetY(3,(ii-left)+1,wavxp);
		}*/
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "FOCUSW_W";
		
	}
	else if(num == IDC_BEAM_FT2)	// Time button
	{

	// Define local variables
	int ii;

	//*********************************************************************
	// Open 'Power.dat' to find the ranges of points > 0.001*norm pow

	bool done_s,done_i,done_p;
	double xval,pows,powi,powp;
	double Pmax_s, Pmax_i, Pmax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right variables to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Pmax_s = Pmax_i = Pmax_p = 0.0;

	long pownt;

	double *pow_s, *pow_i, *pow_p;

	pow_s = dvector(1,m_nt);
	pow_i = dvector(1,m_nt);
	pow_p = dvector(1,m_nt);

	filename = "Power.dat";
	ifstream finpow(filename);

	if(!finpow)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the power data
	for(ii=0;ii<m_nt;++ii)
	{
		finpow >> xval;
		finpow >> pows;
		finpow >> powi;
		finpow >> powp;

		pow_s[ii+1] = pows;
		pow_i[ii+1] = powi;
		pow_p[ii+1] = powp;

		if(Pmax_s < pow_s[ii+1])
		{
			Pmax_s = pow_s[ii+1];
		}
		else if(Pmax_i < pow_i[ii+1])
		{
			Pmax_i = pow_i[ii+1];
		}
		else if(Pmax_p < pow_p[ii+1])
		{
			Pmax_p = pow_p[ii+1];
		}
	}
	finpow.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[ii+1]/Pmax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[ii+1]/Pmax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[ii+1]/Pmax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[m_nt - ii]/Pmax_s > 0.001)
			{
				right_s = m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[m_nt - ii]/Pmax_i > 0.001)
			{
				right_i = m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[m_nt - ii]/Pmax_p > 0.001)
			{
				right_p = m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	pownt = m_nt - ((m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (m_dt*right)*1.e12;
	m_Minprint = (m_dt*(left+1))*1.e12;
	m_Ntprint = pownt;

	// Free up memory
	free_dvector(pow_s,1,m_nt);
	free_dvector(pow_i,1,m_nt);
	free_dvector(pow_p,1,m_nt);
	

	//*****************************************************************
	// Open the 'Beam_t' files for plotting

	filename = "Beams_ts.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ts.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_ti.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_ti.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_tp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_tp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}
//*********************************************************************
// Fill the chart with data

	double tvals,powers,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,poweri,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,powerp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the data into the chart
	for(ii=0;ii<m_nt;++ii)
	{
		fins >> tvals;
		fins >> powers;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> poweri;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> powerp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
/*		
		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals*1.e12);
			pData->SetY(1,(ii-left)+1,wavxs);
			pData->SetY(2,(ii-left)+1,wavxi);
			pData->SetY(3,(ii-left)+1,wavxp);
		}
*/		
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "FOCUST_W";

	}// End if(IDC_BEAM_FT2)
	

	// Set the help box to the m-squared values
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Avg M^2 (Time) in walkoff direction:   ",
		m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp,
		"\nAvg M^2 (Time) perpendicular to wo dir:",
		m_mavgp_ts, m_mavgp_ti, m_mavgp_tp,
		"\nAvg M^2 (Freq) in walkoff direction:   ",
		m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp,
		"\nAvg M^2 (Freq) perpendicular to wo dir:",
		m_mavgp_ws, m_mavgp_wi, m_mavgp_wp);

	SetDlgItemText(IDC_GRAPH_HELP,strings);

}	// End OnFocusW


		//***************************** OnMP Button ***************************
void CGraphDlg::OnMP() 
{
	// Declare local variables
	double msavg,miavg,mpavg;

	CString filename;

		
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);

	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_BEAM_FT,IDC_BEAM_FT2);

	if(num == IDC_BEAM_FT)	// Frequency button
	{

	// Define local variables
	int ii;
	long XFactor = 4;	// factor for spectral smoothing

	//*********************************************************************
	// Open 'Spectra.dat' to find the ranges of points > 0.001*norm spectra

	bool done_s,done_i,done_p;
	double fval,sps,spi,spp;
	double Smax_s, Smax_i, Smax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right values to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Smax_s = Smax_i = Smax_p = 0.0;

	long spnt;

	double *sp_s, *sp_i, *sp_p;

	sp_s = dvector(1,XFactor*m_nt);
	sp_i = dvector(1,XFactor*m_nt);
	sp_p = dvector(1,XFactor*m_nt);

	filename = "Spectra.dat";
	ifstream finsp(filename);

	if(!finsp)
	{
		AfxMessageBox("Spectra.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the spectra data
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		finsp >> fval;
		finsp >> sps;
		finsp >> spi;
		finsp >> spp;

		sp_s[ii+1] = sps;
		sp_i[ii+1] = spi;
		sp_p[ii+1] = spp;

		if(Smax_s < sp_s[ii+1])
		{
			Smax_s = sp_s[ii+1];
		}
		else if(Smax_i < sp_i[ii+1])
		{
			Smax_i = sp_i[ii+1];
		}
		else if(Smax_p < sp_p[ii+1])
		{
			Smax_p = sp_p[ii+1];
		}
	}
	finsp.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[ii+1]/Smax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[ii+1]/Smax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[ii+1]/Smax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[XFactor*m_nt - ii]/Smax_s > 0.001)
			{
				right_s = XFactor*m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[XFactor*m_nt - ii]/Smax_i > 0.001)
			{
				right_i = XFactor*m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[XFactor*m_nt - ii]/Smax_p > 0.001)
			{
				right_p = XFactor*m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	spnt = XFactor*m_nt - ((XFactor*m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (1/(1.e12*XFactor*m_nt*m_dt))*(right-(1+0.5*XFactor*m_nt));
	m_Minprint = (1/(1.e12*XFactor*m_nt*m_dt))*((left+1)-(1+0.5*XFactor*m_nt));
	m_Ntprint = spnt;

	// Free up memory
	free_dvector(sp_s,1,XFactor*m_nt);
	free_dvector(sp_i,1,XFactor*m_nt);
	free_dvector(sp_p,1,XFactor*m_nt);


	//*****************************************************************
	// Open the 'Beam_w' files for plotting

	filename = "Beams_ws.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ws.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wi.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_wi.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_wp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}
//*********************************************************************
// Fill the chart with normalized spectrum data

	double tvals,spects,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,specti,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,spectp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the spectrum data into the chart
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		fins >> tvals;
		fins >> spects;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> specti;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> spectp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
		
/*		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals);
			pData->SetY(1,(ii-left)+1,msqrys);
			pData->SetY(2,(ii-left)+1,msqryi);
			pData->SetY(3,(ii-left)+1,msqryp);

		}
*/	
		if(ii == 0)
		{
			msavg = myavgs;
			miavg = myavgi;
			mpavg = myavgp;
		}
		
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "MW_P";
		
	}
	else if(num == IDC_BEAM_FT2)	// Time button
	{

	// Define local variables
	int ii;

	//*********************************************************************
	// Open 'Power.dat' to find the ranges of points > 0.001*norm pow

	bool done_s,done_i,done_p;
	double xval,pows,powi,powp;
	double Pmax_s, Pmax_i, Pmax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right variables to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Pmax_s = Pmax_i = Pmax_p = 0.0;

	long pownt;

	double *pow_s, *pow_i, *pow_p;

	pow_s = dvector(1,m_nt);
	pow_i = dvector(1,m_nt);
	pow_p = dvector(1,m_nt);

	filename = "Power.dat";
	ifstream finpow(filename);

	if(!finpow)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the power data
	for(ii=0;ii<m_nt;++ii)
	{
		finpow >> xval;
		finpow >> pows;
		finpow >> powi;
		finpow >> powp;

		pow_s[ii+1] = pows;
		pow_i[ii+1] = powi;
		pow_p[ii+1] = powp;

		if(Pmax_s < pow_s[ii+1])
		{
			Pmax_s = pow_s[ii+1];
		}
		else if(Pmax_i < pow_i[ii+1])
		{
			Pmax_i = pow_i[ii+1];
		}
		else if(Pmax_p < pow_p[ii+1])
		{
			Pmax_p = pow_p[ii+1];
		}
	}
	finpow.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[ii+1]/Pmax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[ii+1]/Pmax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[ii+1]/Pmax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[m_nt - ii]/Pmax_s > 0.001)
			{
				right_s = m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[m_nt - ii]/Pmax_i > 0.001)
			{
				right_i = m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[m_nt - ii]/Pmax_p > 0.001)
			{
				right_p = m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	pownt = m_nt - ((m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (m_dt*right)*1.e12;
	m_Minprint = (m_dt*(left+1))*1.e12;
	m_Ntprint = pownt;

	// Free up memory
	free_dvector(pow_s,1,m_nt);
	free_dvector(pow_i,1,m_nt);
	free_dvector(pow_p,1,m_nt);
	

	//*****************************************************************
	// Open the 'Beam_t' files for plotting

	filename = "Beams_ts.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ts.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_ti.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_ti.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_tp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_tp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	
//*********************************************************************
// Fill the chart with normalized spectrum data

	double tvals,powers,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,poweri,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,powerp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the spectrum data into the chart
	for(ii=0;ii<m_nt;++ii)
	{
		fins >> tvals;
		fins >> powers;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> poweri;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> powerp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
/*		
		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals*1.e12);
			pData->SetY(1,(ii-left)+1,msqrys);
			pData->SetY(2,(ii-left)+1,msqryi);
			pData->SetY(3,(ii-left)+1,msqryp);
		}
	
		if(ii == 0)
		{
			msavg = myavgs;
			miavg = myavgi;
			mpavg = myavgp;
		}
*/
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "MT_P";

	}// End if(IDC_BEAM_FT2)


	// Set the help box to the m-squared values
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Avg M^2 (Time) in walkoff direction:   ",
		m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp,
		"\nAvg M^2 (Time) perpendicular to wo dir:",
		m_mavgp_ts, m_mavgp_ti, m_mavgp_tp,
		"\nAvg M^2 (Freq) in walkoff direction:   ",
		m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp,
		"\nAvg M^2 (Freq) perpendicular to wo dir:",
		m_mavgp_ws, m_mavgp_wi, m_mavgp_wp);

	SetDlgItemText(IDC_GRAPH_HELP,strings);
	
}	// End OnMP


		//**************************** OnMW Button ****************************
void CGraphDlg::OnMW() 
{
	// Declare local variables
	//double msavg,miavg,mpavg;

	CString filename;

		
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);


	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_BEAM_FT,IDC_BEAM_FT2);

	if(num == IDC_BEAM_FT)	// Frequency button
	{

	// Define local variables
	int ii;
	long XFactor = 4;	// factor for spectral smoothing

	//*********************************************************************
	// Open 'Spectra.dat' to find the ranges of points > 0.001*norm spectra

	bool done_s,done_i,done_p;
	double fval,sps,spi,spp;
	double Smax_s, Smax_i, Smax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right values to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Smax_s = Smax_i = Smax_p = 0.0;

	long spnt;

	double *sp_s, *sp_i, *sp_p;

	sp_s = dvector(1,XFactor*m_nt);
	sp_i = dvector(1,XFactor*m_nt);
	sp_p = dvector(1,XFactor*m_nt);

	filename = "Spectra.dat";
	ifstream finsp(filename);

	if(!finsp)
	{
		AfxMessageBox("Spectra.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the spectra data
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		finsp >> fval;
		finsp >> sps;
		finsp >> spi;
		finsp >> spp;

		sp_s[ii+1] = sps;
		sp_i[ii+1] = spi;
		sp_p[ii+1] = spp;

		if(Smax_s < sp_s[ii+1])
		{
			Smax_s = sp_s[ii+1];
		}
		else if(Smax_i < sp_i[ii+1])
		{
			Smax_i = sp_i[ii+1];
		}
		else if(Smax_p < sp_p[ii+1])
		{
			Smax_p = sp_p[ii+1];
		}
	}
	finsp.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[ii+1]/Smax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[ii+1]/Smax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[ii+1]/Smax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[XFactor*m_nt - ii]/Smax_s > 0.001)
			{
				right_s = XFactor*m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[XFactor*m_nt - ii]/Smax_i > 0.001)
			{
				right_i = XFactor*m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[XFactor*m_nt - ii]/Smax_p > 0.001)
			{
				right_p = XFactor*m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	spnt = XFactor*m_nt - ((XFactor*m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (1/(1.e12*XFactor*m_nt*m_dt))*(right-(1+0.5*XFactor*m_nt));
	m_Minprint = (1/(1.e12*XFactor*m_nt*m_dt))*((left+1)-(1+0.5*XFactor*m_nt));
	m_Ntprint = spnt;

	// Free up memory
	free_dvector(sp_s,1,XFactor*m_nt);
	free_dvector(sp_i,1,XFactor*m_nt);
	free_dvector(sp_p,1,XFactor*m_nt);


	//*****************************************************************
	// Open the 'Beam_t' files for plotting

	filename = "Beams_ws.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ws.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wi.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_wi.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_wp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	
//*********************************************************************
// Fill the chart with normalized spectrum data

	double tvals,spects,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,specti,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,spectp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the spectrum data into the chart
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		fins >> tvals;
		fins >> spects;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> specti;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> spectp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
/*		
		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals);
			pData->SetY(1,(ii-left)+1,msqrxs);
			pData->SetY(2,(ii-left)+1,msqrxi);
			pData->SetY(3,(ii-left)+1,msqrxp);
		}
	
		if(ii == 0)
		{
			msavg = mxavgs;
			miavg = mxavgi;
			mpavg = mxavgp;
		}
*/
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "MW_W";

	}
	else if(num == IDC_BEAM_FT2)	// Time button
	{

	// Define local variables
	int ii;

	//*********************************************************************
	// Open 'Power.dat' to find the ranges of points > 0.001*norm pow

	bool done_s,done_i,done_p;
	double xval,pows,powi,powp;
	double Pmax_s, Pmax_i, Pmax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right variables to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Pmax_s = Pmax_i = Pmax_p = 0.0;

	long pownt;

	double *pow_s, *pow_i, *pow_p;

	pow_s = dvector(1,m_nt);
	pow_i = dvector(1,m_nt);
	pow_p = dvector(1,m_nt);

	filename = "Power.dat";
	ifstream finpow(filename);

	if(!finpow)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the power data
	for(ii=0;ii<m_nt;++ii)
	{
		finpow >> xval;
		finpow >> pows;
		finpow >> powi;
		finpow >> powp;

		pow_s[ii+1] = pows;
		pow_i[ii+1] = powi;
		pow_p[ii+1] = powp;

		if(Pmax_s < pow_s[ii+1])
		{
			Pmax_s = pow_s[ii+1];
		}
		else if(Pmax_i < pow_i[ii+1])
		{
			Pmax_i = pow_i[ii+1];
		}
		else if(Pmax_p < pow_p[ii+1])
		{
			Pmax_p = pow_p[ii+1];
		}
	}
	finpow.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[ii+1]/Pmax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[ii+1]/Pmax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[ii+1]/Pmax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[m_nt - ii]/Pmax_s > 0.001)
			{
				right_s = m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[m_nt - ii]/Pmax_i > 0.001)
			{
				right_i = m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[m_nt - ii]/Pmax_p > 0.001)
			{
				right_p = m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	pownt = m_nt - ((m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (m_dt*right)*1.e12;
	m_Minprint = (m_dt*(left+1))*1.e12;
	m_Ntprint = pownt;

	// Free up memory
	free_dvector(pow_s,1,m_nt);
	free_dvector(pow_i,1,m_nt);
	free_dvector(pow_p,1,m_nt);


	//*****************************************************************
	// Open the 'Beam_t' files for plotting

	filename = "Beams_ts.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ts.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_ti.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_ti.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_tp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_tp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	
//*********************************************************************
// Fill the chart with normalized spectrum data

	double tvals,powers,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,poweri,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,powerp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the spectrum data into the chart
	for(ii=0;ii<m_nt;++ii)
	{
		fins >> tvals;
		fins >> powers;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> poweri;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> powerp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
/*		
		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals*1.e12);
			pData->SetY(1,(ii-left)+1,msqrxs);
			pData->SetY(2,(ii-left)+1,msqrxi);
			pData->SetY(3,(ii-left)+1,msqrxp);
		}
	
		if(ii == 0)
		{
			msavg = mxavgs;
			miavg = mxavgi;
			mpavg = mxavgp;
		}
*/
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "MT_W";

	}// End if(IDC_BEAM_FT2)


	// Set the help box to the m-squared values
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Avg M^2 (Time) in walkoff direction:   ",
		m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp,
		"\nAvg M^2 (Time) perpendicular to wo dir:",
		m_mavgp_ts, m_mavgp_ti, m_mavgp_tp,
		"\nAvg M^2 (Freq) in walkoff direction:   ",
		m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp,
		"\nAvg M^2 (Freq) perpendicular to wo dir:",
		m_mavgp_ws, m_mavgp_wi, m_mavgp_wp);

	SetDlgItemText(IDC_GRAPH_HELP,strings);
	
}	// End OnMW


		//**************************** OnTilt Button **************************
void CGraphDlg::OnTilt() 
{
	// Declare local variables
	CString filename;

		
//*********************************************************************
// Create the Output Window

	// Find the location of the 3d graph to make the 2d window
	LPRECT t_Rect = &m_Rect;
	//GetDlgItem(IDC_3DGRAPH)->GetWindowRect(t_Rect);

	LPCTSTR szWndClass;

	szWndClass = AfxRegisterWndClass(0);

	// Check the Radio Buttons to see which one is selected
	int num = GetCheckedRadioButton(IDC_BEAM_FT,IDC_BEAM_FT2);

	if(num == IDC_BEAM_FT)	// Frequency button
	{

	// Define local variables
	int ii;
	long XFactor = 4;	// factor for spectral smoothing

	//*********************************************************************
	// Open 'Spectra.dat' to find the ranges of points > 0.001*norm spectra

	bool done_s,done_i,done_p;
	double fval,sps,spi,spp;
	double Smax_s, Smax_i, Smax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right values to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Smax_s = Smax_i = Smax_p = 0.0;

	long spnt;

	double *sp_s, *sp_i, *sp_p;

	sp_s = dvector(1,XFactor*m_nt);
	sp_i = dvector(1,XFactor*m_nt);
	sp_p = dvector(1,XFactor*m_nt);

	filename = "Spectra.dat";
	ifstream finsp(filename);

	if(!finsp)
	{
		AfxMessageBox("Spectra.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the spectra data
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		finsp >> fval;
		finsp >> sps;
		finsp >> spi;
		finsp >> spp;

		sp_s[ii+1] = sps;
		sp_i[ii+1] = spi;
		sp_p[ii+1] = spp;

		if(Smax_s < sp_s[ii+1])
		{
			Smax_s = sp_s[ii+1];
		}
		else if(Smax_i < sp_i[ii+1])
		{
			Smax_i = sp_i[ii+1];
		}
		else if(Smax_p < sp_p[ii+1])
		{
			Smax_p = sp_p[ii+1];
		}
	}
	finsp.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[ii+1]/Smax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[ii+1]/Smax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[ii+1]/Smax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		if(!done_s)			
		{
			if(sp_s[XFactor*m_nt - ii]/Smax_s > 0.001)
			{
				right_s = XFactor*m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(sp_i[XFactor*m_nt - ii]/Smax_i > 0.001)
			{
				right_i = XFactor*m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(sp_p[XFactor*m_nt - ii]/Smax_p > 0.001)
			{
				right_p = XFactor*m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	spnt = XFactor*m_nt - ((XFactor*m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (1/(1.e12*XFactor*m_nt*m_dt))*(right-(1+0.5*XFactor*m_nt));
	m_Minprint = (1/(1.e12*XFactor*m_nt*m_dt))*((left+1)-(1+0.5*XFactor*m_nt));
	m_Ntprint = spnt;

	// Free up memory
	free_dvector(sp_s,1,XFactor*m_nt);
	free_dvector(sp_i,1,XFactor*m_nt);
	free_dvector(sp_p,1,XFactor*m_nt);

	
	//*****************************************************************
	// Open the 'Beam_w' files for plotting

	filename = "Beams_ws.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ws.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wi.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_wi.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_wp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_wp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	
//*********************************************************************
// Fill the chart with data

	double tvals,spects,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,specti,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,spectp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the data into the chart
	for(ii=0;ii<XFactor*m_nt;++ii)
	{
		fins >> tvals;
		fins >> spects;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> specti;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> spectp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;

/*		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals);
			pData->SetY(1,(ii-left)+1,tilts);
			pData->SetY(2,(ii-left)+1,tilti);
			pData->SetY(3,(ii-left)+1,tiltp);
		}
*/			
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "TILTW";
		
	}
	else if(num == IDC_BEAM_FT2)	// Time button
	{

	// Define local variables
	int ii;

	//*********************************************************************
	// Open 'Power.dat' to find the ranges of points > 0.001*norm pow

	bool done_s,done_i,done_p;
	double xval,pows,powi,powp;
	double Pmax_s, Pmax_i, Pmax_p;
	long left_s, left_i, left_p, left;
	long right_s, right_i, right_p, right;

	// Initialize left & right variables to zero
	left_s = left_i = left_p = 0;
	right_s = right_i = right_p = 0;
	Pmax_s = Pmax_i = Pmax_p = 0.0;

	long pownt;

	double *pow_s, *pow_i, *pow_p;

	pow_s = dvector(1,m_nt);
	pow_i = dvector(1,m_nt);
	pow_p = dvector(1,m_nt);

	filename = "Power.dat";
	ifstream finpow(filename);

	if(!finpow)
	{
		AfxMessageBox("Power.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	// Read in the power data
	for(ii=0;ii<m_nt;++ii)
	{
		finpow >> xval;
		finpow >> pows;
		finpow >> powi;
		finpow >> powp;

		pow_s[ii+1] = pows;
		pow_i[ii+1] = powi;
		pow_p[ii+1] = powp;

		if(Pmax_s < pow_s[ii+1])
		{
			Pmax_s = pow_s[ii+1];
		}
		else if(Pmax_i < pow_i[ii+1])
		{
			Pmax_i = pow_i[ii+1];
		}
		else if(Pmax_p < pow_p[ii+1])
		{
			Pmax_p = pow_p[ii+1];
		}
	}
	finpow.close();


	//*********************************************************
	// Keep only the field slices with power > 0.001 norm power  
	
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[ii+1]/Pmax_s > 0.001)
			{
				left_s = ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[ii+1]/Pmax_i > 0.001)
			{
				left_i = ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[ii+1]/Pmax_p > 0.001)
			{
				left_p = ii;
				done_p = true;
			}
		}
	}

	// Find the smallest left value
	left = MIN(left_s,left_i,left_p);
	
	// Reset done
	done_s = false;
	done_i = false;
	done_p = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<m_nt;++ii)
	{
		if(!done_s)			
		{
			if(pow_s[m_nt - ii]/Pmax_s > 0.001)
			{
				right_s = m_nt - ii;
				done_s = true;
			}
		}

		if(!done_i)
		{
			if(pow_i[m_nt - ii]/Pmax_i > 0.001)
			{
				right_i = m_nt - ii;
				done_i = true;
			}
		}

		if(!done_p)
		{
			if(pow_p[m_nt - ii]/Pmax_p > 0.001)
			{
				right_p = m_nt - ii;
				done_p = true;
			}
		}
		
	}

	// Find the largest left value
	right = MAX(right_s,right_i,right_p);

	// Set the new number of time points for the power
	pownt = m_nt - ((m_nt - right) + left);

	// Set the min and max values for printing
	m_Maxprint = (m_dt*right)*1.e12;
	m_Minprint = (m_dt*(left+1))*1.e12;
	m_Ntprint = pownt;

	// Free up memory
	free_dvector(pow_s,1,m_nt);
	free_dvector(pow_i,1,m_nt);
	free_dvector(pow_p,1,m_nt);

		
	//*****************************************************************
	// Open the 'Beam_t' files for plotting

	filename = "Beams_ts.dat";
	ifstream fins(filename);

	if(!fins)
	{
		AfxMessageBox("Beams_ts.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_ti.dat";
	ifstream fini(filename);

	if(!fini)
	{
		AfxMessageBox("Beams_ti.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}

	filename = "Beams_tp.dat";
	ifstream finp(filename);

	if(!finp)
	{
		AfxMessageBox("Beams_tp.dat does not exist.  Press 'Analyze' to create the file.");
		return;
	}


//*********************************************************************
// Fill the chart with data

	double tvals,powers,msqrxs,mxavgs,msqrys,myavgs,wavxs,wavys,tilts;
	double tvali,poweri,msqrxi,mxavgi,msqryi,myavgi,wavxi,wavyi,tilti;
	double tvalp,powerp,msqrxp,mxavgp,msqryp,myavgp,wavxp,wavyp,tiltp;

	// Transfer the data into the chart
	for(ii=0;ii<m_nt;++ii)
	{
		fins >> tvals;
		fins >> powers;
		fins >> msqrxs;
		fins >> mxavgs;
		fins >> msqrys;
		fins >> myavgs;
		fins >> wavxs;
		fins >> wavys;
		fins >> tilts;

		fini >> tvali;
		fini >> poweri;
		fini >> msqrxi;
		fini >> mxavgi;
		fini >> msqryi;
		fini >> myavgi;
		fini >> wavxi;
		fini >> wavyi;
		fini >> tilti;

		finp >> tvalp;
		finp >> powerp;
		finp >> msqrxp;
		finp >> mxavgp;
		finp >> msqryp;
		finp >> myavgp;
		finp >> wavxp;
		finp >> wavyp;
		finp >> tiltp;
/*		
		if((ii>=left) && (ii<right))
		{
			pData->SetX(1,(ii-left)+1,tvals*1.e12);
			pData->SetY(1,(ii-left)+1,tilts);
			pData->SetY(2,(ii-left)+1,tilti);
			pData->SetY(3,(ii-left)+1,tiltp);
		}
*/		
	}
	fins.close();
	fini.close();
	finp.close();
	
	m_b2d = true;	// for printing the 2d chart
	m_Type = "TILTT";
	

	}// End if(IDC_BEAM_FT2)


	// Set the help box to the m-squared values
	CString strings;
	strings.Format("%s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e %s %5.2e %5.2e %5.2e",
		"Avg M^2 (Time) in walkoff direction:   ",
		m_mavgwo_ts, m_mavgwo_ti, m_mavgwo_tp,
		"\nAvg M^2 (Time) perpendicular to wo dir:",
		m_mavgp_ts, m_mavgp_ti, m_mavgp_tp,
		"\nAvg M^2 (Freq) in walkoff direction:   ",
		m_mavgwo_ws, m_mavgwo_wi, m_mavgwo_wp,
		"\nAvg M^2 (Freq) perpendicular to wo dir:",
		m_mavgp_ws, m_mavgp_wi, m_mavgp_wp);

	SetDlgItemText(IDC_GRAPH_HELP,strings);

}	// End OnTilt




//******************* Function SetEnableBtns ***************************
//
// SetEnableBtns will enable or disable the Fluence, Farfield
// and Print buttons.  It will also disable the scrollbar and 
// zoom buttons if bEnableBtns = false.
//
//**********************************************************************

void CGraphDlg::SetEnableBtns(BOOL bEnableBtns)
{
	//GetDlgItem(IDC_LOADPLOT)->EnableWindow(bEnableBtns);

	GetDlgItem(IDC_FLUENCE)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_FARFIELD)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_MOVIE)->EnableWindow(bEnableBtns);

	GetDlgItem(IDC_POWER)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_NORMPOWER)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_SPECTRA)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_PRINTBTN)->EnableWindow(bEnableBtns);

	GetDlgItem(IDC_UPROT)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_DOWNROT)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_LEFTROT)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_RIGHTROT)->EnableWindow(bEnableBtns);
	GetDlgItem(IDC_RESETROT)->EnableWindow(bEnableBtns);

	GetDlgItem(IDC_ANALYZE)->EnableWindow(bEnableBtns);

	if(!bEnableBtns)
	{
		GetDlgItem(IDC_BEAM_FT)->EnableWindow(bEnableBtns);
		GetDlgItem(IDC_BEAM_FT2)->EnableWindow(bEnableBtns);
		GetDlgItem(IDC_M_W)->EnableWindow(bEnableBtns);
		GetDlgItem(IDC_M_P)->EnableWindow(bEnableBtns);
		GetDlgItem(IDC_FOCUS_W)->EnableWindow(bEnableBtns);
		GetDlgItem(IDC_FOCUS_P)->EnableWindow(bEnableBtns);
		GetDlgItem(IDC_TILT)->EnableWindow(bEnableBtns);
	}
	
}


// *********************** Radio Button Functions **********************
//
// These function are called when the radio buttons are clicked.
// They call the function SetRadio from Mix_Prog.cpp in order to
// update information on which radio button is currently selected
//
//**********************************************************************

void CGraphDlg::OnTypeSignal() 
{
	SetRadio(1);
	CheckRadioButton(IDC_TYPE_SIG, IDC_TYPE_PUM, IDC_TYPE_SIG);

	m_pPlot3DDoc->m_nLastAction = KOA_DEMO_PLOT_3D;
	m_pPlot3DDoc->ShowPlot(this);
	m_pPlot3DView->m_nKindOfView = KOV_SURF;
	m_pPlot3DView->m_nObjectOfView = OOV_DEMO_SIMPLE;
	m_pPlot3DFrame->m_wndSplitter.RecalcLayout();
}

void CGraphDlg::OnTypeIdle() 
{
	SetRadio(2);
	CheckRadioButton(IDC_TYPE_SIG, IDC_TYPE_PUM, IDC_TYPE_IDL);

	m_pPlot3DDoc->m_nLastAction = KOA_DEMO_PLOT_3D;
	m_pPlot3DDoc->ShowPlot(this);
	m_pPlot3DView->m_nKindOfView = KOV_SURF;
	m_pPlot3DView->m_nObjectOfView = OOV_DEMO_SIMPLE;
	m_pPlot3DFrame->m_wndSplitter.RecalcLayout();
}

void CGraphDlg::OnTypePump() 
{
	SetRadio(3);
	CheckRadioButton(IDC_TYPE_SIG, IDC_TYPE_PUM, IDC_TYPE_PUM);

	m_pPlot3DDoc->m_nLastAction = KOA_DEMO_PLOT_3D;
	m_pPlot3DDoc->ShowPlot(this);
	m_pPlot3DView->m_nKindOfView = KOV_SURF;
	m_pPlot3DView->m_nObjectOfView = OOV_DEMO_SIMPLE;
	m_pPlot3DFrame->m_wndSplitter.RecalcLayout();
}

void CGraphDlg::OnBeamFt() 
{

}

void CGraphDlg::OnBeamFt2() 
{

}

BOOL CGraphDlg::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}

void CGraphDlg::OnCancel() 
{
	SetStop(true);
	CheckWinMessages(10000);

	// Hide the graph window and show the input window
	CGraphDlg* pGraph = this;
	pGraph->ShowWindow(SW_HIDE);
	m_pMixFrame->ShowWindow(SW_SHOW);
	m_pMixView->OnInitialUpdate();
	// Hide any dialogs open as childs of graph dialog
	if(m_pPlot3DFrame->m_pPlotPropDlg)
		m_pPlot3DFrame->m_pPlotPropDlg->ShowWindow(SW_HIDE);
	
	CDialog::OnCancel();
}

void CGraphDlg::GetFramePointers(CFrameWnd* pFrm,CMixView* pMixView)
{
	m_pMixFrame = pFrm;
	m_pMixView = pMixView;
}


void CGraphDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);
}

void CGraphDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
}

LRESULT CGraphDlg::OnUserRundk(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CGraphDlg::OnLoadPlotData() 
{
	m_pPlot3DDoc->m_nLastAction = KOA_DEMO_PLOT_3D;
	m_pPlot3DDoc->LoadPlotData(this);
	m_pPlot3DView->m_nKindOfView = KOV_SURF;
	m_pPlot3DView->m_nObjectOfView = OOV_DEMO_SIMPLE;
	m_pPlot3DFrame->m_wndSplitter.RecalcLayout();
}

void CGraphDlg::CreatePlotWindow(CWnd *pParent)
{
	// create Plot3D control, which is a CWnd object. See its creation: it generates a doctemplate
	// containing the frame, document and view
	m_Plot3dCtrl.Create(this, IDC_PLOTFRM,
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPlot3dDoc),
		RUNTIME_CLASS(CPlot3dFrameWnd),
		RUNTIME_CLASS(CPlot3dView),
		WS_CHILD | WS_BORDER | WS_VISIBLE, 0L);
	if(!m_Plot3dCtrl)
	{
		m_pPlot3DFrame = (CPlot3dFrameWnd*)m_Plot3dCtrl.m_pDocTemplate->m_pMixFrame;
		m_pPlot3DView = (CPlot3dView*)m_pPlot3DFrame->GetActiveView();

		CMix2dApp *pApp = (CMix2dApp *)AfxGetApp();
		pApp->pPlotWnd = m_pPlot3DFrame;
	}
}
