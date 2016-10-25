/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 2004
//
/////////////////////////////////////////////////////////////////////////////
// PropertyPageTts.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYPAGETTS_H__5997567F_7CE8_417C_9308_0311DC5003A9__INCLUDED_)
#define AFX_PROPERTYPAGETTS_H__5997567F_7CE8_417C_9308_0311DC5003A9__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPropertyPageTts 

class AFX_EXT_CLASS_GENERAL CPropertyPageTts : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageTts)

// Konstruktion
public:
	CPropertyPageTts();
	CPropertyPageTts(UINT nIDTemplate, UINT nIDCaption = 0);
	CPropertyPageTts(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

// Attribute
protected:
	BOOL m_bToolTipMultiline;

// Operationen
public:
	void SetToolTipMultiline(BOOL bToolTipMultiline = TRUE);
	BOOL GetToolTipMultiline() const;

// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CPropertyPageTts)
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CPropertyPageTts();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CPropertyPageTts)
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
inline void CPropertyPageTts::SetToolTipMultiline(BOOL bToolTipMultiline)
	{ m_bToolTipMultiline = bToolTipMultiline; }
inline BOOL CPropertyPageTts::GetToolTipMultiline() const
	{ return m_bToolTipMultiline; }

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PROPERTYPAGETTS_H__5997567F_7CE8_417C_9308_0311DC5003A9__INCLUDED_
