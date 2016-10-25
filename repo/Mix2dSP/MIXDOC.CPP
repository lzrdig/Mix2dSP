// MixDoc.cpp : implementation of the CMixDoc class
//

#include "stdafx.h"
#include "Mix2dsp.h"

#include "MixDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMixDoc

IMPLEMENT_DYNCREATE(CMixDoc, CDocument)

BEGIN_MESSAGE_MAP(CMixDoc, CDocument)
	//{{AFX_MSG_MAP(CMixDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMixDoc construction/destruction

CMixDoc::CMixDoc()
{
	// TODO: add one-time construction code here

}

CMixDoc::~CMixDoc()
{
}

BOOL CMixDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMixDoc serialization

void CMixDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMixDoc diagnostics

#ifdef _DEBUG
void CMixDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMixDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMixDoc commands
