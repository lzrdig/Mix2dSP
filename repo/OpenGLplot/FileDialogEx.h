/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 2001
//
// based on a sample of
// Paul DiLascia <askpd@pobox.com>
//
/////////////////////////////////////////////////////////////////////////////
// FileDialogEx.h : Header-Datei
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDIALOGEX_H__32FEE7F4_6BAD_11D5_8D53_00001CD5E4D1__INCLUDED_)
#define AFX_FILEDIALOGEX_H__32FEE7F4_6BAD_11D5_8D53_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFileDialogEx 

class AFX_EXT_CLASS_GENERAL CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE für FileOpen, FALSE für FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFileDialogEx)
	public:
	virtual int DoModal();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Windows 2000 version of OPENFILENAME.
	// The new version has three extra members.
	// This is copied from commdlg.h
	struct OPENFILENAMEEX : public OPENFILENAME
	{ 
		void* pvReserved;
		DWORD dwReserved;
		DWORD FlagsEx;
	};
	OPENFILENAMEEX m_ofnEx;		// new Windows 2000 version of OPENFILENAME

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFileDialogEx)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FILEDIALOGEX_H__32FEE7F4_6BAD_11D5_8D53_00001CD5E4D1__INCLUDED_
