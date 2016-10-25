#pragma once


// CPropSht2 dialog

class CPropSht2 : public CDialog
{
	DECLARE_DYNAMIC(CPropSht2)

public:
	CPropSht2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropSht2();

// Dialog Data
	enum { IDD = IDD_PLOTPROPSHT2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRadioModel2();
	afx_msg void OnRadioModel1();
	afx_msg void OnRadioModel0();
	void RadioModel();
};
