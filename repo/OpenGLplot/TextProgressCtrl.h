/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: March 1999
//
// based on a sample of
// Chris Maunder <chrismaunder@codeguru.com>
//
/////////////////////////////////////////////////////////////////////////////
// TextProgressCtrl.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
#define AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define CR_DEFAULT		0
#define CR_HIGHLIGHT	1
#define CR_USERDEFINED	2

class AFX_EXT_CLASS_GENERAL CTextProgressCtrl : public CProgressCtrl
{
// Konstruktion
public:
	CTextProgressCtrl();

// Attribute
public:

// Operationen
public:
	int			SetPos(int nPos);
	int			StepIt();
	void		SetRange(int nLower, int nUpper);
	void		SetRangeUnknown(BOOL bRangeUnknown = TRUE);
	int			OffsetPos(int nPos);
	int			SetStep(int nStep);
	double		SetHandWidth(double dHand);
	void		SetForeColor(COLORREF col);
	void		SetBkColor(COLORREF col);
	void		SetTextForeColor(COLORREF col);
	void		SetTextBkColor(COLORREF col);
	COLORREF	GetForeColor();
	COLORREF	GetBkColor();
	COLORREF	GetTextForeColor();
	COLORREF	GetTextBkColor();
	void		DefaultColors();
	void		HighlightColors();

	BOOL		SetShowText(BOOL bShow);
	BOOL		SetShowHand(BOOL bShow);

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CTextProgressCtrl)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CTextProgressCtrl();

	// Generierte Nachrichtenzuordnungsfunktionen
private:
	BOOL		m_bStepDirForward;
protected:
	int			m_nPos, 
				m_nStepSize, 
				m_nMax, 
				m_nMin;
	CString		m_strText;
	BOOL		m_bShowText;
	BOOL		m_bShowHand;
	double		m_dHandWidth;
	int			m_nBarWidth;
	COLORREF	m_colFore,
				m_colBk,
				m_colTextFore,
				m_colTextBk;
	int			m_nStatColFore,
				m_nStatColBk,
				m_nStatColTextFore,
				m_nStatColTextBk;

	//{{AFX_MSG(CTextProgressCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
