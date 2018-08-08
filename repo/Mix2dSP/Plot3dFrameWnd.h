#if !defined(AFX_PLOT3DFRAMEWND_H__04EC151F_9E30_11D5_8F75_0048546F01E7__INCLUDED_)
#define AFX_PLOT3DFRAMEWND_H__04EC151F_9E30_11D5_8F75_0048546F01E7__INCLUDED_

#include "mix2dsp.h"

#include "SplitterWndEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Plot3dFrameWnd.h : header file
//
class CPlotPropDlg;

/////////////////////////////////////////////////////////////////////////////
// CPlot3dFrameWnd frame

class CPlot3dFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CPlot3dFrameWnd)
protected:
	CPlot3dFrameWnd();           // protected constructor used by dynamic creation
	CToolBar    m_wndToolBar;

// Attributes
public:
	bool m_bRotationOn;
	bool m_bPanOn;
	bool m_bEditPlotPropsOn;
	bool m_bSolidSurfOn;
	bool m_bWireSurfOn;
	bool m_bDotSurfOn;
	bool m_bFrmOn;
	CPlotPropDlg* m_pPlotPropDlg;
	CSplitterWndEx m_wndSplitter;

// Operations
public:


// Overrides
	virtual BOOL CreatePlot3dFrame(UINT nIDResource,
				DWORD dwDefaultStyle, CRect rect,
				CWnd* pParentWnd, DWORD dwDefaultStyleEx, 
				CCreateContext* pContext);

// Implementation
protected:
	virtual ~CPlot3dFrameWnd();

	// Generated message map functions
	//{{AFX_MSG(CDFVFrameWnd)
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRotateView();
	afx_msg void OnUpdateRotateView(CCmdUI *pCmdUI);
	afx_msg void OnPanView();
	afx_msg void OnUpdatePanView(CCmdUI *pCmdUI);
	afx_msg void OnEditPlotProps();
	afx_msg void OnUpdateEditPlotProps(CCmdUI *pCmdUI);
	afx_msg void OnSolidSurf();
	afx_msg void OnUpdateOnSolidSurf(CCmdUI *pCmdUI);
	afx_msg void OnWireSurf();
	afx_msg void OnUpdateOnWireSurf(CCmdUI *pCmdUI);
	afx_msg void OnDotSurf();
	afx_msg void OnUpdateOnDotSurf(CCmdUI *pCmdUI);
	afx_msg void OnFrame();
	afx_msg void OnUpdateOnFrame(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void CPlot3dFrameWnd::SizeLeftPane();
	afx_msg void RecalcLayout(BOOL bNotify);
	afx_msg BOOL IsLeftPaneClosed();
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFVFRAMEWND_H__04EC151F_9E30_11D5_8F75_0048546F01E7__INCLUDED_)
