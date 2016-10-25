/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: March 2000
//
/////////////////////////////////////////////////////////////////////////////
// PropertyPageAdjust.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "GlWinApp.h"
#include "PropertyPageAdjust.h"
#include "PropertySheetAdjust.h"
#include "Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CPropertyPageAdjust 

IMPLEMENT_DYNAMIC(CPropertyPageAdjust, CPropertyPageTts)

CPropertyPageAdjust::CPropertyPageAdjust() : CPropertyPageTts()
{
}

CPropertyPageAdjust::CPropertyPageAdjust(UINT nIDTemplate, UINT nIDCaption)
	:CPropertyPageTts(nIDTemplate, nIDCaption)
{
}

CPropertyPageAdjust::CPropertyPageAdjust(LPCTSTR lpszTemplateName, UINT nIDCaption)
	:CPropertyPageTts(lpszTemplateName, nIDCaption)
{
}

CPropertyPageAdjust::~CPropertyPageAdjust()
{
}


BEGIN_MESSAGE_MAP(CPropertyPageAdjust, CPropertyPageTts)
	//{{AFX_MSG_MAP(CPropertyPageAdjust)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPropertyPageAdjust 

BOOL CPropertyPageAdjust::OnSetActive() 
{
	if (CPropertyPageTts::OnSetActive() == FALSE)
		return FALSE;

	CWnd* pParent = GetParent();
	if (pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CPropertySheetAdjust)))
		pParent->SendMessage(WM_ADJUST_PROPERTY_SHEET);

	return TRUE;
}

int CPropertyPageAdjust::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPageTts::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectTemplateDLU = GetTemplateRect(m_psp.pszTemplate);

	return 0;
}
