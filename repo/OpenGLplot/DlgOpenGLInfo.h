/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2000
//
// based on a sample of
// Alessandro Falappa
//
/////////////////////////////////////////////////////////////////////////////
// DlgOpenGLInfo.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGOPENGLINFO_H__F0D35F51_E5D9_11D3_8B54_00001CD5E4D1__INCLUDED_)
#define AFX_DLGOPENGLINFO_H__F0D35F51_E5D9_11D3_8B54_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DialogTts.h"
#include "Helper.h"

#define ICD_DRIVER_MASK  (PFD_GENERIC_ACCELERATED | PFD_GENERIC_FORMAT)

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgOpenGLInfo 

class CDlgOpenGLInfo : public CDialogTts
{
// Konstruktion
public:
	CDlgOpenGLInfo(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgOpenGLInfo)
	enum { IDD = IDD_DLG_OPENGL_INFO };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA
	GlInfoStruct m_info;


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgOpenGLInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgOpenGLInfo)
	afx_msg void OnButtonDetails();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DLGOPENGLINFO_H__F0D35F51_E5D9_11D3_8B54_00001CD5E4D1__INCLUDED_
