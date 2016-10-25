#if !defined(AFX_DFVCTRL_H__04EC151B_9E30_11D5_8F75_0048546F01E7__INCLUDED_)
#define AFX_DFVCTRL_H__04EC151B_9E30_11D5_8F75_0048546F01E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DFVCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDFVCtrl window

#include "Plot3dDocTemplate.h"
#include "MainFrm.h"


class CPlot3dCtrl : public CWnd
{
// Construction
public:
	CPlot3dCtrl();

// Attributes
public:
	CWnd *m_pParent;

	CPlot3dDocTemplate * m_pDocTemplate;

// Operations
public:
	BOOL Create(CWnd *pParent, DWORD ctrlId,
				UINT nIDResource,
				CRuntimeClass* pDocClass, 
				CRuntimeClass* pFrameClass,
				CRuntimeClass* pViewClass,
				DWORD Style, DWORD StyleEx);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDFVCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlot3dCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlot3dCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFVCTRL_H__04EC151B_9E30_11D5_8F75_0048546F01E7__INCLUDED_)
