/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 2004
//
/////////////////////////////////////////////////////////////////////////////
// FormViewEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "FormViewTts.h"
#include "SplitterWndEx.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewTts

IMPLEMENT_DYNAMIC(CFormViewTts, CFormView)

CFormViewTts::CFormViewTts(LPCTSTR lpszTemplateName)
	: CFormView(lpszTemplateName)
{
	m_bToolTipMultiline = FALSE;
}

CFormViewTts::CFormViewTts(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	m_bToolTipMultiline = FALSE;
}

CFormViewTts::~CFormViewTts()
{
}


BEGIN_MESSAGE_MAP(CFormViewTts, CFormView)
	//{{AFX_MSG_MAP(CFormViewTts)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFormViewTts 

BOOL CFormViewTts::OnToolTipText(UINT nId, NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(nId);

	// This code is adapted from CFrameWnd::OnToolTipText.
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) // will be zero on a separator
	{
		// don't handle the message if no string resource found
		if (AfxLoadString(nID, szFullText) == 0)
			return FALSE;

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	if (m_bToolTipMultiline)
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	else
		::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, -1);

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}

void CFormViewTts::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	EnableToolTips();
}

void CFormViewTts::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CWnd* pParent = GetParent();
	if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
		((CSplitterWndEx*)pParent)->RefreshSplitterBars(&bActivate);

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		// AfxGetMainWnd() may return NULL in an SDI environment
		CWnd* pWnd = AfxGetMainWnd();
		if (pWnd != NULL)
		{
			// update toolbars immedeately
			pWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
		}
	}
}

BOOL CFormViewTts::PreTranslateMessage(MSG* pMsg) 
{
	// Let '^Z' be handled rather by the edit controls on the form view
	// then by the frame window (which has an accelerator table entry).
	//
	// Note: '^X, ^C, ^V' are handled now by the message handlers below.
	if (pMsg->message == WM_KEYDOWN && 
		(pMsg->wParam == _T('Z')) && 
		(GetKeyState(VK_CONTROL) & 0x8000))
	{
		TranslateMessage(pMsg);
		DispatchMessage(pMsg);
		return TRUE;
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CFormViewTts::OnEditCut() 
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT(pEdit != NULL && IsEdit(pEdit) && (pEdit->GetStyle() & ES_READONLY) == 0);
	pEdit->Cut();
}

void CFormViewTts::OnEditCopy() 
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT(pEdit != NULL && IsEdit(pEdit));
	pEdit->Copy();
}

void CFormViewTts::OnEditPaste() 
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT(pEdit != NULL && IsEdit(pEdit) && (pEdit->GetStyle() & ES_READONLY) == 0);
	pEdit->Paste();
}

void CFormViewTts::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// UPDATE_COMMAND_UI handler for Edit Cut requires that the current
	// focus is on an edit control (not read only) that has text selected.

	// get the current focus & determine if its on a CEdit control
	CWnd* pWnd = GetFocus();
	if (pWnd == NULL || !IsEdit(pWnd) || (pWnd->GetStyle() & ES_READONLY) != 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		CEdit* pEdit = (CEdit*)pWnd;
		int nBeg, nEnd;

		pEdit->GetSel(nBeg, nEnd);
		pCmdUI->Enable(nBeg != nEnd);
	}
}

void CFormViewTts::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// UPDATE_COMMAND_UI handler for Edit Copy requires that the current
	// focus is on an edit control that has text selected.

	// get the current focus & determine if its on a CEdit control
	CWnd* pWnd = GetFocus();
	if (pWnd == NULL || !IsEdit(pWnd))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		CEdit* pEdit = (CEdit*)pWnd;
		int nBeg, nEnd;

		pEdit->GetSel(nBeg, nEnd);
		pCmdUI->Enable(nBeg != nEnd);
	}
}

void CFormViewTts::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// UPDATE_COMMAND_UI handlers for Edit Paste requires that focus be
	// on an edit control and that the clipboard contains text to be
	// pasted into the control.

	// get the current focus & determine if its on a CEdit control
	// also check to see if the control is read-only.
	CWnd* pWnd = GetFocus();
	if (pWnd == NULL || !IsEdit(pWnd) || (pWnd->GetStyle() & ES_READONLY) != 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
	}
}
