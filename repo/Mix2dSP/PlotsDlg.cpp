// PlotsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "PlotsDlg.h"

#include "Mix_Prog.h"
#include "Inputs.h"
#include "Fields.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotsDlg dialog


CPlotsDlg::CPlotsDlg(CWnd* pParent)
	: CDialog(CPlotsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlotsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);
	m_pParent = pParent;	//pointer to parent window
	m_nID = CPlotsDlg::IDD;	//dialog'sID
	m_GLPixelIndex = 0;		//PixelIndex used for OpenGL plotting
	m_hGLContext = NULL;	//pointer to GL context
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_ScaleZ = 1.0f;
	m_xRotate = 0;
	m_yRotate = 0;
	m_LButtonDown = FALSE;	//initialize mouse left button state
	pField = NULL;			//initialize pointer to CFields type data for plotting fields
	pPulse = NULL;			//initialize pointer to input data stored with CInputs
	pGV = NULL;				//initialize pointer to GlobalVar type of data for plotting
	pFluenceData = NULL;
	m_bFluenceLoaded = FALSE;	//set the indicator of loaded data
}

BOOL CPlotsDlg::Create() 
{
	return CDialog::Create(m_nID, m_pParent);
}

void CPlotsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotsDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotsDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_PLOT_LOAD, OnPlotLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotsDlg message handlers

int CPlotsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)		return -1;
 	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if(SetWindowPixelFormat(hDC)==FALSE)	return 0;	
	if(CreateViewGLContext(hDC)==FALSE)		return 0;

	return 0;
}

void CPlotsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if(wglGetCurrentContext() != NULL)		wglMakeCurrent(NULL,NULL);
	if(m_hGLContext != NULL)	
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	::ReleaseDC(hWnd, hDC);	
}

void CPlotsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete(this);
}

void CPlotsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Useful in multidoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);

	CWnd* pCloseBtn = GetDlgItem(IDOK);
	CWnd* pLoadBtn = GetDlgItem(IDC_PLOT_LOAD);
	pCloseBtn->MoveWindow(395,5,53,23);
	pLoadBtn->MoveWindow(395,30,53,23);

	// Start rendering...
	RenderScene();
	
	// Double buffer
	SwapBuffers(dc.m_ps.hdc);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CPlotsDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LButtonDown = TRUE;
	m_LDownPos = point;	
	CDialog::OnLButtonDown(nFlags, point);
}

void CPlotsDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LButtonDown = FALSE;	
	CDialog::OnLButtonUp(nFlags, point);
}

void CPlotsDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_LButtonDown)
	{
		//CGLSample4Doc* pDoc = GetDocument();
		CSize rotate = m_LDownPos - point;
		m_LDownPos = point;
		m_yRotate -= rotate.cx/2;
		m_xRotate -= rotate.cy/2;
		InvalidateRect(NULL, FALSE);
	}	
	CDialog::OnMouseMove(nFlags, point);
}


//*************************************************************************//
//*************************************************************************//
// User functions
//*************************************************************************//
//*************************************************************************//

void CPlotsDlg::OnPlotsClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnClose();
//	((COutputDlg*)m_pParent)->ResetOutputDlg();
//	DestroyWindow();	
}


int CPlotsDlg::LoadFieldData()
{
	//**********************************************************************
	// Load the electric fields from the selected files output files

	int ii, i2;

	const double Pi = 3.14159265359;
	const double epsilon_0 = 8.854187817e-12;	// Permittivity of vacuum [F/m]
	const double c = 2.99792458e+8;				// Speed of light in vacuum [m/s]
	
	//***********************************************************************
	// Set local variables to their corresponding global variable values

	pPulse	= new CInputs;
	pGV		= new CGlobalVar;

	// Read in the data from BBOPO.in
	Reader(pPulse);
	ConvertToSI(pPulse);
	
	// Get the values for the array sizes
	long nx = long(pPulse->GetVar(NX));
	long ny = long(pPulse->GetVar(NY));
	long nt = long(pPulse->GetVar(NT));
	
	pField = new CFields(nx*ny*nt, nx, ny, nt);

	SetGrids(pPulse,pGV,pField);

	// CInputs variables
	double Lambda_s = pPulse->GetVar(LAMBDA_S);	// Wavelengths [m]
	double Lambda_i = pPulse->GetVar(LAMBDA_I);
	double Lambda_p = pPulse->GetVar(LAMBDA_P);

	double Dist = pPulse->GetVar(DIST);			// Distance to the detector [m]
	long nz = long(pPulse->GetVar(NZ));			// no. of grid pts. z-direction


	// step sizes in x,y,t
	double dx = pGV->dGetGlobal(DX);
	double dy = pGV->dGetGlobal(DY);
	double dt = pGV->dGetGlobal(DT);

	//***********************************************************************
	// Load the field data if the respective files exist

	ifstream streams("EField_s.dat");
	ifstream streami("EField_i.dat");
	ifstream streamp("EField_p.dat");
	if(!streams && !streami && !streamp) return -1;
	else
	{
		pField->pEField_s = new float[2*nx*ny*nt];
		pField->pEField_p = new float[2*nx*ny*nt];
		pField->pEField_i = new float[2*nx*ny*nt];

		for(ii=0;ii<nx*ny*nt;++ii)
		{
			i2=2*(ii+1);

			//read EField_s value
			streams >> pField->pEField_s[i2-1];
			streams >> pField->pEField_s[i2];

			//read EField_i value
			streami >> pField->pEField_i[i2-1];
			streami >> pField->pEField_i[i2];

			//read EField_p value
			streamp >> pField->pEField_p[i2-1];
			streamp >> pField->pEField_p[i2];
		}
		streams.close();
		streami.close();
		streamp.close();
	}
	return 0;
}

int CPlotsDlg::LoadFluenceData()
{
	pPulse	= new CInputs;

	// Read in the data from INPUTS.in
	Reader(pPulse);
	ConvertToSI(pPulse);
	
	// Get the values for the array sizes
	long nx = long(pPulse->GetVar(NX));
	long ny = long(pPulse->GetVar(NY));
	long nt = long(pPulse->GetVar(NT));

	//***********************************************************************
	// Load the fluence data if the respective file exists

	CFileDialog dlg(TRUE, "*.dat", "*.dat");
	dlg.m_ofn.lpstrTitle = "Open output data file";
	CString fileout;
	ifstream stream;

	if(dlg.DoModal() == IDOK) {
		fileout = dlg.GetFileName();
		stream.open(fileout,ios_base::in);
	}	
	if(!stream) return -1;
	else
	{
		int ii, jj, k2;
		pFluenceData = new Fluences;
		pFluenceData->pFluence_s = new double[nx*ny];
		pFluenceData->pFluence_p = new double[nx*ny];
		pFluenceData->pFluence_i = new double[nx*ny];
		pFluenceData->pXgrid = new double[nx*ny];
		pFluenceData->pYgrid = new double[nx*ny];
		
		for(ii=0;ii<ny;++ii)
		{
			for(jj=0;jj<nx;++jj)
			{
				k2=(ii+1) + jj*ny;
				stream >> pFluenceData->pXgrid[k2];
				stream >> pFluenceData->pYgrid[k2];
				stream >> pFluenceData->pFluence_s[k2];
				stream >> pFluenceData->pFluence_i[k2];
				stream >> pFluenceData->pFluence_p[k2];
			}
		}
		stream.close();
	}

	return 0;
}

void CPlotsDlg::BuildList()
{
	// Get the values for the array sizes
	long nx = long(pPulse->GetVar(NX));
	long ny = long(pPulse->GetVar(NY));
	long nt = long(pPulse->GetVar(NT));

	int k2, ii, jj;
	float x, y, z;
	
	//***************************************
	// Begin building the list
	::glNewList(1,GL_COMPILE_AND_EXECUTE);

	for(ii=0;ii<ny;++ii)
	{
		glBegin(GL_LINE_STRIP);
		for (jj=0;jj<nx;++jj)
		{
			k2=(ii+1)+jj*ny;
			glVertex3f(
				pFluenceData->pXgrid[k2] * 3.0e3, 
				pFluenceData->pYgrid[k2] * 3.0e3, 
				pFluenceData->pFluence_s[k2] * 3.0e3
				);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (jj=0;jj<nx;++jj)
		{
			k2=(jj+1)+ii*nx;
			glVertex3f(
				pFluenceData->pXgrid[k2] * 3.0e3, 
				pFluenceData->pYgrid[k2] * 3.0e3, 
				pFluenceData->pFluence_s[k2] * 3.0e3
				);
		}
		glEnd();
	}

	::glEndList();
}

void CPlotsDlg::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearDepth( 1.0 );
	glEnable(GL_DEPTH_TEST);
	
	// Default mode
	glPolygonMode(GL_FRONT,GL_LINE);
	glPolygonMode(GL_BACK,GL_LINE);
	glShadeModel(GL_FLAT);
	//glEnable(GL_NORMALIZE);  // for some reason this line kills it
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0);

	// Lights properties
	GLfloat	ambientProperties[]  = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat	diffuseProperties[]  = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	//Material properties
	GLfloat RedSurface[]   = { 1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat GreenSurface[] = { 0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat BlueSurface[]  = { 0.0f, 0.0f, 1.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, GreenSurface);

	// Default : lighting
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);	//this command also kills it (why?)

	CRect pos;
	GetClientRect(pos);
	GLdouble width, height;
	width = (GLdouble)(pos.right-pos.left);
	height = (GLdouble)(pos.bottom-pos.top);

	//Projection View
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*the following commands move the plot out of the view. Parameters need to be tweaked*/
	//gluPerspective(60, width/height, 0.65, 700.0);
	//glOrtho(-30.0, 30.0, 30.0, -5.0, 50.0, 10000);

	//ModelView
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, height*0.9, height*0.9);

	glPushMatrix();

	glTranslated(0.0, 0.0, 0.0);
	glRotated(m_xRotate, 1.0, 0.0, 0.0);
	glRotated(m_yRotate, 0.0, 0.0, 1.0);

	// Create a test sphere
	//GLUquadricObj* quad = gluNewQuadric();
	//gluSphere(quad, 0.7, 20, 10);

	//if(m_bFluenceLoaded) BuildList();
	BuildList();
	::glCallList(1);
	
	glPopMatrix();
}


BOOL CPlotsDlg::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);

	if(m_hGLContext==NULL)							return FALSE;
	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)		return FALSE;
	return TRUE;
}


BOOL CPlotsDlg::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_STEREO_DONTCARE;
 
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
 	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex==0) // Choose default
	{
		m_GLPixelIndex = 1;
		if(!DescribePixelFormat(hDC,m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc))
			return FALSE;
	}

	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))		return FALSE;
	
	return TRUE;
}


void CPlotsDlg::OnPlotLoad() 
{
	// TODO: Add your control notification handler code here
	//LoadFieldData();		//load field data at the window's creation time

	// Create a test sphere
	GLUquadricObj* quad = gluNewQuadric();
	gluSphere(quad, 0.7, 20, 10);

	glPopMatrix();
}
