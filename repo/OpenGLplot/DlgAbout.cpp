/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DlgAbout.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "GlWinApp.h"
#include "Version.h"
#include "DlgAbout.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgAbout 


CDlgAbout::CDlgAbout() : CDialogTts(CDlgAbout::IDD)
{
	//{{AFX_DATA_INIT(CDlgAbout)
	//}}AFX_DATA_INIT
}


void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogTts::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_MAP
	if (!m_HyperLink.GetURL().IsEmpty())
	{
		DDX_Control(pDX, IDC_STATIC_URL, m_HyperLink);
	}
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialogTts)
	//{{AFX_MSG_MAP(CDlgAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgAbout 

BOOL CDlgAbout::OnInitDialog() 
{
	CString strText, strTitle, strVersion, strCopyright, strURLName, strURLDescription;
	TCHAR szFullPath[_MAX_PATH];
	HINSTANCE hInstance = AfxGetInstanceHandle();

	// Get some version information
	GetModuleFileName(hInstance, szFullPath, _countof(szFullPath));
	GetVersionInfo(szFullPath, _T("ProductVersion"), &strVersion);
	GetVersionInfo(szFullPath, _T("LegalCopyright"), &strCopyright);
	GetVersionInfo(szFullPath, _T("InternalName"), &strTitle);
	GetVersionInfo(szFullPath, _T("URLName"), &strURLName);
	GetVersionInfo(szFullPath, _T("URLDescription"), &strURLDescription);
	if (strURLDescription.IsEmpty())
		GetVersionInfo(szFullPath, _T("CompanyName"), &strURLDescription);

	// Set URL
	m_HyperLink.SetURL(strURLName);

	BOOL bRet = CDialogTts::OnInitDialog();

	// Set text of ctrls
	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(strTitle + " Version " + strVersion);
	GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(strCopyright);
	GetDlgItem(IDC_STATIC_URL)->SetWindowText(strURLDescription);

	GetWindowText(strText);
	SetWindowText(strText + strTitle);

	// Set icon
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ICON);
	pStatic->SetIcon(ExtractIcon(hInstance, szFullPath, 0));

	// Get size of the "copyright" ctrl
	HFONT hFont = (HFONT)GetDlgItem(IDC_STATIC_COPYRIGHT)->SendMessage(WM_GETFONT);
	CClientDC dcScreen(NULL);
	HGDIOBJ hOldFont = NULL;

	if (hFont != NULL)
		hOldFont = dcScreen.SelectObject(hFont);

	strCopyright += _T("  ");
	CSize sizeCopyRight = dcScreen.GetTextExtent(strCopyright);

	if (hOldFont != NULL)
		dcScreen.SelectObject(hOldFont);

	// Auto resize "copyright" ctrl
	CRect rect;
	GetDlgItem(IDC_STATIC_COPYRIGHT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right = rect.left + sizeCopyRight.cx;
	rect.bottom = rect.top + sizeCopyRight.cy;
	GetDlgItem(IDC_STATIC_COPYRIGHT)->MoveWindow(&rect);

	// Move "URL" ctrl beside "copyright" ctrl
	int left = rect.right;
	int top = rect.top;

	GetDlgItem(IDC_STATIC_URL)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.left = left;
	rect.top = top;
	GetDlgItem(IDC_STATIC_URL)->MoveWindow(&rect);

	m_HyperLink.SetAutoSize();

	return bRet;
}
