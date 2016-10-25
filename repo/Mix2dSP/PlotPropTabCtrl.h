#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// CPlotPropTabCtrl dialog

class CPlotPropTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CPlotPropTabCtrl)

public:
	CPlotPropTabCtrl();   // standard constructor
	virtual ~CPlotPropTabCtrl();

	int m_DialogIDs[2];
	CDialog *m_Dialogs[2];

	int m_nPageCount;

	void ActivateTabDialogs();
	void InitDialogs();

// Dialog Data
	enum { IDD = IDC_PLOTTABS };

protected:
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};
