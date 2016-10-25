#if !defined(AFX_PLOTSDLG_H__4D25CAA2_B5CB_11D4_A89B_00B0D0994500__INCLUDED_)
#define AFX_PLOTSDLG_H__4D25CAA2_B5CB_11D4_A89B_00B0D0994500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <fstream>
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>

// PlotsDlg.h : header file
//

typedef struct Fluences
{
	double* pXgrid;
	double* pYgrid;
	double* pFluence_s;
	double* pFluence_i;
	double* pFluence_p;
}Fluences;

typedef struct Energies
{
	double* pXgrid;
	double* pYgrid;
	double* pEnergy_s;
	double* pEnergy_i;
	double* pEnergy_p;
}Energies;

class CFields;
class CInputs;
class CGlobalVar;

/////////////////////////////////////////////////////////////////////////////
// CPlotsDlg dialog

class CPlotsDlg : public CDialog
{
// Construction
public:
	CPlotsDlg(CWnd* pParent);   // standard constructor
	int LoadFluenceData();
	void BuildList();
	int LoadFieldData();
	BOOL Create();
	BOOL CreateViewGLContext(HDC hDC);
	BOOL SetWindowPixelFormat(HDC hDC);
	void RenderScene();

// Dialog Data
	//{{AFX_DATA(CPlotsDlg)
	enum { IDD = IDD_PLOTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd*	m_pParent;
	int		m_nID;
	CPoint	m_LDownPos;
	BOOL	m_LButtonDown;
	BOOL	m_bFluenceLoaded;
	HGLRC	m_hGLContext;
	int		m_GLPixelIndex;
	float	m_ScaleZ;
	float	m_ScaleY;
	float	m_ScaleX;
	GLdouble m_xRotate;
	GLdouble m_yRotate;
	CFields*	pField;
	CInputs*	pPulse;
	CGlobalVar* pGV;
	Fluences* pFluenceData;

	// Generated message map functions
	//{{AFX_MSG(CPlotsDlg)
	afx_msg void OnPlotsClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPlotLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTSDLG_H__4D25CAA2_B5CB_11D4_A89B_00B0D0994500__INCLUDED_)
