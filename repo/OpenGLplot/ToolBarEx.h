/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: January 2000
//
/////////////////////////////////////////////////////////////////////////////
// ToolBarEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBAREX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_TOOLBAREX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if _MSC_VER >= 1200
/////////////////////////////////////////////////////////////////////////////
// CToolBarEx 

class AFX_EXT_CLASS_GENERAL CToolBarEx : public CToolBar
{
	DECLARE_DYNAMIC(CToolBarEx)

protected:
	void DrawGripper(CDC* pDC, const CRect& rect);
	void EraseNonClient();

// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CToolBarEx)
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#endif // _MSC_VER >= 1200

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f�gt zus�tzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_TOOLBAREX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
