/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DlgFilePrintOptions.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGFILEPRINTOPTIONS_H__6C923241_1280_11D2_B619_0000C0F55FDF__INCLUDED_)
#define AFX_DLGFILEPRINTOPTIONS_H__6C923241_1280_11D2_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DialogTts.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgFilePrintOptions 

class CDlgFilePrintOptions : public CDialogTts
{
// Konstruktion
public:
	CDlgFilePrintOptions(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgFilePrintOptions)
	enum { IDD = IDD_DLG_FILE_PRINT_OPTIONS };
	CButton	m_PrintBkgndButton;
	CButton	m_PrinterButton;
	CButton	m_HelpButton;
	CButton	m_MaxPrintMemSizeStandardButton;
	CEdit	m_MaxPrintMemSizeEdit;
	CStatic	m_TextFTwoStatic;
	CStatic	m_TextFOneStatic;
	CButton	m_OptionsButton;
	CButton	m_PrintCenterButton;
	CButton	m_PrintHighestQualityButton;
	int		m_nMaxPrintMemSize;
	int		m_nPrintObject;
	BOOL	m_bPrintHighestQuality;
	BOOL	m_bPrintCenter;
	BOOL	m_bPrintBkgndWhite;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgFilePrintOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	static DWORD m_dwHelpMap[];
	void EnableControls();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgFilePrintOptions)
	afx_msg void OnPrintMemStandard();
	virtual BOOL OnInitDialog();
	afx_msg void OnPrintView();
	afx_msg void OnPrintDoc();
	afx_msg void OnPrintApp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonPrinter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DLGFILEPRINTOPTIONS_H__6C923241_1280_11D2_B619_0000C0F55FDF__INCLUDED_)
