/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
// based on a sample of
// Pierre Alliez <pierre.alliez@cnet.francetelecom.fr>
//
/////////////////////////////////////////////////////////////////////////////
// OpenGLSampleDoc.cpp : Implementierung der Klasse COpenGLSampleDoc
//

#include "StdAfx.h"
#include <istream>

//#include "Matrix.h"
//#include "IntMatrix.h"
//#include "MathAdd.h"

#include "Mix_Prog.h"
#include "MainFrm.h"
#include "Plot3dDoc.h"
#include "Plot3dView.h"
//#include "GraphDlg.h"

using namespace std;
using namespace mymathfuncs;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPlot3dDoc

IMPLEMENT_DYNCREATE(CPlot3dDoc, CGlDocument)

BEGIN_MESSAGE_MAP(CPlot3dDoc, CGlDocument)
	//{{AFX_MSG_MAP(COpenGLSampleDoc)
	//ON_COMMAND(ID_DEMO_ONE, OnDemoOne)
	//ON_COMMAND(ID_DEMO_TWO, OnDemoTwo)
	//ON_COMMAND(ID_DEMO_ZOOM, OnDemoZoom)
	//ON_COMMAND(ID_DEMO_ZOOM_ISO, OnDemoZoomIso)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlot3dDoc Konstruktion/Destruktion

CPlot3dDoc::CPlot3dDoc()
{
	m_nLastAction = KOA_NOTHING;
}

CPlot3dDoc::~CPlot3dDoc()
{
}

void CPlot3dDoc::DeleteContents() 
{
	CGlDocument::DeleteContents();

	m_bIsValid = TRUE;
	m_nLastAction = KOA_NOTHING;
}

BOOL CPlot3dDoc::OnNewDocument()
{
	if (!CGlDocument::OnNewDocument())
		return FALSE;

	ClearViews();

	return TRUE;
}

BOOL CPlot3dDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CGlDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (!m_bIsValid)
	{
		CString mMsg;
		mMsg.Format(IDS_MB_ERROR_OPEN_DOC_MSG, lpszPathName);
		MessageBox(mMsg, NULL, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else
	{
		ClearViews();
	}

	return TRUE;
}

BOOL CPlot3dDoc::OnReopenDocument(BOOL bUpdateViews) 
{
	// UpdateAllViews is not done by ClearViews
	ClearViews(FALSE);

	// UpdateAllViews is not done by CGlDocument::OnReopenDocument
	BOOL bRet = CGlDocument::OnReopenDocument(FALSE);

	if (bUpdateViews)
	{
		UpdateAllViews(NULL);
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CPlot3dDoc Serialisierung

void CPlot3dDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// do not allow storing to file
		ASSERT(FALSE);
	}
	else
	{
		m_bIsValid = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLSampleDoc Diagnose

#ifdef _DEBUG
void CPlot3dDoc::AssertValid() const
{
	CGlDocument::AssertValid();
}

void CPlot3dDoc::Dump(CDumpContext& dc) const
{
	CGlDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLSampleDoc Befehle

/////////////////////////////////////////////////////////////////////////////
// Fenster öffnen/schließen
/////////////////////////////////////////////////////////////////////////////
void CPlot3dDoc::ClearViews(BOOL bUpdateViews)
{
	CView *pView;
	POSITION pos;

	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CPlot3dView)))
		{
			((CPlot3dView*)pView)->m_nKindOfView = KOV_NOTHING;
			((CPlot3dView*)pView)->m_nObjectOfView = OOV_NOTHING;
		}
	}

	if (bUpdateViews)
		UpdateAllViews(NULL);
}

void CPlot3dDoc::ShowViews(BOOL bActivate)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CView *pView;
	POSITION pos;

	// Restore Main Window if it is iconic
	if (bActivate)
	{
		if (pMainFrame->IsIconic())
			pMainFrame->ActivateFrame();
	}

	// Restore Windows and place them
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CPlot3dView)))
		{
			switch (m_nLastAction)
			{
			case KOA_DEMO_PLOT_2D:
				((CPlot3dView*)pView)->m_nKindOfView = (KINDOFVIEW)m_nLastAction;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_SIMPLE;
				break;
			case KOA_DEMO_PLOT_3D:
				((CPlot3dView*)pView)->m_nKindOfView = (KINDOFVIEW)m_nLastAction;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_SIMPLE;
				break;
			case KOA_DEMO_SURF:
				((CPlot3dView*)pView)->m_nKindOfView = (KINDOFVIEW)m_nLastAction;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_SIMPLE;
				break;
			case KOA_DEMO_TOP_VIEW:
				((CPlot3dView*)pView)->m_nKindOfView = (KINDOFVIEW)m_nLastAction;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_SIMPLE;
				break;
			case KOA_DEMO_ZOOM:
				((CPlot3dView*)pView)->m_nKindOfView = KOV_PLOT_2D;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_SIMPLE;
				break;
			case KOA_DEMO_ZOOM_ISOVIEW:
				((CPlot3dView*)pView)->m_nKindOfView = KOV_PLOT_2D;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_ZOOM_ISO;
				break;
			case KOA_DEMO_TWO_OBJECTS:
				((CPlot3dView*)pView)->m_nKindOfView = KOV_SURF;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_TWO_OBJECTS;
				break;
			case KOA_DEMO_ANI:
				((CPlot3dView*)pView)->m_nKindOfView = KOV_PLOT_2D;
				((CPlot3dView*)pView)->m_nObjectOfView = OOV_DEMO_ANI;
				break;
			default:
				{}
			}
			((CPlot3dView*)pView)->InitGeometry();
		}
	}

	UpdateAllViews(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Ergebnisdaten Skalieren
/////////////////////////////////////////////////////////////////////////////
void CPlot3dDoc::Scale2DView(const KINDOFSCALE& nKindOfScale, CScale& matS, mymathfuncs::CMatrix& matX, mymathfuncs::CMatrix& matY)
{
	if (nKindOfScale == KOS_AXIS_EQUAL)						// scale axis simultaneously and do not center them
	{
		double absMaxXX, absMaxYY, absMaxGlob;

		// Detect maxima and minima
		absMaxXX = matMax(fabs(matX));
		absMaxYY = matMax(fabs(matY));
		absMaxGlob = max(absMaxXX, absMaxYY);

		if (absMaxGlob != 0.)
		{
			matX /= absMaxGlob;
			matY /= absMaxGlob;
		}

		matS.m_dXMax = matS.m_dYMax = absMaxGlob;
		matS.m_dXMin = matS.m_dYMin = -absMaxGlob;
	}

	else if (nKindOfScale == KOS_AXIS_EQUAL_CENTER)			// scale axis simultaneously and center them
	{
		double minXX, minYY;
		double maxXX, maxYY;
		double deltaGlob;

		// Detect maxima and minima
		maxXX = matMax(matX);
		minXX = matMin(matX);
		maxYY = matMax(matY);
		minYY = matMin(matY);
		deltaGlob = max(maxXX-minXX, maxYY-minYY);

		matX *= 2.;
		matX -= maxXX + minXX;
		matY *= 2.;
		matY -= maxYY + minYY;
		if (deltaGlob > 0.)
		{
			matX /= deltaGlob;
			matY /= deltaGlob;
		}

		matS.m_dXMax = (maxXX + minXX + deltaGlob)/2.;
		matS.m_dXMin = (maxXX + minXX - deltaGlob)/2.;
		matS.m_dYMax = (maxYY + minYY + deltaGlob)/2.;
		matS.m_dYMin = (maxYY + minYY - deltaGlob)/2.;
	}

	else if (nKindOfScale == KOS_AXIS_SINGLE_CENTER)		// scale axis separately and center them
	{
		double minXX, minYY;
		double maxXX, maxYY;

		// Detect maxima and minima
		maxXX = matMax(matX);
		minXX = matMin(matX);
		maxYY = matMax(matY);
		minYY = matMin(matY);

		matX *= 2.;
		matX -= maxXX + minXX;
		matY *= 2.;
		matY -= maxYY + minYY;

		if (maxXX > minXX)
			matX /= maxXX - minXX;
		if (maxYY > minYY)
			matY /= maxYY - minYY;

		matS.m_dXMax = maxXX;
		matS.m_dXMin = minXX;
		matS.m_dYMax = maxYY;
		matS.m_dYMin = minYY;
	}

	else if (nKindOfScale == KOS_AXIS_SINGLE_CENTER_ZERO)	// scale axis separately and center them, y to zero
	{
		double minXX, minYY;
		double maxXX, maxYY;

		// Detect maxima and minima
		maxXX = matMax(matX);
		minXX = matMin(matX);
		maxYY = max(0., matMax(matY));
		minYY = min(0., matMin(matY));

		matX *= 2.;
		matX -= maxXX + minXX;
		matY *= 2.;
		matY -= maxYY + minYY;

		if (maxXX > minXX)
			matX /= maxXX - minXX;
		if (maxYY > minYY)
			matY /= maxYY - minYY;

		matS.m_dXMax = maxXX;
		matS.m_dXMin = minXX;
		matS.m_dYMax = maxYY;
		matS.m_dYMin = minYY;
	}
}

void CPlot3dDoc::Scale3DView(const KINDOFSCALE& nKindOfScale, CScale& matS, mymathfuncs::CMatrix& matX, mymathfuncs::CMatrix& matY, mymathfuncs::CMatrix& matZ)
{
	if (nKindOfScale == KOS_AXIS_EQUAL)						// scale axis simultaneously and do not center them
	{
		double absMaxXX, absMaxYY, absMaxZZ, absMaxGlob;

		// Detect maxima and minima
		absMaxXX = matMax(fabs(matX));
		absMaxYY = matMax(fabs(matY));
		absMaxZZ = matMax(fabs(matZ));
		absMaxGlob = max(max(absMaxXX, absMaxYY), absMaxZZ);

		if (absMaxGlob != 0.)
		{
			matX /= absMaxGlob;
			matY /= absMaxGlob;
			matZ /= absMaxGlob;
		}

		matS.m_dXMax = matS.m_dYMax = matS.m_dZMax = absMaxGlob;
		matS.m_dXMin = matS.m_dYMin = matS.m_dZMin = -absMaxGlob;
	}

	else if (nKindOfScale == KOS_AXIS_EQUAL_CENTER)			// scale axis simultaneously and center them
	{
		double minXX, minYY, minZZ;
		double maxXX, maxYY, maxZZ;
		double deltaGlob;

		// Detect maxima and minima
		maxXX = matMax(matX);
		minXX = matMin(matX);
		maxYY = matMax(matY);
		minYY = matMin(matY);
		maxZZ = matMax(matZ);
		minZZ = matMin(matZ);
		deltaGlob = max(max(maxXX-minXX, maxYY-minYY), maxZZ-minZZ);

		matX *= 2.;
		matX -= maxXX + minXX;
		matY *= 2.;
		matY -= maxYY + minYY;
		matZ *= 2.;
		matZ -= maxZZ + minZZ;

		if (deltaGlob > 0.)
		{
			matX /= deltaGlob;
			matY /= deltaGlob;
			matZ /= deltaGlob;
		}

		matS.m_dXMax = (maxXX + minXX + deltaGlob)/2.;
		matS.m_dXMin = (maxXX + minXX - deltaGlob)/2.;
		matS.m_dYMax = (maxYY + minYY + deltaGlob)/2.;
		matS.m_dYMin = (maxYY + minYY - deltaGlob)/2.;
		matS.m_dZMax = (maxZZ + minZZ + deltaGlob)/2.;
		matS.m_dZMin = (maxZZ + minZZ - deltaGlob)/2.;
	}

	else if (nKindOfScale == KOS_AXIS_SINGLE_CENTER)		// scale axis separately and center them
	{
		double minXX, minYY, minZZ;
		double maxXX, maxYY, maxZZ;

		// Detect maxima and minima
		maxXX = matMax(matX);
		minXX = matMin(matX);
		maxYY = matMax(matY);
		minYY = matMin(matY);
		maxZZ = matMax(matZ);
		minZZ = matMin(matZ);

		matX *= 2.;
		matX -= maxXX + minXX;
		matY *= 2.;
		matY -= maxYY + minYY;
		matZ *= 2.;
		matZ -= maxZZ + minZZ;

		if (maxXX > minXX)
			matX /= maxXX - minXX;
		if (maxYY > minYY)
			matY /= maxYY - minYY;
		if (maxZZ > minZZ)
			matZ /= maxZZ - minZZ;

		matS.m_dXMax = maxXX;
		matS.m_dXMin = minXX;
		matS.m_dYMax = maxYY;
		matS.m_dYMin = minYY;
		matS.m_dZMax = maxZZ;
		matS.m_dZMin = minZZ;
	}

	else if (nKindOfScale == KOS_AXIS_SINGLE_CENTER_ZERO)	// scale axis separately and center them, z to zero
	{
		double minXX, minYY, minZZ;
		double maxXX, maxYY, maxZZ;

		// Detect maxima and minima
		maxXX = matMax(matX);
		minXX = matMin(matX);
		maxYY = matMax(matY);
		minYY = matMin(matY);
		maxZZ = max(0., matMax(matZ));
		minZZ = min(0., matMin(matZ));

		matX *= 2.;
		matX -= maxXX + minXX;
		matY *= 2.;
		matY -= maxYY + minYY;
		matZ *= 2.;
		matZ -= maxZZ + minZZ;

		if (maxXX > minXX)
			matX /= maxXX - minXX;
		if (maxYY > minYY)
			matY /= maxYY - minYY;
		if (maxZZ > minZZ)
			matZ /= maxZZ - minZZ;

		matS.m_dXMax = maxXX;
		matS.m_dXMin = minXX;
		matS.m_dYMax = maxYY;
		matS.m_dYMin = minYY;
		matS.m_dZMax = maxZZ;
		matS.m_dZMin = minZZ;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Demo Stoff
/////////////////////////////////////////////////////////////////////////////
void CPlot3dDoc::ComputeDemoOne()
{
	int i, j;
	double x, y;

	int anzSchritteAlpha = 40;
	int anzSchrittePsi = 30;

	m_X.SetSize(CSize(anzSchrittePsi, anzSchritteAlpha));
	m_Y.SetSize(CSize(anzSchrittePsi, anzSchritteAlpha));

	for (i = 1; i <= anzSchrittePsi; i++)
	{
		y = 4./(anzSchrittePsi-1.)*(i-1.) - 2.;
		for (j = 1; j <= anzSchritteAlpha; j++)
		{
			x = 4./(anzSchritteAlpha-1.)*(j-1.) - 2.;
			m_X(i,j) = x;
			m_Y(i,j) = y;
		}
	}
	m_Z = exp(-m_X.DotTimes(m_X)-m_Y.DotTimes(m_Y));

	m_X = m_X.GetRows(1);
	m_Y = m_Y.GetColumns(1);
}

void CPlot3dDoc::ComputeDemoTwo()
{
	int i, j;
	double r, alpha;

	int anzSchritteAlpha = 40;
	int anzSchrittePsi = 20;

	m_X.SetSize(CSize(anzSchrittePsi+1, anzSchritteAlpha+1));
	m_Y.SetSize(CSize(anzSchrittePsi+1, anzSchritteAlpha+1));

	for (i = 0; i <= anzSchrittePsi; i++)
	{
		r = 2.*i/double(anzSchrittePsi);
		for (j = 0; j <= anzSchritteAlpha; j++)
		{
			alpha = j*2.*pi/anzSchritteAlpha;
			m_X(i+1,j+1) = r*sin(alpha);
			m_Y(i+1,j+1) = r*cos(alpha);
		}
	}
	m_Z = exp(-m_X.DotTimes(m_X)-m_Y.DotTimes(m_Y));
}



void CPlot3dDoc::OnDemoOne() 
{
	m_nLastAction = KOA_DEMO_PLOT_2D;
	ComputeDemoOne();
	Scale3DView(KOS_AXIS_SINGLE_CENTER, m_S, m_X, m_Y, m_Z);
	ShowViews();
}

void CPlot3dDoc::OnDemoTwo() 
{
	m_nLastAction = KOA_DEMO_PLOT_3D;
	ComputeDemoTwo();
	Scale3DView(KOS_AXIS_SINGLE_CENTER, m_S, m_X, m_Y, m_Z);
	ShowViews();
}


void CPlot3dDoc::OnDemoZoom() 
{
	m_nLastAction = KOA_DEMO_ZOOM;
	//ComputeDemoThree();
	Scale3DView(KOS_AXIS_SINGLE_CENTER, m_S, m_X, m_Y, m_Z);
	ShowViews();
}

void CPlot3dDoc::OnDemoZoomIso() 
{
	m_nLastAction = KOA_DEMO_ZOOM_ISOVIEW;
	//ComputeDemoThree();
	Scale3DView(KOS_AXIS_SINGLE_CENTER, m_S, m_X, m_Y, m_Z);
	ShowViews();
}



void CPlot3dDoc::LoadPlotData(CGraphDlg *pGraphDlg)
{
	// Load the fluence data if the respective file exists
	// The file should contain only five columns. The first two columns
	// contain X and Y coordinate data.

	CFileDialog dlg(TRUE, "*.dat", "*.dat");
	dlg.m_ofn.lpstrTitle = "Open output data file";
	CString fileout;
	ifstream stream;
	int size = 0;
	int nx, ny;
	char buf[300];

	if(dlg.DoModal() == IDOK) {
		fileout = dlg.GetPathName();
		stream.open(fileout.GetBuffer(12));
	}

	if(!stream) 
		return;
	else
	{
		CView *pView;
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			pView = GetNextView(pos);
			if (pView->IsKindOf(RUNTIME_CLASS(CPlot3dView)))
			{
				((CPlot3dView*)pView)->m_nScaleShowGrid = 1;
			}
		}

		// get the total number of points
		while(!stream.eof())
		{
			stream.getline(buf, 300);
			size++;
		}
		stream.seekg(0);
		stream.close();
		size = size - 1;

		// allocate space in memory for data
		Fluences* pData;
		pData = new Fluences;
		delete[] pGraphDlg->m_pFluenceData;
		pGraphDlg->m_pFluenceData = pData;
		pData->pFluence_s = new double[size];
		pData->pFluence_p = new double[size];
		pData->pFluence_i = new double[size];
		pData->pXgrid = new double[size];
		pData->pYgrid = new double[size];

		stream.open(fileout.GetBuffer(12), ifstream::out);
		int k2 = 0;
		while(!stream.eof())
		{			
			stream >> pData->pXgrid[k2]; 
			stream >> pData->pYgrid[k2];
			stream >> pData->pFluence_s[k2]; 
			stream >> pData->pFluence_i[k2];
			stream >> pData->pFluence_p[k2];
			k2++;
		}
		stream.seekg(0);
		stream.close();

		// find the dimensions nx and ny of the matrix
		k2 = 0;
		double temp1 = pData->pXgrid[0];
		double temp2 = pData->pXgrid[1];
		if(temp1 == temp2)
		{
			double temp = pData->pXgrid[0];
			while(pData->pXgrid[k2] == temp)
				k2++;
			nx = k2;
			ny = size/k2;
		}
		else
		{
			double temp = pData->pYgrid[0];
			while(pData->pYgrid[k2] == temp)
				k2++;
			ny = k2;
			nx = size/k2;
		}

		m_X.RemoveAll();
		m_Y.RemoveAll();
		m_Z.RemoveAll();
		m_X.SetSize(CSize(nx, ny));
		m_Y.SetSize(CSize(nx, ny));
		m_Z.SetSize(CSize(nx, ny));

		stream.open(fileout.GetBuffer(12), ifstream::out);
		int ii, jj;
		float xval, yval;
		int datTypeChoice = pGraphDlg->GetCheckedRadioButton(IDC_TYPE_SIG, IDC_TYPE_PUM);
		for(ii=0; ii < ny; ii++)
		{
			for(jj=0; jj < nx; jj++)
			{
				k2 = jj + ii*nx;
				stream >> xval;
				stream >> yval;
				stream >> pData->pFluence_s[k2];
				stream >> pData->pFluence_i[k2];
				stream >> pData->pFluence_p[k2];
				pData->pXgrid[k2] = xval;
				pData->pYgrid[k2] = yval;

				m_X(ii+1, jj+1) = xval;
				m_Y(ii+1, jj+1) = yval;
				
				switch(datTypeChoice)
				{
				case IDC_TYPE_SIG:
					m_Z(ii+1, jj+1) = pData->pFluence_s[k2];
					break;
				case IDC_TYPE_IDL:
					m_Z(ii+1, jj+1) = pData->pFluence_i[k2];
					break;
				default :
					m_Z(ii+1, jj+1) = pData->pFluence_p[k2];
					break;
				}
			}
		}
		k2++;
		stream >> xval;
		stream >> yval;
		stream >> pData->pFluence_s[k2];
		stream >> pData->pFluence_i[k2];
		stream >> pData->pFluence_p[k2];
		pData->pXgrid[k2] = xval;
		pData->pYgrid[k2] = yval;

		m_X(ii+1, jj+1) = xval;
		m_Y(ii+1, jj+1) = yval;

		switch(datTypeChoice)
		{
		case IDC_TYPE_SIG:
			m_Z(ii+1, jj+1) = pData->pFluence_s[k2];
			break;
		case IDC_TYPE_IDL:
			m_Z(ii+1, jj+1) = pData->pFluence_i[k2];
			break;
		default :
			m_Z(ii+1, jj+1) = pData->pFluence_p[k2];
			break;
		}

		pData->nx = nx;
		pData->ny = ny;

		double tmp = m_X(nx-1, ny-1);

		stream.seekg(0);
		stream.close();

		Scale3DView(KOS_AXIS_SINGLE_CENTER, m_S, m_X, m_Y, m_Z);
		ShowViews();
	}
}

void CPlot3dDoc::ShowPlot(CGraphDlg *pGraphDlg)
{
	int nx, ny;

	CView *pView;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CPlot3dView)))
		{
			((CPlot3dView*)pView)->m_nScaleShowGrid = 1;
		}
	}

	Fluences *pData = pGraphDlg->m_pFluenceData;

	if((pData != NULL) && (pData->nx != 0) && (pData->ny != 0))
	{
		nx = pData->nx;
		ny = pData->ny;

		m_X.RemoveAll();
		m_Y.RemoveAll();
		m_Z.RemoveAll();
		m_X.SetSize(CSize(nx, ny));
		m_Y.SetSize(CSize(nx, ny));
		m_Z.SetSize(CSize(nx, ny));

		int datTypeChoice = pGraphDlg->GetCheckedRadioButton(IDC_TYPE_SIG, IDC_TYPE_PUM);

		int ii, jj, k2;
		for(ii=0; ii < ny; ii++)
		{
			for(jj=0; jj < nx; jj++)
			{
				k2 = jj + ii*nx;

				m_X(ii+1, jj+1) = pData->pXgrid[k2];
				m_Y(ii+1, jj+1) = pData->pYgrid[k2];
				
				switch(datTypeChoice)
				{
				case IDC_TYPE_SIG:
					m_Z(ii+1, jj+1) = pData->pFluence_s[k2];
					break;
				case IDC_TYPE_IDL:
					m_Z(ii+1, jj+1) = pData->pFluence_i[k2];
					break;
				default :
					m_Z(ii+1, jj+1) = pData->pFluence_p[k2];
					break;
				}
			}
		}
		k2++;
		m_X(ii+1, jj+1) = pData->pXgrid[k2];
		m_Y(ii+1, jj+1) = pData->pYgrid[k2];

		switch(datTypeChoice)
		{
		case IDC_TYPE_SIG:
			m_Z(ii+1, jj+1) = pData->pFluence_s[k2];
			break;
		case IDC_TYPE_IDL:
			m_Z(ii+1, jj+1) = pData->pFluence_i[k2];
			break;
		default :
			m_Z(ii+1, jj+1) = pData->pFluence_p[k2];
			break;
		}

		Scale3DView(KOS_AXIS_SINGLE_CENTER, m_S, m_X, m_Y, m_Z);
		ShowViews();
	}
}