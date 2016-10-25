/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 2000
//
// based on a sample of
// Chris Maunder <chrismaunder@codeguru.com>
//
/////////////////////////////////////////////////////////////////////////////
// HyperLink.cpp : Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ID_TOOLTIP
#define ID_TOOLTIP 1
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink()
{
	m_hCursorHand		= NULL;					// No cursor as yet
	m_crLinkColor		= RGB(  0,   0, 238);   // Blue
	m_crVisitedColor	= RGB( 85,  26, 139);   // Purple
	m_crHoverColor		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_bOverControl		= FALSE;				// Cursor not yet over control
	m_bVisited			= FALSE;				// Hasn't been visited yet.
	m_bUnderline		= TRUE;					// Underline the link?
	m_bAdjustToFit		= TRUE;					// Resize the window to fit the text?
	m_strURL.Empty();
}

CHyperLink::~CHyperLink()
{
	m_Font.DeleteObject();
	if (m_hCursorHand != NULL)
	{
		::DestroyCursor(m_hCursorHand);
		m_hCursorHand = NULL;
	}
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	//{{AFX_MSG_MAP(CHyperLink)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CHyperLink

BOOL CHyperLink::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

void CHyperLink::OnClicked()
{
	Clicked();
}

void CHyperLink::Clicked()
{
	int result = (int)GotoURL(m_strURL, SW_SHOW);
	m_bVisited = (result > HINSTANCE_ERROR);
	if (!m_bVisited)
	{
		MessageBeep(MB_ICONEXCLAMATION);	// Unable to follow link
		ReportError(result);
	}
	else
		SetVisited();						// Repaint to show visited Color
}

HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(nCtlColor);
	ASSERT(nCtlColor == CTLCOLOR_STATIC);

	if (IsOverControl())
		pDC->SetTextColor(m_crHoverColor);
	else if (m_bVisited)
		pDC->SetTextColor(m_crVisitedColor);
	else
		pDC->SetTextColor(m_crLinkColor);

	// transparent text.
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	CStatic::OnMouseMove(nFlags, point);

	if (m_bOverControl)		// Cursor is currently over control
	{
		// the control will lose capture after
		// GoToUrl() is called, so regain control of mouse
		if (GetCapture() != this)
			SetCapture();

		CRect rect;
		GetClientRect(&rect);

		if (!rect.PtInRect(point))
		{
			m_bOverControl = FALSE;
			ReleaseCapture();
			RedrawWindow();
			return;
		}
	}
	else					  // Cursor has just moved over control
	{
		m_bOverControl = TRUE;
		RedrawWindow();
		SetCapture();
	}
}

BOOL CHyperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	if (m_hCursorHand)
	{
		::SetCursor(m_hCursorHand);
		return TRUE;
	}
	return FALSE;
}

BOOL CHyperLink::IsOverControl()
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	CRect rect;
	GetClientRect(&rect);

	return rect.PtInRect(point);
}

void CHyperLink::PreSubclassWindow() 
{
	// We want to get mouse clicks via STN_CLICKED
	DWORD dwStyle = GetStyle();
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	// Set the URL as the window text
	if (m_strURL.IsEmpty())
		GetWindowText(m_strURL);

	// Check that the window text isn't empty. If it is, set it as the URL.
	CString strWndText;
	GetWindowText(strWndText);
	if (strWndText.IsEmpty())
	{
		ASSERT(!m_strURL.IsEmpty());	// Window and URL both NULL. DUH!
		SetWindowText(m_strURL);
	}

	// Create the font
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfUnderline = (BYTE)m_bUnderline;
	m_Font.CreateFontIndirect(&lf);
	SetFont(&m_Font);

	PositionWindow();		// Adjust size of window to fit URL if necessary
	SetDefaultCursor();		// Try and load up a "hand" cursor

	// Create the tooltip
	CRect rect;
	GetClientRect(&rect);
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(this, m_strURL, rect, ID_TOOLTIP);

	CStatic::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink Operationen

void CHyperLink::SetURL(CString strURL)
{
	m_strURL = strURL;

	if (::IsWindow(GetSafeHwnd()))
	{
		PositionWindow();
		m_ToolTip.UpdateTipText(strURL, this, ID_TOOLTIP);
	}
}

CString CHyperLink::GetURL() const
{ 
	return m_strURL;
}

void CHyperLink::SetColors(COLORREF crLinkColor, COLORREF crVisitedColor,
							COLORREF crHoverColor /* = -1 */) 
{ 
	m_crLinkColor = crLinkColor;
	m_crVisitedColor = crVisitedColor;

	if (crHoverColor == -1)
		m_crHoverColor = ::GetSysColor(COLOR_HIGHLIGHT);
	else
		m_crHoverColor = crHoverColor;

	if (::IsWindow(m_hWnd))
		Invalidate();
}

COLORREF CHyperLink::GetLinkColor() const
{ 
	return m_crLinkColor;
}

COLORREF CHyperLink::GetVisitedColor() const
{
	return m_crVisitedColor;
}

COLORREF CHyperLink::GetHoverColor() const
{
	return m_crHoverColor;
}

void CHyperLink::SetVisited(BOOL bVisited /* = TRUE */) 
{ 
	m_bVisited = bVisited;

	if (::IsWindow(GetSafeHwnd()))
		Invalidate();
}

BOOL CHyperLink::GetVisited() const
{ 
	return m_bVisited;
}

void CHyperLink::SetLinkCursor(HCURSOR hCursor)
{ 
	m_hCursorHand = hCursor;
	if (m_hCursorHand == NULL)
		SetDefaultCursor();
}

HCURSOR CHyperLink::GetLinkCursor() const
{
	return m_hCursorHand;
}

void CHyperLink::SetUnderline(BOOL bUnderline /* = TRUE */)
{
	m_bUnderline = !!bUnderline;

	if (::IsWindow(GetSafeHwnd()))
	{
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfUnderline = (BYTE)m_bUnderline;

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);
		SetFont(&m_Font);

		Invalidate();
	}
}

BOOL CHyperLink::GetUnderline() const
{ 
	return m_bUnderline;
}

void CHyperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
	m_bAdjustToFit = bAutoSize;

	if (::IsWindow(GetSafeHwnd()))
		PositionWindow();
}

BOOL CHyperLink::GetAutoSize() const
{ 
	return m_bAdjustToFit;
}


// Move and resize the window so that the window is the same size
// as the hyperlink text. This stops the hyperlink cursor being active
// when it is not directly over the text. If the text is left justified
// then the window is merely shrunk, but if it is centred or right
// justified then the window will have to be moved as well.
//
// Suggested by Pål K. Tønder 

void CHyperLink::PositionWindow()
{
	if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit)
		return;

	// Get the current window position
	CRect rect;
	GetWindowRect(&rect);

	CWnd* pParent = GetParent();
	if (pParent)
		pParent->ScreenToClient(&rect);

	// Get the size of the window text
	CString strWndText;
	GetWindowText(strWndText);

	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(&m_Font);
	CSize Extent = pDC->GetTextExtent(strWndText);
	if (pOldFont)
		pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	// Get the text justification via the window style
	DWORD dwStyle = GetStyle();

	// Recalc the window size and position based on the text justification
	if (dwStyle & SS_CENTERIMAGE)
		rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);
	else
		rect.bottom = rect.top + Extent.cy;

	if (dwStyle & SS_CENTER)
		rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);
	else if (dwStyle & SS_RIGHT)
		rect.left  = rect.right - Extent.cx;
	else // SS_LEFT = 0, so we can't test for it explicitly 
		rect.right = rect.left + Extent.cx;

	// Move the window
	SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink Implementierung

// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
// It loads a "hand" cursor from the winhlp32.exe module
void CHyperLink::SetDefaultCursor()
{
	if (m_hCursorHand == NULL)				// No cursor handle - load our own
	{
/*		// Get the windows directory
		CString strWndDir;
		GetWindowsDirectory(strWndDir.GetBuffer(_MAX_PATH), _MAX_PATH);
		strWndDir.ReleaseBuffer();

		strWndDir += _T("\\winhlp32.exe");
		// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
		HMODULE hModule = AfxLoadLibrary(strWndDir);
		if (hModule)
		{
			HCURSOR hCursorHand = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
			if (hCursorHand)
				m_hCursorHand = CopyCursor(hCursorHand);
		}
		AfxFreeLibrary(hModule);*/

		HCURSOR hCursorHand = AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);
		if (hCursorHand)
			m_hCursorHand = CopyCursor(hCursorHand);
	}
}

LONG CHyperLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS)
	{
		long datasize = _MAX_PATH;
		TCHAR data[_MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

void CHyperLink::ReportError(int nError)
{
	CString str, msg;
	switch (nError)
	{
		case 0:
			str.LoadString(IDS_ERROR_HYPER_INSUF_MEM);
			break;
		case SE_ERR_PNF:
			str.LoadString(IDS_ERROR_HYPER_PNF);
			break;
		case SE_ERR_FNF:
			str.LoadString(IDS_ERROR_HYPER_FNF);
			break;
		case ERROR_BAD_FORMAT:
			str.LoadString(IDS_ERROR_HYPER_BAD_FORMAT);
			break;
		case SE_ERR_ACCESSDENIED:
			str.LoadString(IDS_ERROR_HYPER_ACCESSDENIED);
			break;
		case SE_ERR_ASSOCINCOMPLETE:
			str.LoadString(IDS_ERROR_HYPER_ASSOCINCOMPLETE);
			break;
		case SE_ERR_DDEBUSY:
			str.LoadString(IDS_ERROR_HYPER_DDEBUSY);
			break;
		case SE_ERR_DDEFAIL:
			str.LoadString(IDS_ERROR_HYPER_DDEFAIL);
			break;
		case SE_ERR_DDETIMEOUT:
			str.LoadString(IDS_ERROR_HYPER_DDETIMEOUT);
			break;
		case SE_ERR_DLLNOTFOUND:
			str.LoadString(IDS_ERROR_HYPER_DLLNOTFOUND);
			break;
		case SE_ERR_NOASSOC:
			str.LoadString(IDS_ERROR_HYPER_NOASSOC);
			break;
		case SE_ERR_OOM:
			str.LoadString(IDS_ERROR_HYPER_OOM);
			break;
		case SE_ERR_SHARE:
			str.LoadString(IDS_ERROR_HYPER_SHARE);
			break;
		default:
			str.Format(IDS_ERROR_HYPER_UNKNOWN, nError);
	}
	msg.Format(IDS_ERROR_HYPER_MSG, str);
	AfxMessageBox(msg, MB_ICONEXCLAMATION | MB_OK);
}

HINSTANCE CHyperLink::GotoURL(LPCTSTR url, int showcmd)
{
	TCHAR key[_MAX_PATH + _MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL, NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR)
	{
		if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS)
		{
			lstrcat(key, _T("\\shell\\open\\command"));

			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS)
			{
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL)
				{
					// No quotes found
					pos = strstr(key, _T("%1"));		// Check for %1, without quotes 
					if (pos == NULL)					// No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';					// Remove the parameter
				}
				else
					*pos = '\0';						// Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				result = (HINSTANCE) WinExec(key, showcmd);
			}
		}
	}

	return result;
}

HCURSOR& CHyperLink::GetLinkCursor()
{
	return m_hCursorHand;
}

CToolTipCtrl& CHyperLink::GetToolTipCtrl()
{
	return m_ToolTip;
}
