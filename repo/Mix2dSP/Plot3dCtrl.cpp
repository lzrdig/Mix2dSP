// DFVCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Plot3dCtrl.h"
#include "GraphDLg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDFVCtrl

CPlot3dCtrl::CPlot3dCtrl()
{
	m_pDocTemplate = NULL;
	m_pParent = NULL;
}

CPlot3dCtrl::~CPlot3dCtrl()
{
}


BEGIN_MESSAGE_MAP(CPlot3dCtrl, CWnd)
	//{{AFX_MSG_MAP(CDFVCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDFVCtrl message handlers

BOOL CPlot3dCtrl::Create(CWnd *pParent, DWORD ctrlId,
		UINT nIDResource,
		CRuntimeClass* pDocClass, 
		CRuntimeClass* pFrameClass,
		CRuntimeClass* pViewClass,
		DWORD Style, DWORD StyleEx)
{
	m_pParent = pParent;

	CWnd* p_grDlg = m_pParent->GetDlgItem(ctrlId);

	m_pDocTemplate = new CPlot3dDocTemplate(
		nIDResource,
		pDocClass,
		pFrameClass,
		pViewClass);

	m_pDocTemplate->m_pParent = pParent;
	
	CRect clientRect;
	p_grDlg->GetWindowRect(clientRect);
    m_pParent->ScreenToClient(&clientRect);
	m_pDocTemplate->m_Rect = clientRect;	
	m_pDocTemplate->m_Style = Style;
	m_pDocTemplate->m_StyleEx = StyleEx;

	((CGraphDlg*)m_pParent)->m_pPlot3DDoc = (CPlot3dDoc*)m_pDocTemplate->OpenDocumentFile(NULL);

	return TRUE;
}
