/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 2004
//
/////////////////////////////////////////////////////////////////////////////
// DialogTts.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGTTS_H__FC391540_74EC_405D_BCB1_7934F77D4FAE__INCLUDED_)
#define AFX_DIALOGTTS_H__FC391540_74EC_405D_BCB1_7934F77D4FAE__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDialogTts 

class AFX_EXT_CLASS_GENERAL CDialogTts : public CDialog
{
	DECLARE_DYNAMIC(CDialogTts)

// Modeless Konstruktion
public:
	CDialogTts();

// Modale Konstruktion
public:
	CDialogTts(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CDialogTts(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// Attribute
protected:
	BOOL m_bToolTipMultiline;

// Operationen
public:
	void SetToolTipMultiline(BOOL bToolTipMultiline = TRUE);
	BOOL GetToolTipMultiline() const;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDialogTts)
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CDialogTts();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDialogTts)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
inline void CDialogTts::SetToolTipMultiline(BOOL bToolTipMultiline)
	{ m_bToolTipMultiline = bToolTipMultiline; }
inline BOOL CDialogTts::GetToolTipMultiline() const
	{ return m_bToolTipMultiline; }

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_DIALOGTTS_H__FC391540_74EC_405D_BCB1_7934F77D4FAE__INCLUDED_
