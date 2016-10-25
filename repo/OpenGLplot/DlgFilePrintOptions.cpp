/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DlgFilePrintOptions.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "DlgFilePrintOptions.h"
#include "DdxDdvSupport.h"
#include "Resource.hm"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgFilePrintOptions 


CDlgFilePrintOptions::CDlgFilePrintOptions(CWnd* pParent /*=NULL*/)
	: CDialogTts(CDlgFilePrintOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFilePrintOptions)
	m_nMaxPrintMemSize = 0;
	m_nPrintObject = -1;
	m_bPrintHighestQuality = FALSE;
	m_bPrintCenter = FALSE;
	m_bPrintBkgndWhite = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgFilePrintOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogTts::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFilePrintOptions)
	DDX_Control(pDX, IDC_PRINT_BKGND_WHITE, m_PrintBkgndButton);
	DDX_Control(pDX, IDC_BUTTON_PRINTER, m_PrinterButton);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpButton);
	DDX_Control(pDX, IDC_PRINT_MEM_STANDARD, m_MaxPrintMemSizeStandardButton);
	DDX_Control(pDX, IDC_PRINT_MEM, m_MaxPrintMemSizeEdit);
	DDX_Control(pDX, IDC_STATIC_TEXT_F_TWO, m_TextFTwoStatic);
	DDX_Control(pDX, IDC_STATIC_TEXT_F_ONE, m_TextFOneStatic);
	DDX_Control(pDX, IDC_STATIC_OPTIONS, m_OptionsButton);
	DDX_Control(pDX, IDC_PRINT_CENTER, m_PrintCenterButton);
	DDX_Control(pDX, IDC_PRINT_HIGHEST_QUALITY, m_PrintHighestQualityButton);
	DDX_Text(pDX, IDC_PRINT_MEM, m_nMaxPrintMemSize);
	DDV_MinInt(pDX, m_nMaxPrintMemSize, 100);
	DDX_Radio(pDX, IDC_PRINT_VIEW, m_nPrintObject);
	DDX_Check(pDX, IDC_PRINT_HIGHEST_QUALITY, m_bPrintHighestQuality);
	DDX_Check(pDX, IDC_PRINT_CENTER, m_bPrintCenter);
	DDX_Check(pDX, IDC_PRINT_BKGND_WHITE, m_bPrintBkgndWhite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFilePrintOptions, CDialogTts)
	//{{AFX_MSG_MAP(CDlgFilePrintOptions)
	ON_BN_CLICKED(IDC_PRINT_MEM_STANDARD, OnPrintMemStandard)
	ON_BN_CLICKED(IDC_PRINT_VIEW, OnPrintView)
	ON_BN_CLICKED(IDC_PRINT_DOC, OnPrintDoc)
	ON_BN_CLICKED(IDC_PRINT_APP, OnPrintApp)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER, OnButtonPrinter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgFilePrintOptions 

void CDlgFilePrintOptions::OnPrintMemStandard() 
{
	CString defaultValue;

	defaultValue.Format("%d", 8*1024);
	m_MaxPrintMemSizeEdit.SetSel(0, -1);
	m_MaxPrintMemSizeEdit.ReplaceSel(defaultValue, TRUE);

	GotoDlgCtrl(&m_MaxPrintMemSizeEdit);
}

void CDlgFilePrintOptions::EnableControls()
{
	m_PrintBkgndButton.EnableWindow(m_nPrintObject != 2);
	m_PrintCenterButton.EnableWindow(m_nPrintObject != 1);
	m_PrintHighestQualityButton.EnableWindow(m_nPrintObject == 2);
	m_MaxPrintMemSizeStandardButton.EnableWindow(m_nPrintObject != 2);
	m_MaxPrintMemSizeEdit.EnableWindow(m_nPrintObject != 2);
	m_TextFOneStatic.EnableWindow(m_nPrintObject != 2);
	m_TextFTwoStatic.EnableWindow(m_nPrintObject != 2);
	m_HelpButton.EnableWindow(m_nPrintObject != 2);
}

BOOL CDlgFilePrintOptions::OnInitDialog() 
{
	BOOL bRet = CDialogTts::OnInitDialog();

	EnableControls();

	return bRet;
}

void CDlgFilePrintOptions::OnPrintView() 
{
	m_nPrintObject = 0;
	EnableControls();
}

void CDlgFilePrintOptions::OnPrintDoc() 
{
	m_nPrintObject = 1;
	EnableControls();
}

void CDlgFilePrintOptions::OnPrintApp() 
{
	m_nPrintObject = 2;
	EnableControls();
}

DWORD CDlgFilePrintOptions::m_dwHelpMap[] =
{
	IDC_STATIC_OPTIONS, HIDC_STATIC_OPTIONS,
	0,0
};

BOOL CDlgFilePrintOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	HWND hWnd = m_OptionsButton.m_hWnd;
	ASSERT(hWnd != NULL);

	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		::WinHelp(hWnd/*(HWND)pHelpInfo->hItemHandle*/,
			AfxGetApp()->m_pszHelpFilePath,
			HELP_WM_HELP,
			(DWORD)(LPVOID)m_dwHelpMap);
	}

	return TRUE;
}

void CDlgFilePrintOptions::OnButtonHelp() 
{
	HELPINFO helpInfo;
	helpInfo.iContextType = HELPINFO_WINDOW;
	OnHelpInfo(&helpInfo);
}

void CDlgFilePrintOptions::OnButtonPrinter() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_PRINT_SETUP);
}
