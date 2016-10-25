/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2002
//
/////////////////////////////////////////////////////////////////////////////
// ColorDialogEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORDIALOGEX_H__A58BA8A2_205E_11D6_8C99_00001CD5E4D1__INCLUDED_)
#define AFX_COLORDIALOGEX_H__A58BA8A2_205E_11D6_8C99_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CColorDialogEx 

class AFX_EXT_CLASS_GENERAL CColorDialogEx : public CColorDialog
{
	DECLARE_DYNAMIC(CColorDialogEx)

public:
	CColorDialogEx(COLORREF clrInit = 0, DWORD dwFlags = 0,
			CWnd* pParentWnd = NULL);
	CColorDialogEx(LPCTSTR szTitle, COLORREF clrInit = 0, DWORD dwFlags = 0,
			CWnd* pParentWnd = NULL);
	CColorDialogEx(int nID, COLORREF clrInit = 0, DWORD dwFlags = 0,
			CWnd* pParentWnd = NULL);

protected:
	BOOL m_bUseStrTitle;
	CString m_strTitle;
	//{{AFX_MSG(CColorDialogEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_COLORDIALOGEX_H__A58BA8A2_205E_11D6_8C99_00001CD5E4D1__INCLUDED_
