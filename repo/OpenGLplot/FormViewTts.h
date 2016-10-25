/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 2004
//
/////////////////////////////////////////////////////////////////////////////
// FormViewTts.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMVIEWTTS_H__1302C8E3_3424_461A_AFEB_6A8CE3220970__INCLUDED_)
#define AFX_FORMVIEWTTS_H__1302C8E3_3424_461A_AFEB_6A8CE3220970__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Formularansicht CFormViewTts 

class AFX_EXT_CLASS_GENERAL CFormViewTts : public CFormView
{
	DECLARE_DYNAMIC(CFormViewTts)
public:
	CFormViewTts(LPCTSTR lpszTemplateName);
	CFormViewTts(UINT nIDTemplate);

// Attribute
protected:
	BOOL m_bToolTipMultiline;

// Operationen
public:
	void SetToolTipMultiline(BOOL bToolTipMultiline = TRUE);
	BOOL GetToolTipMultiline() const;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFormViewTts)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CFormViewTts();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFormViewTts)
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
inline void CFormViewTts::SetToolTipMultiline(BOOL bToolTipMultiline)
	{ m_bToolTipMultiline = bToolTipMultiline; }
inline BOOL CFormViewTts::GetToolTipMultiline() const
	{ return m_bToolTipMultiline; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FORMVIEWTTS_H__1302C8E3_3424_461A_AFEB_6A8CE3220970__INCLUDED_
