/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 2000
//
/////////////////////////////////////////////////////////////////////////////
// DlgOptions.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGOPTIONS_H__D0366E83_57B8_11D4_8BD8_00001CD5E4D1__INCLUDED_)
#define AFX_DLGOPTIONS_H__D0366E83_57B8_11D4_8BD8_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DialogTts.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDlgOptions 

class CDlgOptions : public CDialogTts
{
// Konstruktion
public:
	CDlgOptions(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDlgOptions)
	enum { IDD = IDD_DLG_OPTIONS };
	CEdit	m_CopyResolutionEdit;
	CButton	m_ReopenPolicyButton;
	CButton	m_ReopenPolicyGboxButton;
	CButton	m_SerializeResultBytesGboxButton;
	CButton	m_SerializeResultEightBytesButton;
	CButton	m_SerializeResultFourBytesButton;
	CButton	m_SerializeResultTwoBytesButton;
	CButton	m_SerializeResultOneByteButton;
	int		m_nRadioMousePolicy;
	BOOL	m_bCheckNoHardwareAcceleration;
	BOOL	m_bCheckPropPolicy;
	BOOL	m_bCheckReopenPolicy;
	int		m_nRadioSerializeResultBytes;
	int		m_nRadioCopyResolution;
	int		m_nCopyResolution;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDlgOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void EnableExtraEdit();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDlgOptions)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonShortcuts();
	afx_msg void OnRadioCopyResolution72();
	afx_msg void OnRadioCopyResolution75();
	afx_msg void OnRadioCopyResolution150();
	afx_msg void OnRadioCopyResolution300();
	afx_msg void OnRadioCopyResolution600();
	afx_msg void OnRadioCopyResolutionOther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DLGOPTIONS_H__D0366E83_57B8_11D4_8BD8_00001CD5E4D1__INCLUDED_
