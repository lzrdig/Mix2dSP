/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: march 2001
//
/////////////////////////////////////////////////////////////////////////////
// ListBoxEx.cpp : Implementierung der Klasse CListBoxEx
//

#include "StdAfx.h"
#include "ListBoxEx.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const AFX_MSGMAP_ENTRY* AFXAPI
AfxFindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry,
	UINT nMsg, UINT nCode, UINT nID)
{
#if defined(_M_IX86) && !defined(_AFX_PORTABLE)
// 32-bit Intel 386/486 version.

	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nMessage) == 0);
	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nCode) == 4);
	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nID) == 8);
	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nLastID) == 12);
	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nSig) == 16);

	_asm
	{
			MOV     EBX,lpEntry
			MOV     EAX,nMsg
			MOV     EDX,nCode
			MOV     ECX,nID
	__loop:
			CMP     DWORD PTR [EBX+16],0        ; nSig (0 => end)
			JZ      __failed
			CMP     EAX,DWORD PTR [EBX]         ; nMessage
			JE      __found_message
	__next:
			ADD     EBX,SIZE AFX_MSGMAP_ENTRY
			JMP     short __loop
	__found_message:
			CMP     EDX,DWORD PTR [EBX+4]       ; nCode
			JNE     __next
	// message and code good so far
	// check the ID
			CMP     ECX,DWORD PTR [EBX+8]       ; nID
			JB      __next
			CMP     ECX,DWORD PTR [EBX+12]      ; nLastID
			JA      __next
	// found a match
			MOV     lpEntry,EBX                 ; return EBX
			JMP     short __end
	__failed:
			XOR     EAX,EAX                     ; return NULL
			MOV     lpEntry,EAX
	__end:
	}
	return lpEntry;
#else  // _AFX_PORTABLE
	// C version of search routine
	while (lpEntry->nSig != AfxSig_end)
	{
		if (lpEntry->nMessage == nMsg && lpEntry->nCode == nCode &&
			nID >= lpEntry->nID && nID <= lpEntry->nLastID)
		{
			return lpEntry;
		}
		lpEntry++;
	}
	return NULL;    // not found
#endif  // _AFX_PORTABLE
}

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	//{{AFX_MSG_MAP(CListBoxEx)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_HSCROLL()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx Konstruktion/Destruktion

CListBoxEx::CListBoxEx()
{
	m_bReadOnly = FALSE;
	m_bShowAlways = TRUE;

	m_hBrush = NULL;
}

CListBoxEx::~CListBoxEx()
{
	if (m_hBrush)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Diagnose CListBoxEx

#ifdef _DEBUG
void CListBoxEx::AssertValid() const
{
	CListBox::AssertValid();
}

void CListBoxEx::Dump(CDumpContext& dc) const
{
	CListBox::Dump(dc);

	dc << "\nm_bReadOnly = " << m_bReadOnly;
	dc << "\nm_bShowAlways = " << m_bShowAlways;
	dc << "\nm_hBrush = " << m_hBrush;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx Zeichnen

#if _MSC_VER >= 1200
#pragma warning(push) 
#endif // _MSC_VER >= 1200
#pragma warning(disable : 4238)	//nonstandard extension used : class rvalue used as lvalue
void CListBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rcBounds, rcHighlight, rcLabel, rcWnd;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nItem = lpDrawItemStruct->itemID;

	// Save dc state
	int nSavedDC = pDC->SaveDC();
	TEXTMETRIC tm;
	VERIFY(pDC->GetTextMetrics(&tm));
	SetItemHeight(nItem, tm.tmHeight);

	// Should the item be highlighted
	BOOL bHighlight = ((GetSel(nItem) > 0) && ((GetFocus() == this) || m_bShowAlways));

	// Get rectangles for drawing
	if (nItem >= 0)
		GetItemRect(nItem, rcBounds);
	else
		rcBounds = lpDrawItemStruct->rcItem;
	rcHighlight = rcBounds;

	CString sLabel;
	if (nItem >= 0)
		GetText(nItem, sLabel);

	// Draw the background color
	if (bHighlight)
	{
		if (GetFocus() == this)
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

			pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
		}
		else
		{
			if (IsWindowEnabled())
			{
				pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
			}
			else
			{
				if (m_bReadOnly)
				{
					BYTE redText, greenText, blueText;
					short hue, lumText, lumGrayText, lumBtnFace, sat;
					COLORREF colorText, colorBtnFace, colorGrayText;

					colorText = ::GetSysColor(COLOR_APPWORKSPACE);
					colorGrayText = ::GetSysColor(COLOR_GRAYTEXT);
					colorBtnFace = ::GetSysColor(COLOR_BTNFACE);

					RGB2HLS(GetRValue(colorBtnFace), GetGValue(colorBtnFace), GetBValue(colorBtnFace), hue, lumBtnFace, sat);
					RGB2HLS(GetRValue(colorGrayText), GetGValue(colorGrayText), GetBValue(colorGrayText), hue, lumGrayText, sat);
					RGB2HLS(GetRValue(colorText), GetGValue(colorText), GetBValue(colorText), hue, lumText, sat);
					lumText = (short)MulDiv(lumText, lumGrayText, lumBtnFace);
					HLS2RGB(hue, lumText, sat, redText, greenText, blueText);

					pDC->SetTextColor(RGB(redText, greenText, blueText));
				}
				else
				{
					pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
				}
			}

			if (m_bReadOnly)
			{
				pDC->SetBkColor(::GetSysColor(COLOR_APPWORKSPACE));
				pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_APPWORKSPACE)));
			}
			else
			{
				pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
				pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_BTNFACE)));
			}
		}
	}
	else
	{
		pDC->SetTextColor(IsWindowEnabled() ? ::GetSysColor(COLOR_BTNTEXT) : ::GetSysColor(COLOR_GRAYTEXT));

		if (m_bReadOnly)
		{
			pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_BTNFACE)));
		}
		else
		{
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));
		}
	}

	// Set clip region
	GetClientRect(&rcWnd);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcWnd);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// Labels are offset by a certain amount
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1).cx;

	// Draw item label
	rcLabel = rcBounds;
	rcLabel.left += offset*2/3;

	pDC->DrawText(sLabel, -1, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

	// Draw focus rectangle if item has focus
	if ((nItem == GetCaretIndex()) && (GetFocus() == this))
		pDC->DrawFocusRect(rcHighlight);

	// Restore dc
	pDC->RestoreDC(nSavedDC);
}
#if _MSC_VER >= 1200
#pragma warning(pop) 
#else // _MSC_VER >= 1200
#pragma warning(default : 4238)	//nonstandard extension used : class rvalue used as lvalue
#endif // _MSC_VER >= 1200

void CListBoxEx::RepaintSelectedItems()
{
	CRect rcBounds;

	// Invalidate focused item so it can repaint 
	int nItem = GetCaretIndex();

	if (nItem != LB_ERR)
	{
		GetItemRect(nItem, rcBounds);

		InvalidateRect(rcBounds, FALSE);
	}

	// Invalidate selected items
	int nCount = GetCount();
	for (nItem = 0; nItem < nCount; nItem++)
	{
		if (GetSel(nItem) <= 0)
			continue;

		GetItemRect(nItem, rcBounds);

		InvalidateRect(rcBounds, FALSE);
	}

	UpdateWindow();
}

void CListBoxEx::SetReadOnly(BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;

	if (::IsWindow(m_hWnd))
	{
		if (GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE))
		{
			if (m_hBrush)
				DeleteObject(m_hBrush);

			m_hBrush = CreateSolidBrush(m_bReadOnly ? ::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_WINDOW));
		}
		InvalidateRect(NULL);
#ifdef _DEBUG
		if (!(GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)))
			TRACE0("Warning: Neither LBS_OWNERDRAWFIXED nor LBS_OWNERDRAWVARIABLE style is set, so \"SetReadOnly\" will have no effect.\n");
#endif //_DEBUG
	}
}

void CListBoxEx::SetShowAlways(BOOL bShowAlways)
{
	m_bShowAlways = bShowAlways;

	if (::IsWindow(m_hWnd))
	{
		InvalidateRect(NULL, FALSE);
#ifdef _DEBUG
		if (!(GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)))
			TRACE0("Warning: Neither LBS_OWNERDRAWFIXED nor LBS_OWNERDRAWVARIABLE style is set, so \"SetShowAlways\" will have no effect.\n");
#endif //_DEBUG
	}
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CListBoxEx

void CListBoxEx::OnSetFocus(CWnd* pOldWnd) 
{
	CListBox::OnSetFocus(pOldWnd);

	// check if we are getting focus from label edit box
	if (pOldWnd != NULL && pOldWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	RepaintSelectedItems();
}

void CListBoxEx::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);

	// check if we are losing focus to label edit box
	if (pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	RepaintSelectedItems();
}

void CListBoxEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);

	if (GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE))
 	{
 		RepaintSelectedItems();
 	}
}

void CListBoxEx::OnSelchange() 
{
	InvalidateRect(NULL, FALSE);
}

BOOL CListBoxEx::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	BOOL bRet;

	if (message == WM_CTLCOLORLISTBOX && (GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)))
	{
		// create brush if it does not exist yet
		if (!m_hBrush)
			m_hBrush = CreateSolidBrush(m_bReadOnly ? ::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_WINDOW));

		// return the brush to draw the background
		*pLResult = (LRESULT)m_hBrush;
		bRet = TRUE;
	}
	else
	{
		bRet = CListBox::OnChildNotify(message, wParam, lParam, pLResult);
	}

	// We try to catch the case that LBN_SELCHANGE is handled by this class.
	// Normally the parent window then does not handle it. However, we let
	// the parent handle it too.
	// This piece of code is adapted from CCmdTarget::OnCmdMsg.
	int nID = 0, nCode = HIWORD(wParam);
	nCode = MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND);

	// determine the message number and code (packed into nCode)
	const AFX_MSGMAP* pMessageMap;
	const AFX_MSGMAP_ENTRY* lpEntry;
	UINT nMsg = 0;

	if (nCode != CN_UPDATE_COMMAND_UI)
	{
		nMsg = HIWORD(nCode);
		nCode = LOWORD(nCode);
	}

	// for backward compatibility HIWORD(nCode)==0 is WM_COMMAND
	if (nMsg == 0)
		nMsg = WM_COMMAND;

	// look through message map to see if it applies to us
#ifdef _AFXDLL
	for (pMessageMap = GetMessageMap(); pMessageMap != NULL;
	  pMessageMap = (*pMessageMap->pfnGetBaseMap)())
#else
	for (pMessageMap = GetMessageMap(); pMessageMap != NULL;
	  pMessageMap = pMessageMap->pBaseMap)
#endif
	{
		// Note: catches BEGIN_MESSAGE_MAP(CMyClass, CMyClass)!
#ifdef _AFXDLL
		ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
#else
		ASSERT(pMessageMap != pMessageMap->pBaseMap);
#endif

		lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries, nMsg, nCode, nID);
		if (lpEntry != NULL)
		{
			// found it
			if (lpEntry->pfn == &CListBoxEx::OnSelchange)
				bRet = FALSE;
		}
	}
	return bRet;
}

void CListBoxEx::OnSysColorChange() 
{
	if (GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE))
	{
		if (m_hBrush)
			DeleteObject(m_hBrush);

		m_hBrush = CreateSolidBrush(m_bReadOnly ? ::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_WINDOW));
	}

	CListBox::OnSysColorChange();
}
