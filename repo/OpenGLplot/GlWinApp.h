/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlWinApp.h : Header-Datei für alle OPENGL Windows Anwendungen
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWINAPP_H__D02404F7_14C5_11D2_B619_0000C0F55FDF__INCLUDED_)
#define AFX_GLWINAPP_H__D02404F7_14C5_11D2_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'StdAfx.h' before including this file for PCH
#endif

#include "Resource.h"       // Hauptsymbole

// operating system
//	do not edit since we do 
//	maths upon these constants
typedef enum tagOSRUNNING 
{
	OSUnknown = 0,
	OSWin3 = 1, 
	OSWin95 = 2, 
	OSWin98 = 3, 
	OSWinME = 4, 
	OSWinNT = 5, 
	OSWin2K = 6,
	OSWinXP = 7,
	OSWin2K3 = 8,
	OSWinVista = 9
}
OSRUNNING;

#define IDBACK								0

// properties
#define POLICY_NO_MORE_ZOOM					0
#define POLICY_MOUSE						1
#define POLICY_HARDWARE_ACCELERATION		2
#define POLICY_PROP							3
#define POLICY_REOPEN						4
#define POLICY_SAVE_RESULT					5
#define POLICY_RESOLUTION_REDUCED			6

// file watching
#define FA_FILE_WRITTEN						1
#define FA_FILE_LOST						2

// private window messages
#define WM_SHOW_PROGRESS					(WM_APP + 10001)
#define WM_ENABLE_SYSTEM					(WM_APP + 10002)
#define WM_DISABLE_SYSTEM					(WM_APP + 10003)
#define WM_ADJUST_PROPERTY_SHEET			(WM_APP + 10004)
#define WM_CALCULATE_IN_THREAD_DONE			(WM_APP + 10005)
#define WM_DOC_WAS_MODIFIED_BY_THREAD		(WM_APP + 10006)
#define WM_FILE_ALARM						(WM_APP + 10007)

#define DMORIENT_UNKNOWN					min(DMORIENT_PORTRAIT, DMORIENT_LANDSCAPE)-1

class AFX_EXT_CLASS_GENERAL CGlWinApp : public CWinApp
{
	DECLARE_DYNAMIC(CGlWinApp)
public:
	CGlWinApp();

// Attributes
private:
	static COLORREF m_custColors[];

public:
	int m_nCopyResolution;
	int m_nSerializeResultBytes;
	int m_nMaxPrintMemSize;
	int m_nPrintObject;
	BOOL m_bPrintHighestQuality;
	BOOL m_bPrintCenter;
	BOOL m_bPrintBkgndWhite;

	OSRUNNING m_nOsRunning;

// Options
	BOOL m_bNoHardwareAcceleration;
	BOOL m_bPropPolicy;
	BOOL m_bReopenPolicy;
	BOOL m_bBlackLinesOrRemoveHidden;
	BOOL m_bScaleShowFrame;
	BOOL m_bScaleShowLabels;
	int m_nScaleShowGrid;
	BOOL m_bScaleShowAxis;
	BOOL m_bScaleShowAxisLabels;

// Mouse
	int m_nMousePolicy;

// OpenGL stuff
	LOGFONT m_LogFont;
	COLORREF m_OptionColorFont;
	COLORREF m_OptionColorFore;
	COLORREF m_OptionColorBack;
	COLORREF m_OptionColorLightAmbient;
	COLORREF m_OptionColorLightDiffuse;
	COLORREF m_OptionColorLightSpecular;

	BOOL	m_bLighting;
	BOOL	m_bSmooth;
	BOOL	m_bAntialias;
	BOOL	m_bMonochromeMode;
	int		m_nPolygonMode;

// Printing
	BYTE* m_pBitmapBits;
	CRect m_Rect;
	BITMAPINFO m_bitmapInfo;

// Registry keys
	virtual BOOL LoadOptions();
	virtual BOOL SaveOptions();
	virtual void RegisterShellFileTypes(BOOL bCompat = FALSE, BOOL bOmitPrintCmd = FALSE);

// Operations
	void DoFilePrintAll();
	BOOL BeginPrinting(CDC* pDC);
	BOOL Print(CDC* pDC);
	BOOL EndPrinting(CDC* pDC);

	void SetStatusBarInfo(int nInfoMajor, int nInfoMinor = -1);
	CRecentFileList* GetRecentFileList();
	BOOL ExistShortcut(LPCTSTR szShortcut, BOOL bShift, BOOL bAlt, BOOL bCtrl, BOOL bIncludeHidden = FALSE);
	virtual int DoTimedMessageBox(UINT nTimeout, LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);

	COLORREF* GetCustColors() const;
	int GetCustColorsCount() const;

private:
	static BOOL CALLBACK AbortProc(HDC hdc, int iCode);

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGlWinApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnFileAlarm(UINT wParam, LONG lParam);
	//{{AFX_MSG(CGlWinApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpContents();
	afx_msg void OnHelpShortcuts();
	afx_msg void OnFilePrintSetupEx();
	afx_msg void OnChangeMousePolicy();
	afx_msg void OnChangeHardwareAcceleration();
	afx_msg void OnChangePropPolicy();
	afx_msg void OnChangeReopenPolicy();
	afx_msg void OnSerializeResultOneByte();
	afx_msg void OnSerializeResultTwoBytes();
	afx_msg void OnSerializeResultFourBytes();
	afx_msg void OnSerializeResultEightBytes();
	afx_msg void OnEditOptions();
	afx_msg void OnFilePrintEx();
	afx_msg void OnFilePrintPreviewEx();
	afx_msg void OnUpdateFilePrintEx(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreviewEx(CCmdUI* pCmdUI);
	//}}AFX_MSG
	//afx_msg void OnShowPolicy();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_GLWINAPP_H__D02404F7_14C5_11D2_B619_0000C0F55FDF__INCLUDED_)
