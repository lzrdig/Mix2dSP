/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 1999
//
// special thanks for the printing routines to
// Craig Fahrnbach <craignan@home.com>
//
/////////////////////////////////////////////////////////////////////////////
// GlView.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_GLVIEW_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "Matrix.h"
#include "Scale.h"
#include "Helper.h"

using namespace mymathfuncs;


class CDlgProperties;

class AFX_EXT_CLASS_GENERAL CGlView : public CView
{
friend class CGlDocument;
friend class CDlgProperties;
friend class CFormPropertiesView;

	DECLARE_DYNAMIC(CGlView)
public:
	CGlView();

// Attributes
public:
	CPalette m_Palette;
	BOOL m_bIsDocPrinting;
	BOOL m_bBlackLinesOrRemoveHidden;
	BOOL m_bScaleShowFrame;
	BOOL m_bScaleShowLabels;
	int m_nScaleShowGrid;
	BOOL m_bScaleShowAxis;
	BOOL m_bScaleShowAxisLabels;
	int m_DocPrintHeight;

	CDlgProperties* m_pDlgProperties;

	// OpenGL specific
	CDC* m_pDC;
	HGLRC m_hRC;
	int m_nBitsPerPixel;
	BOOL m_bMonochromeMode;

	// Font
	LOGFONT m_LogFont;
	int m_nLogFontHeight;
	int m_nLogFontWidth;
	CFont m_FontSmall;
	CFont m_FontNormal;
	BOOL m_bFontCreateFlagSmall;
	BOOL m_bFontCreateFlagNormal;
	double m_dFontRatio;

protected:
	// Colors
	float m_ColorFontRed;
	float m_ColorFontGreen;
	float m_ColorFontBlue;
	float m_ColorFontRedTmp;
	float m_ColorFontGreenTmp;
	float m_ColorFontBlueTmp;
	float m_ColorForeRed;
	float m_ColorForeGreen;
	float m_ColorForeBlue;
	float m_ColorForeRedTmp;
	float m_ColorForeGreenTmp;
	float m_ColorForeBlueTmp;
	float m_ColorBackRed;
	float m_ColorBackGreen;
	float m_ColorBackBlue;
	float m_ColorBackRedTmp;
	float m_ColorBackGreenTmp;
	float m_ColorBackBlueTmp;
	float m_ColorLightAmbientRed;
	float m_ColorLightAmbientGreen;
	float m_ColorLightAmbientBlue;
	float m_ColorLightDiffuseRed;
	float m_ColorLightDiffuseGreen;
	float m_ColorLightDiffuseBlue;
	float m_ColorLightSpecularRed;
	float m_ColorLightSpecularGreen;
	float m_ColorLightSpecularBlue;

	double m_dInvertNonRainbowColors;

	// OpenGL specific
	int m_nListBaseSmall;
	int m_nListBaseNormal;

	// Position, rotation ,scaling
	double m_xRotation;
	double m_yRotation;
	double m_xTranslation;
	double m_yTranslation;
	double m_zTranslation;
	double m_xScaling;
	double m_yScaling;
	double m_zScaling;
	double m_xRotationStore;
	double m_yRotationStore;

	// Rendering
	CPtrArray m_XRenderArray;
	CPtrArray m_YRenderArray;
	CPtrArray m_ZRenderArray;
	CPtrArray m_CRenderArray;
	CPtrArray m_SRenderArray;

	// Mouse
	HCURSOR m_hCursorDblCross;
	CPoint m_LeftDownPos;
	BOOL m_bLastActivateWasDoneByLButton;

	GLdouble m_xMin;
	GLdouble m_yMin;
	GLdouble m_xMax;
	GLdouble m_yMax;

	CArray<CMatrix, CMatrix&> m_ZoomHistory;

	BOOL m_bFirstPickPoint;
	BOOL m_bSecondPickPoint;

	CMatrix m_StartPoint;
	CMatrix m_EndPoint;

	// Size
	double m_aspect;

	// Printing
	BOOL m_bStretchDIBits;
	BOOL m_bStretchBlt;

// Operations
public:
	virtual double GetExtraXScaling() const;
	virtual double GetExtraYScaling() const;
	virtual double GetExtraZScaling() const;
	virtual double GetExtraXTranslation() const;
	virtual double GetExtraYTranslation() const;
	virtual double GetExtraZTranslation() const;
	virtual void UpdateSliderScalePos() = 0;
	virtual void UpdateSliderTranslationPos() = 0;
	virtual void UpdateFontColorRect() = 0;
	virtual BOOL Is3D() const = 0;
	virtual BOOL AllowScale() const = 0;
	virtual BOOL AllowMonochrome() const = 0;
	virtual BOOL AllowRemoveHidden() const = 0;
	virtual BOOL AllowModel() const = 0;
	virtual void EditProperties() = 0;
	virtual void UpdateEditProperties(CCmdUI* pCmdUI) = 0;

	// Position, rotation ,scaling
	virtual void InitGeometry();

	// Mouse
	virtual void AddToZoomHistory();
	virtual BOOL GetFromZoomHistory();

	// Text
	void OptionsFont();
	void ScaleFont(double scaleFac);
	void UnScaleFont();
	void CreateFontBitmaps();

	// Colors
	static BOOL CheckColors(const COLORREF& ColorBack, COLORREF& ColorFore);
	static BOOL CheckColors(const float& ColorBackRed, const float& ColorBackGreen, const float& ColorBackBlue, float& ColorForeRed, float& ColorForeGreen, float& ColorForeBlue);
	void CreateRGBPalette();
	void PrepareBkgnd(BOOL bBegin);

	// Plotting
	void Plot2d(mymathfuncs::CMatrix* pXX, mymathfuncs::CMatrix* pYY, mymathfuncs::CMatrix* pCC);
	void Plot2d(CMatrix& matX, CMatrix& matY, CMatrix& matC);
	void Plot2d(CMatrix* pXX, CMatrix* pYY);
	void Plot2d(CMatrix& matX, CMatrix& matY);
	void Plot2d(CMatrix* pYY);
	void Plot2d(CMatrix& matY);
	void Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC);
	void Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC);
	void Plot3d(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ);
	void Plot3d(CMatrix& matX, CMatrix& matY, CMatrix& matZ);
	void Plot3d(CMatrix* pZZ);
	void Plot3d(CMatrix& matZ);
	void Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ, CMatrix* pCC);
	void Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ, CMatrix& matC);
	void Surf(CMatrix* pXX, CMatrix* pYY, CMatrix* pZZ);
	void Surf(CMatrix& matX, CMatrix& matY, CMatrix& matZ);
	void Surf(CMatrix* pZZ);
	void Surf(CMatrix& matZ);
	void TopView(CMatrix* pXX, CMatrix* pYY, CMatrix* pCC);
	void TopView(CMatrix& matX, CMatrix& matY, CMatrix& matC);
	void TopView(CMatrix* pXX, CMatrix* pYY);
	void TopView(CMatrix& matX, CMatrix& matY);
	void TopView(CMatrix* pCC);
	void TopView(CMatrix& matC);
	void Scale2d(CScale* pSS);
	void Scale2d(CScale& matS);
	void Scale3d(CScale* pSS);
	void Scale3d(CScale& matS);

protected:
	virtual void OnDraw(CDC* pDC) = 0;

	void RecreateView();

	BOOL EditCopy(CRect mRect, int nReduceResCount);

	// OpenGL specific
	virtual BOOL InitializeOpenGL();
	void SetOpenGLProperties();
	void SetBackColor();
	void SetAmbientColor();
	void SetDiffuseColor();
	void SetSpecularColor();
	int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd, DWORD dwRemove = 0);
	BOOL SetWindowPixelFormat(BOOL bUseAPI = FALSE);
	BOOL SetMemDcPixelFormat(HDC hMemDC, BOOL bUseAPI = FALSE);
	BOOL CreateViewGLContext();
	BOOL MakeActive();
	void GetCurrent(HDC& hDC, HGLRC& hRC);
	BOOL SetCurrent(HDC hDC, HGLRC hRC);
	GlInfoStruct GetOpenGLInfo();
	void DrawBufferOrStencil(GLenum mode);

	// Mouse
	void DrawZoomRect();
	void DrawRubberBand();
	void DrawHorizontalBands();
	void DrawVerticalBands();

	// Rendering
	virtual void RenderScenePlot2d();
	virtual void RenderScenePlot3d();
	virtual void RenderSceneSurf();
	virtual void RenderSceneTopView();
	virtual void RenderSceneScale2d();
	virtual void RenderSceneScale3d();
	virtual void MakeColorsRainbowOrMonochrome(const double& fcol, double& r, double& g, double& b, int k = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGlView();

#if _MSC_VER < 1200
protected:
	// Advanced: for implementing custom print preview
	BOOL DoPrintPreview(UINT nIDResource, CView* pPrintView,
			CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState);
	afx_msg void OnFilePrintPreview();
#endif // _MSC_VER < 1200

// Generated message map functions
protected:
	//{{AFX_MSG(CGlView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnEditUndo();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCopyEMF();
	afx_msg void OnEditPasteEMF();
	afx_msg void OnEditExportPs();
	afx_msg void OnEditFont();
	afx_msg void OnHelpOpenGLInfo();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline void CGlView::GetCurrent(HDC& hDC, HGLRC& hRC)
{
	hDC = wglGetCurrentDC();
	hRC = wglGetCurrentContext();
}

inline BOOL CGlView::SetCurrent(HDC hDC, HGLRC hRC)
{
	return wglMakeCurrent(hDC, hRC);
}

inline double CGlView::GetExtraXScaling() const
	{ return 1.; }
inline double CGlView::GetExtraYScaling() const
	{ return 1.; }
inline double CGlView::GetExtraZScaling() const
	{ return 1.; }
inline double CGlView::GetExtraXTranslation() const
	{ return 0.; }
inline double CGlView::GetExtraYTranslation() const
	{ return 0.; }
inline double CGlView::GetExtraZTranslation() const
	{ return 0.; }

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1200
class CGlPreviewView : public CPreviewView
{
	DECLARE_DYNCREATE(CGlPreviewView)
	friend class CGlView;
};
#endif // _MSC_VER < 1200

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_GLVIEW_H__06A35633_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
