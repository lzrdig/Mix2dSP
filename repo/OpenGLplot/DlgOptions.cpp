/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 2000
//
/////////////////////////////////////////////////////////////////////////////
// DlgOptions.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "DlgOptions.h"
#include "DdxDdvSupport.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgOptions 


CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CDialogTts(CDlgOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptions)
	m_nRadioMousePolicy = -1;
	m_bCheckNoHardwareAcceleration = FALSE;
	m_bCheckPropPolicy = FALSE;
	m_bCheckReopenPolicy = FALSE;
	m_nRadioSerializeResultBytes = -1;
	m_nRadioCopyResolution = -1;
	m_nCopyResolution = 0;
	//}}AFX_DATA_INIT
}


void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogTts::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptions)
	DDX_Control(pDX, IDC_EDIT_COPY_RESOLUTION, m_CopyResolutionEdit);
	DDX_Control(pDX, IDC_CHECK_REOPEN_POLICY, m_ReopenPolicyButton);
	DDX_Control(pDX, IDC_STATIC_REOPEN_GBOX, m_ReopenPolicyGboxButton);
	DDX_Control(pDX, IDC_STATIC_SERIALZE_RESULT_BYTES_GBOX, m_SerializeResultBytesGboxButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_RESULT_BYTES_8, m_SerializeResultEightBytesButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_RESULT_BYTES_4, m_SerializeResultFourBytesButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_RESULT_BYTES_2, m_SerializeResultTwoBytesButton);
	DDX_Control(pDX, IDC_RADIO_SERIALZE_RESULT_BYTES_1, m_SerializeResultOneByteButton);
	DDX_Radio(pDX, IDC_RADIO_MOUSE_POLICY_0, m_nRadioMousePolicy);
	DDX_Check(pDX, IDC_CHECK_NO_HARDWARE_ACCELERATION, m_bCheckNoHardwareAcceleration);
	DDX_Check(pDX, IDC_CHECK_PROP_POLICY, m_bCheckPropPolicy);
	DDX_Check(pDX, IDC_CHECK_REOPEN_POLICY, m_bCheckReopenPolicy);
	DDX_Radio(pDX, IDC_RADIO_SERIALZE_RESULT_BYTES_1, m_nRadioSerializeResultBytes);
	DDX_Radio(pDX, IDC_RADIO_COPY_RESOLUTION_72, m_nRadioCopyResolution);
	DDX_Text(pDX, IDC_EDIT_COPY_RESOLUTION, m_nCopyResolution);
	DDV_MinIntNB(pDX, m_nCopyResolution, 0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptions, CDialogTts)
	//{{AFX_MSG_MAP(CDlgOptions)
	ON_BN_CLICKED(IDC_BUTTON_SHORTCUTS, OnButtonShortcuts)
	ON_BN_CLICKED(IDC_RADIO_COPY_RESOLUTION_72, OnRadioCopyResolution72)
	ON_BN_CLICKED(IDC_RADIO_COPY_RESOLUTION_75, OnRadioCopyResolution75)
	ON_BN_CLICKED(IDC_RADIO_COPY_RESOLUTION_150, OnRadioCopyResolution150)
	ON_BN_CLICKED(IDC_RADIO_COPY_RESOLUTION_300, OnRadioCopyResolution300)
	ON_BN_CLICKED(IDC_RADIO_COPY_RESOLUTION_600, OnRadioCopyResolution600)
	ON_BN_CLICKED(IDC_RADIO_COPY_RESOLUTION_OTHER, OnRadioCopyResolutionOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDlgOptions 

BOOL CDlgOptions::OnInitDialog() 
{
	switch (m_nCopyResolution)
	{
	case 72:
		m_nRadioCopyResolution = 0;
		break;
	case 75:
		m_nRadioCopyResolution = 1;
		break;
	case 150:
		m_nRadioCopyResolution = 2;
		break;
	case 300:
		m_nRadioCopyResolution = 3;
		break;
	case 600:
		m_nRadioCopyResolution = 4;
		break;
	default:
		m_nRadioCopyResolution = 5;
	}

	BOOL bRet = CDialogTts::OnInitDialog();

	EnableExtraEdit();

	if (m_nRadioCopyResolution >= 0 && m_nRadioCopyResolution <= 4)
		m_CopyResolutionEdit.SetReadOnly();

	return bRet;
}

void CDlgOptions::EnableExtraEdit()
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	BOOL bEnableSerializeResultOneByte = pApp->ExistShortcut(_T("1"), TRUE, FALSE, TRUE), 
		bEnableSerializeResultTwoByte = pApp->ExistShortcut(_T("2"), TRUE, FALSE, TRUE), 
		bEnableSerializeResultFourByte = pApp->ExistShortcut(_T("4"), TRUE, FALSE, TRUE), 
		bEnableSerializeResultEightByte = pApp->ExistShortcut(_T("8"), TRUE, FALSE, TRUE);
	ASSERT(m_nRadioSerializeResultBytes != 0 || bEnableSerializeResultOneByte);
	ASSERT(m_nRadioSerializeResultBytes != 1 || bEnableSerializeResultTwoByte);
	ASSERT(m_nRadioSerializeResultBytes != 2 || bEnableSerializeResultFourByte);
	ASSERT(m_nRadioSerializeResultBytes != 3 || bEnableSerializeResultEightByte);

	BOOL bEnableReopen = (pApp->GetRecentFileList() != NULL);
	BOOL bEnableSerialize = (m_nRadioSerializeResultBytes == 0 
		|| m_nRadioSerializeResultBytes == 1 
		|| m_nRadioSerializeResultBytes == 2 
		|| m_nRadioSerializeResultBytes == 3);

	m_ReopenPolicyGboxButton.ShowWindow(bEnableReopen ? SW_SHOW : SW_HIDE);
	m_ReopenPolicyButton.EnableWindow(bEnableReopen);
	m_ReopenPolicyButton.ShowWindow(bEnableReopen ? SW_SHOW : SW_HIDE);

	m_SerializeResultBytesGboxButton.ShowWindow(bEnableSerialize ? SW_SHOW : SW_HIDE);
	m_SerializeResultOneByteButton.EnableWindow(bEnableSerialize && bEnableSerializeResultOneByte);
	m_SerializeResultOneByteButton.ShowWindow(bEnableSerialize ? SW_SHOW : SW_HIDE);
	m_SerializeResultTwoBytesButton.EnableWindow(bEnableSerialize && bEnableSerializeResultTwoByte);
	m_SerializeResultTwoBytesButton.ShowWindow(bEnableSerialize ? SW_SHOW : SW_HIDE);
	m_SerializeResultFourBytesButton.EnableWindow(bEnableSerialize && bEnableSerializeResultFourByte);
	m_SerializeResultFourBytesButton.ShowWindow(bEnableSerialize ? SW_SHOW : SW_HIDE);
	m_SerializeResultEightBytesButton.EnableWindow(bEnableSerialize && bEnableSerializeResultEightByte);
	m_SerializeResultEightBytesButton.ShowWindow(bEnableSerialize ? SW_SHOW : SW_HIDE);

	int yInflate = 0;
	CRect rect;

	if (!bEnableReopen)
	{
		yInflate -= MapDialogUnitY(48, this);
	}
	if (!bEnableSerialize)
	{
		yInflate -= MapDialogUnitY(48, this);
	}

	GetWindowRect(&rect);
	rect.InflateRect(0, 0, 0, yInflate);
	MoveWindow(rect);
}

void CDlgOptions::OnButtonShortcuts() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_HELP_SHORTCUTS);
}

void CDlgOptions::OnRadioCopyResolution72() 
{
	MSG msg;
	if (m_nRadioCopyResolution != 0)
	{
		// Note: PeekMessage is in here for correct arrow key behaviour (strange, but true)
		//
		// Clicking by mouse yields a WM_MOUSE_MOVE message, so we have the desired behaviour.
		//
		// Using the arrow keys we see this function called twice; the first time we find
		// no WM_MOUSE_... message, the second time we find a WM_MOUSE_MOVE message again.
		// To ensure that the undo feature works correctly we set our variable the first time to
		// a dummy value (-m_nRadioCopyResolution-1) - so we can do what we want 
		// the second time.
		if (m_nRadioCopyResolution < 0 || PeekMessage(&msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			m_nRadioCopyResolution = 0;
		}
		else
		{
			m_nRadioCopyResolution *= -1;
			m_nRadioCopyResolution -= 1;
			return;
		}
		CString newValue;

		newValue.Format("%d", 72);
		m_CopyResolutionEdit.SetSel(0, -1);
		m_CopyResolutionEdit.ReplaceSel(newValue, TRUE);
		m_CopyResolutionEdit.SetReadOnly(TRUE);
	}
}

void CDlgOptions::OnRadioCopyResolution75() 
{
	MSG msg;
	if (m_nRadioCopyResolution != 1)
	{
		// Note: PeekMessage is in here for correct arrow key behaviour (strange, but true)
		//
		// Clicking by mouse yields a WM_MOUSE_MOVE message, so we have the desired behaviour.
		//
		// Using the arrow keys we see this function called twice; the first time we find
		// no WM_MOUSE_... message, the second time we find a WM_MOUSE_MOVE message again.
		// To ensure that the undo feature works correctly we set our variable the first time to
		// a dummy value (-m_nRadioCopyResolution-1) - so we can do what we want 
		// the second time.
		if (m_nRadioCopyResolution < 0 || PeekMessage(&msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			m_nRadioCopyResolution = 1;
		}
		else
		{
			m_nRadioCopyResolution *= -1;
			m_nRadioCopyResolution -= 1;
			return;
		}
		CString newValue;

		newValue.Format("%d", 75);
		m_CopyResolutionEdit.SetSel(0, -1);
		m_CopyResolutionEdit.ReplaceSel(newValue, TRUE);
		m_CopyResolutionEdit.SetReadOnly(TRUE);
	}
}

void CDlgOptions::OnRadioCopyResolution150() 
{
	MSG msg;
	if (m_nRadioCopyResolution != 2)
	{
		// Note: PeekMessage is in here for correct arrow key behaviour (strange, but true)
		//
		// Clicking by mouse yields a WM_MOUSE_MOVE message, so we have the desired behaviour.
		//
		// Using the arrow keys we see this function called twice; the first time we find
		// no WM_MOUSE_... message, the second time we find a WM_MOUSE_MOVE message again.
		// To ensure that the undo feature works correctly we set our variable the first time to
		// a dummy value (-m_nRadioCopyResolution-1) - so we can do what we want 
		// the second time.
		if (m_nRadioCopyResolution < 0 || PeekMessage(&msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			m_nRadioCopyResolution = 2;
		}
		else
		{
			m_nRadioCopyResolution *= -1;
			m_nRadioCopyResolution -= 1;
			return;
		}
		CString newValue;

		newValue.Format("%d", 150);
		m_CopyResolutionEdit.SetSel(0, -1);
		m_CopyResolutionEdit.ReplaceSel(newValue, TRUE);
		m_CopyResolutionEdit.SetReadOnly(TRUE);
	}
}

void CDlgOptions::OnRadioCopyResolution300() 
{
	MSG msg;
	if (m_nRadioCopyResolution != 3)
	{
		// Note: PeekMessage is in here for correct arrow key behaviour (strange, but true)
		//
		// Clicking by mouse yields a WM_MOUSE_MOVE message, so we have the desired behaviour.
		//
		// Using the arrow keys we see this function called twice; the first time we find
		// no WM_MOUSE_... message, the second time we find a WM_MOUSE_MOVE message again.
		// To ensure that the undo feature works correctly we set our variable the first time to
		// a dummy value (-m_nRadioCopyResolution-1) - so we can do what we want 
		// the second time.
		if (m_nRadioCopyResolution < 0 || PeekMessage(&msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			m_nRadioCopyResolution = 3;
		}
		else
		{
			m_nRadioCopyResolution *= -1;
			m_nRadioCopyResolution -= 1;
			return;
		}
		CString newValue;

		newValue.Format("%d", 300);
		m_CopyResolutionEdit.SetSel(0, -1);
		m_CopyResolutionEdit.ReplaceSel(newValue, TRUE);
		m_CopyResolutionEdit.SetReadOnly(TRUE);
	}
}

void CDlgOptions::OnRadioCopyResolution600() 
{
	MSG msg;
	if (m_nRadioCopyResolution != 4)
	{
		// Note: PeekMessage is in here for correct arrow key behaviour (strange, but true)
		//
		// Clicking by mouse yields a WM_MOUSE_MOVE message, so we have the desired behaviour.
		//
		// Using the arrow keys we see this function called twice; the first time we find
		// no WM_MOUSE_... message, the second time we find a WM_MOUSE_MOVE message again.
		// To ensure that the undo feature works correctly we set our variable the first time to
		// a dummy value (-m_nRadioCopyResolution-1) - so we can do what we want 
		// the second time.
		if (m_nRadioCopyResolution < 0 || PeekMessage(&msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			m_nRadioCopyResolution = 4;
		}
		else
		{
			m_nRadioCopyResolution *= -1;
			m_nRadioCopyResolution -= 1;
			return;
		}
		CString newValue;

		newValue.Format("%d", 600);
		m_CopyResolutionEdit.SetSel(0, -1);
		m_CopyResolutionEdit.ReplaceSel(newValue, TRUE);
		m_CopyResolutionEdit.SetReadOnly(TRUE);
	}
}

void CDlgOptions::OnRadioCopyResolutionOther() 
{
	MSG msg;
	if (m_nRadioCopyResolution != 5)
	{
		// Note: PeekMessage is in here for correct arrow key behaviour (strange, but true)
		//
		// Clicking by mouse yields a WM_MOUSE_MOVE message, so we have the desired behaviour.
		//
		// Using the arrow keys we see this function called twice; the first time we find
		// no WM_MOUSE_... message, the second time we find a WM_MOUSE_MOVE message again.
		// To ensure that the undo feature works correctly we set our variable the first time to
		// a dummy value (-m_nRadioCopyResolution-1) - so we can do what we want 
		// the second time.
		if (m_nRadioCopyResolution < 0 || PeekMessage(&msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		{
			m_nRadioCopyResolution = 5;
		}
		else
		{
			m_nRadioCopyResolution *= -1;
			m_nRadioCopyResolution -= 1;
			return;
		}
		m_CopyResolutionEdit.SetReadOnly(FALSE);
	}
}
