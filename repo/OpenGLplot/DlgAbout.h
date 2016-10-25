/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DlgAbout.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGABOUT_H__06A35629_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_DLGABOUT_H__06A35629_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DialogTts.h"
#include "HyperLink.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgAbout 

class CDlgAbout : public CDialogTts
{
public:
	CDlgAbout();

// Dialog Data
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_HyperLink;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDlgAbout)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DLGABOUT_H__06A35629_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)

