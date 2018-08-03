/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 1999
//
// special thanks for the printing routines to
// Craig Fahrnbach <craignan@home.com>
//
// and for WINDOW class registering to
// Alessandro Falappa
//
/////////////////////////////////////////////////////////////////////////////
// GlView.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "GlWinApp.h"
#include "GlView.h"
#include "GlDocument.h"
#include "Helper.h"
#include "SplitterWndEx.h"
#include "DlgOpenGLInfo.h"
#include "DlgProperties.h"
#include "FileDialogEx.h"
#include "PsRenderer.h"
#include "Matrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _MSC_VER < 1200
IMPLEMENT_DYNCREATE(CGlPreviewView, CPreviewView)
#endif // _MSC_VER < 1200


//using namespace mymathfuncs;

/////////////////////////////////////////////////////////////////////////////
// CGlView

IMPLEMENT_DYNAMIC(CGlView, CView)

BEGIN_MESSAGE_MAP(CGlView, CView)
	//{{AFX_MSG_MAP(CGlView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_COPY_EMF, OnEditCopyEMF)
	ON_COMMAND(ID_EDIT_PASTE_EMF, OnEditPasteEMF)
	ON_COMMAND(ID_EDIT_EXPORT_PS, OnEditExportPs)
	ON_COMMAND(ID_EDIT_FONT, OnEditFont)
	ON_COMMAND(ID_HELP_OPENGL_INFO, OnHelpOpenGLInfo)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlView construction/destruction

CGlView::CGlView()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	// OpenGL
	m_pDC = NULL;
	m_hRC = NULL;
	m_nBitsPerPixel = 0;
	m_aspect = 1.0;

	m_nListBaseSmall = 1000;
	m_nListBaseNormal = 2000;

	// Mouse
	// HCURSOR hCursorDblCross = LoadCursor(NULL, IDC_CROSS);
	HCURSOR hCursorDblCross = pApp->LoadCursor(IDC_CURSOR_DBL_CROSS);
	if (hCursorDblCross)
		m_hCursorDblCross = CopyCursor(hCursorDblCross);

	// Colors
	m_ColorBackRed   = (float)GetRValue(pApp->m_OptionColorBack) / 255.0f;
	m_ColorBackGreen = (float)GetGValue(pApp->m_OptionColorBack) / 255.0f;
	m_ColorBackBlue  = (float)GetBValue(pApp->m_OptionColorBack) / 255.0f;

	m_ColorBackRedTmp   = 1.0f;
	m_ColorBackGreenTmp = 1.0f;
	m_ColorBackBlueTmp  = 1.0f;

	m_ColorForeRed   = (float)GetRValue(pApp->m_OptionColorFore) / 255.0f;
	m_ColorForeGreen = (float)GetGValue(pApp->m_OptionColorFore) / 255.0f;
	m_ColorForeBlue  = (float)GetBValue(pApp->m_OptionColorFore) / 255.0f;

	m_ColorForeRedTmp   = 1.0f;
	m_ColorForeGreenTmp = 1.0f;
	m_ColorForeBlueTmp  = 1.0f;

	m_ColorFontRed   = (float)GetRValue(pApp->m_OptionColorFont) / 255.0f;
	m_ColorFontGreen = (float)GetGValue(pApp->m_OptionColorFont) / 255.0f;
	m_ColorFontBlue  = (float)GetBValue(pApp->m_OptionColorFont) / 255.0f;

	m_ColorFontRedTmp   = 1.0f;
	m_ColorFontGreenTmp = 1.0f;
	m_ColorFontBlueTmp  = 1.0f;

	// Set Light0
	m_ColorLightAmbientRed   = (float)GetRValue(pApp->m_OptionColorLightAmbient) / 255.0f;
	m_ColorLightAmbientGreen = (float)GetGValue(pApp->m_OptionColorLightAmbient) / 255.0f;
	m_ColorLightAmbientBlue  = (float)GetBValue(pApp->m_OptionColorLightAmbient) / 255.0f;

	m_ColorLightDiffuseRed   = (float)GetRValue(pApp->m_OptionColorLightDiffuse) / 255.0f;
	m_ColorLightDiffuseGreen = (float)GetGValue(pApp->m_OptionColorLightDiffuse) / 255.0f;
	m_ColorLightDiffuseBlue  = (float)GetBValue(pApp->m_OptionColorLightDiffuse) / 255.0f;

	m_ColorLightSpecularRed   = (float)GetRValue(pApp->m_OptionColorLightSpecular) / 255.0f;
	m_ColorLightSpecularGreen = (float)GetGValue(pApp->m_OptionColorLightSpecular) / 255.0f;
	m_ColorLightSpecularBlue  = (float)GetBValue(pApp->m_OptionColorLightSpecular) / 255.0f;

	m_bFontCreateFlagSmall = FALSE;
	m_bFontCreateFlagNormal = FALSE;
	m_dFontRatio = .5;

	m_bMonochromeMode = pApp->m_bMonochromeMode;

	// Initialize zoom
	m_bLastActivateWasDoneByLButton = FALSE;
	m_bFirstPickPoint = FALSE;
	m_bSecondPickPoint = FALSE;

	m_StartPoint.SetSize(2);
	m_EndPoint.SetSize(2);
	m_StartPoint[0] = m_StartPoint[1] = 0.;
	m_EndPoint[0] = m_EndPoint[1] = 0.;

	m_xMin = -1.;
	m_yMin = -1.;
	m_xMax = 1.;
	m_yMax = 1.;

	// Initialize rendering
	m_XRenderArray.SetSize(0);
	m_YRenderArray.SetSize(0);
	m_ZRenderArray.SetSize(0);
	m_CRenderArray.SetSize(0);
	m_SRenderArray.SetSize(0);

	// Initialize printing
	m_bStretchDIBits = FALSE;
	m_bStretchBlt = FALSE;

	m_bIsDocPrinting = FALSE;
	m_bBlackLinesOrRemoveHidden = pApp->m_bBlackLinesOrRemoveHidden;
	m_bScaleShowFrame = pApp->m_bScaleShowFrame;
	m_bScaleShowLabels = pApp->m_bScaleShowLabels;
	m_nScaleShowGrid = pApp->m_nScaleShowGrid;
	m_bScaleShowAxis = pApp->m_bScaleShowAxis;
	m_bScaleShowAxisLabels = pApp->m_bScaleShowAxisLabels;
	m_DocPrintHeight = 0;
	m_dInvertNonRainbowColors = 1.;

	m_pDlgProperties = NULL;
}

CGlView::~CGlView()
{
	if (m_hCursorDblCross != NULL)
	{
		::DestroyCursor(m_hCursorDblCross);
		m_hCursorDblCross = NULL;
	}
}

void CGlView::OnDraw(CDC*)
{
}

void CGlView::InitGeometry()
{
	m_xRotationStore = m_xRotation = 0.0;
	m_yRotationStore = m_yRotation = 0.0;

	m_xTranslation = 0.0;
	m_yTranslation = 0.0;
	m_zTranslation = 0.0;

	m_xScaling = 1.0;
	m_yScaling = 1.0;
	m_zScaling = 1.0;

	double dAspect = min(m_aspect, 1.);

	if (dAspect != 1. && dAspect != 0.)
	{
		m_xScaling *= dAspect;
		m_yScaling *= dAspect;
		m_zScaling *= dAspect;

		m_xTranslation *= dAspect;
		m_yTranslation *= dAspect;
	}

	m_ZoomHistory.SetSize(0);
}

// this is the new registered window class
#define CUSTOM_CLASSNAME _T("GlViewClass")

BOOL CGlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// these styles are requested by OpenGL
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);			 

	// call base class PreCreateWindow to get the cs.lpszClass filled in with the MFC default class name
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// Register the window class if it has not already been registered.
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	// check if our class has been already registered (typical in MDI environment)
	if (!(::GetClassInfo(hInst, CUSTOM_CLASSNAME, &wndcls)))
	{
		// get default MFC class settings
		if (::GetClassInfo(hInst, cs.lpszClass, &wndcls))
		{
			// set our class name
			wndcls.lpszClassName = CUSTOM_CLASSNAME;

			// these styles are set for GL to work in MDI
			wndcls.style |= (CS_OWNDC | CS_HREDRAW | CS_VREDRAW);
			wndcls.hbrBackground = NULL;

			// try to register class (else throw exception)
			if (!AfxRegisterClass(&wndcls))
				AfxThrowResourceException();
		}
		else
		{
			// default MFC class not registered
			AfxThrowResourceException();
		}
	}

	// set our class name in CREATESTRUCT
	cs.lpszClass = CUSTOM_CLASSNAME;

	// we're all set
	return TRUE;
}

void CGlView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		MSG msg;
		m_bLastActivateWasDoneByLButton = PeekMessage(&msg, m_hWnd, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK, PM_NOREMOVE);

		// Note: PeekMessage is in here for correct parent frame behaviour (strange, but true)
		//
		// The first PeekMessage call yields some message confusion in the parent frame.
		// If the user _single_ clicks to the parent frame's caption, then the parent frame
		// gets a WM_NCLBUTTONDBLCLK message. No idea why the above PeekMessage call causes
		// this confusion. And no idea why the next PeekMessage call does repair it.
		PeekMessage(&msg, GetParentFrame()->GetSafeHwnd(), WM_NCLBUTTONDBLCLK, WM_NCLBUTTONDBLCLK, PM_NOREMOVE);
		PeekMessage(&msg, m_hWnd, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK, PM_NOREMOVE);
	}

	CWnd* pParent = GetParent();
	if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
		((CSplitterWndEx*)pParent)->RefreshSplitterBars(&bActivate);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		MakeActive();

		// AfxGetMainWnd() may return NULL in an SDI environment
		CWnd* pWnd = AfxGetMainWnd();
		if (pWnd != NULL)
		{
			// update toolbars immedeately
			pWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGlView printing

BOOL CGlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// set the page count if not previously set
	if (pInfo->GetMaxPage() == 0xFFFF)
		pInfo->SetMaxPage(pInfo->GetMinPage());

	PrepareBkgnd(TRUE);

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGlView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	if (pApp->m_nPrintObject == 2)
	{
		if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)))
		{
			pApp->BeginPrinting(pDC);
		}
	}

	// Note: This value will only be used in CGlDocument::OnPrint
	CRect	mRect;			// Position of this window's client area
	// Get the Views size (client area!)
	GetClientRect(&mRect);
	m_DocPrintHeight = mRect.Height();

	// Gather some general information about the printer the
	// user selected..
	LPDEVMODE* pMode = (LPDEVMODE*) pInfo->m_pPD->m_pd.hDevMode;
	DEVMODE* mode = *pMode;
	CString mMsg;
	TCHAR* pChar = (TCHAR*)&mode->dmDeviceName[0];

	int nBitsPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nColors		= pDC->GetDeviceCaps(NUMCOLORS);
	int nBitPlanes	= pDC->GetDeviceCaps(PLANES);
	int nColorRes	= pDC->GetDeviceCaps(COLORRES);
	int nRasterCaps	= pDC->GetDeviceCaps(RASTERCAPS);

	// Does the device support raster operations?
	if (nRasterCaps & RC_STRETCHDIB)
		m_bStretchDIBits = TRUE;
	else
		m_bStretchDIBits = FALSE;

	if (nRasterCaps & RC_STRETCHBLT)
		m_bStretchBlt = TRUE;
	else
		m_bStretchBlt = FALSE;

	TRACE("You selected printer '%s'\n", pChar);
	TRACE("BitsPerPixel           = %d\n", nBitsPerPixel);
	TRACE("NumColor               = %d\n", nColors);
	TRACE("NumBitPlanes           = %d\n", nBitPlanes);
	TRACE("ColorResolution        = %d\n", nColorRes);
	TRACE("Supports StretchBlt    = %d\n", m_bStretchBlt);
	TRACE("Supports StretchDIBits = %d\n", m_bStretchDIBits);

	if (!m_bStretchDIBits && !m_bStretchBlt)
	{
		mMsg.Format(IDS_ERROR_PRINTER_NO_SUPPORT_MSG, "\"", pChar, "\" ");
		AfxMessageBox(mMsg, MB_OK | MB_ICONERROR);
	}
}

void CGlView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	UNREFERENCED_PARAMETER(pInfo);

	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	if (pApp->m_nPrintObject == 2)
	{
		if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)))
		{
			pApp->EndPrinting(pDC);
		}
	}

	PrepareBkgnd(FALSE);

	// Note: This value will only be used in CGlDocument::OnPrint
	m_DocPrintHeight = 0;
}

void CGlView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	if (pApp->m_nPrintObject == 1)
	{
		CDocument* pDoc = GetDocument();
		if (pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
		{
			// let the document do the job
			((CGlDocument*)pDoc)->OnPrint(pDC, pInfo, this);
			return;
		}
	}
	else if (pApp->m_nPrintObject == 2)
	{
		if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)))
		{
			pApp->Print(pDC);
			return;
		}
	}

	SIZE	res;			// Device resolution
	RECT	rect;			// Page rectangle
	HBRUSH	brush;			// Background brush for page
	POINT	PageSize;		// Size of printed image
	POINT	ImageOffset;	// Offset from edges for image
	CRect	mRect;			// Position of this window's client area
	int		nXRes, nYRes;
	HDC		hDC, hMemDC, hTmpDC;
	HGLRC	hRC, hMemRC;

	BITMAPINFO	bitmapInfo;
	HBITMAP		hDib;
	LPVOID		pBitmapBits;
	double		fac = 1.;		// try <printer device> DPI

	// Set our device context mapping mode.
	// Each logical unit is converted to 1 device pixel. Positive x is to
	// the right; positive y is down.
	pDC->SetMapMode(MM_TEXT);

	// Get the size of the page (in printer device points)
	rect.top	= 0;
	rect.left   = 0;
	rect.bottom	= 0;
	rect.right   = 0;

	// Get the Views size (client area!)
	GetClientRect(&mRect);

	if (pInfo->m_bPreview)
	{
		PRINTDLG PrtDlg;	// Printer dialog

		if (!pApp->GetPrinterDeviceDefaults(&PrtDlg))
		{
			TRACE("No default printer.\n");
			// Just create a display device context.
			// Note that the DIB image will NOT be 
			// positioned properly, but it will at least
			// be displayed.

			if (pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC)))
			{
				CPreviewDC *pPrevDC = (CPreviewDC *)pDC;
				//m_hAttribDC hold the print/fax DC so...
				rect.right =  GetDeviceCaps(pPrevDC->m_hAttribDC, HORZRES);
				rect.bottom = GetDeviceCaps(pPrevDC->m_hAttribDC, VERTRES);
				res.cx = GetDeviceCaps(pPrevDC->m_hAttribDC, LOGPIXELSX);
				res.cy = GetDeviceCaps(pPrevDC->m_hAttribDC, LOGPIXELSY);
			}
			else
			{
				rect.right = 0;
				rect.bottom = 0;
				res.cx = 0;
				res.cy = 0;
			}
		}
		else
		{
			// Don't display the dialog.  We create the CPrintDialog so that
			// we can create a printer device context.
			CPrintDialog dlg(FALSE);

			dlg.m_pd.hDevMode  = PrtDlg.hDevMode;
			dlg.m_pd.hDevNames = PrtDlg.hDevNames;

			HDC hdc = dlg.CreatePrinterDC();

			// Get the size of the default printer page
			rect.right  = GetDeviceCaps(hdc, HORZRES);
			rect.bottom = GetDeviceCaps(hdc, VERTRES);

			// Get the resolution of the printer device
			res.cx = GetDeviceCaps(hdc, LOGPIXELSX);
			res.cy = GetDeviceCaps(hdc, LOGPIXELSY);

			// Free the device context
			DeleteDC(hdc);
		}
	}
	else
	{
		// Get the size of the actual printed page
		rect.right  = GetDeviceCaps(pDC->GetSafeHdc(), HORZRES);
		rect.bottom = GetDeviceCaps(pDC->GetSafeHdc(), VERTRES);

		// Get the resolution of the printer device
		res.cx = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSX);
		res.cy = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY);
	}

	// Determine whether the Views aspect ratio is bigger or smaller than the printers one
	if (mRect.Width()*(rect.bottom-rect.top) > mRect.Height()*(rect.right-rect.left))
	{
		// View area is wider (in x) than Printer area
		nXRes = rect.right-rect.left;
		nYRes = nXRes*mRect.Height()/mRect.Width();

		// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
		while (3*nXRes*nYRes > (pApp->m_nMaxPrintMemSize)*1024) 
		{
			nXRes /= 2;
			nYRes /= 2;
			fac /= 2.;
		}
		ScaleFont(double(nXRes)/double(mRect.Width()));
	}
	else
	{
		// Printer area is wider (in x) than View area
		nYRes = rect.bottom-rect.top;
		nXRes = nYRes*mRect.Width()/mRect.Height();

		// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
		while (3*nXRes*nYRes > (pApp->m_nMaxPrintMemSize)*1024)
		{
			nXRes /= 2;
			nYRes /= 2;
			fac /= 2.;
		}
		ScaleFont(double(nYRes)/double(mRect.Height()));
	}

	//nXRes = (nXRes + (sizeof(DWORD)-1)) & ~(sizeof(DWORD)-1);	// aligning width to 4 bytes (sizeof(DWORD)) avoids 
	nXRes = nXRes & ~(sizeof(DWORD)-1);							// aligning width to 4 bytes (sizeof(DWORD)) avoids 
																// pixel garbage at the upper line

	// First of all, initialize the bitmap header information...
	memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth		= nXRes;
	bitmapInfo.bmiHeader.biHeight		= nYRes;
	bitmapInfo.bmiHeader.biPlanes		= 1;
	bitmapInfo.bmiHeader.biBitCount		= 24;
	bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * 3;
	bitmapInfo.bmiHeader.biXPelsPerMeter = int(res.cx*fac/.0254);	// res.cx*fac DPI
	bitmapInfo.bmiHeader.biYPelsPerMeter = int(res.cy*fac/.0254);	// res.cy*fac DPI

	// create DIB
	hTmpDC = ::GetDC(m_hWnd);
	hDib = CreateDIBSection(hTmpDC, &bitmapInfo, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(m_hWnd, hTmpDC);

	// create memory device context
	if ((hMemDC = CreateCompatibleDC(m_pDC == NULL ? NULL : m_pDC->GetSafeHdc())) == NULL)
	{
		DeleteObject(hDib);
		return;
	}
	HGDIOBJ hOldDib = SelectObject(hMemDC, hDib);

	// setup pixel format
	if (!SetMemDcPixelFormat(hMemDC) && !SetMemDcPixelFormat(hMemDC, TRUE))
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return;
	}

	// create memory rendering context
	if ((hMemRC = wglCreateContext(hMemDC)) == NULL)
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return;
	}

	// Store current rendering and device contexts
	GetCurrent(hDC, hRC);

	// Make this hMemRC the current OpenGL rendering context.
	SetCurrent(hMemDC, hMemRC);

	SetOpenGLProperties();

	glViewport(0, 0, nXRes, nYRes);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	// must be created once for hMemDC
	CreateFontBitmaps();

	OnDraw(pDC);
	glFinish();	// Finish all OpenGL commands

	// the rendering context will be no longer needed
	SetCurrent(NULL, NULL);
	wglDeleteContext(hMemRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	// Restore the Views original font size
	UnScaleFont();
	CreateFontBitmaps();

	// Get the size of our page and clear to White
	brush = CreateSolidBrush(0x00ffffff);		// White brush
	FillRect(pDC->GetSafeHdc(), &rect, brush);	// Clear the page

	PageSize.x = rect.right;
	PageSize.y = PageSize.x * bitmapInfo.bmiHeader.biHeight / bitmapInfo.bmiHeader.biWidth;
	if (PageSize.y > rect.bottom)
	{
		PageSize.y = rect.bottom;
		PageSize.x = PageSize.y * bitmapInfo.bmiHeader.biWidth / bitmapInfo.bmiHeader.biHeight;
	}
	ImageOffset.x = pApp->m_bPrintCenter ? (rect.right - PageSize.x) / 2 : 0;
	ImageOffset.y = pApp->m_bPrintCenter ? (rect.bottom - PageSize.y) / 2 : 0;

	// Does our device context support StretchDIBits?
	if (m_bStretchDIBits)
	{
		StretchDIBits(pDC->GetSafeHdc(),
								ImageOffset.x, ImageOffset.y,
								PageSize.x, PageSize.y,
								0, 0,
								bitmapInfo.bmiHeader.biWidth,
								bitmapInfo.bmiHeader.biHeight,
								(GLubyte*)pBitmapBits,
								&bitmapInfo,
								DIB_RGB_COLORS,
								SRCCOPY);
	}
	else if (m_bStretchBlt)
	{
		HDC hdc;
		HBITMAP bitmap;

		hdc	= CreateCompatibleDC(pDC->GetSafeHdc());
		bitmap = CreateDIBitmap(hdc,
								&(bitmapInfo.bmiHeader),
								CBM_INIT,
								(GLubyte*)pBitmapBits,
								&bitmapInfo,
								DIB_RGB_COLORS);

		HGDIOBJ oldbitmap = SelectObject(hdc, bitmap);
		StretchBlt(pDC->GetSafeHdc(),
								ImageOffset.x, ImageOffset.y,
								PageSize.x, PageSize.y,
								hdc,
								0, 0,
								bitmapInfo.bmiHeader.biWidth,
								bitmapInfo.bmiHeader.biHeight,
								SRCCOPY);
		if (oldbitmap != NULL)
			SelectObject(hdc, oldbitmap);

		// Free our bitmap and bitmap device context
		DeleteObject(bitmap);
		DeleteDC(hdc);
	}
	if (hOldDib != NULL)
		SelectObject(hMemDC, hOldDib);

	// Delete our brush, DIB and device context
	DeleteObject(brush);
	DeleteObject(hDib);
	DeleteDC(hMemDC);
}

#if _MSC_VER < 1200
void CGlView::OnFilePrintPreview()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.

	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,
							RUNTIME_CLASS(CGlPreviewView), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case

		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}

BOOL CGlView::DoPrintPreview(UINT nIDResource, CView* pPrintView,
	CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState)
{
	ASSERT_VALID_IDR(nIDResource);
	ASSERT_VALID(pPrintView);
	ASSERT(pPreviewViewClass != NULL);
	ASSERT(pPreviewViewClass->IsDerivedFrom(RUNTIME_CLASS(CGlPreviewView)));
	ASSERT(pState != NULL);

	CFrameWnd* pParent = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
	ASSERT_VALID(pParent);

	CCreateContext context;
	context.m_pCurrentFrame = pParent;
	context.m_pCurrentDoc = GetDocument();
	context.m_pLastView = this;

	// Create the preview view object
	CGlPreviewView* pView = (CGlPreviewView*)pPreviewViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE0("Error: Failed to create preview view.\n");
		return FALSE;
	}
	ASSERT_KINDOF(CGlPreviewView, pView);
	pView->m_pPreviewState = pState;        // save pointer

	pParent->OnSetPreviewMode(TRUE, pState);    // Take over Frame Window

	// Create the toolbar from the dialog resource
	pView->m_pToolBar = new CDialogBar;
	if (!pView->m_pToolBar->Create(pParent, MAKEINTRESOURCE(nIDResource),
		CBRS_TOP, AFX_IDW_PREVIEW_BAR))
	{
		TRACE0("Error: Preview could not create toolbar dialog.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		delete pView->m_pToolBar;       // not autodestruct yet
		pView->m_pToolBar = NULL;
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}
	pView->m_pToolBar->m_bAutoDelete = TRUE;    // automatic cleanup

	// Create the preview view as a child of the App Main Window.  This
	// is a sibling of this view if this is an SDI app.  This is NOT a sibling
	// if this is an MDI app.

	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE0("Error: couldn't create preview view for frame.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}

	// Preview window shown now

	pState->pViewActiveOld = pParent->GetActiveView();
	// Note: We downcast pActiveView to type "CGlView*" to get access to the protected "OnActivateView".
	//       Even if pActiveView points to an object which is not derived from "CGlView" 
	//       the correct virtual "OnActivateView" will be called.
	CGlView* pActiveView = (CGlView*)pParent->GetActiveFrame()->GetActiveView();
	if (pActiveView != NULL)
		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

	if (!pView->SetPrintView(pPrintView))
	{
		pView->OnPreviewClose();
		return TRUE;            // signal that OnEndPrintPreview was called
	}

	pParent->SetActiveView(pView);  // set active view - even for MDI

	// update toolbar and redraw everything
	pView->m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();

	return TRUE;
}
#endif // _MSC_VER < 1200

/////////////////////////////////////////////////////////////////////////////
// OpenGL specific

BOOL CGlView::InitializeOpenGL()
{
	if (m_pDC != NULL)
	{
		delete m_pDC;
		m_pDC = NULL;
		m_nBitsPerPixel = 0;
	}

	m_pDC = new CWindowDC(this);

	if (m_pDC == NULL) // failure to get DC
	{
		return FALSE;
	}

	m_nBitsPerPixel = m_pDC->GetDeviceCaps(BITSPIXEL);

	if (!SetWindowPixelFormat() && !SetWindowPixelFormat(TRUE))
	{
		return FALSE;
	}

	CreateRGBPalette();

	if (!CreateViewGLContext())
	{
		return FALSE;
	}

	SetOpenGLProperties();
	CreateFontBitmaps();

	return TRUE;
}

void CGlView::CreateRGBPalette()
{
	if (m_Palette.m_hObject != NULL)
	{
		m_pDC->SelectPalette(&m_Palette, FALSE);
		m_pDC->RealizePalette();
		return;
	}

	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE  *pPal;
	WORD		n, i;
	int			nGLPixelIndex;

	nGLPixelIndex = GetPixelFormat(m_pDC->GetSafeHdc());
	DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		n = (WORD)(1 << pfd.cColorBits);

		pPal = (PLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];
		ASSERT(pPal != NULL);

		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;

		for (i = 0; i < n; i++)
		{
			pPal->palPalEntry[i].peRed = ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
			pPal->palPalEntry[i].peGreen = ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
			pPal->palPalEntry[i].peBlue = ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
			pPal->palPalEntry[i].peFlags = 0;
		}

		// fix up the palette to include the default GDI palette
		if ((pfd.cColorBits == 8)									&&
			(pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
			(pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
			(pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6))
		{
			DefaultPaletteEntries(pPal);
		}

		m_Palette.CreatePalette(pPal);
		delete [] pPal;

		m_pDC->SelectPalette(&m_Palette, FALSE);
		m_pDC->RealizePalette();
	}
}

void CGlView::SetOpenGLProperties()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	// Default mode
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	specularProperties[] = {0.1f, 0.1f, 0.1f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularProperties);

	SetBackColor();
	glClearDepth(1.0);

	GLfloat light0Position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);
	SetAmbientColor();
	SetDiffuseColor();
	SetSpecularColor();
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	// Default antialiasing
	if (pApp->m_bAntialias)
	{
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		if (pApp->m_nPolygonMode == 2)
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

	// Default smooth
	if (pApp->m_bSmooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	// Default lighting
	if (pApp->m_bLighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	// Default polygonmode
	switch (pApp->m_nPolygonMode)
	{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		default:
			{}
	}
}

void CGlView::SetBackColor()
{
	glClearColor(m_ColorBackRed, m_ColorBackGreen, m_ColorBackBlue, 1.0f);
}

void CGlView::SetAmbientColor()
{
	GLfloat	ambientLight[]  = {m_ColorLightAmbientRed, m_ColorLightAmbientGreen, m_ColorLightAmbientBlue, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
}

void CGlView::SetDiffuseColor()
{
	GLfloat	diffuseLight[]  = {m_ColorLightDiffuseRed, m_ColorLightDiffuseGreen, m_ColorLightDiffuseBlue, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
}

void CGlView::SetSpecularColor()
{
	GLfloat	specularLight[] = {m_ColorLightSpecularRed, m_ColorLightSpecularGreen, m_ColorLightSpecularBlue, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

BOOL CGlView::MakeActive()
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

void CGlView::DrawBufferOrStencil(GLenum mode)
{
	PIXELFORMATDESCRIPTOR pfd;

	int nGLPixelIndex = GetPixelFormat(m_pDC->GetSafeHdc());
	DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// This is just a workaround. It happens that we get ugly artefacts
	//	when three conditions meet:
	//	(a) a generic pixel format is used
	//	(b) hidden lines are removed/black lines are drawn on the surface
	//		using the stencil buffer
	//	(c) the glDrawBuffer(GL_NONE) is used to get some string metrics.
	//
	//	If a stencil buffer is availible we use it to get the text metrics
	//	as well. Else we use the glDrawBuffer method.
	if (pfd.cStencilBits != 0)
	{
		if (mode == GL_NONE)
		{
			glEnable(GL_STENCIL_TEST);
			glClear(GL_STENCIL_BUFFER_BIT);

			glStencilFunc(GL_NEVER, 0, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}
	else
	{
		glDrawBuffer(mode);
	}
}

/////////////////////////////////////////////////////////////////////////////
// SetPixelFormat

int CGlView::ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd, DWORD dwRemove)
{
	PIXELFORMATDESCRIPTOR pfd;
	int nSuccess, nPixelIndex = 0, nBestPixelIndex = -1, nBestNotMatchedFlag = 0x7FFFFFFF, nNotMatchedFlag;
	const int 
		nPixelTypeFailed	= 0x40000000,			// 1 bit reserved
		nColorBitsFailed	= 0x02000000,			// 5 bits reserved
		nAccelTypeBad		= 0x00100000,			// 2 bits reserved
		nStencilBitsFailed	= 0x00040000,			// 5 bits reserved
		nDepthBitsFailed	= 0x00002000,			// 5 bits reserved
		nAlphaBitsFailed	= 0x00000400,			// 3 bits reserved
		nAccumBitsFailed	= 0x00000010,			// 6 bits reserved
		nAuxBuffersFailed	= 0x00000002,			// 3 bits reserved
		nLayerTypeFailed	= 0x00000001;			// 1 bit reserved

	// Check for correct initialization
	if (ppfd->nSize != sizeof(PIXELFORMATDESCRIPTOR) || ppfd->nVersion != 1)
	{
		SetLastError(ERROR_SUCCESS);
		return 0;
	}

	// Iterate over all availible pixel formats
	while ((nSuccess = DescribePixelFormat(hdc, ++nPixelIndex, 
				sizeof(PIXELFORMATDESCRIPTOR), &pfd)) != 0)
	{
		// take only pixel formats into account which satisfy the flags requested/not requested
		ASSERT(!(ppfd->dwFlags & PFD_DOUBLEBUFFER) || !(dwRemove & PFD_DOUBLEBUFFER));
		if (!(ppfd->dwFlags & PFD_DOUBLEBUFFER_DONTCARE))
		{
			if ((ppfd->dwFlags & PFD_DOUBLEBUFFER) && !(pfd.dwFlags & PFD_DOUBLEBUFFER))
				continue;
			else if ((dwRemove & PFD_DOUBLEBUFFER) && (pfd.dwFlags & PFD_DOUBLEBUFFER))
				continue;
		}

		ASSERT(!(ppfd->dwFlags & PFD_STEREO) || !(dwRemove & PFD_STEREO));
		if (!(ppfd->dwFlags & PFD_STEREO_DONTCARE))
		{
			if ((ppfd->dwFlags & PFD_STEREO) && !(pfd.dwFlags & PFD_STEREO))
				continue;
			else if ((dwRemove & PFD_STEREO) && (pfd.dwFlags & PFD_STEREO))
				continue;
		}

		ASSERT(!(ppfd->dwFlags & PFD_DRAW_TO_WINDOW) || !(dwRemove & PFD_DRAW_TO_WINDOW));
		if ((ppfd->dwFlags & PFD_DRAW_TO_WINDOW) && !(pfd.dwFlags & PFD_DRAW_TO_WINDOW))
			continue;
		else if ((dwRemove & PFD_DRAW_TO_WINDOW) && (pfd.dwFlags & PFD_DRAW_TO_WINDOW))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_DRAW_TO_BITMAP) || !(dwRemove & PFD_DRAW_TO_BITMAP));
		if ((ppfd->dwFlags & PFD_DRAW_TO_BITMAP) && !(pfd.dwFlags & PFD_DRAW_TO_BITMAP))
			continue;
		else if ((dwRemove & PFD_DRAW_TO_BITMAP) && (pfd.dwFlags & PFD_DRAW_TO_BITMAP))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_SUPPORT_GDI) || !(dwRemove & PFD_SUPPORT_GDI));
		if ((ppfd->dwFlags & PFD_SUPPORT_GDI) && !(pfd.dwFlags & PFD_SUPPORT_GDI))
			continue;
		else if ((dwRemove & PFD_SUPPORT_GDI) && (pfd.dwFlags & PFD_SUPPORT_GDI))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_SUPPORT_OPENGL) || !(dwRemove & PFD_SUPPORT_OPENGL));
		if ((ppfd->dwFlags & PFD_SUPPORT_OPENGL) && !(pfd.dwFlags & PFD_SUPPORT_OPENGL))
			continue;
		else if ((dwRemove & PFD_SUPPORT_OPENGL) && (pfd.dwFlags & PFD_SUPPORT_OPENGL))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_GENERIC_FORMAT) || !(dwRemove & PFD_GENERIC_FORMAT));
		if ((ppfd->dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_FORMAT))
			continue;
		else if ((dwRemove & PFD_GENERIC_FORMAT) && (pfd.dwFlags & PFD_GENERIC_FORMAT))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_NEED_PALETTE) || !(dwRemove & PFD_NEED_PALETTE));
		if ((ppfd->dwFlags & PFD_NEED_PALETTE) && !(pfd.dwFlags & PFD_NEED_PALETTE))
			continue;
		else if ((dwRemove & PFD_NEED_PALETTE) && (pfd.dwFlags & PFD_NEED_PALETTE))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_NEED_SYSTEM_PALETTE) || !(dwRemove & PFD_NEED_SYSTEM_PALETTE));
		if ((ppfd->dwFlags & PFD_NEED_SYSTEM_PALETTE) && !(pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE))
			continue;
		else if ((dwRemove & PFD_NEED_SYSTEM_PALETTE) && (pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_SWAP_EXCHANGE) || !(dwRemove & PFD_SWAP_EXCHANGE));
		if ((ppfd->dwFlags & PFD_SWAP_EXCHANGE) && !(pfd.dwFlags & PFD_SWAP_EXCHANGE))
			continue;
		else if ((dwRemove & PFD_SWAP_EXCHANGE) && (pfd.dwFlags & PFD_SWAP_EXCHANGE))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_SWAP_COPY) || !(dwRemove & PFD_SWAP_COPY));
		if ((ppfd->dwFlags & PFD_SWAP_COPY) && !(pfd.dwFlags & PFD_SWAP_COPY))
			continue;
		else if ((dwRemove & PFD_SWAP_COPY) && (pfd.dwFlags & PFD_SWAP_COPY))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_SWAP_LAYER_BUFFERS) || !(dwRemove & PFD_SWAP_LAYER_BUFFERS));
		if ((ppfd->dwFlags & PFD_SWAP_LAYER_BUFFERS) && !(pfd.dwFlags & PFD_SWAP_LAYER_BUFFERS))
			continue;
		else if ((dwRemove & PFD_SWAP_LAYER_BUFFERS) && (pfd.dwFlags & PFD_SWAP_LAYER_BUFFERS))
			continue;

		ASSERT(!(ppfd->dwFlags & PFD_GENERIC_ACCELERATED) || !(dwRemove & PFD_GENERIC_ACCELERATED));
		if ((ppfd->dwFlags & PFD_GENERIC_ACCELERATED) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
			continue;
		else if ((dwRemove & PFD_GENERIC_ACCELERATED) && (pfd.dwFlags & PFD_GENERIC_ACCELERATED))
			continue;

#if _MSC_VER >= 1200
		ASSERT(!(ppfd->dwFlags & PFD_SUPPORT_DIRECTDRAW) || !(dwRemove & PFD_SUPPORT_DIRECTDRAW));
		if ((ppfd->dwFlags & PFD_SUPPORT_DIRECTDRAW) && !(pfd.dwFlags & PFD_SUPPORT_DIRECTDRAW))
			continue;
		else if ((dwRemove & PFD_SUPPORT_DIRECTDRAW) && (pfd.dwFlags & PFD_SUPPORT_DIRECTDRAW))
			continue;
#endif // _MSC_VER >= 1200

		// evaluate the other properties
		nNotMatchedFlag = 0;

		if (ppfd->iPixelType != pfd.iPixelType)
			nNotMatchedFlag |= nPixelTypeFailed;

		// ICD allowed, ...
		if (!(ppfd->dwFlags & PFD_GENERIC_FORMAT))
		{
			// ... but ICD not obtained
			if (pfd.dwFlags & PFD_GENERIC_FORMAT)
			{
				// Software obtained
				if ((pfd.dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
				{
					nNotMatchedFlag |= nAccelTypeBad * 2;
				}
				// MCD obtained
				else if ((pfd.dwFlags & PFD_GENERIC_FORMAT) && (pfd.dwFlags & PFD_GENERIC_ACCELERATED))
				{
					nNotMatchedFlag |= nAccelTypeBad;
				}
			}
		}
		// ICD not allowed, but MCD allowed, ...
		else if (!(dwRemove & PFD_GENERIC_ACCELERATED))
		{
			// ... but MCD not obtained
			if (!(pfd.dwFlags & PFD_GENERIC_FORMAT) || 
				!(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
			{
				// ICD obtained
				if (!(pfd.dwFlags & PFD_GENERIC_FORMAT))
				{
					nNotMatchedFlag |= nAccelTypeBad * 2;
				}
				// Software obtained
				else if ((pfd.dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
				{
					nNotMatchedFlag |= nAccelTypeBad;
				}
			}
		}
		// ICD not allowed and MCD not allowed, ...
		else
		{
			// ... but Software not obtained
			if ((!(pfd.dwFlags & PFD_GENERIC_FORMAT) || 
				(pfd.dwFlags & PFD_GENERIC_ACCELERATED)))
			{
				// ICD obtained
				if (!(pfd.dwFlags & PFD_GENERIC_FORMAT))
				{
					nNotMatchedFlag |= nAccelTypeBad * 2;
				}
				// MCD obtained
				else if ((pfd.dwFlags & PFD_GENERIC_FORMAT) && (pfd.dwFlags & PFD_GENERIC_ACCELERATED))
				{
					nNotMatchedFlag |= nAccelTypeBad;
				}
			}
		}

		if (ppfd->cColorBits < pfd.cColorBits)
			nNotMatchedFlag |= nColorBitsFailed * (32-pfd.cColorBits);
		else if (ppfd->cColorBits > pfd.cColorBits)
			nNotMatchedFlag |= nColorBitsFailed * (32-pfd.cColorBits-1);

		if (ppfd->cAlphaBits < pfd.cAlphaBits)
			nNotMatchedFlag |= nAlphaBitsFailed * (8-pfd.cAlphaBits);
		else if (ppfd->cAlphaBits > pfd.cAlphaBits)
			nNotMatchedFlag |= nAlphaBitsFailed * (8-pfd.cAlphaBits-1);

		if (ppfd->cAccumBits < pfd.cAccumBits)
			nNotMatchedFlag |= nAccumBitsFailed * (64-pfd.cAccumBits);
		else if (ppfd->cAccumBits > pfd.cAccumBits)
			nNotMatchedFlag |= nAccumBitsFailed * (64-pfd.cAccumBits-1);

		if (ppfd->cDepthBits < pfd.cDepthBits)
			nNotMatchedFlag |= nDepthBitsFailed * (32-pfd.cDepthBits);
		else if (ppfd->cDepthBits > pfd.cDepthBits)
			nNotMatchedFlag |= nDepthBitsFailed * (32-pfd.cDepthBits-1);

		if (ppfd->cStencilBits < pfd.cStencilBits)
			nNotMatchedFlag |= nStencilBitsFailed * (32-pfd.cStencilBits);
		else if (ppfd->cStencilBits > pfd.cStencilBits)
			nNotMatchedFlag |= nStencilBitsFailed * (32-pfd.cStencilBits-1);

		if (ppfd->cAuxBuffers < pfd.cAuxBuffers)
			nNotMatchedFlag |= nAuxBuffersFailed * (8-pfd.cAuxBuffers);
		else if (ppfd->cAuxBuffers > pfd.cAuxBuffers)
			nNotMatchedFlag |= nAuxBuffersFailed * (8-pfd.cAuxBuffers-1);

		if (ppfd->iLayerType != pfd.iLayerType)
			nNotMatchedFlag |= nLayerTypeFailed;

		// choose the one with the best evaluation result
		if (nNotMatchedFlag < nBestNotMatchedFlag)
		{
			nBestPixelIndex = nPixelIndex;
			nBestNotMatchedFlag = nNotMatchedFlag;
		}
	}

	if (nBestPixelIndex == -1)
	{
		// no appropriate pixel format found, so let the API retry it
		return ::ChoosePixelFormat(hdc, ppfd);
	}
	else
	{
		// check for correct color depth
		DescribePixelFormat(hdc, nBestPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		if (pfd.cColorBits != GetDeviceCaps(hdc, PLANES) * GetDeviceCaps(hdc, BITSPIXEL))
			TRACE0("Warning: The requested pixel format does not satisfy the color depth of the device context.\n");

		// one based
		return nBestPixelIndex;
	}
}

BOOL CGlView::SetWindowPixelFormat(BOOL bUseAPI)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	BYTE cColorBits = BYTE(GetDeviceCaps(m_pDC->GetSafeHdc(), PLANES) * GetDeviceCaps(m_pDC->GetSafeHdc(), BITSPIXEL));
	DWORD dwRemove = 0;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure
		PFD_DRAW_TO_WINDOW |			// Draw to window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER |   			// Double buffered mode
		PFD_STEREO_DONTCARE,			// Don't need stereo mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		cColorBits,						// Number of color bitplanes
		0,0,0,0,0,0,					// Not used to select mode
		0,								// Number of alpha bitplanes
		0,								// Not used to select mode
		64,								// Number of bitplanes in the accumulation buffer
		0,0,0,0,						// Not used to select mode
		32,								// Size of depth buffer
		8,								// Size of stencil buffer
		0,								// Size of auxiliary buffer
		PFD_MAIN_PLANE,					// Draw in main plane
		0,0,0,0 };						// Not used to select mode

	if (pApp->m_bNoHardwareAcceleration)
	{
		pfd.dwFlags |= PFD_GENERIC_FORMAT;
		dwRemove |= PFD_GENERIC_ACCELERATED;
	}

	int nGLPixelIndex = bUseAPI ? 
		::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd) : 
		ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd, dwRemove);
	if (nGLPixelIndex == 0) // Choose default
	{
		int nSuccess;

		// first we try to get a double buffered format
		while ((nSuccess = DescribePixelFormat(m_pDC->GetSafeHdc(), ++nGLPixelIndex, 
					sizeof(PIXELFORMATDESCRIPTOR), &pfd)) != 0)
		{
			if (pfd.dwFlags & PFD_DOUBLEBUFFER)
				break;
		}

		// now we take even a single buffered format
		if (nSuccess == 0)
			nGLPixelIndex = 1;
	}

	if (DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, 
		sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
		return FALSE;

	if (!SetPixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, &pfd))
		return FALSE;

	return TRUE;
}

BOOL CGlView::SetMemDcPixelFormat(HDC hMemDC, BOOL bUseAPI)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure
		PFD_DRAW_TO_BITMAP |			// Draw to bitmap (not to window)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_STEREO_DONTCARE,			// Don't need stereo mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		24,								// Number of color bitplanes
		0,0,0,0,0,0,					// Not used to select mode
		0,								// Number of alpha bitplanes
		0,								// Not used to select mode
		64,								// Number of bitplanes in the accumulation buffer
		0,0,0,0,						// Not used to select mode
		32,								// Size of depth buffer
		8,								// Size of stencil buffer
		0,								// Size of auxiliary buffer
		PFD_MAIN_PLANE,					// Draw in main plane
		0,0,0,0 };						// Not used to select mode

	int nGLPixelIndex = bUseAPI ? 
		::ChoosePixelFormat(hMemDC, &pfd) : 
		ChoosePixelFormat(hMemDC, &pfd);
	if (nGLPixelIndex == 0) // Choose default
		nGLPixelIndex = 1;

	if (DescribePixelFormat(hMemDC, nGLPixelIndex, 
		sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
		return FALSE;

	if (!SetPixelFormat(hMemDC, nGLPixelIndex, &pfd))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CreateViewGLContext

BOOL CGlView::CreateViewGLContext()
{
	// It may happen that CreateViewGLContext is
	// called recursively:
	//
	// When in MakeActive the SetCurrent failes 
	// then InitializeOpenGL may be called again which
	// calles CreateViewGLContext a second time.
	//
	// To avoid an infinite recursion we check
	// m_hRC. If m_hRC is not NULL, then we know that
	// CreateViewGLContext was called recursively
	// in the way described above.
	// Then we delete the existing rendering context 
	// before creating a new one (to avoid resource leaks)
	// and do NOT call MakeActive again.

	BOOL bRecursivelyCalled = (m_hRC != NULL);
	if (bRecursivelyCalled)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());

	if (m_hRC == NULL)
		return FALSE;

	if (bRecursivelyCalled)
		return TRUE;

	return MakeActive();
}

void CGlView::AddToZoomHistory()
{
	mymathfuncs::CMatrix tmpMatrix;
	tmpMatrix.SetSize(4);
	tmpMatrix[0] = m_xScaling;
	tmpMatrix[1] = m_yScaling;
	tmpMatrix[2] = m_xTranslation;
	tmpMatrix[3] = m_yTranslation;

	m_ZoomHistory.Add(tmpMatrix);
}

BOOL CGlView::GetFromZoomHistory()
{
	if (m_ZoomHistory.GetSize() == 0)
		return FALSE;	// Nothing to do

	int tmpUpperBound = m_ZoomHistory.GetUpperBound();
	mymathfuncs::CMatrix& tmpMatrix = m_ZoomHistory.ElementAt(tmpUpperBound);

	m_xScaling = tmpMatrix[0];
	m_yScaling = tmpMatrix[1];
	m_xTranslation = tmpMatrix[2];
	m_yTranslation = tmpMatrix[3];

	UpdateSliderScalePos();
	UpdateSliderTranslationPos();

	m_ZoomHistory.RemoveAt(tmpUpperBound);
	m_ZoomHistory.SetSize(tmpUpperBound);

	InvalidateRect(NULL, FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Drawing stuff
/////////////////////////////////////////////////////////////////////////////

void CGlView::OnEditUndo() 
{
	GetFromZoomHistory();
}

void CGlView::OnEditCopy()
{
	CRect	mRect;			// Position of this window's client area

	BeginWaitCursor();

	// Get the Views size (client area!)
	GetClientRect(&mRect);

	SetRedraw(FALSE);
	int nReduceResCount = 0;
	while (!EditCopy(mRect, nReduceResCount++))
	{
		// retry again with reduced resolution
		if (nReduceResCount >= 10)
			break;
	}
	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);

	EndWaitCursor();
}

BOOL CGlView::EditCopy(CRect mRect, int nReduceResCount)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	RECT	rect;
	HDC		hDC, hMemDC, hTmpDC;
	HGLRC	hRC, hMemRC;

	BITMAPINFO	bitmapInfo;
	HBITMAP		hDib;
	LPVOID		pBitmapBits;
	double		fac = pApp->m_nCopyResolution/72.;		// 72 DPI (screen) --> <user selected> DPI (clipboard)
	int			nXRes, nYRes;
	BOOL		bSuccess = FALSE;

	for (int k = nReduceResCount; k > 0; k--)
		fac /= 2.;

	rect = mRect;
	ASSERT(rect.left == 0);
	ASSERT(rect.top == 0);
	rect.right = int(rect.right*fac);
	rect.bottom = int(rect.bottom*fac);

	if (mRect.Width() == 0 || mRect.Height() == 0)
	{
		// Get the Views size (client area!)
		GetClientRect(&mRect);
	}

	nXRes = rect.right;
	nYRes = rect.bottom;
	ScaleFont(fac);

	//nXRes = (nXRes + (sizeof(DWORD)-1)) & ~(sizeof(DWORD)-1);	// aligning width to 4 bytes (sizeof(DWORD)) avoids 
	nXRes = nXRes & ~(sizeof(DWORD)-1);							// aligning width to 4 bytes (sizeof(DWORD)) avoids 
																// pixel garbage at the upper line

	// First of all, initialize the bitmap header information...
	memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth		= nXRes;
	bitmapInfo.bmiHeader.biHeight		= nYRes;
	bitmapInfo.bmiHeader.biPlanes		= 1;
	bitmapInfo.bmiHeader.biBitCount		= 24;
	bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * 3;
	bitmapInfo.bmiHeader.biXPelsPerMeter = int(72.*fac/.0254);	// 72*fac DPI
	bitmapInfo.bmiHeader.biYPelsPerMeter = int(72.*fac/.0254);	// 72*fac DPI

	// create DIB
	hTmpDC = ::GetDC(m_hWnd);
	hDib = CreateDIBSection(hTmpDC, &bitmapInfo, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(m_hWnd, hTmpDC);

	// create memory device context
	if ((hMemDC = CreateCompatibleDC(m_pDC == NULL ? NULL : m_pDC->GetSafeHdc())) == NULL)
	{
		DeleteObject(hDib);
		return FALSE;
	}
	HGDIOBJ hOldDib = SelectObject(hMemDC, hDib);

	// setup pixel format
	if (!SetMemDcPixelFormat(hMemDC) && !SetMemDcPixelFormat(hMemDC, TRUE))
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return FALSE;
	}

	// create memory rendering context
	if ((hMemRC = wglCreateContext(hMemDC)) == NULL)
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return FALSE;
	}

	// Store current rendering and device contexts
	GetCurrent(hDC, hRC);

	// Make this hMemRC the current OpenGL rendering context.
	SetCurrent(hMemDC, hMemRC);

	SetOpenGLProperties();

	glViewport(0, 0, nXRes, nYRes);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	// must be created once for hMemDC
	CreateFontBitmaps();

	CDC *pDummyDC = GetDC();
	pDummyDC->m_bPrinting = TRUE;	// this does the trick in OnDraw: it prevents changing rendering context and swapping buffers
	OnDraw(pDummyDC);
	ReleaseDC(pDummyDC);
	glFinish();	// Finish all OpenGL commands

	// the rendering context will be no longer needed
	SetCurrent(NULL, NULL);
	wglDeleteContext(hMemRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	// Restore the Views original font size
	UnScaleFont();
	CreateFontBitmaps();

	if (OpenClipboard())
	{
		HGLOBAL hClipboardCopy = ::GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(BITMAPINFOHEADER) + bitmapInfo.bmiHeader.biSizeImage);
		if (hClipboardCopy != NULL)
		{
			LPVOID lpClipboardCopy, lpBitmapBitsOffset;
			lpClipboardCopy = ::GlobalLock((HGLOBAL) hClipboardCopy);
			lpBitmapBitsOffset = (LPVOID)((BYTE*)lpClipboardCopy + sizeof(BITMAPINFOHEADER));

			memcpy(lpClipboardCopy, &bitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
			memcpy(lpBitmapBitsOffset, pBitmapBits, bitmapInfo.bmiHeader.biSizeImage);
			::GlobalUnlock(hClipboardCopy);

			EmptyClipboard();

			if (SetClipboardData(CF_DIB, hClipboardCopy) != NULL)
			{
				bSuccess = TRUE;
				if (nReduceResCount > 0)
					pApp->SetStatusBarInfo(POLICY_RESOLUTION_REDUCED, (int)::round(fac*72.));
			}
			else
			{
				GlobalFree(hClipboardCopy);
			}
			CloseClipboard();
		}
	}
	if (hOldDib != NULL)
		SelectObject(hMemDC, hOldDib);

	// Delete our DIB and device context
	DeleteObject(hDib);
	DeleteDC(hMemDC);

	return bSuccess;
}

void CGlView::OnEditFont()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	LOGFONT logFont = m_LogFont;
	CFontDialog dlg(&logFont);
	dlg.m_cf.rgbColors = RGB(int(::round(m_ColorFontRed*255.0f)),int(::round(m_ColorFontGreen*255.0f)),int(::round(m_ColorFontBlue*255.0f)));
	if (dlg.DoModal() == IDOK)
	{
		pApp->m_OptionColorFont = dlg.GetColor();
		pApp->m_LogFont = logFont;
		if (pApp->m_bPropPolicy)
		{
			CDocTemplate* pDocTemplate = GetDocument()->GetDocTemplate();
			if (pDocTemplate)
			{
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
							((CGlView *)pView)->OptionsFont();
						}
					}
				}
			}
		}
		else
			OptionsFont();

		CheckColors(pApp->m_OptionColorBack, pApp->m_OptionColorFont);
	}
}

void CGlView::OptionsFont()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	HDC hDC = NULL;
	HGLRC hRC = NULL;
	HGDIOBJ hOldObject;
	LOGFONT logFont;

	m_ColorFontRed   = (float)GetRValue(pApp->m_OptionColorFont) / 255.0f;
	m_ColorFontGreen = (float)GetGValue(pApp->m_OptionColorFont) / 255.0f;
	m_ColorFontBlue  = (float)GetBValue(pApp->m_OptionColorFont) / 255.0f;
	CheckColors(m_ColorBackRed, m_ColorBackGreen, m_ColorBackBlue, m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

	logFont = m_LogFont = pApp->m_LogFont;
	logFont.lfHeight = int(m_dFontRatio*m_LogFont.lfHeight);
	logFont.lfWidth = int(m_dFontRatio*m_LogFont.lfWidth);
	hOldObject = m_FontSmall.Detach();
	if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
		m_FontSmall.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);
	hOldObject = m_FontNormal.Detach();
	if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&m_LogFont)) == FALSE)
		m_FontNormal.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);

	// store height/width
	m_nLogFontHeight = m_LogFont.lfHeight;
	m_nLogFontWidth = m_LogFont.lfWidth;

	GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	MakeActive();				//Make view's rendering context current
	CreateFontBitmaps();

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	InvalidateRect(NULL, FALSE);
	UpdateFontColorRect();
}

BOOL CGlView::CheckColors(const COLORREF& OptionColorBack, COLORREF& OptionColorFore)
{
	const float ColorBackRed   = (float)GetRValue(OptionColorBack) / 255.0f;
	const float ColorBackGreen = (float)GetGValue(OptionColorBack) / 255.0f;
	const float ColorBackBlue  = (float)GetBValue(OptionColorBack) / 255.0f;

	float ColorForeRed   = (float)GetRValue(OptionColorFore) / 255.0f;
	float ColorForeGreen = (float)GetGValue(OptionColorFore) / 255.0f;
	float ColorForeBlue  = (float)GetBValue(OptionColorFore) / 255.0f;

	if (!CheckColors(ColorBackRed, ColorBackGreen, ColorBackBlue, ColorForeRed, ColorForeGreen, ColorForeBlue))
	{

		OptionColorFore = RGB(int(::round(ColorForeRed*255.0f)),int(::round(ColorForeGreen*255.0f)),int(::round(ColorForeBlue*255.0f)));

		return FALSE;	// color had to be changed
	}

	return TRUE;		// nothing to be changed
}

BOOL CGlView::CheckColors(const float& ColorBackRed, const float& ColorBackGreen, const float& ColorBackBlue, float& ColorForeRed, float& ColorForeGreen, float& ColorForeBlue)
{
	if ((ColorBackRed-ColorForeRed)*(ColorBackRed-ColorForeRed)+(ColorBackGreen-ColorForeGreen)*(ColorBackGreen-ColorForeGreen)+(ColorBackBlue-ColorForeBlue)*(ColorBackBlue-ColorForeBlue) < .12)
	{
		ColorForeRed = (float)::round(1.0-ColorBackRed);
		ColorForeGreen = (float)::round(1.0-ColorBackGreen);
		ColorForeBlue = (float)::round(min((ColorBackRed+ColorBackGreen)/2.,1.-(ColorBackRed+ColorBackGreen)/2.));

		return FALSE;	// color had to be changed
	}

	return TRUE;		// nothing to be changed
}

void CGlView::ScaleFont(double scaleFac)
{
	LOGFONT logFont;
	HGDIOBJ hOldObject;

	if (m_FontSmall.GetLogFont(&logFont))
	{
		logFont.lfHeight = int(m_dFontRatio*scaleFac*m_LogFont.lfHeight);
		logFont.lfWidth = int(m_dFontRatio*scaleFac*m_LogFont.lfWidth);
		hOldObject = m_FontSmall.Detach();
		if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
			m_FontSmall.Attach(hOldObject);
		else if (hOldObject != NULL)
			DeleteObject(hOldObject);
	}
	if (m_FontNormal.GetLogFont(&logFont))
	{
		logFont.lfHeight = int(scaleFac*m_LogFont.lfHeight);
		logFont.lfWidth = int(scaleFac*m_LogFont.lfWidth);
		hOldObject = m_FontNormal.Detach();
		if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&logFont)) == FALSE)
			m_FontNormal.Attach(hOldObject);
		else if (hOldObject != NULL)
			DeleteObject(hOldObject);
	}

	// modify height/width
	m_LogFont.lfHeight = int(scaleFac*m_LogFont.lfHeight);
	m_LogFont.lfWidth = int(scaleFac*m_LogFont.lfWidth);
}

void CGlView::UnScaleFont()
{
	LOGFONT logFont = m_LogFont;
	HGDIOBJ hOldObject;

	// restore height/width
	m_LogFont.lfHeight = m_nLogFontHeight;
	m_LogFont.lfWidth = m_nLogFontWidth;

	logFont.lfHeight = int(m_dFontRatio*m_LogFont.lfHeight);
	logFont.lfWidth = int(m_dFontRatio*m_LogFont.lfWidth);
	hOldObject = m_FontSmall.Detach();
	if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
		m_FontSmall.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);
	hOldObject = m_FontNormal.Detach();
	if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&m_LogFont)) == FALSE)
		m_FontNormal.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);
}

void CGlView::CreateFontBitmaps()
{
	HGDIOBJ hOldFont = NULL;
	int retry;

	if (m_bFontCreateFlagSmall)
		hOldFont = SelectObject(m_pDC->GetSafeHdc(), m_FontSmall);
	else
		hOldFont = SelectObject(m_pDC->GetSafeHdc(), GetStockObject(SYSTEM_FONT));

	// the display list numbering starts at m_nListBaseSmall, an arbitrary choice 
	for (retry = 0; retry < 10; retry++)
	{
		// this loop is due to a NT bug while printing
		// it seems wglUseFontBitmaps returns FALSE the first time
		// try it at most 10 times
		if (wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, m_nListBaseSmall))
			break;
	}

	if (m_bFontCreateFlagNormal)
		SelectObject(m_pDC->GetSafeHdc(), m_FontNormal);
	else
		SelectObject(m_pDC->GetSafeHdc(), GetStockObject(SYSTEM_FONT));

	// the display list numbering starts at m_nListBaseNormal, an arbitrary choice 
	for (retry = 0; retry < 10; retry++)
	{
		// this loop is due to a NT bug while printing
		// it seems wglUseFontBitmaps returns FALSE the first time
		// try it at most 10 times
		if (wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, m_nListBaseNormal))
			break;
	}

	if (hOldFont != NULL)
		SelectObject(m_pDC->GetSafeHdc(), hOldFont);
}

void CGlView::DrawZoomRect()
{
	GLfloat tmpLineWidth[1];
	glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
	glLineWidth(1.5);

	GLushort lineStipple, pattern;
	GLint repeat;
	glEnable(GL_LINE_STIPPLE);
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);

	lineStipple = 0x087f;
	glLineStipple(1, lineStipple);

	glColor3d(::round(static_cast<double>(1.0-m_ColorBackRed)), ::round(1.0-m_ColorBackGreen), ::round(min((m_ColorBackRed+m_ColorBackGreen)/2.,1.-(m_ColorBackRed+m_ColorBackGreen)/2.)));
	glBegin(GL_LINE_LOOP);
	glVertex2d(m_StartPoint[0], m_StartPoint[1]);
	glVertex2d(m_EndPoint[0], m_StartPoint[1]);
	glVertex2d(m_EndPoint[0], m_EndPoint[1]);
	glVertex2d(m_StartPoint[0], m_EndPoint[1]);
	glEnd();

	glLineStipple(repeat, pattern);

	glLineWidth(tmpLineWidth[0]);
}

void CGlView::DrawRubberBand()
{
	GLfloat tmpLineWidth[1];
	glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
	glLineWidth(1.5);

	GLushort lineStipple, pattern;
	GLint repeat;
	glEnable(GL_LINE_STIPPLE);
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);

	lineStipple = 0x087f;
	glLineStipple(1, lineStipple);

	glColor3d(::round(1.0-m_ColorBackRed), ::round(1.0-m_ColorBackGreen), ::round(min((m_ColorBackRed+m_ColorBackGreen)/2.,1.-(m_ColorBackRed+m_ColorBackGreen)/2.)));
	glBegin(GL_LINE_STRIP);
	glVertex2d(m_StartPoint[0], m_StartPoint[1]);
	glVertex2d(m_EndPoint[0], m_EndPoint[1]);
	glEnd();

	glLineStipple(repeat, pattern);

	glBegin(GL_LINE_LOOP);
	glVertex2d(m_StartPoint[0]-.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_StartPoint[1]-.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
	glVertex2d(m_StartPoint[0]-.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_StartPoint[1]+.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
	glVertex2d(m_StartPoint[0]+.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_StartPoint[1]+.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
	glVertex2d(m_StartPoint[0]+.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_StartPoint[1]-.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
	glEnd();
	if (m_bFirstPickPoint)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(m_EndPoint[0], m_EndPoint[1]-.071/((m_yScaling==0.0) ? 1.0 : m_yScaling));
		glVertex2d(m_EndPoint[0]-.071/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_EndPoint[1]);
		glVertex2d(m_EndPoint[0], m_EndPoint[1]+.071/((m_yScaling==0.0) ? 1.0 : m_yScaling));
		glVertex2d(m_EndPoint[0]+.071/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_EndPoint[1]);
		glEnd();
	}
	if (m_bSecondPickPoint)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2d(m_EndPoint[0]-.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_EndPoint[1]-.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
		glVertex2d(m_EndPoint[0]-.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_EndPoint[1]+.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
		glVertex2d(m_EndPoint[0]+.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_EndPoint[1]+.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
		glVertex2d(m_EndPoint[0]+.05/((m_xScaling==0.0) ? 1.0 : m_xScaling), m_EndPoint[1]-.05/((m_yScaling==0.0) ? 1.0 : m_yScaling));
		glEnd();
	}

	glLineWidth(tmpLineWidth[0]);
}

void CGlView::DrawHorizontalBands()
{
	GLfloat tmpLineWidth[1];
	glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
	glLineWidth(1.5);

	GLushort lineStipple, pattern;
	GLint repeat;
	glEnable(GL_LINE_STIPPLE);
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);

	lineStipple = 0x087f;
	glLineStipple(1, lineStipple);

	glColor3d(::round(1.0-m_ColorBackRed), ::round(1.0-m_ColorBackGreen), ::round(min((m_ColorBackRed+m_ColorBackGreen)/2.,1.-(m_ColorBackRed+m_ColorBackGreen)/2.)));
	glBegin(GL_LINE_LOOP);
	glVertex2d(-1., m_StartPoint[1]);
	glVertex2d(1., m_StartPoint[1]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex2d(-1., m_EndPoint[1]);
	glVertex2d(1., m_EndPoint[1]);
	glEnd();

	glLineStipple(repeat, pattern);

	glLineWidth(tmpLineWidth[0]);
}

void CGlView::DrawVerticalBands()
{
	GLfloat tmpLineWidth[1];
	glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
	glLineWidth(1.5);

	GLushort lineStipple, pattern;
	GLint repeat;
	glEnable(GL_LINE_STIPPLE);
	glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
	glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);

	lineStipple = 0x087f;
	glLineStipple(1, lineStipple);

	glColor3d(::round(1.0-m_ColorBackRed), ::round(1.0-m_ColorBackGreen), ::round(min((m_ColorBackRed+m_ColorBackGreen)/2.,1.-(m_ColorBackRed+m_ColorBackGreen)/2.)));
	glBegin(GL_LINE_LOOP);
	glVertex2d(m_StartPoint[0], -1.);
	glVertex2d(m_StartPoint[0], 1.);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex2d(m_EndPoint[0], -1.);
	glVertex2d(m_EndPoint[0], 1.);
	glEnd();

	glLineStipple(repeat, pattern);

	glLineWidth(tmpLineWidth[0]);
}

/////////////////////////////////////////////////////////////////////////////
// Plot and surf

void CGlView::Plot2d(mymathfuncs::CMatrix* pXX, mymathfuncs::CMatrix* pYY, mymathfuncs::CMatrix* pCC)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pCC);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix& matX, CMatrix& matY, CMatrix& matC)					// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matC);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix* pXX, CMatrix* pYY)									// Aufruf mit 2 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pYY);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix& matX, CMatrix& matY)									// Aufruf mit 2 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matY);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix* pYY)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;

	if (pYY == NULL)
		return;

	matX.SetSize(pYY->GetSize());

	int sizeX = pYY->GetSize().cx;
	int sizeY = pYY->GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
	{
		for (int j = 1; j <= sizeY; j++)
		{
			matX(i,j) = (2.*double(i) - (sizeX + 1.));
			if (sizeX > 1)
				matX(i,j) /= (sizeX - 1.);
		}
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pYY);

	RenderScenePlot2d();
}

void CGlView::Plot2d(CMatrix& matY)													// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;

	matX.SetSize(matY.GetSize());

	int sizeX = matY.GetSize().cx;
	int sizeY = matY.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
	{
		for (int j = 1; j <= sizeY; j++)
		{
			matX(i,j) = (2.*double(i) - (sizeX + 1.));
			if (sizeX > 1)
				matX(i,j) /= (sizeX - 1.);
		}
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matY);

	RenderScenePlot2d();
}

void CGlView::Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC)		// Aufruf mit 4 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pCC);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC)	// Aufruf mit 4 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matC);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ)					// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix* pZZ)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	if (pZZ == NULL)
		return;

	int sizeY = pZZ->GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (sizeY + 1.));
		if (sizeY > 1)
			matX(1,j) /= (sizeY - 1.);
	}

	int sizeX = pZZ->GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (sizeX + 1.));
		if (sizeX > 1)
			matY(i,1) /= (sizeX - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderScenePlot3d();
}

void CGlView::Plot3d(CMatrix& matZ)													// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	int sizeY = matZ.GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (sizeY + 1.));
		if (sizeY > 1)
			matX(1,j) /= (sizeY - 1.);
	}

	int sizeX = matZ.GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (sizeX + 1.));
		if (sizeX > 1)
			matY(i,1) /= (sizeX - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderScenePlot3d();
}

void CGlView::Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC)			// Aufruf mit 4 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pCC);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC)		// Aufruf mit 4 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matC);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ)						// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix* pZZ)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	if (pZZ == NULL)
		return;

	int sizeY = pZZ->GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (sizeY + 1.));
		if (sizeY > 1)
			matX(1,j) /= (sizeY - 1.);
	}

	int sizeX = pZZ->GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (sizeX + 1.));
		if (sizeX > 1)
			matY(i,1) /= (sizeX - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, pZZ);
	m_CRenderArray.SetAt(0, pZZ);

	RenderSceneSurf();
}

void CGlView::Surf(CMatrix& matZ)													// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	int sizeY = matZ.GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (sizeY + 1.));
		if (sizeY > 1)
			matX(1,j) /= (sizeY - 1.);
	}

	int sizeX = matZ.GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (sizeX + 1.));
		if (sizeX > 1)
			matY(i,1) /= (sizeX - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_ZRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_ZRenderArray.SetAt(0, &matZ);
	m_CRenderArray.SetAt(0, &matZ);

	RenderSceneSurf();
}

void CGlView::TopView(CMatrix* pXX, CMatrix* pYY, CMatrix* pCC)						// Aufruf mit 3 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pCC);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix& matX, CMatrix& matY, CMatrix& matC)					// Aufruf mit 3 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matC);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix* pXX, CMatrix* pYY)									// Aufruf mit 2 Pointern auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, pXX);
	m_YRenderArray.SetAt(0, pYY);
	m_CRenderArray.SetAt(0, pYY);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix& matX, CMatrix& matY)									// Aufruf mit 2 Referenzen auf CMatrix
{
	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matY);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix* pCC)													// Aufruf mit 1 Pointer auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	if (pCC == NULL)
		return;

	int sizeY = pCC->GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (sizeY + 1.));
		if (sizeY > 1)
			matX(1,j) /= (sizeY - 1.);
	}

	int sizeX = pCC->GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (sizeX + 1.));
		if (sizeX > 1)
			matY(i,1) /= (sizeX - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, pCC);

	RenderSceneTopView();
}

void CGlView::TopView(CMatrix& matC)												// Aufruf mit 1 Referenz auf CMatrix
{
	CMatrix matX;
	CMatrix matY;

	int sizeY = matC.GetSize().cy;
	matX.SetSize(CSize(1,sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		matX(1,j) = (2.*double(j) - (sizeY + 1.));
		if (sizeY > 1)
			matX(1,j) /= (sizeY - 1.);
	}

	int sizeX = matC.GetSize().cx;
	matY.SetSize(CSize(sizeX,1));
	for (int i = 1; i <= sizeX; i++)
	{
		matY(i,1) = (2.*double(i) - (sizeX + 1.));
		if (sizeX > 1)
			matY(i,1) /= (sizeX - 1.);
	}

	m_XRenderArray.SetSize(1);
	m_YRenderArray.SetSize(1);
	m_CRenderArray.SetSize(1);

	m_XRenderArray.SetAt(0, &matX);
	m_YRenderArray.SetAt(0, &matY);
	m_CRenderArray.SetAt(0, &matC);

	RenderSceneTopView();
}

void CGlView::Scale2d(CScale* pSS)													// Aufruf mit 1 Pointer auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, pSS);

	RenderSceneScale2d();
}

void CGlView::Scale2d(CScale& matS)													// Aufruf mit 1 Referenz auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, &matS);

	RenderSceneScale2d();
}

void CGlView::Scale3d(CScale* pSS)													// Aufruf mit 1 Pointer auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, pSS);

	RenderSceneScale3d();
}

void CGlView::Scale3d(CScale& matS)													// Aufruf mit 1 Referenz auf CScale
{
	m_SRenderArray.SetSize(1);

	m_SRenderArray.SetAt(0, &matS);

	RenderSceneScale3d();
}

/////////////////////////////////////////////////////////////////////////////
// Rendering

void CGlView::RenderScenePlot2d()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x, y, c;
	double r, g, b;
	double fCC;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -= .5;
		maxCC += .5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 0)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j <= sizeY; j++)
			{
				glBegin(GL_LINE_STRIP);
				for (i = 1; i <= sizeX; i++)
				{
					x = (*pXX)(bIsXXRow ? 1 : i,j);
					y = (*pYY)(i,bIsYYCol ? 1 : j);
					c = (*pCC)(i,j);

					fCC = (c-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r, g, b, k);

					glColor3d(r,g,b);
					glVertex2d(x,y);
				}
				glEnd();
			}
		}
	}
}

void CGlView::RenderScenePlot3d()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pZZ, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x, y, z, c;
	double r, g, b;
	double fCC;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_ZRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	GLdouble tmpNormal[3];
	glGetDoublev(GL_CURRENT_NORMAL, tmpNormal);

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -= .5;
		maxCC += .5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && pZZ->GetSize() == pCC->GetSize() && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 0)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// set outer normals (to the sky)
			glNormal3d(0., 1., 0.);

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j <= sizeY; j++)
			{
				glBegin(GL_LINE_STRIP);
				for (i = 1; i <= sizeX; i++)
				{
					x = (*pXX)(bIsXXRow ? 1 : i,j);
					y = (*pYY)(i,bIsYYCol ? 1 : j);
					z = (*pZZ)(i,j);
					c = (*pCC)(i,j);

					fCC = (c-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r, g, b, k);

					glColor3d(r,g,b);
					glVertex3d(x,z,-y);
				}
				glEnd();
			}
		}
	}
	glNormal3dv(tmpNormal);
}

void CGlView::RenderSceneSurf()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pZZ, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x11, x12, x21, x22;
	double y11, y12, y21, y22;
	double z11, z12, z21, z22;
	double c11, c12, c21, c22;
	double r11, g11, b11;
	double r12, g12, b12;
	double r21, g21, b21;
	double r22, g22, b22;
	double fCC;
	double xNormal, yNormal, zNormal;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_ZRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	GLdouble tmpNormal[3];
	glGetDoublev(GL_CURRENT_NORMAL, tmpNormal);

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -= .5;
		maxCC += .5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pZZ = (CMatrix*) m_ZRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pZZ == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && pZZ->GetSize() == pCC->GetSize() && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 1)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// Is surface?
			GLint tmpPolygonMode[2];
			glGetIntegerv(GL_POLYGON_MODE, tmpPolygonMode);
			GLboolean tmpLighting = glIsEnabled(GL_LIGHTING);

			if (m_bBlackLinesOrRemoveHidden)
			{
				glEnable(GL_STENCIL_TEST);
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilMask(1);
				glStencilFunc(GL_ALWAYS, 0, 1);
				glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
			}

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j < sizeY; j++)
			{
				for (i = 1; i < sizeX; i++)
				{
					x11 = (*pXX)(bIsXXRow ? 1 : i,j);
					x12 = (*pXX)(bIsXXRow ? 1 : i,j+1);
					x21 = (*pXX)(bIsXXRow ? 1 : i+1,j);
					x22 = (*pXX)(bIsXXRow ? 1 : i+1,j+1);

					y11 = (*pYY)(i,bIsYYCol ? 1 : j);
					y12 = (*pYY)(i,bIsYYCol ? 1 : j+1);
					y21 = (*pYY)(i+1,bIsYYCol ? 1 : j);
					y22 = (*pYY)(i+1,bIsYYCol ? 1 : j+1);

					z11 = (*pZZ)(i,j);
					z12 = (*pZZ)(i,j+1);
					z21 = (*pZZ)(i+1,j);
					z22 = (*pZZ)(i+1,j+1);

					c11 = (*pCC)(i,j);
					c12 = (*pCC)(i,j+1);
					c21 = (*pCC)(i+1,j);
					c22 = (*pCC)(i+1,j+1);

					fCC = (c11-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r11, g11, b11, k);

					fCC = (c12-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r12, g12, b12, k);

					fCC = (c21-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r21, g21, b21, k);

					fCC = (c22-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r22, g22, b22, k);

					// set outer normals
					crossprod(x11-x22, y11-y22, z11-z22, x12-x21, y12-y21, z12-z21, xNormal, yNormal, zNormal);
					if (zNormal < 0.)
					{
						xNormal *= -1.;
						yNormal *= -1.;
						zNormal *= -1.;
					}
					glNormal3d(xNormal, zNormal, -yNormal);

					if (m_bBlackLinesOrRemoveHidden)
					{
						// Stencil buffer operations
						if (tmpPolygonMode[1] == GL_FILL)
						{
							// Draw Black lines on the surface
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glColor3d(0.,0.,0.);
							glBegin(GL_QUADS);

							glVertex3d(x11,z11,-y11);
							glVertex3d(x12,z12,-y12);
							glVertex3d(x22,z22,-y22);
							glVertex3d(x21,z21,-y21);

							glEnd();

							glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
						}
						else if (tmpPolygonMode[1] == GL_POINT || tmpPolygonMode[1] == GL_LINE)
						{
							// Draw the points/lines themselves
							glBegin(GL_QUADS);

							glColor3d(r11,g11,b11);
							glVertex3d(x11,z11,-y11);
							glColor3d(r12,g12,b12);
							glVertex3d(x12,z12,-y12);
							glColor3d(r22,g22,b22);
							glVertex3d(x22,z22,-y22);
							glColor3d(r21,g21,b21);
							glVertex3d(x21,z21,-y21);

							glEnd();
						}
						glStencilFunc(GL_EQUAL, 0, 1);
						glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					}

					// Normal operations
					if (!m_bBlackLinesOrRemoveHidden || tmpPolygonMode[1] == GL_FILL)
					{
						// Draw the faces themselves
						glBegin(GL_QUADS);

						glColor3d(r11,g11,b11);
						glVertex3d(x11,z11,-y11);
						glColor3d(r12,g12,b12);
						glVertex3d(x12,z12,-y12);
						glColor3d(r22,g22,b22);
						glVertex3d(x22,z22,-y22);
						glColor3d(r21,g21,b21);
						glVertex3d(x21,z21,-y21);

						glEnd();
					}
					else if (tmpPolygonMode[1] == GL_POINT || tmpPolygonMode[1] == GL_LINE)
					{
						// Draw background color to inner face
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						if (tmpLighting)
							glDisable(GL_LIGHTING);

						glColor3d(m_ColorBackRed,m_ColorBackGreen,m_ColorBackBlue);
						glBegin(GL_QUADS);

						glVertex3d(x11,z11,-y11);
						glVertex3d(x12,z12,-y12);
						glVertex3d(x22,z22,-y22);
						glVertex3d(x21,z21,-y21);

						glEnd();

						glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
						if (tmpLighting)
							glEnable(GL_LIGHTING);
					}

					if (m_bBlackLinesOrRemoveHidden)
					{
						// Stencil buffer operations
						glStencilFunc(GL_ALWAYS, 0, 1);
						glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
						if (tmpPolygonMode[1] == GL_FILL)
						{
							// Draw Black lines on Surfaces
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glColor3d(0.,0.,0.);
							glBegin(GL_QUADS);

							glVertex3d(x11,z11,-y11);
							glVertex3d(x12,z12,-y12);
							glVertex3d(x22,z22,-y22);
							glVertex3d(x21,z21,-y21);

							glEnd();

							glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
						}
						else if (tmpPolygonMode[1] == GL_POINT || tmpPolygonMode[1] == GL_LINE)
						{
							// Draw the points/lines themselves
							glBegin(GL_QUADS);

							glColor3d(r11,g11,b11);
							glVertex3d(x11,z11,-y11);
							glColor3d(r12,g12,b12);
							glVertex3d(x12,z12,-y12);
							glColor3d(r22,g22,b22);
							glVertex3d(x22,z22,-y22);
							glColor3d(r21,g21,b21);
							glVertex3d(x21,z21,-y21);

							glEnd();
						}
					}
				}
			}

			if (m_bBlackLinesOrRemoveHidden)
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
	glNormal3dv(tmpNormal);
}

void CGlView::RenderSceneTopView()
{
	// Default rendering 
	CMatrix *pXX, *pYY, *pCC;
	int i, j, k, size, sizeX, sizeY;
	double maxCC = -HUGE, minCC = HUGE;
	double x1, x2;
	double y1, y2;
	double c1, c2;
	double r1, g1, b1;
	double r2, g2, b2;
	double fCC;

	if (m_XRenderArray.GetSize() != m_YRenderArray.GetSize() || m_XRenderArray.GetSize() != m_CRenderArray.GetSize())
		return;

	k = -1;
	size = m_XRenderArray.GetSize();
	while (++k < size)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		// Detect color maxima and minima
		maxCC = max(maxCC, matMaxFinite(*pCC));
		minCC = min(minCC, matMinFinite(*pCC));
	}

	if (fabs(maxCC-minCC) <= eps)
	{
		minCC -= .5;
		maxCC += .5;
	}

	for (k = 0; k < size; k++)
	{
		pXX = (CMatrix*) m_XRenderArray.GetAt(k);
		pYY = (CMatrix*) m_YRenderArray.GetAt(k);
		pCC = (CMatrix*) m_CRenderArray.GetAt(k);
		if (pXX == NULL || pYY == NULL || pCC == NULL)
			continue;

		if (pXX->GetSize().cy == pCC->GetSize().cy && pYY->GetSize().cx == pCC->GetSize().cx && 
			(pXX->GetSize().cx == pCC->GetSize().cx || pXX->GetSize().cx == 1) && 
			(pYY->GetSize().cy == pCC->GetSize().cy || pYY->GetSize().cy == 1) && 
			pCC->GetSize().cx > 1 && pCC->GetSize().cy > 1)
		{
			BOOL bIsXXRow = (pXX->GetSize().cx == 1);
			BOOL bIsYYCol = (pYY->GetSize().cy == 1);

			// Plot them
			sizeY = pCC->GetSize().cy;
			sizeX = pCC->GetSize().cx;
			for (j = 1; j < sizeY; j++)
			{
				glBegin(GL_QUAD_STRIP);
				for (i = 1; i <= sizeX; i++)
				{
					x1 = (*pXX)(bIsXXRow ? 1 : i,j);
					x2 = (*pXX)(bIsXXRow ? 1 : i,j+1);

					y1 = (*pYY)(i,bIsYYCol ? 1 : j);
					y2 = (*pYY)(i,bIsYYCol ? 1 : j+1);

					c1 = (*pCC)(i,j);
					c2 = (*pCC)(i,j+1);

					fCC = (c1-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r1, g1, b1, k);

					fCC = (c2-minCC)/(maxCC-minCC);
					MakeColorsRainbowOrMonochrome(fCC, r2, g2, b2, k);

					glColor3d(r1,g1,b1);
					glVertex2d(x1,y1);
					glColor3d(r2,g2,b2);
					glVertex2d(x2,y2);
				}
				glEnd();
			}
		}
	}
}

void CGlView::RenderSceneScale2d()
{
	// Default rendering 
	CScale *pSS;
	CString strValue;
	GLubyte dummyBitmap = 1;
	GLdouble tmpCurrentRasterPosition[4];
	double negStrWidth = 0., negStrHeight = 0.;
	int k, size;
	const double OneMinus = -1., OnePlus = 1.;
	double OneMinusX = OneMinus, OneMinusY = OneMinus, OnePlusX = OnePlus, OnePlusY = OnePlus;
	double rasterPosXLeft, rasterPosXRight, rasterPosXLeftLimit, rasterPosXRightLimit;
	double rasterPosYTop, rasterPosYBottom, rasterPosYTopLimit, rasterPosYBottomLimit;

	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	GLdouble objxLeft, objxRight, objyTop, objyBottom, objz;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	if (viewport[2] == 0 || viewport[3] == 0)
	{
		// This only happens when being called from OnEditCopyEMF().
		//	However calling glViewport in OnEditCopyEMF() does not yield the expected result either.
		//	So this seems to be an appropriate workaround.
		CRect rect;
		GetClientRect(&rect);
		viewport[2] = rect.Width();
		viewport[3] = rect.Height();
	}

	GLint drawBuffer;
	glGetIntegerv(GL_DRAW_BUFFER, &drawBuffer);

	GLint tmpPolygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, tmpPolygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

	size = m_SRenderArray.GetSize();
	for (k = 0; k < size; k++)
	{
		pSS = (CScale*) m_SRenderArray.GetAt(k);
		if (pSS == NULL)
			continue;

		if (m_xScaling == 0. || m_yScaling == 0.)
			continue;

		if (m_bScaleShowFrame)
		{
			double xLeftMost = viewport[0] + viewport[2]*.05/m_aspect;
			double xRightMost = viewport[0] + viewport[2]*(1.-.05/m_aspect);
			double yTopMost = viewport[1] + viewport[3]*(1.-.05);
			double yBottomMost = viewport[1] + viewport[3]*.05;

			if (m_bScaleShowLabels)
			{
				double negStrWidth1, negStrWidth2;
				double objx, objy;

				gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 0., 
								modelMatrix, projMatrix, viewport, 
								&objx, &objy, &objz);

				strValue.Format("%.4g", pSS->m_dYMin);
				glRasterPos3d(objx, objy, -eps6);

				glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
				negStrWidth1 = tmpCurrentRasterPosition[0];
				DrawBufferOrStencil(GL_NONE);
				glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				DrawBufferOrStencil(drawBuffer);
				glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
				negStrWidth1 -= tmpCurrentRasterPosition[0];

				strValue.Format("%.4g", pSS->m_dYMax);
				glRasterPos3d(objx, objy, -eps6);

				glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
				negStrWidth2 = tmpCurrentRasterPosition[0];
				DrawBufferOrStencil(GL_NONE);
				glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				DrawBufferOrStencil(drawBuffer);
				glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
				negStrWidth2 -= tmpCurrentRasterPosition[0];

				negStrWidth = min(negStrWidth1, negStrWidth2);
				xRightMost += negStrWidth;

				negStrHeight = -abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY));
				yBottomMost -= negStrHeight;
			}

			gluUnProject(xLeftMost, yBottomMost, 0., 
							modelMatrix, projMatrix, viewport, 
							&objxLeft, &objyBottom, &objz);
			gluUnProject(xRightMost, yTopMost, 0., 
							modelMatrix, projMatrix, viewport, 
							&objxRight, &objyTop, &objz);

			OneMinusX = max(OneMinus, objxLeft);
			OneMinusY = max(OneMinus, objyBottom);
			OnePlusX = min(OnePlus, objxRight);
			OnePlusY = min(OnePlus, objyTop);

			if (OneMinusX >= OnePlusX)
			{
				OneMinusX = OneMinus;
				OnePlusX = OnePlus;
			}
			if (OneMinusY >= OnePlusY)
			{
				OneMinusY = OneMinus;
				OnePlusY = OnePlus;
			}

			// Draw the lines themselves
			glBegin(GL_QUADS);

			glVertex2d(OneMinusX, OneMinusY);
			glVertex2d(OneMinusX, OnePlusY);
			glVertex2d(OnePlusX, OnePlusY);
			glVertex2d(OnePlusX, OneMinusY);

			glEnd();

			CMatrix coarseGridX, coarseGridY;
			CMatrix coarseGridXNormalized, coarseGridYNormalized;
			CMatrix fineGridXNormalized, fineGridYNormalized;

			if (m_nScaleShowGrid != 0)
			{
				int l, h;

				if (gluUnProject(viewport[0], viewport[1], 0., 
								modelMatrix, projMatrix, viewport, 
								&objxLeft, &objyBottom, &objz) != GL_TRUE)
								continue;
				if (gluUnProject(viewport[0] + viewport[2], viewport[1] + viewport[3], 0., 
								modelMatrix, projMatrix, viewport, 
								&objxRight, &objyTop, &objz) != GL_TRUE)
								continue;

				double dXMin, dXMax, dXMinTmp, dXMaxTmp, dDiffX, dDiffXOrig;
				dXMin = pSS->m_dXMin;
				dXMax = pSS->m_dXMax;
				dDiffXOrig = dXMax - dXMin;

				if (!_finite(dDiffXOrig) || dDiffXOrig <= 0.)
				{
					dXMin = -1.;
					dXMax = 1.;
				}
				ASSERT(objxLeft <= objxRight);
				if (objxLeft > objxRight)
					objxLeft = objxRight = (objxLeft+objxRight)/2.;

				if (max(-1., objxLeft) < min(1., objxRight))
				{
					dXMinTmp = ((dXMax-dXMin) * max(-1., objxLeft) + (dXMax+dXMin))/2.;
					dXMaxTmp = ((dXMax-dXMin) * min(1., objxRight) + (dXMax+dXMin))/2.;
				}
				else
				{
					dXMinTmp = ((dXMax-dXMin) * objxLeft + (dXMax+dXMin))/2.;
					dXMaxTmp = ((dXMax-dXMin) * objxRight + (dXMax+dXMin))/2.;
				}
				dDiffX = dXMaxTmp - dXMinTmp;
				ASSERT(dDiffX > 0.);
				if (dDiffX <= 0.)
					dDiffX = eps;

				double dDiffX1, dDiffX2, dStepSizeX;
				double dDiffXLog = log10(dDiffX);
				double dDiffXLog1 = floor(dDiffXLog);
				double dDiffXLog2 = dDiffXLog - dDiffXLog1;

				dDiffX1 = pow(10., dDiffXLog1);
				dDiffX2 = pow(10., dDiffXLog2);

				if (dDiffX2 <= 2.)
					dStepSizeX = .2 * dDiffX1;
				else if (dDiffX2 <= 5.)
					dStepSizeX = .5 * dDiffX1;
				else
					dStepSizeX = dDiffX1;

				double dFirstX = ceil(dXMinTmp/dStepSizeX)*dStepSizeX;
				if (dFirstX == dXMinTmp)
					dFirstX += dStepSizeX;

				double dLastX = floor(dXMaxTmp/dStepSizeX)*dStepSizeX;
				if (dLastX == dXMaxTmp)
					dLastX -= dStepSizeX;

				ASSERT(dFirstX <= dLastX);
				if (dFirstX > dLastX)
					dFirstX = dLastX = (dFirstX + dLastX)/2.;

				coarseGridX = CMatrix(dFirstX, dLastX, dStepSizeX);

				// omit last value if very close to penultimate
				size = coarseGridX.GetSize().cy;
				if (size > 1 && fabs(coarseGridX[size-1]-coarseGridX[size-2])/fabs(coarseGridX[1]-coarseGridX[0]) < .5)
					coarseGridX.SetSize(CSize(1, size-1));

				coarseGridXNormalized = coarseGridX;
				coarseGridXNormalized *= 2.;
				coarseGridXNormalized -= dXMax + dXMin;
				coarseGridXNormalized /= dXMax - dXMin;

				if (!_finite(dDiffXOrig) || dDiffXOrig <= 0.)
				{
					coarseGridX.Empty();
				}

				if (m_nScaleShowGrid == 2)
				{
					int nRefinementX;
					if (dDiffX2 <= 2.)
						nRefinementX = 4;
					else if (dDiffX2 <= 5.)
						nRefinementX = 5;
					else
						nRefinementX = 5;

					ASSERT(coarseGridXNormalized.GetSize().cx == 1);
					size = coarseGridXNormalized.GetSize().cy;
					if (size > 1)
						dStepSizeX = coarseGridXNormalized[1]-coarseGridXNormalized[0];
					else
						dStepSizeX = 0.;
					fineGridXNormalized.SetSize(CSize(1, (size+1)*(nRefinementX-1)));

					double coarseGridFirst = 0., value;
					if (coarseGridXNormalized.GetTotalSize() > 0)
						coarseGridFirst = coarseGridXNormalized[0];
					int counter, contraCounter = 0;
					for (l = -1; l < size; l++)
					{
						for (h = 1; h < nRefinementX; h++)
						{
							value = coarseGridFirst + l*dStepSizeX + h*dStepSizeX/(double)nRefinementX;
							if (value < -1. || value > 1.)
							{
								contraCounter++;
								continue;
							}
							else
							{
								counter = (l+1)*(nRefinementX-1)+h-1 - contraCounter;
								fineGridXNormalized[counter] = value;
							}
						}
					}
					if (contraCounter > 0)
						fineGridXNormalized.SetSize(CSize(fineGridXNormalized.GetSize().cx, fineGridXNormalized.GetSize().cy-contraCounter));
				}

				double dYMin, dYMax, dYMinTmp, dYMaxTmp, dDiffY, dDiffYOrig;
				dYMin = pSS->m_dYMin;
				dYMax = pSS->m_dYMax;
				dDiffYOrig = dYMax - dYMin;

				if (!_finite(dDiffYOrig) || dDiffYOrig <= 0.)
				{
					dYMin = -1.;
					dYMax = 1.;
				}
				ASSERT(objyBottom <= objyTop);
				if (objyBottom > objyTop)
					objyBottom = objyTop = (objyBottom+objyTop)/2.;

				if (max(-1., objyBottom) < min(1., objyTop))
				{
					dYMinTmp = ((dYMax-dYMin) * max(-1., objyBottom) + (dYMax+dYMin))/2.;
					dYMaxTmp = ((dYMax-dYMin) * min(1., objyTop) + (dYMax+dYMin))/2.;
				}
				else
				{
					dYMinTmp = ((dYMax-dYMin) * objyBottom + (dYMax+dYMin))/2.;
					dYMaxTmp = ((dYMax-dYMin) * objyTop + (dYMax+dYMin))/2.;
				}
				dDiffY = dYMaxTmp - dYMinTmp;
				ASSERT(dDiffY > 0.);
				if (dDiffY <= 0.)
					dDiffY = eps;

				double dDiffY1, dDiffY2, dStepSizeY;
				double dDiffYLog = log10(dDiffY);
				double dDiffYLog1 = floor(dDiffYLog);
				double dDiffYLog2 = dDiffYLog - dDiffYLog1;

				dDiffY1 = pow(10., dDiffYLog1);
				dDiffY2 = pow(10., dDiffYLog2);

				if (dDiffY2 <= 2.)
					dStepSizeY = .2 * dDiffY1;
				else if (dDiffY2 <= 5.)
					dStepSizeY = .5 * dDiffY1;
				else
					dStepSizeY = dDiffY1;

				double dFirstY = ceil(dYMinTmp/dStepSizeY)*dStepSizeY;
				if (dFirstY == dYMinTmp)
					dFirstY += dStepSizeY;

				double dLastY = floor(dYMaxTmp/dStepSizeY)*dStepSizeY;
				if (dLastY == dYMaxTmp)
					dLastY -= dStepSizeY;

				ASSERT(dFirstY <= dLastY);
				if (dFirstY > dLastY)
					dFirstY = dLastY = (dFirstY + dLastY)/2.;

				coarseGridY = CMatrix(dFirstY, dLastY, dStepSizeY);

				// omit last value if very close to penultimate
				size = coarseGridY.GetSize().cy;
				if (size > 1 && fabs(coarseGridY[size-1]-coarseGridY[size-2])/fabs(coarseGridY[1]-coarseGridY[0]) < .5)
					coarseGridY.SetSize(CSize(1, size-1));

				coarseGridYNormalized = coarseGridY;
				coarseGridYNormalized *= 2.;
				coarseGridYNormalized -= dYMax + dYMin;
				coarseGridYNormalized /= dYMax - dYMin;

				if (!_finite(dDiffYOrig) || dDiffYOrig <= 0.)
				{
					coarseGridY.Empty();
				}

				if (m_nScaleShowGrid == 2)
				{
					int nRefinementY;
					if (dDiffY2 <= 2.)
						nRefinementY = 4;
					else if (dDiffY2 <= 5.)
						nRefinementY = 5;
					else
						nRefinementY = 5;

					ASSERT(coarseGridYNormalized.GetSize().cx == 1);
					size = coarseGridYNormalized.GetSize().cy;
					if (size > 1)
						dStepSizeY = coarseGridYNormalized[1]-coarseGridYNormalized[0];
					else
						dStepSizeY = 0.;
					fineGridYNormalized.SetSize(CSize(1, (size+1)*(nRefinementY-1)));

					double coarseGridFirst = 0., value;
					if (coarseGridYNormalized.GetTotalSize() > 0)
						coarseGridFirst = coarseGridYNormalized[0];
					int counter, contraCounter = 0;
					for (l = -1; l < size; l++)
					{
						for (h = 1; h < nRefinementY; h++)
						{
							value = coarseGridFirst + l*dStepSizeY + h*dStepSizeY/(double)nRefinementY;
							if (value < -1. || value > 1.)
							{
								contraCounter++;
								continue;
							}
							else
							{
								counter = (l+1)*(nRefinementY-1)+h-1 - contraCounter;
								fineGridYNormalized[counter] = value;
							}
						}
					}
					if (contraCounter > 0)
						fineGridYNormalized.SetSize(CSize(fineGridYNormalized.GetSize().cx, fineGridYNormalized.GetSize().cy-contraCounter));
				}

				GLushort lineStipple, pattern;
				GLint repeat;
				glEnable(GL_LINE_STIPPLE);
				glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
				glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);

				lineStipple = 0x5555;
				glLineStipple(1, lineStipple);

				size = coarseGridXNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					if (coarseGridXNormalized[l] < OneMinusX || coarseGridXNormalized[l] > OnePlusX)
						continue;

					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex2d(coarseGridXNormalized[l], OneMinusY);
					glVertex2d(coarseGridXNormalized[l], OnePlusY);
					glEnd();
				}
				size = coarseGridYNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					if (coarseGridYNormalized[l] < OneMinusY || coarseGridYNormalized[l] > OnePlusY)
						continue;

					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex2d(OneMinusX, coarseGridYNormalized[l]);
					glVertex2d(OnePlusX, coarseGridYNormalized[l]);
					glEnd();
				}

				glColor3f((m_ColorFontRed+m_ColorBackRed*2.f)/3.f, (m_ColorFontGreen+m_ColorBackGreen*2.f)/3.f, (m_ColorFontBlue+m_ColorBackBlue*2.f)/3.f);

				size = fineGridXNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					if (fineGridXNormalized[l] < OneMinusX || fineGridXNormalized[l] > OnePlusX)
						continue;

					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex2d(fineGridXNormalized[l], OneMinusY);
					glVertex2d(fineGridXNormalized[l], OnePlusY);
					glEnd();
				}
				size = fineGridYNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					if (fineGridYNormalized[l] < OneMinusY || fineGridYNormalized[l] > OnePlusY)
						continue;

					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex2d(OneMinusX, fineGridYNormalized[l]);
					glVertex2d(OnePlusX, fineGridYNormalized[l]);
					glEnd();
				}

				glColor3f(m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

				glLineStipple(repeat, pattern);
			}

			if (m_bScaleShowLabels)
			{
				int l;

				glListBase(m_nListBaseSmall);

				glPushAttrib(GL_CURRENT_RASTER_POSITION);

				// text x min
				if ((OneMinusX == OneMinus && _finite(pSS->m_dXMin)) || (m_nScaleShowGrid == 0 && _finite(pSS->m_dXMin) && _finite(pSS->m_dXMax)))
				{
					if (OneMinusX == OneMinus)
						strValue.Format("%.4g", pSS->m_dXMin);
					else
						strValue.Format("%.4g", ((pSS->m_dXMax - pSS->m_dXMin) * OneMinusX + (pSS->m_dXMax + pSS->m_dXMin)) / 2.);
					glRasterPos3d(OneMinusX, OneMinusY, -eps6);

					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth = tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth -= tmpCurrentRasterPosition[0];

					rasterPosXLeftLimit = tmpCurrentRasterPosition[0];

					glRasterPos2d(OneMinusX, OneMinusY);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}
				else
				{
					rasterPosXLeftLimit = -HUGE;
				}

				// text x max
				if ((OnePlusX == OnePlus && _finite(pSS->m_dXMax)) || (m_nScaleShowGrid == 0 && _finite(pSS->m_dXMin) && _finite(pSS->m_dXMax)))
				{
					if (OnePlusX == OnePlus)
						strValue.Format("%.4g", pSS->m_dXMax);
					else
						strValue.Format("%.4g", ((pSS->m_dXMax - pSS->m_dXMin) * OnePlusX + (pSS->m_dXMax + pSS->m_dXMin)) / 2.);
					if (!pSS->m_strXLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strXLabel;
					glRasterPos3d(OnePlusX, OneMinusY, -eps6);

					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth = tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth -= tmpCurrentRasterPosition[0];

					rasterPosXRightLimit = tmpCurrentRasterPosition[0] + 2*negStrWidth;

					glRasterPos2d(OnePlusX, OneMinusY);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, (float)negStrWidth, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}
				else
				{
					rasterPosXRightLimit = HUGE;
				}

				// text x grid
				size = coarseGridX.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					if (coarseGridXNormalized[l] < OneMinusX || coarseGridXNormalized[l] > OnePlusX)
						continue;

					strValue.Format("%.10g", coarseGridX[l]);
					glRasterPos3d(coarseGridXNormalized[l], OneMinusY, -eps6);

					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth = tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					negStrWidth -= tmpCurrentRasterPosition[0];

					rasterPosXLeft = tmpCurrentRasterPosition[0] + 2.*negStrWidth;
					rasterPosXRight = tmpCurrentRasterPosition[0];

					if ((_finite(rasterPosXLeftLimit) && rasterPosXLeft < rasterPosXLeftLimit) || 
						(_finite(rasterPosXRightLimit) && rasterPosXRight > rasterPosXRightLimit))
					{
						continue;
					}

					glRasterPos2d(coarseGridXNormalized[l], OneMinusY);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, (float)negStrWidth/2.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}

				// text y min
				if ((OneMinusY == OneMinus && _finite(pSS->m_dYMin)) || (m_nScaleShowGrid == 0 && _finite(pSS->m_dYMin) && _finite(pSS->m_dYMax)))
				{
					if (OneMinusY == OneMinus)
						strValue.Format("  %.4g", pSS->m_dYMin);
					else
						strValue.Format("  %.4g", ((pSS->m_dYMax - pSS->m_dYMin) * OneMinusY + (pSS->m_dYMax + pSS->m_dYMin)) / 2.);
					glRasterPos2d(OnePlusX, OneMinusY);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);

					rasterPosYBottomLimit = tmpCurrentRasterPosition[1];
					rasterPosYBottomLimit -= negStrHeight;

					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}
				else
				{
					rasterPosYBottomLimit = -HUGE;
				}

				// text y max
				if ((OnePlusY == OnePlus && _finite(pSS->m_dYMax)) || (m_nScaleShowGrid == 0 && _finite(pSS->m_dYMin) && _finite(pSS->m_dYMax)))
				{
					if (OnePlusY == OnePlus)
						strValue.Format("  %.4g", pSS->m_dYMax);
					else
						strValue.Format("  %.4g", ((pSS->m_dYMax - pSS->m_dYMin) * OnePlusY + (pSS->m_dYMax + pSS->m_dYMin)) / 2.);
					if (!pSS->m_strYLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strYLabel;
					glRasterPos2d(OnePlusX, OnePlusY);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);

					rasterPosYTopLimit = tmpCurrentRasterPosition[1];

					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}
				else
				{
					rasterPosYTopLimit = HUGE;
				}

				// text y grid
				size = coarseGridY.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					if (coarseGridYNormalized[l] < OneMinusY || coarseGridYNormalized[l] > OnePlusY)
						continue;

					strValue.Format("  %.10g", coarseGridY[l]);
					glRasterPos2d(OnePlusX, coarseGridYNormalized[l]);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio*.5/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);

					rasterPosYTop = tmpCurrentRasterPosition[1] - 1.25*negStrHeight;
					rasterPosYBottom = tmpCurrentRasterPosition[1] + .25*negStrHeight;

					if ((_finite(rasterPosYBottomLimit) && rasterPosYBottom < rasterPosYBottomLimit) || 
						(_finite(rasterPosYTopLimit) && rasterPosYTop > rasterPosYTopLimit))
					{
						continue;
					}

					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}

				glPopAttrib();
			}
		}

		if (m_bScaleShowAxis)
		{
			double x = QNAN, y = QNAN;

			if (pSS->m_dXMax == HUGE && pSS->m_dXMin == -HUGE)
				x = 0.;
			else if (pSS->m_dXMax == HUGE)
				x = OneMinus;
			else if (pSS->m_dXMin == -HUGE)
				x = OnePlus;
			else if (pSS->m_dXMax > pSS->m_dXMin)
				x = max(OneMinus, min(OnePlus, -(pSS->m_dXMax + pSS->m_dXMin)/(pSS->m_dXMax - pSS->m_dXMin)));

			if (pSS->m_dYMax == HUGE && pSS->m_dYMin == -HUGE)
				y = 0.;
			else if (pSS->m_dYMax == HUGE)
				y = OneMinus;
			else if (pSS->m_dYMin == -HUGE)
				y = OnePlus;
			else if (pSS->m_dYMax > pSS->m_dYMin)
				y = max(OneMinus, min(OnePlus, -(pSS->m_dYMax + pSS->m_dYMin)/(pSS->m_dYMax - pSS->m_dYMin)));

			GLfloat tmpLineWidth[1];
			glGetFloatv(GL_LINE_WIDTH, tmpLineWidth);
			glLineWidth(1.5);

			// Draw the lines themselves
			if (_finite(y) && OneMinusY <= y && y <= OnePlusY)
			{
				glBegin(GL_LINE_STRIP);
				glVertex2d(OneMinusX,y);
				glVertex2d(OnePlusX,y);
				glEnd();
			}

			if (_finite(x) && OneMinusX <= x && x <= OnePlusX)
			{
				glBegin(GL_LINE_STRIP);
				glVertex2d(x, OneMinusY);
				glVertex2d(x, OnePlusY);
				glEnd();
			}

			glLineWidth(tmpLineWidth[0]);
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
}

void CGlView::RenderSceneScale3d()
{
	// Default rendering 
	CScale *pSS;
	CString strValue;
	GLubyte dummyBitmap = 1;
	GLdouble tmpCurrentRasterPosition[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	GLdouble winx, winy, winz;
	GLdouble objx, objy, objz;
	double posStrWidth, posStrHeight, diffWidth, diffHeight;

	CRect rectFirst, rectLast, rectGrid, rectTest;

	int k, size;
	double distFactor = 1.025, rotXShift = (1.-cos(pi/180*(m_xRotation+45.)))/2.;
	const double OneMinus = -1., OnePlus = 1.;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	if (viewport[2] == 0 || viewport[3] == 0)
	{
		// This only happens when being called from OnEditCopyEMF().
		//	However calling glViewport in OnEditCopyEMF() does not yield the expected result either.
		//	So this seems to be an appropriate workaround.
		CRect rect;
		GetClientRect(&rect);
		viewport[2] = rect.Width();
		viewport[3] = rect.Height();
	}

	GLint drawBuffer;
	glGetIntegerv(GL_DRAW_BUFFER, &drawBuffer);

	GLint tmpPolygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, tmpPolygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLdouble tmpNormal[3];
	glGetDoublev(GL_CURRENT_NORMAL, tmpNormal);

	glColor3f(m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

	size = m_SRenderArray.GetSize();
	for (k = 0; k < size; k++)
	{
		pSS = (CScale*) m_SRenderArray.GetAt(k);
		if (pSS == NULL)
			continue;

		if ((m_xScaling == 0. && m_yScaling == 0.) || (m_xScaling == 0. && m_zScaling == 0.) || (m_yScaling == 0. && m_zScaling == 0.))
			continue;

		if (m_bScaleShowFrame)
		{
			// Draw the lines themselves
			glBegin(GL_QUADS);

			glNormal3d( OneMinus, 0.0,      0.0);
			glVertex3d( OnePlus,  OnePlus,  OnePlus);
			glVertex3d( OnePlus,  OneMinus, OnePlus);
			glVertex3d( OnePlus,  OneMinus, OneMinus);
			glVertex3d( OnePlus,  OnePlus,  OneMinus);

			glNormal3d( OnePlus,  0.0,      0.0);
			glVertex3d( OneMinus, OneMinus, OnePlus);
			glVertex3d( OneMinus, OnePlus,  OnePlus);
			glVertex3d( OneMinus, OnePlus,  OneMinus);
			glVertex3d( OneMinus, OneMinus, OneMinus);

			glNormal3d( 0.0,      OneMinus, 0.0);
			glVertex3d( OnePlus,  OnePlus,  OnePlus);
			glVertex3d( OneMinus, OnePlus,  OnePlus);
			glVertex3d( OneMinus, OnePlus,  OneMinus);
			glVertex3d( OnePlus,  OnePlus,  OneMinus);

			glNormal3d( 0.0,      OnePlus,  0.0);
			glVertex3d( OneMinus, OneMinus, OnePlus);
			glVertex3d( OnePlus,  OneMinus, OnePlus);
			glVertex3d( OnePlus,  OneMinus, OneMinus);
			glVertex3d( OneMinus, OneMinus, OneMinus);

			glNormal3d( 0.0,      0.0,      OneMinus);
			glVertex3d( OnePlus,  OnePlus,  OnePlus);
			glVertex3d( OneMinus, OnePlus,  OnePlus);
			glVertex3d( OneMinus, OneMinus, OnePlus);
			glVertex3d( OnePlus,  OneMinus, OnePlus);

			glNormal3d( 0.0,      0.0,      OnePlus);
			glVertex3d( OneMinus, OnePlus,  OneMinus);
			glVertex3d( OnePlus,  OnePlus,  OneMinus);
			glVertex3d( OnePlus,  OneMinus, OneMinus);
			glVertex3d( OneMinus, OneMinus, OneMinus);

			glEnd();

			CMatrix coarseGridX, coarseGridY, coarseGridZ;
			CMatrix coarseGridXNormalized(-1., 1., .2);
			CMatrix coarseGridYNormalized(-1., 1., .2);
			CMatrix coarseGridZNormalized(-1., 1., .2);
			CMatrix fineGridXNormalized;
			CMatrix fineGridYNormalized;
			CMatrix fineGridZNormalized;

			double dRasterPos1Remember = 0., dRasterPos2Remember = 0., dRasterPos3Remember = 0.;

			if (m_nScaleShowGrid != 0)
			{
				int l, h;

				double dXMin, dXMax, dDiffX, dDiffXOrig;
				dXMin = pSS->m_dXMin;
				dXMax = pSS->m_dXMax;
				dDiffXOrig = dXMax - dXMin;

				if (!_finite(dDiffXOrig) || dDiffXOrig <= 0.)
				{
					dXMin = -1.;
					dXMax = 1.;
				}
				dDiffX = dXMax - dXMin;
				ASSERT(dDiffX > 0.);
				if (dDiffX <= 0.)
					dDiffX = eps;

				double dDiffX1, dDiffX2, dStepSizeX;
				double dDiffXLog = log10(dDiffX);
				double dDiffXLog1 = floor(dDiffXLog);
				double dDiffXLog2 = dDiffXLog - dDiffXLog1;

				dDiffX1 = pow(10., dDiffXLog1);
				dDiffX2 = pow(10., dDiffXLog2);

				if (dDiffX2 <= 2.)
					dStepSizeX = .2 * dDiffX1;
				else if (dDiffX2 <= 5.)
					dStepSizeX = .5 * dDiffX1;
				else
					dStepSizeX = dDiffX1;

				double dFirstX = ceil(dXMin/dStepSizeX)*dStepSizeX;
				if (dFirstX == dXMin)
					dFirstX += dStepSizeX;

				double dLastX = floor(dXMax/dStepSizeX)*dStepSizeX;
				if (dLastX == dXMax)
					dLastX -= dStepSizeX;

				ASSERT(dFirstX <= dLastX);
				if (dFirstX > dLastX)
					dFirstX = dLastX = (dFirstX + dLastX)/2.;

				coarseGridX = CMatrix(dFirstX, dLastX, dStepSizeX);

				// omit last value if very close to penultimate
				size = coarseGridX.GetSize().cy;
				if (size > 1 && fabs(coarseGridX[size-1]-coarseGridX[size-2])/fabs(coarseGridX[1]-coarseGridX[0]) < .5)
					coarseGridX.SetSize(CSize(1, size-1));

				coarseGridXNormalized = coarseGridX;
				coarseGridXNormalized *= 2.;
				coarseGridXNormalized -= dXMax + dXMin;
				coarseGridXNormalized /= dXMax - dXMin;

				if (!_finite(dDiffXOrig) || dDiffXOrig <= 0.)
				{
					coarseGridX.Empty();
				}

				if (m_nScaleShowGrid == 2)
				{
					int nRefinementX;
					if (dDiffX2 <= 2.)
						nRefinementX = 4;
					else if (dDiffX2 <= 5.)
						nRefinementX = 5;
					else
						nRefinementX = 5;

					ASSERT(coarseGridXNormalized.GetSize().cx == 1);
					size = coarseGridXNormalized.GetSize().cy;
					if (size > 1)
						dStepSizeX = coarseGridXNormalized[1]-coarseGridXNormalized[0];
					else
						dStepSizeX = 0.;
					fineGridXNormalized.SetSize(CSize(1, (size+1)*(nRefinementX-1)));

					double coarseGridFirst = 0., value;
					if (coarseGridXNormalized.GetTotalSize() > 0)
						coarseGridFirst = coarseGridXNormalized[0];
					int counter, contraCounter = 0;
					for (l = -1; l < size; l++)
					{
						for (h = 1; h < nRefinementX; h++)
						{
							value = coarseGridFirst + l*dStepSizeX + h*dStepSizeX/(double)nRefinementX;
							if (value < -1. || value > 1.)
							{
								contraCounter++;
								continue;
							}
							else
							{
								counter = (l+1)*(nRefinementX-1)+h-1 - contraCounter;
								fineGridXNormalized[counter] = value;
							}
						}
					}
					if (contraCounter > 0)
						fineGridXNormalized.SetSize(CSize(fineGridXNormalized.GetSize().cx, fineGridXNormalized.GetSize().cy-contraCounter));
				}

				double dYMin, dYMax, dDiffY, dDiffYOrig;
				dYMin = pSS->m_dYMin;
				dYMax = pSS->m_dYMax;
				dDiffYOrig = dYMax - dYMin;

				if (!_finite(dDiffYOrig) || dDiffYOrig <= 0.)
				{
					dYMin = -1.;
					dYMax = 1.;
				}
				dDiffY = dYMax - dYMin;
				ASSERT(dDiffY > 0.);
				if (dDiffY <= 0.)
					dDiffY = eps;

				double dDiffY1, dDiffY2, dStepSizeY;
				double dDiffYLog = log10(dDiffY);
				double dDiffYLog1 = floor(dDiffYLog);
				double dDiffYLog2 = dDiffYLog - dDiffYLog1;

				dDiffY1 = pow(10., dDiffYLog1);
				dDiffY2 = pow(10., dDiffYLog2);

				if (dDiffY2 <= 2.)
					dStepSizeY = .2 * dDiffY1;
				else if (dDiffY2 <= 5.)
					dStepSizeY = .5 * dDiffY1;
				else
					dStepSizeY = dDiffY1;

				double dFirstY = ceil(dYMin/dStepSizeY)*dStepSizeY;
				if (dFirstY == dYMin)
					dFirstY += dStepSizeY;

				double dLastY = floor(dYMax/dStepSizeY)*dStepSizeY;
				if (dLastY == dYMax)
					dLastY -= dStepSizeY;

				ASSERT(dFirstY <= dLastY);
				if (dFirstY > dLastY)
					dFirstY = dLastY = (dFirstY + dLastY)/2.;

				coarseGridY = CMatrix(dFirstY, dLastY, dStepSizeY);

				// omit last value if very close to penultimate
				size = coarseGridY.GetSize().cy;
				if (size > 1 && fabs(coarseGridY[size-1]-coarseGridY[size-2])/fabs(coarseGridY[1]-coarseGridY[0]) < .5)
					coarseGridY.SetSize(CSize(1, size-1));

				coarseGridYNormalized = coarseGridY;
				coarseGridYNormalized *= 2.;
				coarseGridYNormalized -= dYMax + dYMin;
				coarseGridYNormalized /= dYMax - dYMin;

				if (!_finite(dDiffYOrig) || dDiffYOrig <= 0.)
				{
					coarseGridY.Empty();
				}

				if (m_nScaleShowGrid == 2)
				{
					int nRefinementY;
					if (dDiffY2 <= 2.)
						nRefinementY = 4;
					else if (dDiffY2 <= 5.)
						nRefinementY = 5;
					else
						nRefinementY = 5;

					ASSERT(coarseGridYNormalized.GetSize().cx == 1);
					size = coarseGridYNormalized.GetSize().cy;
					if (size > 1)
						dStepSizeY = coarseGridYNormalized[1]-coarseGridYNormalized[0];
					else
						dStepSizeY = 0.;
					fineGridYNormalized.SetSize(CSize(1, (size+1)*(nRefinementY-1)));

					double coarseGridFirst = 0., value;
					if (coarseGridYNormalized.GetTotalSize() > 0)
						coarseGridFirst = coarseGridYNormalized[0];
					int counter, contraCounter = 0;
					for (l = -1; l < size; l++)
					{
						for (h = 1; h < nRefinementY; h++)
						{
							value = coarseGridFirst + l*dStepSizeY + h*dStepSizeY/(double)nRefinementY;
							if (value < -1. || value > 1.)
							{
								contraCounter++;
								continue;
							}
							else
							{
								counter = (l+1)*(nRefinementY-1)+h-1 - contraCounter;
								fineGridYNormalized[counter] = value;
							}
						}
					}
					if (contraCounter > 0)
						fineGridYNormalized.SetSize(CSize(fineGridYNormalized.GetSize().cx, fineGridYNormalized.GetSize().cy-contraCounter));
				}

				double dZMin, dZMax, dDiffZ, dDiffZOrig;
				dZMin = pSS->m_dZMin;
				dZMax = pSS->m_dZMax;
				dDiffZOrig = dZMax - dZMin;

				if (!_finite(dDiffZOrig) || dDiffZOrig <= 0.)
				{
					dZMin = -1.;
					dZMax = 1.;
				}
				dDiffZ = dZMax - dZMin;
				ASSERT(dDiffZ > 0.);
				if (dDiffZ <= 0.)
					dDiffZ = eps;

				double dDiffZ1, dDiffZ2, dStepSizeZ;
				double dDiffZLog = log10(dDiffZ);
				double dDiffZLog1 = floor(dDiffZLog);
				double dDiffZLog2 = dDiffZLog - dDiffZLog1;

				dDiffZ1 = pow(10., dDiffZLog1);
				dDiffZ2 = pow(10., dDiffZLog2);

				if (dDiffZ2 <= 2.)
					dStepSizeZ = .2 * dDiffZ1;
				else if (dDiffZ2 <= 5.)
					dStepSizeZ = .5 * dDiffZ1;
				else
					dStepSizeZ = dDiffZ1;

				double dFirstZ = ceil(dZMin/dStepSizeZ)*dStepSizeZ;
				if (dFirstZ == dZMin)
					dFirstZ += dStepSizeZ;

				double dLastZ = floor(dZMax/dStepSizeZ)*dStepSizeZ;
				if (dLastZ == dZMax)
					dLastZ -= dStepSizeZ;

				ASSERT(dFirstZ <= dLastZ);
				if (dFirstZ > dLastZ)
					dFirstZ = dLastZ = (dFirstZ + dLastZ)/2.;

				coarseGridZ = CMatrix(dFirstZ, dLastZ, dStepSizeZ);

				// omit last value if very close to penultimate
				size = coarseGridZ.GetSize().cy;
				if (size > 1 && fabs(coarseGridZ[size-1]-coarseGridZ[size-2])/fabs(coarseGridZ[1]-coarseGridZ[0]) < .5)
					coarseGridZ.SetSize(CSize(1, size-1));

				coarseGridZNormalized = coarseGridZ;
				coarseGridZNormalized *= 2.;
				coarseGridZNormalized -= dZMax + dZMin;
				coarseGridZNormalized /= dZMax - dZMin;

				if (!_finite(dDiffZOrig) || dDiffZOrig <= 0.)
				{
					coarseGridZ.Empty();
				}

				if (m_nScaleShowGrid == 2)
				{
					int nRefinementZ;
					if (dDiffZ2 <= 2.)
						nRefinementZ = 4;
					else if (dDiffZ2 <= 5.)
						nRefinementZ = 5;
					else
						nRefinementZ = 5;

					ASSERT(coarseGridZNormalized.GetSize().cx == 1);
					size = coarseGridZNormalized.GetSize().cy;
					if (size > 1)
						dStepSizeZ = coarseGridZNormalized[1]-coarseGridZNormalized[0];
					else
						dStepSizeZ = 0.;
					fineGridZNormalized.SetSize(CSize(1, (size+1)*(nRefinementZ-1)));

					double coarseGridFirst = 0., value;
					if (coarseGridZNormalized.GetTotalSize() > 0)
						coarseGridFirst = coarseGridZNormalized[0];
					int counter, contraCounter = 0;
					for (l = -1; l < size; l++)
					{
						for (h = 1; h < nRefinementZ; h++)
						{
							value = coarseGridFirst + l*dStepSizeZ + h*dStepSizeZ/(double)nRefinementZ;
							if (value < -1. || value > 1.)
							{
								contraCounter++;
								continue;
							}
							else
							{
								counter = (l+1)*(nRefinementZ-1)+h-1 - contraCounter;
								fineGridZNormalized[counter] = value;
							}
						}
					}
					if (contraCounter > 0)
						fineGridZNormalized.SetSize(CSize(fineGridZNormalized.GetSize().cx, fineGridZNormalized.GetSize().cy-contraCounter));
				}

				GLushort lineStipple, pattern;
				GLint repeat;
				glEnable(GL_LINE_STIPPLE);
				glGetIntegerv(GL_LINE_STIPPLE_PATTERN, (GLint*)&pattern);
				glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &repeat);

				lineStipple = 0x5555;
				glLineStipple(1, lineStipple);

				double dRasterPos1 = 0., dRasterPos2 = 0., dRasterPos3 = 0., zMin;

				// select best x plane
				zMin = HUGE;
				gluProject(-1., 0., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos1Remember = dRasterPos1 = OnePlus;
				}
				gluProject(1., 0., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos1Remember = dRasterPos1 = OneMinus;
				}

				glNormal3d(-dRasterPos1, 0.0, 0.0);

				size = coarseGridYNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(dRasterPos1, OneMinus, -coarseGridYNormalized[l]);
					glVertex3d(dRasterPos1, OnePlus, -coarseGridYNormalized[l]);
					glEnd();
				}
				size = coarseGridZNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(dRasterPos1, coarseGridZNormalized[l], OnePlus);
					glVertex3d(dRasterPos1, coarseGridZNormalized[l], OneMinus);
					glEnd();
				}

				glColor3f((m_ColorFontRed+m_ColorBackRed*2.f)/3.f, (m_ColorFontGreen+m_ColorBackGreen*2.f)/3.f, (m_ColorFontBlue+m_ColorBackBlue*2.f)/3.f);

				size = fineGridYNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(dRasterPos1, OneMinus, -fineGridYNormalized[l]);
					glVertex3d(dRasterPos1, OnePlus, -fineGridYNormalized[l]);
					glEnd();
				}
				size = fineGridZNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(dRasterPos1, fineGridZNormalized[l], OnePlus);
					glVertex3d(dRasterPos1, fineGridZNormalized[l], OneMinus);
					glEnd();
				}

				glColor3f(m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

				// select best y plane
				zMin = HUGE;
				gluProject(0., 0., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos3Remember = dRasterPos3 = OnePlus;
				}
				gluProject(0., 0., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos3Remember = dRasterPos3 = OneMinus;
				}

				glNormal3d(0.0, 0.0, -dRasterPos3);

				size = coarseGridXNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(coarseGridXNormalized[l], OneMinus, dRasterPos3);
					glVertex3d(coarseGridXNormalized[l], OnePlus, dRasterPos3);
					glEnd();
				}
				size = coarseGridZNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(OneMinus, coarseGridZNormalized[l], dRasterPos3);
					glVertex3d(OnePlus, coarseGridZNormalized[l], dRasterPos3);
					glEnd();
				}

				glColor3f((m_ColorFontRed+m_ColorBackRed*2.f)/3.f, (m_ColorFontGreen+m_ColorBackGreen*2.f)/3.f, (m_ColorFontBlue+m_ColorBackBlue*2.f)/3.f);

				size = fineGridXNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(fineGridXNormalized[l], OneMinus, dRasterPos3);
					glVertex3d(fineGridXNormalized[l], OnePlus, dRasterPos3);
					glEnd();
				}
				size = fineGridZNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(OneMinus, fineGridZNormalized[l], dRasterPos3);
					glVertex3d(OnePlus, fineGridZNormalized[l], dRasterPos3);
					glEnd();
				}

				glColor3f(m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

				// select best z plane
				zMin = HUGE;
				gluProject(0., -1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos2Remember = dRasterPos2 = OnePlus;
				}
				gluProject(0., 1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winz < zMin)
				{
					zMin = winz;
					dRasterPos2Remember = dRasterPos2 = OneMinus;
				}

				glNormal3d(0.0, -dRasterPos2, 0.0);

				size = coarseGridXNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(coarseGridXNormalized[l], dRasterPos2, OnePlus);
					glVertex3d(coarseGridXNormalized[l], dRasterPos2, OneMinus);
					glEnd();
				}
				size = coarseGridYNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(OneMinus, dRasterPos2, -coarseGridYNormalized[l]);
					glVertex3d(OnePlus, dRasterPos2, -coarseGridYNormalized[l]);
					glEnd();
				}

				glColor3f((m_ColorFontRed+m_ColorBackRed*2.f)/3.f, (m_ColorFontGreen+m_ColorBackGreen*2.f)/3.f, (m_ColorFontBlue+m_ColorBackBlue*2.f)/3.f);

				size = fineGridXNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(fineGridXNormalized[l], dRasterPos2, OnePlus);
					glVertex3d(fineGridXNormalized[l], dRasterPos2, OneMinus);
					glEnd();
				}
				size = fineGridYNormalized.GetTotalSize();
				for (l = 0; l < size; l++)
				{
					// Draw the lines themselves
					glBegin(GL_LINE_STRIP);
					glVertex3d(OneMinus, dRasterPos2, -fineGridYNormalized[l]);
					glVertex3d(OnePlus, dRasterPos2, -fineGridYNormalized[l]);
					glEnd();
				}

				glColor3f(m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

				glLineStipple(repeat, pattern);
			}

			if (m_bScaleShowLabels)
			{
				int l;

				posStrHeight = abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY));

				glNormal3d(0., 1., 0.);

				glListBase(m_nListBaseSmall);

				glPushAttrib(GL_CURRENT_RASTER_POSITION);

				double dRasterPos1 = 0., dRasterPos2 = 0., dRasterPos3 = 0., xMax = -HUGE, yMin = HUGE, yMax = -HUGE;

				// select best x axis
				gluProject(0., 1., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(m_nScaleShowGrid == 0 || dRasterPos2Remember == OnePlus || dRasterPos3Remember == OnePlus))
				{
					yMin = winy;
					dRasterPos2 = 1.;
					dRasterPos3 = 1.;
				}
				gluProject(0., 1., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(m_nScaleShowGrid == 0 || dRasterPos2Remember == OnePlus || dRasterPos3Remember == OneMinus))
				{
					yMin = winy;
					dRasterPos2 = 1.;
					dRasterPos3 = -1.;
				}
				gluProject(0., -1., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(m_nScaleShowGrid == 0 || dRasterPos2Remember == OneMinus || dRasterPos3Remember == OneMinus))
				{
					yMin = winy;
					dRasterPos2 = -1.;
					dRasterPos3 = -1.;
				}
				gluProject(0., -1., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy < yMin && 
					(m_nScaleShowGrid == 0 || dRasterPos2Remember == OneMinus || dRasterPos3Remember == OnePlus))
				{
					yMin = winy;
					dRasterPos2 = -1.;
					dRasterPos3 = 1.;
				}

				// text x min
				if (_finite(pSS->m_dXMin) && m_xScaling != 0.)
				{
					strValue.Format("%.4g", pSS->m_dXMin);
					glRasterPos3d(OneMinus, dRasterPos2*distFactor, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectFirst.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectFirst.top = (int)floor(tmpCurrentRasterPosition[1]);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectFirst.right = (int)ceil(tmpCurrentRasterPosition[0]);
					rectFirst.bottom = (int)ceil(tmpCurrentRasterPosition[1] + posStrHeight);
				}

				// text x max
				if (_finite(pSS->m_dXMax) && m_xScaling != 0.)
				{
					strValue.Format("%.4g", pSS->m_dXMax);
					if (!pSS->m_strXLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strXLabel;
					glRasterPos3d(OnePlus, dRasterPos2*distFactor, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectLast.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectLast.top = (int)floor(tmpCurrentRasterPosition[1]);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectLast.right = (int)ceil(tmpCurrentRasterPosition[0]);
					rectLast.bottom = (int)ceil(tmpCurrentRasterPosition[1] + posStrHeight);
				}

				// text x grid
				size = coarseGridX.GetTotalSize();
				for (l = 0; l < size && m_xScaling != 0.; l++)
				{
					strValue.Format("%.10g", coarseGridX[l]);

					gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 1.-eps1, 
									modelMatrix, projMatrix, viewport, 
									&objx, &objy, &objz);
					glRasterPos3d(objx, objy, objz);
#ifdef _DEBUG
					GLboolean bCurrentRasterPositionValid;
					glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &bCurrentRasterPositionValid);
					ASSERT(bCurrentRasterPositionValid == GL_TRUE);
#endif // _DEBUG
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth = -tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth += tmpCurrentRasterPosition[0];

					glRasterPos3d(coarseGridXNormalized[l], dRasterPos2*distFactor, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(1.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectGrid.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectGrid.top = (int)floor(tmpCurrentRasterPosition[1]);
					rectGrid.right = rectGrid.left + (int)ceil(posStrWidth);
					rectGrid.bottom = rectGrid.top + (int)ceil(posStrHeight);
					if (rectTest.IntersectRect(rectGrid, rectFirst) || rectTest.IntersectRect(rectGrid, rectLast))
						continue;
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}


				// select best y axis
				gluProject(1., 1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OnePlus || dRasterPos2Remember == OnePlus))
				{
					yMax = winy;
					dRasterPos1 = 1.;
					dRasterPos2 = 1.;
				}
				gluProject(1., -1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OnePlus || dRasterPos2Remember == OneMinus))
				{
					yMax = winy;
					dRasterPos1 = 1.;
					dRasterPos2 = -1.;
				}
				gluProject(-1., -1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OneMinus || dRasterPos2Remember == OneMinus))
				{
					yMax = winy;
					dRasterPos1 = -1.;
					dRasterPos2 = -1.;
				}
				gluProject(-1., 1., 0., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winy > yMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OneMinus || dRasterPos2Remember == OnePlus))
				{
					yMax = winy;
					dRasterPos1 = -1.;
					dRasterPos2 = 1.;
				}

				// text y min
				if (_finite(pSS->m_dYMin) && m_zScaling != 0.)
				{
					strValue.Format("%.4g", pSS->m_dYMin);
					glRasterPos3d(dRasterPos1*distFactor, dRasterPos2*distFactor, -OneMinus);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectFirst.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectFirst.top = (int)floor(tmpCurrentRasterPosition[1]);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectFirst.right = (int)ceil(tmpCurrentRasterPosition[0]);
					rectFirst.bottom = (int)ceil(tmpCurrentRasterPosition[1] + posStrHeight);
				}

				// text y max
				if (_finite(pSS->m_dYMax) && m_zScaling != 0.)
				{
					strValue.Format("%.4g", pSS->m_dYMax);
					if (!pSS->m_strYLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strYLabel;
					glRasterPos3d(dRasterPos1*distFactor, dRasterPos2*distFactor, -OnePlus);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectLast.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectLast.top = (int)floor(tmpCurrentRasterPosition[1]);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectLast.right = (int)ceil(tmpCurrentRasterPosition[0]);
					rectLast.bottom = (int)ceil(tmpCurrentRasterPosition[1] + posStrHeight);
				}

				// text y grid
				size = coarseGridY.GetTotalSize();
				for (l = 0; l < size && m_zScaling != 0.; l++)
				{
					strValue.Format("%.10g", coarseGridY[l]);

					gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 1.-eps1, 
									modelMatrix, projMatrix, viewport, 
									&objx, &objy, &objz);
					glRasterPos3d(objx, objy, objz);
#ifdef _DEBUG
					GLboolean bCurrentRasterPositionValid;
					glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &bCurrentRasterPositionValid);
					ASSERT(bCurrentRasterPositionValid == GL_TRUE);
#endif // _DEBUG
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth = -tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth += tmpCurrentRasterPosition[0];

					glRasterPos3d(dRasterPos1*distFactor, dRasterPos2*distFactor, -coarseGridYNormalized[l]);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(.25*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectGrid.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectGrid.top = (int)floor(tmpCurrentRasterPosition[1]);
					rectGrid.right = rectGrid.left + (int)ceil(posStrWidth);
					rectGrid.bottom = rectGrid.top + (int)ceil(posStrHeight);
					if (rectTest.IntersectRect(rectGrid, rectFirst) || rectTest.IntersectRect(rectGrid, rectLast))
						continue;
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}


				// select best z axis
				gluProject(1., 0., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OnePlus || dRasterPos3Remember == OnePlus))
				{
					xMax = winx;
					dRasterPos1 = 1.;
					dRasterPos3 = 1.;
				}
				gluProject(1., 0., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OnePlus || dRasterPos3Remember == OneMinus))
				{
					xMax = winx;
					dRasterPos1 = 1.;
					dRasterPos3 = -1.;
				}
				gluProject(-1., 0., -1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OneMinus || dRasterPos3Remember == OneMinus))
				{
					xMax = winx;
					dRasterPos1 = -1.;
					dRasterPos3 = -1.;
				}
				gluProject(-1., 0., 1., 
								modelMatrix, projMatrix, viewport, 
								&winx, &winy, &winz);
				if (winx > xMax && 
					(m_nScaleShowGrid == 0 || dRasterPos1Remember == OneMinus || dRasterPos3Remember == OnePlus))
				{
					xMax = winx;
					dRasterPos1 = -1.;
					dRasterPos3 = 1.;
				}

				// text z min
				if (_finite(pSS->m_dZMin) && m_yScaling != 0.)
				{
					strValue.Format(" %.4g", pSS->m_dZMin);
					glRasterPos3d(dRasterPos1*distFactor, OneMinus, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(rotXShift*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectFirst.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectFirst.top = (int)floor(tmpCurrentRasterPosition[1]);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectFirst.right = (int)ceil(tmpCurrentRasterPosition[0]);
					rectFirst.bottom = (int)ceil(tmpCurrentRasterPosition[1] + posStrHeight);
				}

				// text z max
				if (_finite(pSS->m_dZMax) && m_yScaling != 0.)
				{
					strValue.Format(" %.4g", pSS->m_dZMax);
					if (!pSS->m_strZLabel.IsEmpty())
						strValue += _T(" ")+pSS->m_strZLabel;
					glRasterPos3d(dRasterPos1*distFactor, OnePlus, dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float((1.-rotXShift)*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectLast.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectLast.top = (int)floor(tmpCurrentRasterPosition[1]);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectLast.right = (int)ceil(tmpCurrentRasterPosition[0]);
					rectLast.bottom = (int)ceil(tmpCurrentRasterPosition[1] + posStrHeight);
				}

				// text z grid
				size = coarseGridZ.GetTotalSize();
				for (l = 0; l < size && m_yScaling != 0.; l++)
				{
					strValue.Format(" %.10g", coarseGridZ[l]);

					gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 1.-eps1, 
									modelMatrix, projMatrix, viewport, 
									&objx, &objy, &objz);
					glRasterPos3d(objx, objy, objz);
#ifdef _DEBUG
					GLboolean bCurrentRasterPositionValid;
					glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &bCurrentRasterPositionValid);
					ASSERT(bCurrentRasterPositionValid == GL_TRUE);
#endif // _DEBUG
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth = -tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth += tmpCurrentRasterPosition[0];

					glRasterPos3d(dRasterPos1*distFactor, coarseGridZNormalized[l], dRasterPos3*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float((1.-rotXShift)*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					rectGrid.left = (int)floor(tmpCurrentRasterPosition[0]);
					rectGrid.top = (int)floor(tmpCurrentRasterPosition[1]);
					rectGrid.right = rectGrid.left + (int)ceil(posStrWidth);
					rectGrid.bottom = rectGrid.top + (int)ceil(posStrHeight);
					if (rectTest.IntersectRect(rectGrid, rectFirst) || rectTest.IntersectRect(rectGrid, rectLast))
						continue;
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}

				glPopAttrib();
			}
		}

		if (m_bScaleShowAxis)
		{
			double x = QNAN, y = QNAN, z = QNAN;

			if (pSS->m_dXMax == HUGE && pSS->m_dXMin == -HUGE)
				x = 0.;
			else if (pSS->m_dXMax == HUGE)
				x = OneMinus;
			else if (pSS->m_dXMin == -HUGE)
				x = OnePlus;
			else if (pSS->m_dXMax > pSS->m_dXMin)
				x = max(OneMinus, min(OnePlus, -(pSS->m_dXMax + pSS->m_dXMin)/(pSS->m_dXMax - pSS->m_dXMin)));

			if (pSS->m_dYMax == HUGE && pSS->m_dYMin == -HUGE)
				y = 0.;
			else if (pSS->m_dYMax == HUGE)
				y = OneMinus;
			else if (pSS->m_dYMin == -HUGE)
				y = OnePlus;
			else if (pSS->m_dYMax > pSS->m_dYMin)
				y = max(OneMinus, min(OnePlus, -(pSS->m_dYMax + pSS->m_dYMin)/(pSS->m_dYMax - pSS->m_dYMin)));

			if (pSS->m_dZMax == HUGE && pSS->m_dZMin == -HUGE)
				z = 0.;
			else if (pSS->m_dZMax == HUGE)
				z = OneMinus;
			else if (pSS->m_dZMin == -HUGE)
				z = OnePlus;
			else if (pSS->m_dZMax > pSS->m_dZMin)
				z = max(OneMinus, min(OnePlus, -(pSS->m_dZMax + pSS->m_dZMin)/(pSS->m_dZMax - pSS->m_dZMin)));

			// Draw the lines themselves
			glNormal3d(0., 1., 0.);

			if (_finite(y) && _finite(z))
			{
				glBegin(GL_LINE_STRIP);
				glVertex3d(OneMinus,z,-y);
				glVertex3d(OnePlus,z,-y);
				glEnd();
			}

			if (_finite(z) && _finite(x))
			{
				glBegin(GL_LINE_STRIP);
				glVertex3d(x,z,-OnePlus);
				glVertex3d(x,z,-OneMinus);
				glEnd();
			}

			if (_finite(x) && _finite(y))
			{
				glBegin(GL_LINE_STRIP);
				glVertex3d(x,OneMinus,-y);
				glVertex3d(x,OnePlus,-y);
				glEnd();
			}

			// text x
			if (m_bScaleShowAxisLabels)
			{
				posStrHeight = abs(m_LogFont.lfHeight)*float(72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY));

				glNormal3d(0., 1., 0.);

				glListBase(m_nListBaseSmall);

				glPushAttrib(GL_CURRENT_RASTER_POSITION);

				/*if (_finite(y) && _finite(z))
				{
					strValue.Format("%s", _T("x"));

					glRasterPos3d(OnePlus*distFactor, z*distFactor, -y*distFactor);
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, 0.0f, -abs(m_LogFont.lfHeight)*float(0.*72.*m_dFontRatio/m_pDC->GetDeviceCaps(LOGPIXELSY)), &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}*/
				if (_finite(y) && _finite(z))
				{
					strValue.Format("%s", _T("x"));

					gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 1.-eps1, 
									modelMatrix, projMatrix, viewport, 
									&objx, &objy, &objz);
					glRasterPos3d(objx, objy, objz);
#ifdef _DEBUG
					GLboolean bCurrentRasterPositionValid;
					glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &bCurrentRasterPositionValid);
					ASSERT(bCurrentRasterPositionValid == GL_TRUE);
#endif // _DEBUG
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth = -tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth += tmpCurrentRasterPosition[0];

					glRasterPos3d(0., z, -y);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					diffWidth = -tmpCurrentRasterPosition[0];
					diffHeight = -tmpCurrentRasterPosition[1];
					glRasterPos3d(OnePlus*distFactor, z*distFactor, -y*distFactor);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					diffWidth += tmpCurrentRasterPosition[0];
					diffHeight += tmpCurrentRasterPosition[1];
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, (diffWidth < 0.) ? float(-posStrWidth) : 0.f, (diffHeight < 0.) ? float(-posStrHeight) : 0.f, &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}

				if (_finite(z) && _finite(x))
				{
					strValue.Format("%s", _T("y"));

					gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 1.-eps1, 
									modelMatrix, projMatrix, viewport, 
									&objx, &objy, &objz);
					glRasterPos3d(objx, objy, objz);
#ifdef _DEBUG
					GLboolean bCurrentRasterPositionValid;
					glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &bCurrentRasterPositionValid);
					ASSERT(bCurrentRasterPositionValid == GL_TRUE);
#endif // _DEBUG
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth = -tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth += tmpCurrentRasterPosition[0];

					glRasterPos3d(x,z,0.);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					diffWidth = -tmpCurrentRasterPosition[0];
					diffHeight = -tmpCurrentRasterPosition[1];
					glRasterPos3d(x*distFactor,z*distFactor,-OnePlus*distFactor);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					diffWidth += tmpCurrentRasterPosition[0];
					diffHeight += tmpCurrentRasterPosition[1];
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, (diffWidth < 0.) ? float(-posStrWidth) : 0.f, (diffHeight < 0.) ? float(-posStrHeight) : 0.f, &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}

				if (_finite(x) && _finite(y))
				{
					strValue.Format("%s", _T("z"));

					gluUnProject(viewport[0] + viewport[2]/2., viewport[1] + viewport[3]/2., 1.-eps1, 
									modelMatrix, projMatrix, viewport, 
									&objx, &objy, &objz);
					glRasterPos3d(objx, objy, objz);
#ifdef _DEBUG
					GLboolean bCurrentRasterPositionValid;
					glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &bCurrentRasterPositionValid);
					ASSERT(bCurrentRasterPositionValid == GL_TRUE);
#endif // _DEBUG
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth = -tmpCurrentRasterPosition[0];
					DrawBufferOrStencil(GL_NONE);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
					DrawBufferOrStencil(drawBuffer);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					posStrWidth += tmpCurrentRasterPosition[0];

					glRasterPos3d(x,0.,-y);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					diffWidth = -tmpCurrentRasterPosition[0];
					diffHeight = -tmpCurrentRasterPosition[1];
					glRasterPos3d(x*distFactor,OnePlus*distFactor,-y*distFactor);
					glGetDoublev(GL_CURRENT_RASTER_POSITION, tmpCurrentRasterPosition);
					diffWidth += tmpCurrentRasterPosition[0];
					diffHeight += tmpCurrentRasterPosition[1];
					// Note: LOGFONT::lfHeight = PointSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72 
					//       if map mode is MM_TEXT
					glBitmap(1, 1, 0.0f, 0.0f, (diffWidth < 0.) ? float(-posStrWidth) : 0.f, (diffHeight < 0.) ? float(-posStrHeight) : 0.f, &dummyBitmap);
					glCallLists(strValue.GetLength(), GL_UNSIGNED_BYTE, strValue);
				}

				glPopAttrib();
			}

		}
	}
	glNormal3dv(tmpNormal);
	glPolygonMode(GL_FRONT_AND_BACK, tmpPolygonMode[1]);
}

void CGlView::MakeColorsRainbowOrMonochrome(const double& fcol, double& r, double& g, double& b, int k)
{
	UNREFERENCED_PARAMETER(k);
	ASSERT(fabs(m_dInvertNonRainbowColors) == 1.);

	if (_isnan(fcol))
		r = g = b = .5;
	else if (fcol == HUGE*m_dInvertNonRainbowColors)
		r = g = b = 0.;
	else if (fcol == -HUGE*m_dInvertNonRainbowColors)
		r = g = b = 1.;
	else
	{
		if (m_bMonochromeMode)
		{
			r = m_ColorForeRed;
			g = m_ColorForeGreen;
			b = m_ColorForeBlue;
		}
		else
		{
			//r = max(0.,min(-3.5*fcol+4.,min(1.,max(0.,7.*fcol-3.))));
			//g = max(0.,min(-3.5*fcol+3.,min(1.,max(0.,7.*fcol-1.))));
			//b = max(0.,min(-7.*fcol+3.,min(1.,max(0.,3.5*fcol+0.5))));
			r = max(0.,min(-17./4.*fcol+19./4.,min(1.,max(0.,17./3.*fcol-8./3.))));
			g = max(0.,min(-17./4.*fcol+15./4.,min(1.,max(0.,17./3.*fcol-2./3.))));
			b = max(0.,min(-17./3.*fcol+8./3.,min(1.,max(0.,17./4.*fcol+1./2.))));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CGlView 

/////////////////////////////////////////////////////////////////////////////
// OnCreate
// Create OpenGL rendering context 
/////////////////////////////////////////////////////////////////////////////

int CGlView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	LOGFONT logFont;
	HGDIOBJ hOldObject;

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	logFont = m_LogFont = pApp->m_LogFont;
	logFont.lfHeight = int(m_dFontRatio*m_LogFont.lfHeight);
	logFont.lfWidth = int(m_dFontRatio*m_LogFont.lfWidth);

	hOldObject = m_FontSmall.Detach();
	if ((m_bFontCreateFlagSmall = m_FontSmall.CreateFontIndirect(&logFont)) == FALSE)
		m_FontSmall.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);

	hOldObject = m_FontNormal.Detach();
	if ((m_bFontCreateFlagNormal = m_FontNormal.CreateFontIndirect(&m_LogFont)) == FALSE)
		m_FontNormal.Attach(hOldObject);
	else if (hOldObject != NULL)
		DeleteObject(hOldObject);

	// store height/width
	m_nLogFontHeight = m_LogFont.lfHeight;
	m_nLogFontWidth = m_LogFont.lfWidth;

	InitializeOpenGL();
	return 0;
}

void CGlView::OnDestroy() 
{
	if (wglGetCurrentContext() != NULL)
		SetCurrent(NULL, NULL);

	if (m_hRC != NULL)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	if (m_pDC != NULL)
	{
		delete m_pDC;
		m_pDC = NULL;
		m_nBitsPerPixel = 0;
	}

	if (m_pDlgProperties != NULL)
	{
		delete m_pDlgProperties;
		m_pDlgProperties = NULL;
	}

	CView::OnDestroy();
}

void CGlView::OnSize(UINT nType, int cx, int cy) 
{
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	MakeActive();				//Make view's rendering context current

	// Set OpenGL perspective, viewport and mode
	double dOldAspect, dNewAspect;

	dOldAspect = min(m_aspect, 1.);
	if (cx != 0 && cy != 0)
		m_aspect = (double)cx/(double)cy;
	dNewAspect = min(m_aspect, 1.);

	ASSERT(dOldAspect != 0. && dNewAspect != 0.);
	if (dNewAspect != dOldAspect)
	{
		m_xScaling *= dNewAspect/dOldAspect;
		m_yScaling *= dNewAspect/dOldAspect;
		m_zScaling *= dNewAspect/dOldAspect;

		m_xTranslation *= dNewAspect/dOldAspect;
		m_yTranslation *= dNewAspect/dOldAspect;

		UpdateSliderScalePos();
		UpdateSliderTranslationPos();
	}

	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	CView::OnSize(nType, cx, cy);
}

BOOL CGlView::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);

	// OpenGL erases the background itself
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

BOOL CGlView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	// There seem to be problems diplaying the standard IDC_ARROW
	// cursor by the default wnd proc. So we try to do it explicitely.

	//return CView::OnSetCursor(pWnd, nHitTest, message);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}

void CGlView::OnHelpOpenGLInfo() 
{
	CDlgOpenGLInfo dlgOpenGLInfo;
	dlgOpenGLInfo.m_info = GetOpenGLInfo();
	dlgOpenGLInfo.DoModal();
}

GlInfoStruct CGlView::GetOpenGLInfo()
{
	GlInfoStruct info;
	PIXELFORMATDESCRIPTOR pfd;
	HDC hDC = NULL;
	HGLRC hRC = NULL;

	// Get information about the DC's current pixel format 
	int nGLPixelIndex = GetPixelFormat(m_pDC->GetSafeHdc());
	DescribePixelFormat(m_pDC->GetSafeHdc(), nGLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	GetCurrent(hDC, hRC);		//Store current rendering and device contexts
	MakeActive();				//Make view's rendering context current

	// Extract driver information
	if ((ICD_DRIVER_MASK & pfd.dwFlags) == 0)
	{
		info.strGlAcceleration.LoadString(IDS_STR_ACCEL_ICD);		// fully in hardware (fastest)
	}
	else if ((ICD_DRIVER_MASK & pfd.dwFlags) == ICD_DRIVER_MASK)
	{
		info.strGlAcceleration.LoadString(IDS_STR_ACCEL_MCD);		// partially in hardware (pretty fast, maybe..)
	}
	else
	{
		info.strGlAcceleration.LoadString(IDS_STR_ACCEL_SOFTWARE);	// software
	}

	// get the company name responsible for this implementation
	info.strGlVendor = (char*)::glGetString(GL_VENDOR);
	if (::glGetError() != GL_NO_ERROR)
		info.strGlVendor.Format(IDS_STR_NOT_AVAIL);				// failed!

	// get the renderer name; this is specific of an hardware configuration
	info.strGlRenderer = (char*)::glGetString(GL_RENDERER);
	if (::glGetError() != GL_NO_ERROR)
		info.strGlRenderer.Format(IDS_STR_NOT_AVAIL);			// failed!

	// get the version of the GL library
	info.strGlVersion = (char*)::glGetString(GL_VERSION);
	if (::glGetError() != GL_NO_ERROR)
		info.strGlVersion.Format(IDS_STR_NOT_AVAIL);			// failed!

	// return a space separated list of extensions
	info.strGlExtensions = (char*)::glGetString(GL_EXTENSIONS);
	if (::glGetError() != GL_NO_ERROR)
		info.strGlExtensions.Format(IDS_STR_NOT_AVAIL);			// failed!

	// get the version of the GLU library
	info.strGluVersion = (char*)::gluGetString(GLU_VERSION);
	if (::glGetError() != GL_NO_ERROR)
		info.strGluVersion.Format(IDS_STR_NOT_AVAIL);			// failed!

	// as above a space separated list of extensions
	info.strGluExtensions = (char*)::gluGetString(GLU_EXTENSIONS);
	if (::glGetError() != GL_NO_ERROR)
		info.strGluExtensions.Format(IDS_STR_NOT_AVAIL);		// failed!

	glGetIntegerv(GL_MAX_LIGHTS, &info.nMaxLights);
	glGetIntegerv(GL_MAX_CLIP_PLANES, &info.nMaxClipPlanes);
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &info.nMaxModelStackDepth);
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &info.nMaxProjectionStackDepth);
	glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &info.nMaxTextureStackDepth);
	glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &info.nMaxNameStackDepth);
	glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &info.nMaxAttributeStackDepth);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info.nMaxTextureSize);
	glGetIntegerv(GL_MAX_LIST_NESTING, &info.nMaxListNesting);
	glGetIntegerv(GL_MAX_EVAL_ORDER, &info.nMaxEvalOrder);
	int tmpInt[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tmpInt);
	info.maxViewportDims.cx = tmpInt[0];
	info.maxViewportDims.cy = tmpInt[1];
	glGetIntegerv(GL_AUX_BUFFERS, &info.nAuxiliaryBuffers);
	float tmpFloat[2];
	glGetFloatv(GL_POINT_SIZE_RANGE, tmpFloat);
	info.minSmoothPointSize = tmpFloat[0];
	info.maxSmoothPointSize = tmpFloat[1];
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &info.smoothPointGranularity);
	glGetFloatv(GL_LINE_WIDTH_RANGE, tmpFloat);
	info.minSmoothLineSize = tmpFloat[0];
	info.maxSmoothLineSize = tmpFloat[1];
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &info.smoothLineGranularity);
	glGetIntegerv(GL_RED_BITS, &info.nRedBits);
	glGetIntegerv(GL_GREEN_BITS, &info.nGreenBits);
	glGetIntegerv(GL_BLUE_BITS, &info.nBlueBits);
	glGetIntegerv(GL_ALPHA_BITS, &info.nAlphaBits);
	glGetIntegerv(GL_DEPTH_BITS, &info.nDepthBits);
	glGetIntegerv(GL_STENCIL_BITS, &info.nStencilBits);

	//Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	return info;
}

void CGlView::PrepareBkgnd(BOOL bBegin) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	if (bBegin)
	{
		if (pApp->m_bPrintBkgndWhite)
		{
			// store original colors
			m_ColorBackRedTmp   = m_ColorBackRed;
			m_ColorBackGreenTmp = m_ColorBackGreen;
			m_ColorBackBlueTmp  = m_ColorBackBlue;
			m_ColorForeRedTmp   = m_ColorForeRed;
			m_ColorForeGreenTmp = m_ColorForeGreen;
			m_ColorForeBlueTmp  = m_ColorForeBlue;
			m_ColorFontRedTmp   = m_ColorFontRed;
			m_ColorFontGreenTmp = m_ColorFontGreen;
			m_ColorFontBlueTmp  = m_ColorFontBlue;

			// change colors
			m_ColorBackRed   = 1.0f;
			m_ColorBackGreen = 1.0f;
			m_ColorBackBlue  = 1.0f;
			CheckColors(m_ColorBackRed, m_ColorBackGreen, m_ColorBackBlue, m_ColorForeRed, m_ColorForeGreen, m_ColorForeBlue);
			CheckColors(m_ColorBackRed, m_ColorBackGreen, m_ColorBackBlue, m_ColorFontRed, m_ColorFontGreen, m_ColorFontBlue);

			m_dInvertNonRainbowColors = -1.;
		}

		if (pApp->m_nPrintObject == 1)
		{
			CDocument* pDoc = GetDocument();
			CView *pView;
			POSITION pos;

			pos = pDoc->GetFirstViewPosition();
			while (pos != NULL)
			{
				pView = pDoc->GetNextView(pos);
				if (pView != this && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					// store original colors
					((CGlView*)pView)->m_ColorBackRedTmp   = ((CGlView*)pView)->m_ColorBackRed;
					((CGlView*)pView)->m_ColorBackGreenTmp = ((CGlView*)pView)->m_ColorBackGreen;
					((CGlView*)pView)->m_ColorBackBlueTmp  = ((CGlView*)pView)->m_ColorBackBlue;
					((CGlView*)pView)->m_ColorForeRedTmp   = ((CGlView*)pView)->m_ColorForeRed;
					((CGlView*)pView)->m_ColorForeGreenTmp = ((CGlView*)pView)->m_ColorForeGreen;
					((CGlView*)pView)->m_ColorForeBlueTmp  = ((CGlView*)pView)->m_ColorForeBlue;
					((CGlView*)pView)->m_ColorFontRedTmp   = ((CGlView*)pView)->m_ColorFontRed;
					((CGlView*)pView)->m_ColorFontGreenTmp = ((CGlView*)pView)->m_ColorFontGreen;
					((CGlView*)pView)->m_ColorFontBlueTmp  = ((CGlView*)pView)->m_ColorFontBlue;

					// change colors
					((CGlView*)pView)->m_ColorBackRed   = m_ColorBackRed;
					((CGlView*)pView)->m_ColorBackGreen = m_ColorBackGreen;
					((CGlView*)pView)->m_ColorBackBlue  = m_ColorBackBlue;
					((CGlView*)pView)->m_ColorForeRed   = m_ColorForeRed;
					((CGlView*)pView)->m_ColorForeGreen = m_ColorForeGreen;
					((CGlView*)pView)->m_ColorForeBlue  = m_ColorForeBlue;
					((CGlView*)pView)->m_ColorFontRed   = m_ColorFontRed;
					((CGlView*)pView)->m_ColorFontGreen = m_ColorFontGreen;
					((CGlView*)pView)->m_ColorFontBlue  = m_ColorFontBlue;

					((CGlView*)pView)->m_dInvertNonRainbowColors = m_dInvertNonRainbowColors;
				}
			}
		}
	}
	else
	{
		if (pApp->m_bPrintBkgndWhite)
		{
			// restore original colors
			m_ColorBackRed   = m_ColorBackRedTmp;
			m_ColorBackGreen = m_ColorBackGreenTmp;
			m_ColorBackBlue  = m_ColorBackBlueTmp;
			m_ColorForeRed   = m_ColorForeRedTmp;
			m_ColorForeGreen = m_ColorForeGreenTmp;
			m_ColorForeBlue  = m_ColorForeBlueTmp;
			m_ColorFontRed   = m_ColorFontRedTmp;
			m_ColorFontGreen = m_ColorFontGreenTmp;
			m_ColorFontBlue  = m_ColorFontBlueTmp;

			m_dInvertNonRainbowColors = 1.;
			InvalidateRect(NULL, FALSE);
		}

		if (pApp->m_nPrintObject == 1)
		{
			CDocument* pDoc = GetDocument();
			CView *pView;
			POSITION pos;

			pos = pDoc->GetFirstViewPosition();
			while (pos != NULL)
			{
				pView = pDoc->GetNextView(pos);
				if (pView != this && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					// restore original colors
					((CGlView*)pView)->m_ColorBackRed   = ((CGlView*)pView)->m_ColorBackRedTmp;
					((CGlView*)pView)->m_ColorBackGreen = ((CGlView*)pView)->m_ColorBackGreenTmp;
					((CGlView*)pView)->m_ColorBackBlue  = ((CGlView*)pView)->m_ColorBackBlueTmp;
					((CGlView*)pView)->m_ColorForeRed   = ((CGlView*)pView)->m_ColorForeRedTmp;
					((CGlView*)pView)->m_ColorForeGreen = ((CGlView*)pView)->m_ColorForeGreenTmp;
					((CGlView*)pView)->m_ColorForeBlue  = ((CGlView*)pView)->m_ColorForeBlueTmp;
					((CGlView*)pView)->m_ColorFontRed   = ((CGlView*)pView)->m_ColorFontRedTmp;
					((CGlView*)pView)->m_ColorFontGreen = ((CGlView*)pView)->m_ColorFontGreenTmp;
					((CGlView*)pView)->m_ColorFontBlue  = ((CGlView*)pView)->m_ColorFontBlueTmp;

					((CGlView*)pView)->m_dInvertNonRainbowColors = 1.;
					((CGlView*)pView)->InvalidateRect(NULL, FALSE);
				}
			}
		}
	}
}

void CGlView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CView::OnSettingChange(uFlags, lpszSection);

	CString strSection;
	strSection.LoadString(IDS_SETTINGCHANGE_LPARAM_FORCE);
	if (m_pDC->GetDeviceCaps(BITSPIXEL) != m_nBitsPerPixel || (lpszSection && strSection.Compare(lpszSection) == 0))
	{
		// Normally the parent window sends a 
		// 		SendMessageToDescendants(WM_SETTINGCHANGE, ...);
		// message to all it's child windows, which calls OnSettingChange() for all views.
		//
		// If we recreating this view at this time, then the 
		// WM_SETTINGCHANGE message gets lost for the following views.
		// So recreate the view by a timer.
		SetTimer(WM_SETTINGCHANGE, 0, NULL);
	}
}

void CGlView::OnEditCopyEMF()
{
	BeginWaitCursor();

	CRect rect;
	GetClientRect(&rect);

	// Create metafile device context
	HDC hMetaDC = CreateEnhMetaFile(m_pDC->GetSafeHdc(), NULL, &rect, NULL);
	if(!hMetaDC)
	{
		TRACE0("Unable to create MetaFile.");
		EndWaitCursor();
		return;
	}

	HGLRC hMetaRC = wglCreateContext(hMetaDC);

	// Store current rendering and device contexts
	HDC hDC = NULL;
	HGLRC hRC = NULL;
	GetCurrent(hDC, hRC);

	SetCurrent(hMetaDC, hMetaRC);

	// Do rendering
	CDC *pDummyDC = GetDC();
	pDummyDC->m_bPrinting = TRUE;	// this does the trick in OnDraw: it prevents changing rendering context and swapping buffers
	OnDraw(pDummyDC);
	ReleaseDC(pDummyDC);
	glFinish();	// Finish all OpenGL commands

	// Close metafile
	HENHMETAFILE hMetaFile = CloseEnhMetaFile(hMetaDC);

	// The rendering context will be no longer needed
	SetCurrent(NULL, NULL);
	wglDeleteContext(hMetaRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		SetCurrent(hDC, hRC);

	// Fill the clipboard
	if (OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData(CF_ENHMETAFILE, hMetaFile);
		CloseClipboard();
	}

	EndWaitCursor();
}

void CGlView::OnEditPasteEMF()
{
	CRect rect;				// Window rectangle
	HENHMETAFILE hMetaFile;	// Metafile handle

	if (!IsClipboardFormatAvailable(CF_ENHMETAFILE))
		return;

	// Get dimensions of the current window
	GetClientRect(&rect);

	// Open the metafile
	OpenClipboard();
	hMetaFile = (HENHMETAFILE)GetClipboardData(CF_ENHMETAFILE);

	// Restore original aspect in OpenGL manner
	int nWndWidth, nMetaWidth;
	ENHMETAHEADER emhdr;
	GetEnhMetaFileHeader(hMetaFile, sizeof(ENHMETAHEADER), &emhdr);
	nWndWidth = rect.Width();
	rect.right = (int)::round(double(rect.bottom*(emhdr.rclFrame.right-emhdr.rclFrame.left))/double(emhdr.rclFrame.bottom-emhdr.rclFrame.top));
	nMetaWidth = rect.Width();
	rect.left -= (nMetaWidth - nWndWidth)/2;
	rect.right -= (nMetaWidth - nWndWidth)/2;

	// Play the metafile into the OpenGL context
	PlayEnhMetaFile(m_pDC->GetSafeHdc(), hMetaFile, &rect);
	CloseClipboard();

	// Swap buffers
	if (emhdr.bOpenGL)
		SwapBuffers(m_pDC->GetSafeHdc());

	CString strMsg, title;
	title.LoadString(IDS_MB_NOTE_TITLE);
	strMsg.LoadString(IDS_MB_PASTE_EMF_MSG);

	::MessageBox(m_hWnd, strMsg, title, MB_OK | MB_ICONINFORMATION);
	InvalidateRect(NULL, FALSE);
}

void CGlView::OnEditExportPs()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	CString strTitle, strFilename;
	CPsRenderer PsRenderer;
	UINT nFilterIndex;
	RECT rect;
	PRINTDLG PrtDlg;	// Printer dialog

	// Prepare PsRenderer
	if (!pApp->GetPrinterDeviceDefaults(&PrtDlg))
	{
		TRACE("No default printer.\n");
		nFilterIndex = 2;	// EPS
	}
	else
	{
		// Don't display the dialog.  We create the CPrintDialog so that
		// we can create a printer device context.
		CPrintDialog dlg(FALSE);

		dlg.m_pd.hDevMode  = PrtDlg.hDevMode;
		dlg.m_pd.hDevNames = PrtDlg.hDevNames;

		HDC hdc = dlg.CreatePrinterDC();

		// Get the size of the default printer page
		rect.top	= 0;
		rect.left   = 0;
		rect.right  = (72*GetDeviceCaps(hdc, HORZRES))/GetDeviceCaps(hdc, LOGPIXELSX);
		rect.bottom = (72*GetDeviceCaps(hdc, VERTRES))/GetDeviceCaps(hdc, LOGPIXELSY);

		PsRenderer.SetBoundingBox(&rect);

		// Free the device context
		DeleteDC(hdc);

		// Get printer orientation
		LPDEVMODE pDevMode = dlg.GetDevMode();
		if (pDevMode)
		{
			PsRenderer.SetOrientation(pDevMode->dmOrientation);
			GlobalUnlock(pDevMode);
		}
		nFilterIndex = 1;	// PS
	}

	// Generate the file name
	int nPos = 0, nCount = 0;
	CDocument* pDoc = GetDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
		{
			nCount++;
			if (pView == this)
			{
				nPos = nCount;
			}
		}
	}
	strFilename = pDoc->GetTitle();
	if (strFilename.ReverseFind('*') > -1)
		strFilename = strFilename.Left(strFilename.ReverseFind('*'));
	if (strFilename.ReverseFind('.') > -1)
		strFilename = strFilename.Left(strFilename.ReverseFind('.'));
	if (nCount > 1)
	{
		// strTitle used as tmp string
		strTitle = strFilename;
		strFilename.Format("%s_%d", strTitle, nPos);
	}
	strFilename += (nFilterIndex == 2) ? ".eps" : ".ps";

	// Show file dialog
	static char BASED_CODE szFilter[512];
	AfxLoadString(IDS_FILE_DLG_EXPORT_PS, szFilter, 512);
	UINT nFilterCount = (CharCount(szFilter, '|')-1)/2;

	CFileDialogEx dlgExport(FALSE, "", strFilename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	strTitle.LoadString(IDS_FILE_DLG_EXPORT_PS_TITLE);
	dlgExport.m_ofn.lpstrTitle = strTitle;
	dlgExport.m_ofn.nFilterIndex = nFilterIndex;
	if (dlgExport.m_ofn.nFilterIndex > nFilterCount)
		dlgExport.m_ofn.nFilterIndex = 0;
	if (dlgExport.DoModal() == IDOK)
	{
		BeginWaitCursor();

		// Allocate buffer
		//	no way to predict this, you may change it 
		//	for large meshes. 
		const int size = (int)6e6;
		GLfloat *pFeedbackBuffer = new GLfloat[size];
		ASSERT(pFeedbackBuffer);

		// Store current rendering and device contexts
		HDC hDC = NULL;
		HGLRC hRC = NULL;
		GetCurrent(hDC, hRC);

		MakeActive();

		// Set feedback mode
		glFeedbackBuffer(size, GL_3D_COLOR, pFeedbackBuffer);
		glRenderMode(GL_FEEDBACK);

		// Do rendering
		CDC *pDummyDC = GetDC();
		pDummyDC->m_bPrinting = TRUE;	// this does the trick in OnDraw: it prevents changing rendering context and swapping buffers
		OnDraw(pDummyDC);
		ReleaseDC(pDummyDC);
		glFinish();	// Finish all OpenGL commands

		// Set render mode again
		int nValues = glRenderMode(GL_RENDER);
		TRACE1("Note: %d entries were placed in the feedback buffer\n", nValues);

		// Restore last rendering and device contexts
		if (hDC != NULL && hRC != NULL)
			SetCurrent(hDC, hRC);

		// Export feedback buffer to EPS
		CString strPathName = dlgExport.GetPathName();
		PsRenderer.Run(strPathName, TRUE, nValues, pFeedbackBuffer);

		// Cleanup buffer
		delete [] pFeedbackBuffer;

		EndWaitCursor();
	}
}

/////////////////////////////////////////////////////////////////////////////
// OnTimer
/////////////////////////////////////////////////////////////////////////////
// Currently animation timer
/////////////////////////////////////////////////////////////////////////////

void CGlView::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 1:
		// Rotation
		m_yRotation += 2.5;
		InvalidateRect(NULL, FALSE);
		break;
	case WM_SETTINGCHANGE:
		RecreateView();
		KillTimer(WM_SETTINGCHANGE);
		break;
	default:
		{}
	}

	CView::OnTimer(nIDEvent);
}

void CGlView::RecreateView()
{
	if (wglGetCurrentContext() != NULL)
		SetCurrent(NULL, NULL);

	if (m_hRC != NULL)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	if (m_pDC != NULL)
	{
		delete m_pDC;
		m_pDC = NULL;
		m_nBitsPerPixel = 0;
	}

	// Must temporarily remove the doc so MFC won't have a conniption.
	CDocument* pDocSave = m_pDocument;
	m_pDocument = NULL; // remove

	// Preserve same window ID (usually AFX_IDW_PANE_FIRST)
	UINT nIDView = GetDlgCtrlID();

	// Preserve same parent (frame)
	CWnd* pParent = GetParent();
	ASSERT(pParent);

	// Preserve same window pos and style
	CRect rect;
	GetWindowRect(&rect);
	pParent->ScreenToClient(&rect);
	DWORD dwStyle = GetStyle();

	// Remove the current view (window) and destroy it.
	HWND hWnd = UnsubclassWindow();
	ASSERT(hWnd);
	::DestroyWindow(hWnd);

	// Now the view is in more or less the same state as after calling
	//
	//		new CGlView;
	//
	// There is no window, no m_hWnd, and no document. So now we can 
	// re-create the view. 
	// Don't need to create context since document already exists.
	VERIFY(Create(NULL, NULL, dwStyle, rect, pParent, nIDView, NULL));

	// restore document
	m_pDocument = pDocSave;
}
