#pragma once


// CPropSht1 dialog

class CPropSht1 : public CDialog
{
	DECLARE_DYNAMIC(CPropSht1)

public:
	CPropSht1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropSht1();

// Dialog Data
	enum { IDD = IDD_PLOTPROPSHT1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChckFrame();
	BOOL m_bFrame;
	BOOL m_bAxisLabel;
	int m_nGrid;
	afx_msg void OnChkGrid();
};
