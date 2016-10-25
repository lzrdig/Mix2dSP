#pragma once

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
