// Plot3dView.cpp : implementation file
//

#include "stdafx.h"
#include "mix2dsp.h"
#include "Plot3dView.h"
#include "Plot3dDoc.h"
#include "Plot3dFrameWnd.h"

// CPlot3dView

IMPLEMENT_DYNCREATE(CPlot3dView, CGlView)

CPlot3dView::CPlot3dView()
{
	m_nKindOfView = KOV_NOTHING;
	m_nObjectOfView = OOV_NOTHING;
	CGlView::InitGeometry();
}

CPlot3dView::~CPlot3dView()
{
}

BEGIN_MESSAGE_MAP(CPlot3dView, CGlView)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT2, &CPlot3dView::OnEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT2, &CPlot3dView::OnUpdateEditProperties)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()


// CPlot3dView drawing

void CPlot3dView::OnDraw(CDC* pDC)
{
	CPlot3dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	GLfloat light0Position[4];
	PIXELFORMATDESCRIPTOR pfd;
	int	nGLPixelIndex;
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	// Do not make m_hRC aktive when printing
	if (!pDC->IsPrinting())
	{
		GetCurrent(hDC, hRC);		//Store current rendering and device contexts
		MakeActive();				//Make view's rendering context current
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF)
		gluPerspective(35.0,m_aspect,1.0,15.0);
	else
		gluPerspective(35.0,m_aspect,5.0,15.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (!m_bIsDocPrinting)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	// Default light position
	light0Position[0] = light0Position[1] = light0Position[3] = 0.0f;
	light0Position[2] = 1.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	// Light position
	glTranslated(0.0, 0.0, -5.0);
	if (m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF)
	{
		light0Position[0] = -5.0f;
		light0Position[1] = 2.0f;
		light0Position[2] = 5.0f;
		light0Position[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	}

	// Position / translation / scale
	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotated(m_xRotation, 1.0, 0.0, 0.0);
	glRotated(m_yRotation, 0.0, 1.0, 0.0);
	glScaled(m_xScaling, m_yScaling, m_zScaling);

	// Start rendering...
	if (m_nObjectOfView == OOV_DEMO_SIMPLE || m_nObjectOfView == OOV_DEMO_ZOOM_ISO || m_nObjectOfView == OOV_DEMO_ANI)	//simple demo
	{
		if (m_nKindOfView == KOV_PLOT_2D)						//plot2d
		{
			GLfloat tmpLineWidth[1];
			glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
			glLineWidth(1.5);
			glTranslated(0.0, 0.0, -eps5);	// Put the Plot2d object an epsilon back
			Plot2d(pDoc->m_X, pDoc->m_Y, pDoc->m_Z);
			glTranslated(0.0, 0.0, eps5);	// Done
			if (m_bFirstPickPoint)
				DrawZoomRect();
			glLineWidth(tmpLineWidth[0]);
		}
		else if (m_nKindOfView == KOV_PLOT_3D)					//plot3d
		{
			GLfloat tmpLineWidth[1];
			glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
			glLineWidth(1.5);
			Plot3d(pDoc->m_X, pDoc->m_Y, pDoc->m_Z);
			glLineWidth(tmpLineWidth[0]);
		}
		else if (m_nKindOfView == KOV_SURF)						//surf
		{
			Surf(pDoc->m_X, pDoc->m_Y, pDoc->m_Z);
		}
		else if (m_nKindOfView == KOV_TOP_VIEW)					//topview
		{
			GLfloat tmpLineWidth[1];
			glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
			glLineWidth(1.0);
			glTranslated(0.0, 0.0, -eps5);	// Put the TopView object an epsilon back
			//TopView(pDoc->m_X, pDoc->m_Y, pDoc->m_Z);
			glTranslated(0.0, 0.0, eps5);	// Done
			if (m_bFirstPickPoint)
				DrawZoomRect();
			glLineWidth(tmpLineWidth[0]);
		}
		if (m_nKindOfView == KOV_PLOT_2D || m_nKindOfView == KOV_TOP_VIEW)
		{
			glTranslated(0.0, 0.0, -2.*eps5);	// Put the Scale2d object two epsilons back
			Scale2d(pDoc->m_S);
			glTranslated(0.0, 0.0, 2.*eps5);	// Done
		}
		else if (m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF)
		{
			Scale3d(pDoc->m_S);
		}
	}
	else if (m_nObjectOfView == OOV_DEMO_TWO_OBJECTS)																		//special demo
	{
		//SurfMulti(pDoc->m_X, pDoc->m_Y, pDoc->m_Z, pDoc->m_X2, pDoc->m_Y2, pDoc->m_Z2, pDoc->m_C2);
		//Scale3d(pDoc->m_S);
	}

	glPopMatrix();

	// Swap the buffers in case of double buffering
	nGLPixelIndex = GetPixelFormat(pDC->GetSafeHdc());
	DescribePixelFormat(pDC->GetSafeHdc(), nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (pfd.dwFlags & PFD_DOUBLEBUFFER)
		SwapBuffers(pDC->GetSafeHdc());

	if (!pDC->IsPrinting() && hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);					//Restore last rendering and device contexts
	
	CGlView::OnDraw(pDC);
}


// CPlot3dView diagnostics

#ifdef _DEBUG
void CPlot3dView::AssertValid() const
{
	CGlView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlot3dView::Dump(CDumpContext& dc) const
{
	CGlView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlot3dView message handlers
/////////////////////////////////////////////////////////////////////////////
// GetFormPropertiesView
/////////////////////////////////////////////////////////////////////////////
CView* CPlot3dView::GetFormPropertiesView() 
{
	CPlot3dFrameWnd *pFrame = (CPlot3dFrameWnd *)GetParentFrame();
	CView *pView = (CView *)pFrame->m_wndSplitter.GetPaneEx(0,0);
	if (!pView->IsKindOf(RUNTIME_CLASS(CFormPropertiesView)))
		pView = NULL;
	//CView *pView = NULL;

	return pView;
}

CPlot3dDoc* CPlot3dView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlot3dDoc)));
	return (CPlot3dDoc*)m_pDocument;
}
void CPlot3dView::UpdateSliderScalePos()
{
	CFormPropertiesView* pView = (CFormPropertiesView *)GetFormPropertiesView();
	if (pView)
	{
		pView->UpdateSliderScalePos();
	}
}

void CPlot3dView::UpdateSliderTranslationPos()
{
	CFormPropertiesView* pView = (CFormPropertiesView *)GetFormPropertiesView();
	if (pView)
	{
		pView->UpdateSliderTranslationPos();
	}
}

void CPlot3dView::UpdateFontColorRect()
{
	//CFormPropertiesView* pView = (CFormPropertiesView *)GetFormPropertiesView();
	//if (pView)
	//{
	//	pView->UpdateFontColorRect();
	//}
}
BOOL CPlot3dView::Is3D() const
{
	return (m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF);
}

BOOL CPlot3dView::AllowScale() const
{
	BOOL bAllowScale = FALSE;
	if (m_nObjectOfView == OOV_DEMO_SIMPLE || 
		m_nObjectOfView == OOV_DEMO_ZOOM_ISO || 
		m_nObjectOfView == OOV_DEMO_ANI)
	{
		if (m_nKindOfView == KOV_PLOT_2D)
		{
			bAllowScale = TRUE;
		}
		else if (m_nKindOfView == KOV_PLOT_3D)
		{
			bAllowScale = TRUE;
		}
		else if (m_nKindOfView == KOV_SURF)
		{
			bAllowScale = TRUE;
		}
		else if (m_nKindOfView == KOV_TOP_VIEW)
		{
			bAllowScale = TRUE;
		}
	}
	else if (m_nObjectOfView == OOV_DEMO_TWO_OBJECTS)																		//special demo
	{
		bAllowScale = TRUE;
	}
	return bAllowScale;
}

BOOL CPlot3dView::AllowMonochrome() const
{
	return (m_nKindOfView == KOV_PLOT_2D || m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF);
}
BOOL CPlot3dView::AllowRemoveHidden() const
{
	return (m_nKindOfView == KOV_SURF);
}

BOOL CPlot3dView::AllowModel() const
{
	return (m_nKindOfView == KOV_SURF || m_nKindOfView == KOV_TOP_VIEW);
}

void CPlot3dView::EditProperties() 
{
	CPlot3dFrameWnd *pFrame = (CPlot3dFrameWnd *)GetParentFrame();

	if (pFrame->IsLeftPaneClosed())
	{
		if (m_nKindOfView > KOV_NOTHING)
		{
			pFrame->m_wndSplitter.ShowColumn(0);
			pFrame->SizeLeftPane();

			UpdateSliderScalePos();
			UpdateSliderTranslationPos();
		}
	}
	else
		pFrame->m_wndSplitter.HideColumn(0);

	pFrame->m_wndSplitter.RecalcLayout();
}
void CPlot3dView::UpdateEditProperties(CCmdUI* pCmdUI) 
{
	BOOL bIsColHidden = ((CPlot3dFrameWnd*)GetParentFrame())->m_wndSplitter.IsColHidden(0);
	//pCmdUI->Enable(m_nKindOfView > KOV_NOTHING || !bIsColHidden);
	pCmdUI->SetCheck(!bIsColHidden);
}

void CPlot3dView::OnEditProperties() 
{
	EditProperties();
}

void CPlot3dView::OnUpdateEditProperties(CCmdUI* pCmdUI) 
{
	UpdateEditProperties(pCmdUI);
}
int CPlot3dView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGlView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CPlot3dView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CGlView::OnUpdate(pSender, lHint, pHint);
}

BOOL CPlot3dView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CGlView::PreCreateWindow(cs);
}

void CPlot3dView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPlot3dFrameWnd* pFrmWnd = (CPlot3dFrameWnd*)GetParentFrame();

	// TODO: Add your message handler code here and/or call default
	if(pFrmWnd->m_bRotationOn == true)
	{
		if (m_nKindOfView == KOV_PLOT_2D || m_nKindOfView == KOV_TOP_VIEW)			// 2D plot or topview
		{
			if (m_bFirstPickPoint)
			{
				GLdouble modelMatrix[16];
				GLdouble projMatrix[16];
				GLint viewport[4];
				GLdouble objx, objy, objz;
				HDC hDC = NULL;
				HGLRC hRC = NULL;

				GetCurrent(hDC, hRC);		//Store current rendering and device contexts
				MakeActive();				//Make view's rendering context current
				glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
				glGetIntegerv(GL_VIEWPORT, viewport);

				CRect rect;
				GetClientRect(&rect);
				point.y = rect.Height() - point.y;
				gluUnProject(point.x, point.y, 0., 
								modelMatrix, projMatrix, viewport, 
								&objx, &objy, &objz);

				//Restore last rendering and device contexts
				if (hDC != NULL && hRC != NULL)
					SetCurrent(hDC, hRC);

				m_EndPoint[0] = (objx-m_xTranslation)/m_xScaling;
				m_EndPoint[1] = (objy-m_yTranslation)/m_yScaling;
				InvalidateRect(NULL, FALSE);
			}
		}
		else if (m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF)		// plot3d or surf
		{
			if (m_bFirstPickPoint)
			{
				m_yRotation -= (double)(m_LeftDownPos.x - point.x)/3.0;
				m_xRotation -= (double)(m_LeftDownPos.y - point.y)/3.0;
				m_LeftDownPos = point;
				InvalidateRect(NULL, FALSE);
			}
		}
	}

	CGlView::OnMouseMove(nFlags, point);
}

void CPlot3dView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMix2dApp *pApp = (CMix2dApp *)AfxGetApp();

	if (pApp->m_nMousePolicy == 0)
		LButtonSwitchOff(point);
	else if (m_bLastActivateWasDoneByLButton)
	{
		m_bLastActivateWasDoneByLButton = FALSE;
		return;
	}
	else
	{
		if (pApp->m_nMousePolicy == 1)
		{
			if (m_bFirstPickPoint)
				LButtonSwitchOff(point);
			else
				LButtonSwitchOn(point);
		}
		if (pApp->m_nMousePolicy == 2)
			LButtonSwitchOn(point);
	}

	CGlView::OnLButtonDown(nFlags, point);
}

void CPlot3dView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMix2dApp *pApp = (CMix2dApp *)AfxGetApp();

	if (pApp->m_nMousePolicy == 2)
		LButtonSwitchOff(point);

	CGlView::OnLButtonUp(nFlags, point);
}

void CPlot3dView::LButtonSwitchOn(CPoint point) 
{
	if (!m_bFirstPickPoint)
	{
		if (m_nKindOfView == KOV_PLOT_2D || m_nKindOfView == KOV_TOP_VIEW)			// 2D plot or topview
		{
			GLdouble modelMatrix[16];
			GLdouble projMatrix[16];
			GLint viewport[4];
			GLdouble objx, objy, objz;
			HDC hDC = NULL;
			HGLRC hRC = NULL;

			GetCurrent(hDC, hRC);		//Store current rendering and device contexts
			MakeActive();				//Make view's rendering context current
			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
			glGetIntegerv(GL_VIEWPORT, viewport);

			CRect rect;
			GetClientRect(&rect);
			int y = rect.Height() - point.y;
			gluUnProject(point.x, y, 0., 
							modelMatrix, projMatrix, viewport, 
							&objx, &objy, &objz);

			//Restore last rendering and device contexts
			if (hDC != NULL && hRC != NULL)
				SetCurrent(hDC, hRC);

			m_EndPoint[0] = m_StartPoint[0] = (objx-m_xTranslation)/m_xScaling;
			m_EndPoint[1] = m_StartPoint[1] = (objy-m_yTranslation)/m_yScaling;

			m_xMin = min(m_StartPoint[0], m_EndPoint[0]);
			m_xMax = max(m_StartPoint[0], m_EndPoint[0]);
			m_yMin = min(m_StartPoint[1], m_EndPoint[1]);
			m_yMax = max(m_StartPoint[1], m_EndPoint[1]);
			if (m_xMin == m_xMax)
			{
				m_xMin -= eps;
				m_xMax += eps;
			}
			if (m_yMin == m_yMax)
			{
				m_yMin -= eps;
				m_yMax += eps;
			}
		}
		else if (m_nKindOfView == KOV_PLOT_3D || m_nKindOfView == KOV_SURF)		// plot3d or surf
		{
			m_xRotationStore = m_xRotation;
			m_yRotationStore = m_yRotation;
			m_LeftDownPos = point;
		}

		m_bFirstPickPoint = TRUE;
	}
}

void CPlot3dView::LButtonSwitchOff(CPoint point) 
{
	CPlot3dFrameWnd* pFrmWnd = (CPlot3dFrameWnd*)GetParentFrame();
	if (m_bFirstPickPoint)
	{
		m_bFirstPickPoint = FALSE;
		pFrmWnd->m_bRotationOn = false;

		if (m_nKindOfView == KOV_PLOT_2D || m_nKindOfView == KOV_TOP_VIEW)			// 2D plot or topview
		{
			GLdouble modelMatrix[16];
			GLdouble projMatrix[16];
			GLint viewport[4];
			GLdouble objx, objy, objz;
			HDC hDC = NULL;
			HGLRC hRC = NULL;

			GetCurrent(hDC, hRC);		//Store current rendering and device contexts
			MakeActive();				//Make view's rendering context current
			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
			glGetIntegerv(GL_VIEWPORT, viewport);

			CRect rect;
			GetClientRect(&rect);
			int y = rect.Height() - point.y;
			gluUnProject(point.x, y, 0., 
							modelMatrix, projMatrix, viewport, 
							&objx, &objy, &objz);

			//Restore last rendering and device contexts
			if (hDC != NULL && hRC != NULL)
				SetCurrent(hDC, hRC);

			m_EndPoint[0] = (objx-m_xTranslation)/m_xScaling;
			m_EndPoint[1] = (objy-m_yTranslation)/m_yScaling;
			if (m_StartPoint[0] == m_EndPoint[0] && m_StartPoint[1] == m_EndPoint[1])
			{
				return;
			}

			m_xMin = min(m_StartPoint[0], m_EndPoint[0]);
			m_xMax = max(m_StartPoint[0], m_EndPoint[0]);
			m_yMin = min(m_StartPoint[1], m_EndPoint[1]);
			m_yMax = max(m_StartPoint[1], m_EndPoint[1]);
			if (m_xMin == m_xMax)
			{
				m_xMin -= eps;
				m_xMax += eps;
			}
			if (m_yMin == m_yMax)
			{
				m_yMin -= eps;
				m_yMax += eps;
			}

			AddToZoomHistory();

			// 2. is due to COpenGLSampleDoc::Scale2DView
			if (m_nObjectOfView == OOV_DEMO_ZOOM_ISO)
			{
				m_xScaling = min(m_aspect*2./(m_xMax-m_xMin), 2./(m_yMax-m_yMin));
				m_yScaling = min(m_aspect*2./(m_xMax-m_xMin), 2./(m_yMax-m_yMin));
			}
			else
			{
				m_xScaling = m_aspect*2./(m_xMax-m_xMin);
				m_yScaling = 2./(m_yMax-m_yMin);
			}
			m_xTranslation = -(m_xMax+m_xMin)/2.*m_xScaling;
			m_yTranslation = -(m_yMax+m_yMin)/2.*m_yScaling;

			if (m_xScaling > 1e5 || m_yScaling > 1e5)
			{
				CMix2dApp *pApp = (CMix2dApp *)AfxGetApp();
				pApp->SetStatusBarInfo(POLICY_NO_MORE_ZOOM);

				GetFromZoomHistory();
				m_bSecondPickPoint = FALSE;
				return;
			}

			UpdateSliderScalePos();
			UpdateSliderTranslationPos();

			InvalidateRect(NULL, FALSE);
		}
	}
}

BOOL CPlot3dView::MakeActive()
{
	HDC hDC = m_pDC->GetSafeHdc();
	BOOL bRet = FALSE;
	int i = 0;

	// check if m_hRC is already current
	if (wglGetCurrentContext() == m_hRC)
		return TRUE;

	while ((bRet = SetCurrent(hDC, m_hRC)) == FALSE)
	{
		if (++i > 1000)										// Try it only 1000 times
		{
			TRACE("Abort \"MakeActive\" after %d successless trials\n", i);
			break;
		}
		if (GetLastError() == ERROR_INVALID_PIXEL_FORMAT)	// The pixel format is invalid.
		{
			InitializeOpenGL();
		}
	}
#ifdef _DEBUG
	if (i > 0 && bRet)
		TRACE("\"MakeActive\" was successfull after %d trials.\n", i);
#endif // _DEBUG

	return bRet;
}
void CPlot3dView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CString strSection;

	CGlView::OnSettingChange(uFlags, lpszSection);

	// TODO: Add your message handler code here
	strSection.LoadString(IDS_SETTINGCHANGE_LPARAM_FORCE);

	if (m_pDC->GetDeviceCaps(BITSPIXEL) != m_nBitsPerPixel || (lpszSection && strSection.Compare(lpszSection) == 0))
	{
		CFormPropertiesView* pView = (CFormPropertiesView *)GetFormPropertiesView();
		if (pView && pView->m_bVRotate)
		{
			pView->m_bVRotate = FALSE;
			pView->m_CheckVrotationButton.SetCheck(0);
		}
	}
}
