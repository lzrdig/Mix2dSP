/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 2001
//
// based on a sample of
// Paul DiLascia <askpd@pobox.com>
//
/////////////////////////////////////////////////////////////////////////////
// DocMgrEx.h : Header-Datei
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCMANAGEREX_H__32FEE7F4_6BAD_11D5_8D53_00001CD5E4D1__INCLUDED_)
#define AFX_DOCMANAGEREX_H__32FEE7F4_6BAD_11D5_8D53_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocManagerEx - extension of CDocManager that uses CFileDialogEx

class AFX_EXT_CLASS_GENERAL CDocManagerEx : public CDocManager
{
	DECLARE_DYNAMIC(CDocManagerEx)
public:
	CDocManagerEx();

	// helper for standard commdlg dialogs
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual BOOL DoPromptFileName(CFileDialog& dlgFile, UINT nIDSTitle,
			DWORD lFlags, CDocTemplate* pTemplate);
	virtual void OnFileOpen();

protected:
	// new function to create the file dialog
	virtual CFileDialog* OnCreateFileDialog(BOOL bOpenFileDialog);

public:
	virtual ~CDocManagerEx();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DOCMANAGEREX_H__32FEE7F4_6BAD_11D5_8D53_00001CD5E4D1__INCLUDED_
