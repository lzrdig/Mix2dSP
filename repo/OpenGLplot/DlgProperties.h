/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@jenoptik.com>
// created: July 1998
//
// based on a sample of
// Pierre Alliez <pierre.alliez@cnet.francetelecom.fr>
//
/////////////////////////////////////////////////////////////////////////////
// DlgProperties.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGPROPERTIES_H__EC509621_22F2_11D2_8A55_00E0290F649D__INCLUDED_)
#define AFX_DLGPROPERTIES_H__EC509621_22F2_11D2_8A55_00E0290F649D__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DialogTts.h"
#include "StaticExtra.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgProperties 

class AFX_EXT_CLASS_GENERAL CDlgProperties : public CDialogTts
{
// Konstruktion
public:
	CDlgProperties(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgProperties)
	enum { IDD = IDD_DLG_PROPERTIES };
	CStatic	m_ForeTextStatic;
	CStatic	m_LightTextStatic;
	CSliderCtrl	m_ScaleZSlider;
	CSliderCtrl	m_ScaleYSlider;
	CSliderCtrl	m_ScaleXSlider;
	CSliderCtrl	m_TranslationZSlider;
	CSliderCtrl	m_TranslationYSlider;
	CSliderCtrl	m_TranslationXSlider;
	CStaticExtra	m_ColorLightAmbientStatic;
	CStaticExtra	m_ColorLightDiffuseStatic;
	CStaticExtra	m_ColorLightSpecularStatic;
	CStaticExtra	m_ColorBackStatic;
	CStaticExtra	m_ColorForeStatic;
	CStaticExtra	m_ColorFontStatic;
	CButton	m_CheckLinkScaleButton;
	CButton	m_CheckLinkTranslationButton;
	CButton	m_CheckVrotationButton;
	CButton	m_CheckSmoothButton;
	CButton	m_CheckLightingButton;
	CButton	m_CheckAntialiasButton;
	CButton	m_CheckRemoveHiddenButton;
	CButton	m_RadioRainbowButton;
	CButton	m_RadioMonochromeButton;
	CButton	m_RadioModel0Button;
	CButton	m_RadioModel1Button;
	CButton	m_RadioModel2Button;
	CButton	m_CheckScaleShowFrameButton;
	CButton	m_CheckScaleShowLabelsButton;
	CButton	m_CheckScaleShowGridButton;
	CButton	m_CheckScaleShowAxisButton;
	CButton	m_CheckScaleShowAxisLabelsButton;
	CStatic m_ScaleZStatic;
	CStatic m_TranslationZStatic;
	CButton m_VrotationGboxButton;
	CButton m_ScaleGboxButton;
	BOOL	m_bScaleShowAxisLabels;
	BOOL	m_bScaleShowAxis;
	int		m_nScaleShowGrid;
	BOOL	m_bScaleShowLabels;
	BOOL	m_bScaleShowFrame;
	BOOL	m_bBlackLinesOrRemoveHidden;
	BOOL	m_bLighting;
	BOOL	m_bSmooth;
	BOOL	m_bAntialias;
	BOOL	m_bMonochromeMode;
	int		m_nPolygonMode;
	BOOL	m_bVRotate;
	BOOL	m_bLinkScale;
	BOOL	m_bLinkTranslation;
	//}}AFX_DATA
protected:
	BOOL	m_bMonochromeModeTmp;
	BOOL	m_bMonochromeModeIsSwapped;

// Attributes
public:
	void UpdateScale(CWnd* pSender);
	void UpdateTranslation(CWnd* pSender);
	void SetGlView(CView* pView);
	CView* GetGlView();
	virtual void Update();

	BOOL m_bWasAlreadyVisible;
	COLORREF m_OptionColorFont;
	COLORREF m_OptionColorFore;
	COLORREF m_OptionColorBack;
	COLORREF m_OptionColorLightAmbient;
	COLORREF m_OptionColorLightDiffuse;
	COLORREF m_OptionColorLightSpecular;

protected:
	CView* m_pGlView;

// Operations
public:
	void ChangeFontColor();
	void ChangeForeColor();
	void ChangeBackColor();
	void ChangeAmbientColor();
	void ChangeDiffuseColor();
	void ChangeSpecularColor();
	void CheckAntialias();
	void CheckSmooth();
	void CheckLighting();
	void CheckRemoveHidden();
	void RadioChrome(BOOL bUseWinAppValue = TRUE);
	void RadioModel();
	void CheckScaleShowFrame();
	void CheckScaleShowLabels();
	void CheckScaleShowGrid();
	void CheckScaleShowAxis();
	void CheckScaleShowAxisLabels();
	void UpdateSliderScalePos();
	void UpdateSliderTranslationPos();
	void UpdateFontColorRect();

	void EnableForeCtrls(BOOL bEnable);
	void EnableLightCtrls(BOOL bEnable);
	void LeftCenterWindow(CWnd* pAlternateOwner = NULL);

	void SwapMonochromeModeVariables();

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckScaleShowAxisLabels();
	afx_msg void OnCheckScaleShowAxis();
	afx_msg void OnCheckScaleShowGrid();
	afx_msg void OnCheckScaleShowLabels();
	afx_msg void OnCheckScaleShowFrame();
	afx_msg void OnRadioModel0();
	afx_msg void OnRadioModel1();
	afx_msg void OnRadioModel2();
	afx_msg void OnCheckRemoveHidden();
	afx_msg void OnCheckLighting();
	afx_msg void OnCheckSmooth();
	afx_msg void OnCheckAntialias();
	afx_msg void OnRadioMonochrome();
	afx_msg void OnRadioRainbow();
	afx_msg void OnCheckVrotation();
	afx_msg void OnCheckLinkScale();
	afx_msg void OnCheckLinkTranslation();
	afx_msg void OnFrameColorFont();
	afx_msg void OnFrameColorFore();
	afx_msg void OnFrameColorBack();
	afx_msg void OnFrameColorLightAmbient();
	afx_msg void OnFrameColorLightDiffuse();
	afx_msg void OnFrameColorLightSpecular();
	afx_msg void OnEditFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DLGPROPERTIES_H__EC509621_22F2_11D2_8A55_00E0290F649D__INCLUDED_)
