#if !defined(AFX_DELTAKDLG_H__D3042301_B13C_11D4_A894_00E029404812__INCLUDED_)
#define AFX_DELTAKDLG_H__D3042301_B13C_11D4_A894_00E029404812__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//********************************
// DeltakDlg.h : header file
//

class CMixView;

extern CMixView* g_pView;

/////////////////////////////////////////////////////////////////////////////
// CDeltakDlg dialog

class CDeltakDlg : public CDialog
{
// Construction
public:
	CDeltakDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeltakDlg)
	enum { IDD = IDD_DELTAK };
	double	m_dkInit;
	double	m_dkFinal;
	int		m_Ndk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeltakDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeltakDlg)
	afx_msg void OnDeltakUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELTAKDLG_H__D3042301_B13C_11D4_A894_00E029404812__INCLUDED_)
