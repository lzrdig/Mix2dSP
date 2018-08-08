// DFVFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Plot3dFrameWnd.h"
#include "Plot3dDocTemplate.h"
#include "PlotPropDlg.h"
#include "Plot3dView.h"
#include "GraphDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDFVFrameWnd

IMPLEMENT_DYNCREATE(CPlot3dFrameWnd, CFrameWnd)

CPlot3dFrameWnd::CPlot3dFrameWnd()
{
	m_bRotationOn = false;
	m_bPanOn = false;
	m_bEditPlotPropsOn = false;
	m_bSolidSurfOn = true;
	m_bWireSurfOn = false;
	m_bDotSurfOn = false;
	m_bFrmOn = true;
	m_pPlotPropDlg = NULL;
}

CPlot3dFrameWnd::~CPlot3dFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CPlot3dFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CPlot3dFrameWnd)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_COMMAND(ID_ROTATE, &CPlot3dFrameWnd::OnRotateView)
	ON_UPDATE_COMMAND_UI(ID_ROTATE, &CPlot3dFrameWnd::OnUpdateRotateView)
	ON_COMMAND(ID_PAN, &CPlot3dFrameWnd::OnPanView)
	ON_UPDATE_COMMAND_UI(ID_PAN, &CPlot3dFrameWnd::OnUpdatePanView)
	ON_COMMAND(ID_EDITPLOT, &CPlot3dFrameWnd::OnEditPlotProps)
	ON_UPDATE_COMMAND_UI(ID_EDITPLOT, &CPlot3dFrameWnd::OnUpdateEditPlotProps)
	ON_COMMAND(ID_SOLID, &CPlot3dFrameWnd::OnSolidSurf)
	ON_UPDATE_COMMAND_UI(ID_SOLID, &CPlot3dFrameWnd::OnUpdateOnSolidSurf)
	ON_COMMAND(ID_WIRE, &CPlot3dFrameWnd::OnWireSurf)
	ON_UPDATE_COMMAND_UI(ID_WIRE, &CPlot3dFrameWnd::OnUpdateOnWireSurf)
	ON_COMMAND(ID_DOTS, &CPlot3dFrameWnd::OnDotSurf)
	ON_UPDATE_COMMAND_UI(ID_DOTS, &CPlot3dFrameWnd::OnUpdateOnDotSurf)
	ON_COMMAND(ID_FRAME, &CPlot3dFrameWnd::OnFrame)
	ON_UPDATE_COMMAND_UI(ID_FRAME, &CPlot3dFrameWnd::OnUpdateOnFrame)
	ON_WM_MOUSEMOVE()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDFVFrameWnd message handlers
BOOL CPlot3dFrameWnd::CreatePlot3dFrame(UINT nIDResource,
				DWORD dwDefaultStyle, CRect rect,
				CWnd* pParentWnd, DWORD dwDefaultStyleEx, 
				CCreateContext* pContext)
{
	// only do this once
//	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	CString strFullString;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(m_strTitle, strFullString, 0);    // first sub-string

//	VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));

	// attempt to create the window
	LPCTSTR lpszClass = GetIconWndClass(dwDefaultStyle, nIDResource);
	LPCTSTR lpszTitle = m_strTitle;
	if (!Create(lpszClass, lpszTitle, dwDefaultStyle, rect,
	  pParentWnd, MAKEINTRESOURCE(nIDResource), dwDefaultStyleEx, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// save the default menu handle
	ASSERT(m_hWnd != NULL);
	m_hMenuDefault = ::GetMenu(m_hWnd);

	// load accelerator resource
	LoadAccelTable(MAKEINTRESOURCE(nIDResource));

	return TRUE;
}


void CPlot3dFrameWnd::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString newName;
	CPlot3dDocTemplate * pTemplate = (CPlot3dDocTemplate *) GetActiveDocument()->GetDocTemplate();

	if (!pTemplate->DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE))
		return; // open cancelled

	pTemplate->OpenDocumentFile(newName);
}

void CPlot3dFrameWnd::OnFileNew() 
{
	// TODO: Add your command handler code here
	CPlot3dDocTemplate * pTemplate = (CPlot3dDocTemplate *) GetActiveDocument()->GetDocTemplate();
	pTemplate->OpenDocumentFile(NULL);
}

int CPlot3dFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	// Create toolbar and attach to the frame
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_PLOT_CTRL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Delete these three lines if you don't want the toolbar to
	// be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	m_wndToolBar.EnableToolTips(true);

	return 0;
}
BOOL CPlot3dFrameWnd::IsLeftPaneClosed() 
{
	// ZU ERLEDIGEN: Code für Befehlsbehandlungsroutine hier einfügen
	return (m_wndSplitter.IsColHidden(0));
}
void CPlot3dFrameWnd::OnRotateView()
{
	// TODO: Add your command handler code here
	m_bRotationOn = true;
	m_bPanOn = false;
	m_bEditPlotPropsOn = false;
}

void CPlot3dFrameWnd::OnUpdateRotateView(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bRotationOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
void CPlot3dFrameWnd::OnPanView()
{
	// TODO: Add your command handler code here
	m_bPanOn = true;
	m_bRotationOn = false;
	m_bEditPlotPropsOn = false;
}

void CPlot3dFrameWnd::OnUpdatePanView(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bPanOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
void CPlot3dFrameWnd::OnEditPlotProps()
{
	// TODO: Add your command handler code here
	m_bEditPlotPropsOn = true;
	m_bRotationOn = false;
	m_bPanOn = false;
	m_bSolidSurfOn = false;
	m_bWireSurfOn = false;

	// Create the edit plot properties dialog and run it modal.
	if(m_pPlotPropDlg == NULL)
	{
		m_pPlotPropDlg = new CPlotPropDlg();
		m_pPlotPropDlg->Create(IDD_PLOTPROP,this);
		//CToolBarCtrl *pBar;
		//pBar = &(m_wndToolBar.GetToolBarCtrl());
		//pBar->SetState(ID_EDITPLOT, TBSTATE_PRESSED);
	}
	m_pPlotPropDlg->ShowWindow(SW_SHOW);
}

void CPlot3dFrameWnd::OnUpdateEditPlotProps(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_pPlotPropDlg != NULL)
	{
		if(!m_pPlotPropDlg->IsWindowVisible())
			m_bEditPlotPropsOn = false;
	}

	if(m_bEditPlotPropsOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
void CPlot3dFrameWnd::OnWireSurf()
{
	// TODO: added by Dorel
	m_bWireSurfOn = true;
	m_bSolidSurfOn = false;
	m_bEditPlotPropsOn = false;
	m_bRotationOn = false;
	m_bPanOn = false;
	m_bDotSurfOn = false;

	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CWnd *pParent = GetParent();
	CPlot3dView *pView = (CPlot3dView *)(((CGraphDlg*)pParent)->m_pPlot3DView);
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	hDC = wglGetCurrentDC();
	hRC = wglGetCurrentContext();
	pView->MakeActive();
	pApp->m_nPolygonMode = 1;
	switch (pApp->m_nPolygonMode)
	{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			if (pApp->m_bAntialias)
				glLineWidth(1.5);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			if (pApp->m_bAntialias)
				glLineWidth(1.5);
			break;
		default:
			{}
	}

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		wglMakeCurrent(hDC, hRC);
	pView->InvalidateRect(NULL, FALSE);
}

void CPlot3dFrameWnd::OnUpdateOnWireSurf(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bWireSurfOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPlot3dFrameWnd::OnSolidSurf()
{
	// TODO: added by Dorel
	m_bSolidSurfOn = true;
	m_bWireSurfOn = false;
	m_bEditPlotPropsOn = false;
	m_bRotationOn = false;
	m_bPanOn = false;
	m_bDotSurfOn = false;

	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CWnd *pParent = GetParent();
	CPlot3dView *pView = (CPlot3dView *)(((CGraphDlg*)pParent)->m_pPlot3DView);
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	hDC = wglGetCurrentDC();
	hRC = wglGetCurrentContext();
	pView->MakeActive();
	pApp->m_nPolygonMode = 2;
	switch (pApp->m_nPolygonMode)
	{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			if (pApp->m_bAntialias)
				glLineWidth(1.5);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			if (pApp->m_bAntialias)
				glLineWidth(1.5);
			break;
		default:
			{}
	}

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		wglMakeCurrent(hDC, hRC);

	pView->InvalidateRect(NULL, FALSE);
}

void CPlot3dFrameWnd::OnUpdateOnSolidSurf(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bSolidSurfOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPlot3dFrameWnd::OnDotSurf()
{
	// TODO: added by Dorel
	m_bDotSurfOn = true;
	m_bWireSurfOn = false;
	m_bSolidSurfOn = false;
	m_bEditPlotPropsOn = false;
	m_bRotationOn = false;
	m_bPanOn = false;

	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CWnd *pParent = GetParent();
	CPlot3dView *pView = (CPlot3dView *)(((CGraphDlg*)pParent)->m_pPlot3DView);
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pApp->m_nPolygonMode = 0;

	hDC = wglGetCurrentDC();
	hRC = wglGetCurrentContext();
	pView->MakeActive();
	switch (pApp->m_nPolygonMode)
	{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			if (pApp->m_bAntialias)
				glLineWidth(1.5);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			if (pApp->m_bAntialias)
				glLineWidth(1.5);
			break;
		default:
			{}
	}

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		wglMakeCurrent(hDC, hRC);
	pView->InvalidateRect(NULL, FALSE);
}

void CPlot3dFrameWnd::OnUpdateOnDotSurf(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bDotSurfOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPlot3dFrameWnd::OnFrame()
{
	// added by Dorel
	m_bFrmOn = !m_bFrmOn;

	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CWnd *pParent = GetParent();
	CPlot3dView *pView = (CPlot3dView *)(((CGraphDlg*)pParent)->m_pPlot3DView);

	if(pView)
	{
		pView->m_bScaleShowFrame = m_bFrmOn;
		pView->Invalidate();
		UpdateData(true);
	}
}

void CPlot3dFrameWnd::OnUpdateOnFrame(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bFrmOn == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPlot3dFrameWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnMouseMove(nFlags, point);
}

BOOL CPlot3dFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// First splitter pane
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CFormPropertiesView), CSize(0, 0), pContext))
	{
		TRACE("Failed to create command view pane\n");
		return FALSE;
	}

	// Second splitter pane
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CPlot3dView), CSize(0, 0), pContext))
	{
		TRACE("Failed to create preview pane\n");
		return FALSE;
	}
	m_wndSplitter.GetPaneEx(0,0)->SendMessage(WM_INITIALUPDATE);	// must be called before CWinApp::OnFileNew -> ... ->
																	// CPlot3dView::OnNewDocument -> CPlot3dView::ClearViews -> 
																	// tries to setup CFormPropertiesView sliders and other controls.
	m_wndSplitter.SetActivePane(0,1);
	m_wndSplitter.HideColumn(0);
	m_wndSplitter.m_bEnableResizing = FALSE;

	return TRUE;

	//return CFrameWnd::OnCreateClient(lpcs, pContext); // commenting this out is important for the view to work
}

void CPlot3dFrameWnd::SizeLeftPane() 
{
	CFormPropertiesView* pLeftView = (CFormPropertiesView*)m_wndSplitter.GetPaneEx(0,0);
	CPlot3dView* pRightView = (CPlot3dView*)m_wndSplitter.GetPaneEx(0,1);

	if (pLeftView)
	{
		CRect rect;
		pRightView->GetClientRect(&rect);
		int nHeight = rect.Height();

		rect = GetTemplateRect(CFormPropertiesView::IDD);
		MapDialogRect(pLeftView->m_hWnd, rect);

		if ((pLeftView->GetStyle() & WS_VSCROLL) || rect.Height() > nHeight)
			rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);

		m_wndSplitter.SetColumnInfo(0,max(rect.Width(),0),0);
		pLeftView->SendMessage(WM_SIZE);	// this does the trick to guarantee correct scroll bars
	}
}

void CPlot3dFrameWnd::RecalcLayout(BOOL bNotify) 
{
	CFrameWnd::RecalcLayout(bNotify);
	if (IsWindowVisible() && !IsLeftPaneClosed())
	{
		SizeLeftPane();
		m_wndSplitter.RecalcLayout();
	}
}

void CPlot3dFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWnd::OnSettingChange(uFlags, lpszSection);

	// TODO: Add your message handler code here
	SendMessage(WM_SIZE);
}
