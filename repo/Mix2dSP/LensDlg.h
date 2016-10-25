#if !defined(AFX_LENSDLG_H__37C96D91_B6E8_11D4_A89B_00B0D0994500__INCLUDED_)
#define AFX_LENSDLG_H__37C96D91_B6E8_11D4_A89B_00B0D0994500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LensDlg.h : header file
//

class CMixView;

extern CMixView* g_pView;

/////////////////////////////////////////////////////////////////////////////
// CLensDlg dialog

class CLensDlg : public CDialog
{
// Construction
public:
	CLensDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLensDlg)
	enum { IDD = IDD_LENSOPT };
	float	m_fBeamSize;
	float	m_fDistance;
	float	m_fFocus;
	float	m_fDistance_f;
	int		m_nNdist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLensDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLensDlg)
	afx_msg void OnLensOpt();
	afx_msg void OnLensUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LENSDLG_H__37C96D91_B6E8_11D4_A89B_00B0D0994500__INCLUDED_)
