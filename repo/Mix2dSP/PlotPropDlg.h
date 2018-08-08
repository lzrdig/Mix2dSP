#if !defined(AFX_PLOTPROPDLG_H__04EC151F_9E30_11D5_8F75_0048546F01E9__INCLUDED_)
#define AFX_PLOTPROPDLG_H__04EC151F_9E30_11D5_8F75_0048546F01E9__INCLUDED_

#include "PlotPropTabCtrl.h"


// CPlotPropDlg dialog

class CPlotPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlotPropDlg)

public:
	CPlotPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotPropDlg();

// Dialog Data
	enum { IDD = IDD_PLOTPROP };
	CPlotPropTabCtrl	m_tbCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

#endif // !defined(AFX_PLOTPROPDLG_H__04EC151F_9E30_11D5_8F75_0048546F01E9__INCLUDED_)