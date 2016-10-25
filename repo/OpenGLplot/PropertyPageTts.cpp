/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 2004
//
/////////////////////////////////////////////////////////////////////////////
// PropertyPageTts.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "PropertyPageTts.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CPropertyPageTts 

IMPLEMENT_DYNAMIC(CPropertyPageTts, CPropertyPage)

CPropertyPageTts::CPropertyPageTts() : CPropertyPage()
{
	m_bToolTipMultiline = FALSE;
}

CPropertyPageTts::CPropertyPageTts(UINT nIDTemplate, UINT nIDCaption)
	:CPropertyPage(nIDTemplate, nIDCaption)
{
	m_bToolTipMultiline = FALSE;
}

CPropertyPageTts::CPropertyPageTts(LPCTSTR lpszTemplateName, UINT nIDCaption)
	:CPropertyPage(lpszTemplateName, nIDCaption)
{
	m_bToolTipMultiline = FALSE;
}

CPropertyPageTts::~CPropertyPageTts()
{
}


BEGIN_MESSAGE_MAP(CPropertyPageTts, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyPageTts)
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
// Behandlungsroutinen für Nachrichten CPropertyPageTts 

BOOL CPropertyPageTts::OnToolTipText(UINT nId, NMHDR* pNMHDR, LRESULT* pResult)
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

BOOL CPropertyPageTts::OnInitDialog() 
{
	BOOL bRet = CPropertyPage::OnInitDialog();

	EnableToolTips();

	return bRet;
}

void CPropertyPageTts::OnEditCut() 
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT(pEdit != NULL && IsEdit(pEdit) && (pEdit->GetStyle() & ES_READONLY) == 0);
	pEdit->Cut();
}

void CPropertyPageTts::OnEditCopy() 
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT(pEdit != NULL && IsEdit(pEdit));
	pEdit->Copy();
}

void CPropertyPageTts::OnEditPaste() 
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT(pEdit != NULL && IsEdit(pEdit) && (pEdit->GetStyle() & ES_READONLY) == 0);
	pEdit->Paste();
}

void CPropertyPageTts::OnUpdateEditCut(CCmdUI* pCmdUI) 
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

void CPropertyPageTts::OnUpdateEditCopy(CCmdUI* pCmdUI) 
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

void CPropertyPageTts::OnUpdateEditPaste(CCmdUI* pCmdUI) 
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
