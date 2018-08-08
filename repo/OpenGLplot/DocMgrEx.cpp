/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 2001
//
// based on a sample of
// Paul DiLascia <askpd@pobox.com>
//
/////////////////////////////////////////////////////////////////////////////
// DocMgrEx.cpp: Implementierungsdatei
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "General.h"
#include "DocMgrEx.h"
#include "FileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4457)

static void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
		!strFilterExt.IsEmpty() &&
		pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
		!strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

IMPLEMENT_DYNAMIC(CDocManagerEx, CDocManager)

CDocManagerEx::CDocManagerEx()
{
}

CDocManagerEx::~CDocManagerEx()
{
}

CFileDialog* CDocManagerEx::OnCreateFileDialog(BOOL bOpenFileDialog)
{
	return new CFileDialogEx(bOpenFileDialog);
}

BOOL CDocManagerEx::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	// call virtual function to create dialog
	CFileDialog* pDlg = OnCreateFileDialog(bOpenFileDialog);
	ASSERT(pDlg);
	CFileDialog& dlgFile = *pDlg;

	// different to CDocManager::DoPromptFileName
	CString fileNameCopy(fileName);
	if (!fileNameCopy.IsEmpty() && fileNameCopy.Right(1) == _T("\\"))
	{
		dlgFile.m_ofn.lpstrInitialDir = fileNameCopy;
		fileName.Empty();
	}

	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = DoPromptFileName(dlgFile, nIDSTitle, lFlags, pTemplate);
	fileName.ReleaseBuffer();

	delete pDlg;

	return nResult;
}

BOOL CDocManagerEx::DoPromptFileName(CFileDialog& dlgFile, UINT nIDSTitle, DWORD lFlags, CDocTemplate* pTemplate)
{
	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	int nResult = dlgFile.DoModal();
	return nResult == IDOK;
}

void CDocManagerEx::OnFileOpen()
{
	CString newName;

	DWORD lFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		lFlags |= OFN_ALLOWMULTISELECT;

	// prompt the user (with all document templates)
	CFileDialog* pDlg = OnCreateFileDialog(TRUE);
	ASSERT(pDlg);
	CFileDialog& dlgFile = *pDlg;

	if (DoPromptFileName(dlgFile, AFX_IDS_OPENFILE, lFlags, NULL))
	{
		// open not cancelled
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			newName = dlgFile.GetNextPathName(pos);
			AfxGetApp()->OpenDocumentFile(newName);
				// if returns NULL, the user has already been alerted
		}
	}

	delete pDlg;
}
