#if !defined(AFX_ERGDLG_H__BF4E1F21_B5A7_11D4_A89B_00B0D0994500__INCLUDED_)
#define AFX_ERGDLG_H__BF4E1F21_B5A7_11D4_A89B_00B0D0994500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErgDlg.h : header file
//
class CMixView;

extern CMixView* g_pView;

/////////////////////////////////////////////////////////////////////////////
// CErgDlg dialog

class CErgDlg : public CDialog
{
// Construction
public:
	CErgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CErgDlg)
	enum { IDD = IDD_ENERGY };
	double	m_dEidle_f;
	double	m_dEidle_i;
	double	m_dEpump_f;
	double	m_dEpump_i;
	double	m_dEsig_f;
	double	m_dEsig_i;
	int		m_nNerg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CErgDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnErgUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERGDLG_H__BF4E1F21_B5A7_11D4_A89B_00B0D0994500__INCLUDED_)
