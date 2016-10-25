/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 2001
//
/////////////////////////////////////////////////////////////////////////////
// CommandLineInfoEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "General.h"
#include "CommandLineInfoEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandLineInfoEx

CCommandLineInfoEx::CCommandLineInfoEx()
{
}

CCommandLineInfoEx::~CCommandLineInfoEx()
{
}

void CCommandLineInfoEx::ParseParam(const TCHAR* pszParam,BOOL bFlag,BOOL bLast)
{
	if (bFlag)
	{
		USES_CONVERSION;
		ParseParamFlagEx(T2CA(pszParam));
	}
	else
		ParseParamNotFlag(pszParam);

	ParseLast(bLast);
}

#ifdef UNICODE
void CCommandLineInfoEx::ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag)
		ParseParamFlagEx(pszParam);
	else
		ParseParamNotFlag(pszParam);

	ParseLast(bLast);
}
#endif // UNICODE

void CCommandLineInfoEx::ParseParamFlagEx(const char* pszParam)
{
	// OLE command switches are case insensitive, while
	// shell command switches are case sensitive

	CWinApp* pApp = AfxGetApp();
	BOOL bMultiDocTemplate = FALSE;

	POSITION pos = pApp->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(pos);
		if (pDocTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)))
		{
			bMultiDocTemplate = TRUE;
			break;
		}
	}

	if (lstrcmpA(pszParam, "e") == 0 && bMultiDocTemplate)
		m_nShellCommand = FileNothing;
	else
		CCommandLineInfo::ParseParamFlag(pszParam);
}
