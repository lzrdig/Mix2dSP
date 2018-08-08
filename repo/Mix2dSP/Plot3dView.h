#if !defined(AFX_PLOT3DVIEW_H__04EC151F_9E30_11D5_8F75_0048546F01E8__INCLUDED_)
#define AFX_PLOT3DVIEW_H__04EC151F_9E30_11D5_8F75_0048546F01E8__INCLUDED_

#include "GlView.h"
#include "Matrix.h"
#include "Scale.h"
#include "FormPropertiesView.h"
#include "MathAdd.h"

class CPlot3dDoc;

// CPlot3dView view

class CPlot3dView : public CGlView
{
	DECLARE_DYNCREATE(CPlot3dView)

protected:
	CPlot3dView();           // protected constructor used by dynamic creation
	virtual ~CPlot3dView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	virtual void UpdateSliderScalePos();
	virtual void UpdateSliderTranslationPos();
	virtual void UpdateFontColorRect();
	virtual BOOL Is3D() const;
	virtual BOOL AllowScale() const;
	virtual BOOL AllowMonochrome() const;
	virtual BOOL AllowRemoveHidden() const;
	virtual BOOL AllowModel() const;
	virtual void EditProperties();
	virtual void UpdateEditProperties(CCmdUI* pCmdUI);
	CView* GetFormPropertiesView();
	CPlot3dDoc* GetDocument();

	KINDOFVIEW m_nKindOfView;
	OBJECTOFVIEW m_nObjectOfView;

protected:
	DECLARE_MESSAGE_MAP()
public:
// Mouse 
	virtual void LButtonSwitchOn(CPoint point);
	virtual void LButtonSwitchOff(CPoint point);
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditProperties();
	afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);
	BOOL MakeActive();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
};

#endif // !defined(AFX_PLOT3DVIEW_H__04EC151F_9E30_11D5_8F75_0048546F01E8__INCLUDED_)
