/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@jenoptik.com>
// created: July 1998
//
// based on a sample of
// Pierre Alliez <pierre.alliez@cnet.francetelecom.fr>
//
/////////////////////////////////////////////////////////////////////////////
// DlgProperties.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "GlView.h"
#include "DlgProperties.h"
#include "ColorDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgProperties 


CDlgProperties::CDlgProperties(CWnd* pParent /*=NULL*/)
	: CDialogTts(CDlgProperties::IDD, pParent)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	m_OptionColorFont = pApp->m_OptionColorFont;
	m_OptionColorFore = pApp->m_OptionColorFore;
	m_OptionColorBack = pApp->m_OptionColorBack;
	m_OptionColorLightAmbient = pApp->m_OptionColorLightAmbient;
	m_OptionColorLightDiffuse = pApp->m_OptionColorLightDiffuse;
	m_OptionColorLightSpecular = pApp->m_OptionColorLightSpecular;

	m_pGlView = NULL;
	m_bWasAlreadyVisible = FALSE;

	SetGlView((CView *)pParent);
	//{{AFX_DATA_INIT(CDlgProperties)
	m_bScaleShowAxisLabels = pApp->m_bScaleShowAxisLabels;
	m_bScaleShowAxis = pApp->m_bScaleShowAxis;
	m_nScaleShowGrid = pApp->m_nScaleShowGrid;
	m_bScaleShowLabels = pApp->m_bScaleShowLabels;
	m_bScaleShowFrame = pApp->m_bScaleShowFrame;
	m_bBlackLinesOrRemoveHidden = pApp->m_bBlackLinesOrRemoveHidden;
	m_bLighting = pApp->m_bLighting;
	m_bSmooth = pApp->m_bSmooth;
	m_bAntialias = pApp->m_bAntialias;
	m_bMonochromeMode = pApp->m_bMonochromeMode;
	m_nPolygonMode = pApp->m_nPolygonMode;
	m_bVRotate = FALSE;
	m_bLinkScale = FALSE;
	m_bLinkTranslation = FALSE;
	//}}AFX_DATA_INIT
	m_bMonochromeModeTmp = FALSE;
	m_bMonochromeModeIsSwapped = FALSE;
}


void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialogTts::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperties)
	DDX_Control(pDX, IDC_STATIC_FORE_TEXT, m_ForeTextStatic);
	DDX_Control(pDX, IDC_STATIC_LIGHT_TEXT, m_LightTextStatic);
	DDX_Control(pDX, IDC_SLIDER_SCALE_Z, m_ScaleZSlider);
	DDX_Control(pDX, IDC_SLIDER_SCALE_Y, m_ScaleYSlider);
	DDX_Control(pDX, IDC_SLIDER_SCALE_X, m_ScaleXSlider);
	DDX_Control(pDX, IDC_SLIDER_TRANSLATION_Z, m_TranslationZSlider);
	DDX_Control(pDX, IDC_SLIDER_TRANSLATION_Y, m_TranslationYSlider);
	DDX_Control(pDX, IDC_SLIDER_TRANSLATION_X, m_TranslationXSlider);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_AMBIENT, m_ColorLightAmbientStatic);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_DIFFUSE, m_ColorLightDiffuseStatic);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_SPECULAR, m_ColorLightSpecularStatic);
	DDX_Control(pDX, IDC_FRAME_COLOR_BACK, m_ColorBackStatic);
	DDX_Control(pDX, IDC_FRAME_COLOR_FORE, m_ColorForeStatic);
	DDX_Control(pDX, IDC_FRAME_COLOR_FONT, m_ColorFontStatic);
	DDX_Control(pDX, IDC_CHECK_LINK_SCALE, m_CheckLinkScaleButton);
	DDX_Control(pDX, IDC_CHECK_LINK_TRANSLATION, m_CheckLinkTranslationButton);
	DDX_Control(pDX, IDC_CHECK_VROTATION, m_CheckVrotationButton);
	DDX_Control(pDX, IDC_CHECK_SMOOTH, m_CheckSmoothButton);
	DDX_Control(pDX, IDC_CHECK_LIGHTING, m_CheckLightingButton);
	DDX_Control(pDX, IDC_CHECK_ANTIALIAS, m_CheckAntialiasButton);
	DDX_Control(pDX, IDC_CHECK_REMOVE_HIDDEN, m_CheckRemoveHiddenButton);
	DDX_Control(pDX, IDC_RADIO_RAINBOW, m_RadioRainbowButton);
	DDX_Control(pDX, IDC_RADIO_MONOCHROME, m_RadioMonochromeButton);
	DDX_Control(pDX, IDC_RADIO_MODEL_0, m_RadioModel0Button);
	DDX_Control(pDX, IDC_RADIO_MODEL_1, m_RadioModel1Button);
	DDX_Control(pDX, IDC_RADIO_MODEL_2, m_RadioModel2Button);
	DDX_Control(pDX, IDC_CHECK_SCALE_SHOW_FRAME, m_CheckScaleShowFrameButton);
	DDX_Control(pDX, IDC_CHECK_SCALE_SHOW_LABELS, m_CheckScaleShowLabelsButton);
	DDX_Control(pDX, IDC_CHECK_SCALE_SHOW_GRID, m_CheckScaleShowGridButton);
	DDX_Control(pDX, IDC_CHECK_SCALE_SHOW_AXIS, m_CheckScaleShowAxisButton);
	DDX_Control(pDX, IDC_CHECK_SCALE_SHOW_AXIS_LABELS, m_CheckScaleShowAxisLabelsButton);
	DDX_Control(pDX, IDC_STATIC_SCALE_Z, m_ScaleZStatic);
	DDX_Control(pDX, IDC_STATIC_TRANSLATION_Z, m_TranslationZStatic);
	DDX_Control(pDX, IDC_STATIC_VROTATION_GBOX, m_VrotationGboxButton);
	DDX_Control(pDX, IDC_STATIC_SCALE_GBOX, m_ScaleGboxButton);
	DDX_Check(pDX, IDC_CHECK_SCALE_SHOW_AXIS_LABELS, m_bScaleShowAxisLabels);
	DDX_Check(pDX, IDC_CHECK_SCALE_SHOW_AXIS, m_bScaleShowAxis);
	DDX_Check(pDX, IDC_CHECK_SCALE_SHOW_LABELS, m_bScaleShowLabels);
	DDX_Check(pDX, IDC_CHECK_SCALE_SHOW_FRAME, m_bScaleShowFrame);
	DDX_Check(pDX, IDC_CHECK_REMOVE_HIDDEN, m_bBlackLinesOrRemoveHidden);
	DDX_Check(pDX, IDC_CHECK_LIGHTING, m_bLighting);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_bSmooth);
	DDX_Check(pDX, IDC_CHECK_ANTIALIAS, m_bAntialias);
	DDX_Radio(pDX, IDC_RADIO_RAINBOW, m_bMonochromeMode);
	DDX_Radio(pDX, IDC_RADIO_MODEL_0, m_nPolygonMode);
	DDX_Check(pDX, IDC_CHECK_VROTATION, m_bVRotate);
	DDX_Check(pDX, IDC_CHECK_LINK_SCALE, m_bLinkScale);
	DDX_Check(pDX, IDC_CHECK_LINK_TRANSLATION, m_bLinkTranslation);
	//}}AFX_DATA_MAP
	//DDX_Check(pDX, IDC_CHECK_SCALE_SHOW_GRID, m_nScaleShowGrid);
	CheckButtonSpecial(m_CheckScaleShowGridButton, m_nScaleShowGrid, IDS_GRID, pDX->m_bSaveAndValidate);
}


BEGIN_MESSAGE_MAP(CDlgProperties, CDialogTts)
	//{{AFX_MSG_MAP(CDlgProperties)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_SCALE_SHOW_AXIS_LABELS, OnCheckScaleShowAxisLabels)
	ON_BN_CLICKED(IDC_CHECK_SCALE_SHOW_AXIS, OnCheckScaleShowAxis)
	ON_BN_CLICKED(IDC_CHECK_SCALE_SHOW_GRID, OnCheckScaleShowGrid)
	ON_BN_CLICKED(IDC_CHECK_SCALE_SHOW_LABELS, OnCheckScaleShowLabels)
	ON_BN_CLICKED(IDC_CHECK_SCALE_SHOW_FRAME, OnCheckScaleShowFrame)
	ON_BN_CLICKED(IDC_RADIO_MODEL_0, OnRadioModel0)
	ON_BN_CLICKED(IDC_RADIO_MODEL_1, OnRadioModel1)
	ON_BN_CLICKED(IDC_RADIO_MODEL_2, OnRadioModel2)
	ON_BN_CLICKED(IDC_CHECK_REMOVE_HIDDEN, OnCheckRemoveHidden)
	ON_BN_CLICKED(IDC_CHECK_LIGHTING, OnCheckLighting)
	ON_BN_CLICKED(IDC_CHECK_SMOOTH, OnCheckSmooth)
	ON_BN_CLICKED(IDC_CHECK_ANTIALIAS, OnCheckAntialias)
	ON_BN_CLICKED(IDC_RADIO_MONOCHROME, OnRadioMonochrome)
	ON_BN_CLICKED(IDC_RADIO_RAINBOW, OnRadioRainbow)
	ON_BN_CLICKED(IDC_CHECK_VROTATION, OnCheckVrotation)
	ON_BN_CLICKED(IDC_CHECK_LINK_SCALE, OnCheckLinkScale)
	ON_BN_CLICKED(IDC_CHECK_LINK_TRANSLATION, OnCheckLinkTranslation)
	ON_BN_CLICKED(IDC_FRAME_COLOR_FONT, OnFrameColorFont)
	ON_BN_CLICKED(IDC_FRAME_COLOR_FORE, OnFrameColorFore)
	ON_BN_CLICKED(IDC_FRAME_COLOR_BACK, OnFrameColorBack)
	ON_BN_CLICKED(IDC_FRAME_COLOR_LIGHT_AMBIENT, OnFrameColorLightAmbient)
	ON_BN_CLICKED(IDC_FRAME_COLOR_LIGHT_DIFFUSE, OnFrameColorLightDiffuse)
	ON_BN_CLICKED(IDC_FRAME_COLOR_LIGHT_SPECULAR, OnFrameColorLightSpecular)
	ON_BN_CLICKED(IDC_EDIT_FONT, OnEditFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgProperties 

/////////////////////////////////////////////////////////////////////////////
// OnInitDialog
/////////////////////////////////////////////////////////////////////////////
BOOL CDlgProperties::OnInitDialog() 
{
	BOOL bRet = CDialogTts::OnInitDialog();

#if _MSC_VER < 1200
	if (m_nFlags & WF_TOOLTIPS)
#else
	if (m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS))
#endif // _MSC_VER < 1200
	{
		// If tooltips (or tracking tooltips) are enabled
		//	we have to change the Z-order to display
		//	all tips correctly.
		m_ScaleGboxButton.SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		m_VrotationGboxButton.SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}

	// Slider
	TRACE("Sliders : updating...\n");
	m_ScaleXSlider.SetRange(0,100,TRUE);
	m_ScaleYSlider.SetRange(0,100,TRUE);
	m_ScaleZSlider.SetRange(0,100,TRUE);

	m_ScaleXSlider.SetPos(50);
	m_ScaleYSlider.SetPos(50);
	m_ScaleZSlider.SetPos(50);

	m_TranslationXSlider.SetRange(-50,50,TRUE);
	m_TranslationYSlider.SetRange(-50,50,TRUE);
	m_TranslationZSlider.SetRange(-50,50,TRUE);

	m_TranslationXSlider.SetPos(0);
	m_TranslationYSlider.SetPos(0);
	m_TranslationZSlider.SetPos(0);

	CString strText;
	switch (m_nPolygonMode)
	{
		case 0:
		case 1:
			strText.LoadString(IDS_FOREGROUND_ONLY);
			m_CheckRemoveHiddenButton.SetWindowText(strText);
			break;
		case 2:
			strText.LoadString(IDS_BLACK_LINES);
			m_CheckRemoveHiddenButton.SetWindowText(strText);
			break;
		default:
			{}
	}
	m_ColorFontStatic.SetBkColor(m_OptionColorFont);
	m_ColorForeStatic.SetBkColor(m_OptionColorFore);
	m_ColorBackStatic.SetBkColor(m_OptionColorBack);
	m_ColorLightAmbientStatic.SetBkColor(m_OptionColorLightAmbient);
	m_ColorLightDiffuseStatic.SetBkColor(m_OptionColorLightDiffuse);
	m_ColorLightSpecularStatic.SetBkColor(m_OptionColorLightSpecular);
	EnableForeCtrls(m_bMonochromeMode);
	EnableLightCtrls(m_bLighting);
	Update();

	return bRet;
}

void CDlgProperties::Update() 
{
	CGlView *pView = (CGlView *)GetGlView();

	if (!pView)		// GetGlView failed
		return;

	UpdateSliderScalePos();
	UpdateSliderTranslationPos();

	if (m_bVRotate)
	{
		OnCheckVrotation();
		m_CheckVrotationButton.SetCheck(0);
	}

	if (pView->Is3D())
	{
		m_VrotationGboxButton.EnableWindow(TRUE);
		m_CheckVrotationButton.EnableWindow(TRUE);
	}
	else
	{
		m_VrotationGboxButton.EnableWindow(FALSE);
		m_CheckVrotationButton.EnableWindow(FALSE);
	}

	if (pView->AllowRemoveHidden())
	{
		m_CheckRemoveHiddenButton.EnableWindow(TRUE);
	}
	else
	{
		m_CheckRemoveHiddenButton.EnableWindow(FALSE);
	}

	if (pView->AllowModel())
	{
		m_RadioModel0Button.EnableWindow(TRUE);
		m_RadioModel1Button.EnableWindow(TRUE);
		m_RadioModel2Button.EnableWindow(TRUE);
	}
	else
	{
		m_RadioModel0Button.EnableWindow(FALSE);
		m_RadioModel1Button.EnableWindow(FALSE);
		m_RadioModel2Button.EnableWindow(FALSE);
	}

	if (pView->AllowScale())
	{
		m_ScaleGboxButton.EnableWindow(TRUE);
		m_CheckScaleShowFrameButton.EnableWindow(TRUE);
		m_CheckScaleShowLabelsButton.EnableWindow(m_bScaleShowFrame);
		m_CheckScaleShowGridButton.EnableWindow(m_bScaleShowFrame);
		m_CheckScaleShowAxisButton.EnableWindow(TRUE);
		m_CheckScaleShowAxisLabelsButton.EnableWindow(m_bScaleShowAxis && pView->Is3D());
	}
	else
	{
		m_ScaleGboxButton.EnableWindow(FALSE);
		m_CheckScaleShowFrameButton.EnableWindow(FALSE);
		m_CheckScaleShowLabelsButton.EnableWindow(FALSE);
		m_CheckScaleShowGridButton.EnableWindow(FALSE);
		m_CheckScaleShowAxisButton.EnableWindow(FALSE);
		m_CheckScaleShowAxisLabelsButton.EnableWindow(FALSE);
	}

	if (pView->AllowMonochrome())
	{
		if (m_bMonochromeModeIsSwapped)
		{
			SwapMonochromeModeVariables();
		}
	}
	else
	{
		if (!m_bMonochromeModeIsSwapped)
		{
			SwapMonochromeModeVariables();
		}
	}
}

void CDlgProperties::SwapMonochromeModeVariables()
{
	m_bMonochromeModeIsSwapped = !m_bMonochromeModeIsSwapped;

	BOOL bTmp = m_bMonochromeModeTmp;
	m_bMonochromeModeTmp = m_bMonochromeMode;
	m_bMonochromeMode = bTmp;

	m_RadioRainbowButton.EnableWindow(!m_bMonochromeModeIsSwapped);
	m_RadioMonochromeButton.EnableWindow(!m_bMonochromeModeIsSwapped);

	m_RadioRainbowButton.SetCheck(!m_bMonochromeMode);
	m_RadioMonochromeButton.SetCheck(!!m_bMonochromeMode);

	ASSERT(!m_bMonochromeModeIsSwapped || !m_bMonochromeMode);
	RadioChrome(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnPaint
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnPaint() 
{
	// Device context for painting
	CPaintDC dc(this);

	// Options are stored in Application
	CRect rect;

	// Focus rect font
	if (m_ColorFontStatic.HasFocusChanged())
	{
		m_ColorFontStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(2, 2, 2, 2);
		dc.DrawFocusRect(&rect);
		m_ColorFontStatic.ResetFocusChangedFlag();
	}

	// Focus rect fore
	if (m_ColorForeStatic.HasFocusChanged())
	{
		m_ColorForeStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(2, 2, 2, 2);
		dc.DrawFocusRect(&rect);
		m_ColorForeStatic.ResetFocusChangedFlag();
	}

	// Focus rect back
	if (m_ColorBackStatic.HasFocusChanged())
	{
		m_ColorBackStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(2, 2, 2, 2);
		dc.DrawFocusRect(&rect);
		m_ColorBackStatic.ResetFocusChangedFlag();
	}

	// Focus rect light ambient
	if (m_ColorLightAmbientStatic.HasFocusChanged())
	{
		m_ColorLightAmbientStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(2, 2, 2, 2);
		dc.DrawFocusRect(&rect);
		m_ColorLightAmbientStatic.ResetFocusChangedFlag();
	}

	// Focus rect light diffuse
	if (m_ColorLightDiffuseStatic.HasFocusChanged())
	{
		m_ColorLightDiffuseStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(2, 2, 2, 2);
		dc.DrawFocusRect(&rect);
		m_ColorLightDiffuseStatic.ResetFocusChangedFlag();
	}

	// Focus rect light specular
	if (m_ColorLightSpecularStatic.HasFocusChanged())
	{
		m_ColorLightSpecularStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(2, 2, 2, 2);
		dc.DrawFocusRect(&rect);
		m_ColorLightSpecularStatic.ResetFocusChangedFlag();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Mouse
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::ChangeFontColor() 
{
	CRect rect;
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView* pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	// Option font color
	m_ColorFontStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_OptionColorFont = pApp->m_OptionColorFont;

	// Refresh font
	pView->CheckColors(m_OptionColorBack, m_OptionColorFont);
	pView->m_ColorFontRed   = (float)GetRValue(m_OptionColorFont) / 255.0f;
	pView->m_ColorFontGreen = (float)GetGValue(m_OptionColorFont) / 255.0f;
	pView->m_ColorFontBlue  = (float)GetBValue(m_OptionColorFont) / 255.0f;

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	// Refresh views
	m_ColorFontStatic.SetBkColor(m_OptionColorFont);
	InvalidateRect(&rect, FALSE);
	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::ChangeForeColor() 
{
	CRect rect;
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView* pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	// Option fore color
	m_ColorForeStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_OptionColorFore = pApp->m_OptionColorFore;

	// Refresh foreground
	pView->CheckColors(m_OptionColorBack, m_OptionColorFore);
	pView->m_ColorForeRed   = (float)GetRValue(m_OptionColorFore) / 255.0f;
	pView->m_ColorForeGreen = (float)GetGValue(m_OptionColorFore) / 255.0f;
	pView->m_ColorForeBlue  = (float)GetBValue(m_OptionColorFore) / 255.0f;

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	// Refresh views
	m_ColorForeStatic.SetBkColor(m_OptionColorFore);
	InvalidateRect(&rect, FALSE);
	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::ChangeBackColor() 
{
	CRect rectFont, rectFore, rectBack;
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView* pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	// Option font color
	m_ColorFontStatic.GetWindowRect(&rectFont);
	ScreenToClient(&rectFont);

	// Option fore color
	m_ColorForeStatic.GetWindowRect(&rectFore);
	ScreenToClient(&rectFore);

	// Option back color
	m_ColorBackStatic.GetWindowRect(&rectBack);
	ScreenToClient(&rectBack);

	m_OptionColorBack = pApp->m_OptionColorBack;

	// Refresh background
	if (!pView->CheckColors(m_OptionColorBack, m_OptionColorFore))
	{
		pView->m_ColorForeRed   = (float)GetRValue(m_OptionColorFore) / 255.0f;
		pView->m_ColorForeGreen = (float)GetGValue(m_OptionColorFore) / 255.0f;
		pView->m_ColorForeBlue  = (float)GetBValue(m_OptionColorFore) / 255.0f;
	}
	if (!pView->CheckColors(m_OptionColorBack, m_OptionColorFont))
	{
		pView->m_ColorFontRed   = (float)GetRValue(m_OptionColorFont) / 255.0f;
		pView->m_ColorFontGreen = (float)GetGValue(m_OptionColorFont) / 255.0f;
		pView->m_ColorFontBlue  = (float)GetBValue(m_OptionColorFont) / 255.0f;
	}
	pView->m_ColorBackRed   = (float)GetRValue(m_OptionColorBack) / 255.0f;
	pView->m_ColorBackGreen = (float)GetGValue(m_OptionColorBack) / 255.0f;
	pView->m_ColorBackBlue  = (float)GetBValue(m_OptionColorBack) / 255.0f;
	pView->SetBackColor();

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	// Refresh views
	m_ColorFontStatic.SetBkColor(m_OptionColorFont);
	InvalidateRect(&rectFont, FALSE);
	m_ColorForeStatic.SetBkColor(m_OptionColorFore);
	InvalidateRect(&rectFore, FALSE);
	m_ColorBackStatic.SetBkColor(m_OptionColorBack);
	InvalidateRect(&rectBack, FALSE);
	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::ChangeAmbientColor() 
{
	CRect rect;
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView* pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	// Option ambient light color
	m_ColorLightAmbientStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_OptionColorLightAmbient = pApp->m_OptionColorLightAmbient;

	// Refresh Light0
	pView->m_ColorLightAmbientRed   = (float)GetRValue(m_OptionColorLightAmbient) / 255.0f;
	pView->m_ColorLightAmbientGreen = (float)GetGValue(m_OptionColorLightAmbient) / 255.0f;
	pView->m_ColorLightAmbientBlue  = (float)GetBValue(m_OptionColorLightAmbient) / 255.0f;
	pView->SetAmbientColor();

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	// Refresh views
	m_ColorLightAmbientStatic.SetBkColor(m_OptionColorLightAmbient);
	InvalidateRect(&rect, FALSE);
	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::ChangeDiffuseColor() 
{
	CRect rect;
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView* pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	// Option diffuse light color
	m_ColorLightDiffuseStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_OptionColorLightDiffuse = pApp->m_OptionColorLightDiffuse;

	// Refresh Light1
	pView->m_ColorLightDiffuseRed   = (float)GetRValue(m_OptionColorLightDiffuse) / 255.0f;
	pView->m_ColorLightDiffuseGreen = (float)GetGValue(m_OptionColorLightDiffuse) / 255.0f;
	pView->m_ColorLightDiffuseBlue  = (float)GetBValue(m_OptionColorLightDiffuse) / 255.0f;
	pView->SetDiffuseColor();

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	// Refresh views
	m_ColorLightDiffuseStatic.SetBkColor(m_OptionColorLightDiffuse);
	InvalidateRect(&rect, FALSE);
	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::ChangeSpecularColor() 
{
	CRect rect;
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView* pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	// Option specular light color
	m_ColorLightSpecularStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_OptionColorLightSpecular = pApp->m_OptionColorLightSpecular;

	// Refresh Light2
	pView->m_ColorLightSpecularRed   = (float)GetRValue(m_OptionColorLightSpecular) / 255.0f;
	pView->m_ColorLightSpecularGreen = (float)GetGValue(m_OptionColorLightSpecular) / 255.0f;
	pView->m_ColorLightSpecularBlue  = (float)GetBValue(m_OptionColorLightSpecular) / 255.0f;
	pView->SetSpecularColor();

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	// Refresh views
	m_ColorLightSpecularStatic.SetBkColor(m_OptionColorLightSpecular);
	InvalidateRect(&rect, FALSE);
	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnHScroll
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode)
	{
	case TB_BOTTOM:
	case TB_ENDTRACK:
	case TB_LINEDOWN:
	case TB_LINEUP:
	case TB_PAGEDOWN:
	case TB_PAGEUP:
	case TB_THUMBPOSITION:
	case TB_THUMBTRACK:
	case TB_TOP:
		UpdateScale(pScrollBar);
		UpdateTranslation(pScrollBar);
		break;
	default:
		{}
	}

	// Update scaling and translation
	GetGlView()->InvalidateRect(NULL, FALSE);

	CDialogTts::OnHScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////////////
// GetGlView
/////////////////////////////////////////////////////////////////////////////
CView* CDlgProperties::GetGlView() 
{
	return m_pGlView;
}

/////////////////////////////////////////////////////////////////////////////
// SetGlView
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::SetGlView(CView* pView) 
{
	m_pGlView = pView;
}

/////////////////////////////////////////////////////////////////////////////
// Rainbow/Monochrome
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnRadioRainbow() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bMonochromeMode = FALSE;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->RadioChrome();
							if (pDlg != this && ((CGlView *)pView)->AllowMonochrome())
							{
								pDlg->m_RadioRainbowButton.SetCheck(1);
								pDlg->m_RadioMonochromeButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		RadioChrome();
}

void CDlgProperties::OnRadioMonochrome() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bMonochromeMode = TRUE;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->RadioChrome();
							if (pDlg != this && ((CGlView *)pView)->AllowMonochrome())
							{
								pDlg->m_RadioRainbowButton.SetCheck(0);
								pDlg->m_RadioMonochromeButton.SetCheck(1);
							}
						}
					}
				}
			}
		}
	}
	else
		RadioChrome();
}

void CDlgProperties::RadioChrome(BOOL bUseWinAppValue) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	if (bUseWinAppValue && pView->AllowMonochrome())
		m_bMonochromeMode = pApp->m_bMonochromeMode;

	pView->m_bMonochromeMode = m_bMonochromeMode;

	pView->InvalidateRect(NULL, FALSE);
	EnableForeCtrls(m_bMonochromeMode);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckAntialias
// Toggle antialiased lines
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckAntialias() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bAntialias = !m_bAntialias;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckAntialias();
							if (pDlg != this)
							{
								if (pApp->m_bAntialias)
									pDlg->m_CheckAntialiasButton.SetCheck(1);
								else
									pDlg->m_CheckAntialiasButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckAntialias();
}

void CDlgProperties::CheckAntialias() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	m_bAntialias = pApp->m_bAntialias;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current
	if (m_bAntialias)
	{
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		if (m_nPolygonMode == 2)
			glLineWidth(1.0);
		else
			glLineWidth(1.5);
		glPointSize(3.0);
	}
	else
	{
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glLineWidth(1.0);
		glPointSize(2.0);
	}

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckSmooth
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckSmooth() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bSmooth = !m_bSmooth;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckSmooth();
							if (pDlg != this)
							{
								if (pApp->m_bSmooth)
									pDlg->m_CheckSmoothButton.SetCheck(1);
								else
									pDlg->m_CheckSmoothButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckSmooth();
}

void CDlgProperties::CheckSmooth() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	m_bSmooth = pApp->m_bSmooth;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current
	if (m_bSmooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckLighting
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckLighting() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bLighting = !m_bLighting;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckLighting();
							if (pDlg != this)
							{
								if (pApp->m_bLighting)
									pDlg->m_CheckLightingButton.SetCheck(1);
								else
									pDlg->m_CheckLightingButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckLighting();
}

void CDlgProperties::CheckLighting() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	m_bLighting = pApp->m_bLighting;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current
	if (m_bLighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	pView->InvalidateRect(NULL, FALSE);
	EnableLightCtrls(m_bLighting);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckRemoveHidden
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckRemoveHidden() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bBlackLinesOrRemoveHidden = !pView->m_bBlackLinesOrRemoveHidden;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckRemoveHidden();
							if (pDlg != this)
							{
								if (pApp->m_bBlackLinesOrRemoveHidden)
									pDlg->m_CheckRemoveHiddenButton.SetCheck(1);
								else
									pDlg->m_CheckRemoveHiddenButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckRemoveHidden();
}

void CDlgProperties::CheckRemoveHidden() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	m_bBlackLinesOrRemoveHidden = pView->m_bBlackLinesOrRemoveHidden = pApp->m_bBlackLinesOrRemoveHidden;

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Model
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnRadioModel0() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_nPolygonMode = 0;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->RadioModel();
							if (pDlg != this)
							{
								pDlg->m_RadioModel0Button.SetCheck(1);
								pDlg->m_RadioModel1Button.SetCheck(0);
								pDlg->m_RadioModel2Button.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		RadioModel();
}

void CDlgProperties::OnRadioModel1() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_nPolygonMode = 1;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->RadioModel();
							if (pDlg != this)
							{
								pDlg->m_RadioModel0Button.SetCheck(0);
								pDlg->m_RadioModel1Button.SetCheck(1);
								pDlg->m_RadioModel2Button.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		RadioModel();
}

void CDlgProperties::OnRadioModel2() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_nPolygonMode = 2;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->RadioModel();
							if (pDlg != this)
							{
								pDlg->m_RadioModel0Button.SetCheck(0);
								pDlg->m_RadioModel1Button.SetCheck(0);
								pDlg->m_RadioModel2Button.SetCheck(1);
							}
						}
					}
				}
			}
		}
	}
	else
		RadioModel();
}

void CDlgProperties::RadioModel() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	m_nPolygonMode = pApp->m_nPolygonMode;

	pView->GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	pView->MakeActive();				//Make view's rendering context current

	CString strText;
	switch (m_nPolygonMode)
	{
		case 0:
			strText.LoadString(IDS_FOREGROUND_ONLY);
			m_CheckRemoveHiddenButton.SetWindowText(strText);
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
		case 1:
			strText.LoadString(IDS_FOREGROUND_ONLY);
			m_CheckRemoveHiddenButton.SetWindowText(strText);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			if (m_bAntialias)
				glLineWidth(1.5);
			break;
		case 2:
			strText.LoadString(IDS_BLACK_LINES);
			m_CheckRemoveHiddenButton.SetWindowText(strText);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			if (m_bAntialias)
				glLineWidth(1.5);
			break;
		default:
			{}
	}

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pView->SetCurrent(hDC, hRC);

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckScaleShowFrame
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckScaleShowFrame() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bScaleShowFrame = !pView->m_bScaleShowFrame;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckScaleShowFrame();
							if (pDlg != this)
							{
								if (pApp->m_bScaleShowFrame)
									pDlg->m_CheckScaleShowFrameButton.SetCheck(1);
								else
									pDlg->m_CheckScaleShowFrameButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckScaleShowFrame();
}

void CDlgProperties::CheckScaleShowFrame() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	m_bScaleShowFrame = pView->m_bScaleShowFrame = pApp->m_bScaleShowFrame;

	m_CheckScaleShowLabelsButton.EnableWindow(m_bScaleShowFrame && pView->AllowScale());
	m_CheckScaleShowGridButton.EnableWindow(m_bScaleShowFrame && pView->AllowScale());

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckScaleShowLabels
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckScaleShowLabels() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bScaleShowLabels = !pView->m_bScaleShowLabels;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckScaleShowLabels();
							if (pDlg != this)
							{
								if (pApp->m_bScaleShowLabels)
									pDlg->m_CheckScaleShowLabelsButton.SetCheck(1);
								else
									pDlg->m_CheckScaleShowLabelsButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckScaleShowLabels();
}

void CDlgProperties::CheckScaleShowLabels() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	m_bScaleShowLabels = pView->m_bScaleShowLabels = pApp->m_bScaleShowLabels;

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckScaleShowGrid
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckScaleShowGrid() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_nScaleShowGrid = pView->m_nScaleShowGrid;
	if (++pApp->m_nScaleShowGrid > 2)
		pApp->m_nScaleShowGrid = 0;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckScaleShowGrid();
							/*if (pDlg != this)
							{
								if (pApp->m_nScaleShowGrid == 2)
									pDlg->m_CheckScaleShowGridButton.SetCheck(2);
								else if (pApp->m_nScaleShowGrid == 1)
									pDlg->m_CheckScaleShowGridButton.SetCheck(1);
								else
									pDlg->m_CheckScaleShowGridButton.SetCheck(0);
							}*/
						}
					}
				}
			}
		}
	}
	else
		CheckScaleShowGrid();
}

void CDlgProperties::CheckScaleShowGrid() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	m_nScaleShowGrid = pView->m_nScaleShowGrid = pApp->m_nScaleShowGrid;
	CheckButtonSpecial(m_CheckScaleShowGridButton, m_nScaleShowGrid, IDS_GRID, FALSE);

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckScaleShowAxis
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckScaleShowAxis() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bScaleShowAxis = !pView->m_bScaleShowAxis;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckScaleShowAxis();
							if (pDlg != this)
							{
								if (pApp->m_bScaleShowAxis)
									pDlg->m_CheckScaleShowAxisButton.SetCheck(1);
								else
									pDlg->m_CheckScaleShowAxisButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckScaleShowAxis();
}

void CDlgProperties::CheckScaleShowAxis() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	m_bScaleShowAxis = pView->m_bScaleShowAxis = pApp->m_bScaleShowAxis;

	m_CheckScaleShowAxisLabelsButton.EnableWindow(m_bScaleShowAxis && pView->Is3D() && pView->AllowScale());

	pView->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// OnCheckScaleShowAxisLabels
/////////////////////////////////////////////////////////////////////////////
void CDlgProperties::OnCheckScaleShowAxisLabels() 
{
	// Make context current is good to be called here
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();

	pApp->m_bScaleShowAxisLabels = !pView->m_bScaleShowAxisLabels;
	if (pApp->m_bPropPolicy)
	{
		POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
		while (posDocTemplate != NULL)
		{
			CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
			POSITION posDoc = pDocTemplate->GetFirstDocPosition();
			while (posDoc != NULL)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
				POSITION posView = pDoc->GetFirstViewPosition();
				while (posView != NULL)
				{
					CView* pView = pDoc->GetNextView(posView);
					if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
					{
						CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
						if (pDlg)
						{
							pDlg->CheckScaleShowAxisLabels();
							if (pDlg != this)
							{
								if (pApp->m_bScaleShowAxisLabels)
									pDlg->m_CheckScaleShowAxisLabelsButton.SetCheck(1);
								else
									pDlg->m_CheckScaleShowAxisLabelsButton.SetCheck(0);
							}
						}
					}
				}
			}
		}
	}
	else
		CheckScaleShowAxisLabels();
}

void CDlgProperties::CheckScaleShowAxisLabels() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	CGlView *pView = (CGlView *)GetGlView();

	m_bScaleShowAxisLabels = pView->m_bScaleShowAxisLabels = pApp->m_bScaleShowAxisLabels;

	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::OnCheckVrotation() 
{
	m_bVRotate = !m_bVRotate;
	CGlView *pView = (CGlView *)GetGlView();
	if (m_bVRotate)
		pView->SetTimer(1,10,NULL);
	else
		pView->KillTimer(1);
}

void CDlgProperties::OnCheckLinkScale() 
{
	CGlView *pView = (CGlView *)GetGlView();

	m_bLinkScale = !m_bLinkScale;
	if (m_bLinkScale)
	{
		if (pView->Is3D())
		{
			m_ScaleYSlider.SetPos(m_ScaleXSlider.GetPos());
			pView->m_zScaling = pView->GetExtraYScaling()/pView->GetExtraXScaling()*pView->m_xScaling;
			m_ScaleZSlider.SetPos(m_ScaleXSlider.GetPos());
			pView->m_yScaling = pView->GetExtraZScaling()/pView->GetExtraXScaling()*pView->m_xScaling;
		}
		else
		{
			m_ScaleYSlider.SetPos(m_ScaleXSlider.GetPos());
			pView->m_yScaling = pView->GetExtraYScaling()/pView->GetExtraXScaling()*pView->m_xScaling;
		}
	}

	if (pView->Is3D())
	{
		m_ScaleZSlider.EnableWindow(TRUE);
		m_ScaleZStatic.EnableWindow(TRUE);
	}
	else
	{
		m_ScaleZSlider.EnableWindow(FALSE);
		m_ScaleZStatic.EnableWindow(FALSE);
	}

	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::OnCheckLinkTranslation() 
{
	CGlView *pView = (CGlView *)GetGlView();

	m_bLinkTranslation = !m_bLinkTranslation;
	if (m_bLinkTranslation)
	{
		if (pView->Is3D())
		{
			m_TranslationYSlider.SetPos(m_TranslationXSlider.GetPos());
			pView->m_zTranslation = -(pView->GetExtraYTranslation()-pView->GetExtraXTranslation()+pView->m_xTranslation);
			m_TranslationZSlider.SetPos(m_TranslationXSlider.GetPos());
			pView->m_yTranslation = pView->GetExtraZTranslation()-pView->GetExtraXTranslation()+pView->m_xTranslation;
		}
		else
		{
			m_TranslationYSlider.SetPos(m_TranslationXSlider.GetPos());
			pView->m_yTranslation = pView->GetExtraYTranslation()-pView->GetExtraXTranslation()+pView->m_xTranslation;
		}
	}

	if (pView->Is3D())
	{
		m_TranslationZSlider.EnableWindow(TRUE);
		m_TranslationZStatic.EnableWindow(TRUE);
	}
	else
	{
		m_TranslationZSlider.EnableWindow(FALSE);
		m_TranslationZStatic.EnableWindow(FALSE);
	}

	pView->InvalidateRect(NULL, FALSE);
}

void CDlgProperties::UpdateScale(CWnd* pSender) 
{
	CGlView *pView = (CGlView *)GetGlView();

	int nXMin, nXMax, nYMin, nYMax, nZMin, nZMax;
	m_ScaleXSlider.GetRange(nXMin, nXMax);
	m_ScaleYSlider.GetRange(nYMin, nYMax);
	m_ScaleZSlider.GetRange(nZMin, nZMax);

	pView->m_xScaling = pView->GetExtraXScaling()*m_ScaleXSlider.GetPos()/((nXMax-nXMin)/2.);
	if (pView->Is3D())
	{
		pView->m_zScaling = pView->GetExtraYScaling()*m_ScaleYSlider.GetPos()/((nYMax-nYMin)/2.);
		pView->m_yScaling = pView->GetExtraZScaling()*m_ScaleZSlider.GetPos()/((nZMax-nZMin)/2.);
	}
	else
	{
		pView->m_yScaling = pView->GetExtraYScaling()*m_ScaleYSlider.GetPos()/((nYMax-nYMin)/2.);
	}

	if (m_bLinkScale)
	{
		if (pSender == &m_ScaleXSlider)
		{
			if (pView->Is3D())
			{
				m_ScaleYSlider.SetPos(m_ScaleXSlider.GetPos());
				pView->m_zScaling = pView->GetExtraYScaling()/pView->GetExtraXScaling()*pView->m_xScaling;
				m_ScaleZSlider.SetPos(m_ScaleXSlider.GetPos());
				pView->m_yScaling = pView->GetExtraZScaling()/pView->GetExtraXScaling()*pView->m_xScaling;
			}
			else
			{
				m_ScaleYSlider.SetPos(m_ScaleXSlider.GetPos());
				pView->m_yScaling = pView->GetExtraYScaling()/pView->GetExtraXScaling()*pView->m_xScaling;
			}
		}
		else if (pSender == &m_ScaleYSlider)
		{
			if (pView->Is3D())
			{
				m_ScaleXSlider.SetPos(m_ScaleYSlider.GetPos());
				pView->m_xScaling = pView->GetExtraXScaling()/pView->GetExtraYScaling()*pView->m_zScaling;
				m_ScaleZSlider.SetPos(m_ScaleYSlider.GetPos());
				pView->m_yScaling = pView->GetExtraZScaling()/pView->GetExtraYScaling()*pView->m_zScaling;
			}
			else
			{
				m_ScaleXSlider.SetPos(m_ScaleYSlider.GetPos());
				pView->m_xScaling = pView->GetExtraXScaling()/pView->GetExtraYScaling()*pView->m_yScaling;
			}
		}
		else if (pSender == &m_ScaleZSlider)
		{
			if (pView->Is3D())
			{
				m_ScaleXSlider.SetPos(m_ScaleZSlider.GetPos());
				pView->m_xScaling = pView->GetExtraXScaling()/pView->GetExtraZScaling()*pView->m_yScaling;
				m_ScaleYSlider.SetPos(m_ScaleZSlider.GetPos());
				pView->m_zScaling = pView->GetExtraYScaling()/pView->GetExtraZScaling()*pView->m_yScaling;
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
}

void CDlgProperties::UpdateTranslation(CWnd* pSender) 
{
	CGlView *pView = (CGlView *)GetGlView();

	int nXMin, nXMax, nYMin, nYMax, nZMin, nZMax;
	m_TranslationXSlider.GetRange(nXMin, nXMax);
	m_TranslationYSlider.GetRange(nYMin, nYMax);
	m_TranslationZSlider.GetRange(nZMin, nZMax);

	pView->m_xTranslation = pView->GetExtraXTranslation()+m_TranslationXSlider.GetPos()/((nXMax-nXMin)/2.);
	if (pView->Is3D())
	{
		pView->m_zTranslation = -(pView->GetExtraYTranslation()+m_TranslationYSlider.GetPos()/((nYMax-nYMin)/2.));
		pView->m_yTranslation = pView->GetExtraZTranslation()+m_TranslationZSlider.GetPos()/((nZMax-nZMin)/2.);
	}
	else
	{
		pView->m_yTranslation = pView->GetExtraYTranslation()+m_TranslationYSlider.GetPos()/((nYMax-nYMin)/2.);
	}

	if (m_bLinkTranslation)
	{
		if (pSender == &m_TranslationXSlider)
		{
			if (pView->Is3D())
			{
				m_TranslationYSlider.SetPos(m_TranslationXSlider.GetPos());
				pView->m_zTranslation = -(pView->GetExtraYTranslation()-pView->GetExtraXTranslation()+pView->m_xTranslation);
				m_TranslationZSlider.SetPos(m_TranslationXSlider.GetPos());
				pView->m_yTranslation = pView->GetExtraZTranslation()-pView->GetExtraXTranslation()+pView->m_xTranslation;
			}
			else
			{
				m_TranslationYSlider.SetPos(m_TranslationXSlider.GetPos());
				pView->m_yTranslation = pView->GetExtraYTranslation()-pView->GetExtraXTranslation()+pView->m_xTranslation;
			}
		}
		else if (pSender == &m_TranslationYSlider)
		{
			if (pView->Is3D())
			{
				m_TranslationXSlider.SetPos(m_TranslationYSlider.GetPos());
				pView->m_xTranslation = pView->GetExtraXTranslation()-pView->GetExtraYTranslation()-pView->m_zTranslation;
				m_TranslationZSlider.SetPos(m_TranslationYSlider.GetPos());
				pView->m_yTranslation = pView->GetExtraZTranslation()-pView->GetExtraYTranslation()-pView->m_zTranslation;
			}
			else
			{
				m_TranslationXSlider.SetPos(m_TranslationYSlider.GetPos());
				pView->m_xTranslation = pView->GetExtraXTranslation()-pView->GetExtraYTranslation()+pView->m_yTranslation;
			}
		}
		else if (pSender == &m_TranslationZSlider)
		{
			if (pView->Is3D())
			{
				m_TranslationXSlider.SetPos(m_TranslationZSlider.GetPos());
				pView->m_xTranslation = pView->GetExtraXTranslation()-pView->GetExtraZTranslation()+pView->m_yTranslation;
				m_TranslationYSlider.SetPos(m_TranslationZSlider.GetPos());
				pView->m_zTranslation = -(pView->GetExtraYTranslation()-pView->GetExtraZTranslation()+pView->m_yTranslation);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
}

void CDlgProperties::UpdateSliderScalePos()
{
	CGlView *pView = (CGlView *)GetGlView();

	if (pView->Is3D())
	{
		m_ScaleZSlider.EnableWindow(TRUE);
		m_ScaleZStatic.EnableWindow(TRUE);
	}
	else
	{
		m_ScaleZSlider.EnableWindow(FALSE);
		m_ScaleZStatic.EnableWindow(FALSE);
	}

	int nXMin, nXMax, nYMin, nYMax, nZMin, nZMax;
	m_ScaleXSlider.GetRange(nXMin, nXMax);
	m_ScaleYSlider.GetRange(nYMin, nYMax);
	m_ScaleZSlider.GetRange(nZMin, nZMax);

	m_ScaleXSlider.SetPos(min(nXMax,max(nXMin,int((nXMax-nXMin)/2.*(pView->m_xScaling/pView->GetExtraXScaling())))));
	if (pView->Is3D())
	{
		m_ScaleYSlider.SetPos(min(nYMax,max(nYMin,int((nYMax-nYMin)/2.*(pView->m_zScaling/pView->GetExtraYScaling())))));
		m_ScaleZSlider.SetPos(min(nZMax,max(nZMin,int((nZMax-nZMin)/2.*(pView->m_yScaling/pView->GetExtraZScaling())))));
	}
	else
	{
		m_ScaleYSlider.SetPos(min(nYMax,max(nYMin,int((nYMax-nYMin)/2.*(pView->m_yScaling/pView->GetExtraYScaling())))));
	}

	if (pView->Is3D())
	{
		m_bLinkScale = ((int((nXMax-nXMin)/2.*(pView->m_xScaling/pView->GetExtraXScaling())) == int((nYMax-nYMin)/2.*(pView->m_zScaling/pView->GetExtraYScaling()))) && 
						(int((nXMax-nXMin)/2.*(pView->m_xScaling/pView->GetExtraXScaling())) == int((nZMax-nZMin)/2.*(pView->m_yScaling/pView->GetExtraZScaling()))));
	}
	else
	{
		m_bLinkScale = (int((nXMax-nXMin)/2.*(pView->m_xScaling/pView->GetExtraXScaling())) == int((nYMax-nYMin)/2.*(pView->m_yScaling/pView->GetExtraYScaling())));
	}
	m_CheckLinkScaleButton.SetCheck(!!m_bLinkScale);

	InvalidateRect(NULL, FALSE);
}

void CDlgProperties::UpdateSliderTranslationPos()
{
	CGlView *pView = (CGlView *)GetGlView();

	if (pView->Is3D())
	{
		m_TranslationZSlider.EnableWindow(TRUE);
		m_TranslationZStatic.EnableWindow(TRUE);
	}
	else
	{
		m_TranslationZSlider.EnableWindow(FALSE);
		m_TranslationZStatic.EnableWindow(FALSE);
	}

	int nXMin, nXMax, nYMin, nYMax, nZMin, nZMax;
	m_TranslationXSlider.GetRange(nXMin, nXMax);
	m_TranslationYSlider.GetRange(nYMin, nYMax);
	m_TranslationZSlider.GetRange(nZMin, nZMax);

	m_TranslationXSlider.SetPos(min(nXMax,max(nXMin,int((nXMax-nXMin)/2.*(pView->m_xTranslation-pView->GetExtraXTranslation())))));
	if (pView->Is3D())
	{
		m_TranslationYSlider.SetPos(min(nYMax,max(nYMin,int((nYMax-nYMin)/2.*(-pView->m_zTranslation-pView->GetExtraYTranslation())))));
		m_TranslationZSlider.SetPos(min(nZMax,max(nZMin,int((nZMax-nZMin)/2.*(pView->m_yTranslation-pView->GetExtraZTranslation())))));
	}
	else
	{
		m_TranslationYSlider.SetPos(min(nYMax,max(nYMin,int((nYMax-nYMin)/2.*(pView->m_yTranslation-pView->GetExtraYTranslation())))));
	}

	m_bLinkTranslation = FALSE;
	m_CheckLinkTranslationButton.SetCheck(!!m_bLinkTranslation);

	InvalidateRect(NULL, FALSE);
}

void CDlgProperties::UpdateFontColorRect()
{
	CGlView *pView = (CGlView *)GetGlView();

	CRect rect;

	m_OptionColorFont = RGB(int(round(pView->m_ColorFontRed*255.0f)),int(round(pView->m_ColorFontGreen*255.0f)),int(round(pView->m_ColorFontBlue*255.0f)));

	m_ColorFontStatic.SetBkColor(m_OptionColorFont);
	m_ColorFontStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.InflateRect(2, 2, 2, 2);
	InvalidateRect(&rect, FALSE);
}

void CDlgProperties::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	if (IsWindowVisible())
	{
		if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL))
			EndDialog(IDOK);
		else
			ShowWindow(SW_HIDE);
	}

	CDialogTts::OnContextMenu(pWnd, point);
}

void CDlgProperties::OnEditFont() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	GotoDlgCtrl(this);

	CGlView *pView = (CGlView *)GetGlView();
	pView->MakeActive();
	pView->OnEditFont();
}

void CDlgProperties::EnableForeCtrls(BOOL bEnable)
{
	CRect rect;

	m_ForeTextStatic.EnableWindow(bEnable);
	m_ColorForeStatic.EnableWindow(bEnable);

	if (bEnable)
	{
		m_ColorForeStatic.ModifyStyle(SS_GRAYRECT | WS_BORDER, 0);
	}
	else
	{
		m_ColorForeStatic.ModifyStyle(0, SS_GRAYRECT | WS_BORDER);
	}

	m_ColorForeStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.InflateRect(2, 2, 2, 2);
	InvalidateRect(&rect, FALSE);
}

void CDlgProperties::EnableLightCtrls(BOOL bEnable)
{
	CRect rect;

	m_LightTextStatic.EnableWindow(bEnable);
	m_ColorLightAmbientStatic.EnableWindow(bEnable);
	m_ColorLightDiffuseStatic.EnableWindow(bEnable);
	m_ColorLightSpecularStatic.EnableWindow(bEnable);

	if (bEnable)
	{
		m_ColorLightAmbientStatic.ModifyStyle(SS_GRAYRECT | WS_BORDER, 0);
		m_ColorLightDiffuseStatic.ModifyStyle(SS_GRAYRECT | WS_BORDER, 0);
		m_ColorLightSpecularStatic.ModifyStyle(SS_GRAYRECT | WS_BORDER, 0);
	}
	else
	{
		m_ColorLightAmbientStatic.ModifyStyle(0, SS_GRAYRECT | WS_BORDER);
		m_ColorLightDiffuseStatic.ModifyStyle(0, SS_GRAYRECT | WS_BORDER);
		m_ColorLightSpecularStatic.ModifyStyle(0, SS_GRAYRECT | WS_BORDER);
	}

	m_ColorLightAmbientStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.InflateRect(2, 2, 2, 2);
	InvalidateRect(&rect, FALSE);

	m_ColorLightDiffuseStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.InflateRect(2, 2, 2, 2);
	InvalidateRect(&rect, FALSE);

	m_ColorLightSpecularStatic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.InflateRect(2, 2, 2, 2);
	InvalidateRect(&rect, FALSE);
}

void CDlgProperties::OnFrameColorFont() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CColorDialogEx dlg(IDS_COLOR_FONT, m_OptionColorFont, CC_ENABLETEMPLATE | CC_PREVENTFULLOPEN);
	dlg.m_cc.hInstance = (HWND) AfxFindResourceHandle(MAKEINTRESOURCE(IDD_DLG_COLOR_BASIC), RT_DIALOG);
	dlg.m_cc.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_COLOR_BASIC);
	dlg.m_cc.lpCustColors = pApp->GetCustColors();
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorFont = dlg.GetColor();
		if (pApp->m_bPropPolicy)
		{
			POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
			while (posDocTemplate != NULL)
			{
				CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
							if (pDlg)
							{
								pDlg->ChangeFontColor();
							}
						}
					}
				}
			}
		}
		else
			ChangeFontColor();

		CGlView::CheckColors(pApp->m_OptionColorBack, pApp->m_OptionColorFont);
	}
	m_ColorFontStatic.SetFocus();
}

void CDlgProperties::OnFrameColorFore() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CColorDialogEx dlg(IDS_COLOR_FORE, m_OptionColorFore);
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorFore = dlg.GetColor();
		if (pApp->m_bPropPolicy)
		{
			POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
			while (posDocTemplate != NULL)
			{
				CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
							if (pDlg)
							{
								pDlg->ChangeForeColor();
							}
						}
					}
				}
			}
		}
		else
			ChangeForeColor();

		CGlView::CheckColors(pApp->m_OptionColorBack, pApp->m_OptionColorFore);
	}
	m_ColorForeStatic.SetFocus();
}

void CDlgProperties::OnFrameColorBack() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CColorDialogEx dlg(IDS_COLOR_BACK, m_OptionColorBack);
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorBack = dlg.GetColor();
		if (pApp->m_bPropPolicy)
		{
			POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
			while (posDocTemplate != NULL)
			{
				CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
							if (pDlg)
							{
								pDlg->ChangeBackColor();
							}
						}
					}
				}
			}
		}
		else
			ChangeBackColor();
	}
	m_ColorBackStatic.SetFocus();
}

void CDlgProperties::OnFrameColorLightAmbient() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CColorDialogEx dlg(IDS_COLOR_LIGHT_AMBIENT, m_OptionColorLightAmbient);
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorLightAmbient = dlg.GetColor();
		if (pApp->m_bPropPolicy)
		{
			POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
			while (posDocTemplate != NULL)
			{
				CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
							if (pDlg)
							{
								pDlg->ChangeAmbientColor();
							}
						}
					}
				}
			}
		}
		else
			ChangeAmbientColor();
	}
	m_ColorLightAmbientStatic.SetFocus();
}

void CDlgProperties::OnFrameColorLightDiffuse() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CColorDialogEx dlg(IDS_COLOR_LIGHT_DIFFUSE, m_OptionColorLightDiffuse);
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorLightDiffuse = dlg.GetColor();
		if (pApp->m_bPropPolicy)
		{
			POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
			while (posDocTemplate != NULL)
			{
				CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
							if (pDlg)
							{
								pDlg->ChangeDiffuseColor();
							}
						}
					}
				}
			}
		}
		else
			ChangeDiffuseColor();
	}
	m_ColorLightDiffuseStatic.SetFocus();
}

void CDlgProperties::OnFrameColorLightSpecular() 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CColorDialogEx dlg(IDS_COLOR_LIGHT_SPECULAR, m_OptionColorLightSpecular);
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorLightSpecular = dlg.GetColor();
		if (pApp->m_bPropPolicy)
		{
			POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
			while (posDocTemplate != NULL)
			{
				CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
				POSITION posDoc = pDocTemplate->GetFirstDocPosition();
				while (posDoc != NULL)
				{
					CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
					POSITION posView = pDoc->GetFirstViewPosition();
					while (posView != NULL)
					{
						CView* pView = pDoc->GetNextView(posView);
						if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
						{
							CDlgProperties* pDlg = ((CGlView *)pView)->m_pDlgProperties;
							if (pDlg)
							{
								pDlg->ChangeSpecularColor();
							}
						}
					}
				}
			}
		}
		else
			ChangeSpecularColor();
	}
	m_ColorLightSpecularStatic.SetFocus();
}

void CDlgProperties::LeftCenterWindow(CWnd* pAlternateOwner)
{
	// LeftCenterWindow is adapted from CWnd::CenterWindow;
	// only one line was modified (original code as comment)

	ASSERT(::IsWindow(m_hWnd));

	// determine owner window to center against
	DWORD dwStyle = GetStyle();
	HWND hWndCenter = pAlternateOwner->GetSafeHwnd();
	if (pAlternateOwner == NULL)
	{
		if (dwStyle & WS_CHILD)
			hWndCenter = ::GetParent(m_hWnd);
		else
			hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
		if (hWndCenter != NULL)
		{
			// let parent determine alternate center window
			HWND hWndTemp =
				(HWND)::SendMessage(hWndCenter, WM_QUERYCENTERWND, 0, 0);
			if (hWndTemp != NULL)
				hWndCenter = hWndTemp;
		}
	}

	// get coordinates of the window relative to its parent
	CRect rcDlg;
	GetWindowRect(&rcDlg);
	CRect rcArea;
	CRect rcCenter;
	HWND hWndParent;
	if (!(dwStyle & WS_CHILD))
	{
		// don't center against invisible or minimized windows
		if (hWndCenter != NULL)
		{
			DWORD dwStyle = ::GetWindowLong(hWndCenter, GWL_STYLE);
			if (!(dwStyle & WS_VISIBLE) || (dwStyle & WS_MINIMIZE))
				hWndCenter = NULL;
		}
		// center within screen coordinates
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);

		if (hWndCenter == NULL)
			rcCenter = rcArea;
		else
		{
#ifndef _MAC
			::GetWindowRect(hWndCenter, &rcCenter);
#else
			::GetClientRect(hWndCenter, &rcCenter);
			::MapWindowPoints(hWndCenter, HWND_DESKTOP, (POINT*)&rcCenter, 2);
#endif
		}
	}
	else
	{
		// center within parent client coordinates
		hWndParent = ::GetParent(m_hWnd);
		ASSERT(::IsWindow(hWndParent));

		::GetClientRect(hWndParent, &rcArea);
		ASSERT(::IsWindow(hWndCenter));
		::GetClientRect(hWndCenter, &rcCenter);
		::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
	}

#ifndef _MAC
	// find dialog's upper left based on rcCenter
	// int xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.Width() / 2;
// new code:
	int xLeft = rcCenter.left - rcDlg.Width() * 4 / 5;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg.Height() / 2;
#else
	// find dialog's upper left based on rcCenter
	// (Mac UI puts 1/5th of parent window above dialog instead of 1/2)
	// int xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg.Width() / 2;
// new code:
	int xLeft = rcCenter.left - rcDlg.Width() * 4 / 5;
	int yTop = (rcCenter.bottom - rcCenter.top) - rcDlg.Height();
	yTop = rcCenter.top + yTop / 5;
#endif

	// if the dialog is outside the screen, move it inside
	if (xLeft < rcArea.left)
		xLeft = rcArea.left;
	else if (xLeft + rcDlg.Width() > rcArea.right)
		xLeft = rcArea.right - rcDlg.Width();

	if (yTop < rcArea.top)
		yTop = rcArea.top;
	else if (yTop + rcDlg.Height() > rcArea.bottom)
		yTop = rcArea.bottom - rcDlg.Height();

	// map screen coordinates to child coordinates
	SetWindowPos(NULL, xLeft, yTop, -1, -1,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
