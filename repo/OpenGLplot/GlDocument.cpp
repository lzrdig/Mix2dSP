/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: September 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlDocument.cpp: implementation of class CGlDocument
//

#include "StdAfx.h"
#include "General.h"
#include "GlWinApp.h"
#include "GlDocument.h"
#include "GlView.h"
#include "SplitterWndEx.h"
#include "Helper.h"
#include "Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);

/////////////////////////////////////////////////////////////////////////////
// CGlDocument

IMPLEMENT_DYNAMIC(CGlDocument, CDocument)

BEGIN_MESSAGE_MAP(CGlDocument, CDocument)
	//{{AFX_MSG_MAP(CGlDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlDocument construction/destruction

CGlDocument::CGlDocument()
{
	if (m_pWatchWorkerThread == NULL)
	{
		m_pWatchWorkerThread =
			AfxBeginThread(WatchThreadProc, this, THREAD_PRIORITY_BELOW_NORMAL);
	}

	m_hwndNotifyFileChanged = AfxGetSafeMainWnd()->GetSafeHwnd();
}

CGlDocument::~CGlDocument()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGlDocument Diagnose

#ifdef _DEBUG
void CGlDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CGlDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);

	dc << "\nm_bFileLost = " << m_bFileLost;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGlDocument Message handlers

int CGlDocument::MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CWnd* pWnd = AfxGetMainWnd();
	return pWnd->MessageBox(lpszText, lpszCaption, nType);
}

void CGlDocument::AddStarToTitle()
{
	CString mTitle = GetTitle();
	if (mTitle.GetLength() > 0)
	{
		if (mTitle.Right(1) != "*")
		{
			mTitle += "*";
			SetTitle(mTitle);
		}
	}
}

void CGlDocument::RemoveStarFromTitle()
{
	CString mTitle = GetTitle();
	if (mTitle.GetLength() > 0)
	{
		if (mTitle.Right(1) == "*")
		{
			mTitle = mTitle.Left(mTitle.GetLength()-1);
			SetTitle(mTitle);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Dokument öffnen/schließen
/////////////////////////////////////////////////////////////////////////////

void CGlDocument::DeleteContents() 
{
	// stop file watching
	SetEvent(m_hEventWatchDone);

	// In a SDI environment m_hwndNotifyFileChanged may have been set
	//	to NULL at doc's construction time.
	if (!m_hwndNotifyFileChanged)
		m_hwndNotifyFileChanged = AfxGetSafeMainWnd()->GetSafeHwnd();

	CDocument::DeleteContents();
}

BOOL CGlDocument::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// save the filename here since we need it in the watching worker tread
	// (by default this is done in OpenDocumentFile AFTER OnOpenDocument is done)
	TCHAR szFullPath[_MAX_PATH];
	AfxFullPath(szFullPath, lpszPathName);
	m_strPathName = szFullPath;

	// start file watching
	SetEvent(m_hEventStartWatch);

	return TRUE;
}

BOOL CGlDocument::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// stop file watching
	SetEvent(m_hEventWatchDone);

	BOOL bRet = CDocument::OnSaveDocument(lpszPathName);

	// start file watching
	SetEvent(m_hEventStartWatch);

	return bRet;
}

void CGlDocument::OnCloseDocument() 
{
	ShutDownWatchThreadSafely();

	CDocument::OnCloseDocument();
}

BOOL CGlDocument::OnReopenDocument(BOOL bUpdateViews) 
{
	// In derived classes override this function in order to 
	// reinitialize views...

	CString strPathName = GetPathName();
	ASSERT(!strPathName.IsEmpty());

	if (!OnOpenDocument(strPathName))
		return FALSE;

	if (bUpdateViews)
		UpdateAllViews(NULL);

	return TRUE;
}

int CGlDocument::OnFileAlarm(int nCause, LPVOID pExtra)
{
	// In derived classes override this function in order to 
	// achieve thread synchronistion with another worker thread...

	// To be used in some overridden function only
	UNREFERENCED_PARAMETER(pExtra);
	ASSERT(pExtra == NULL);

	int nRet = -1;
	UINT nFormatID, nType;
	CString strMsg;

	// inform user
	switch (nCause)
	{
	case FA_FILE_WRITTEN:
		if (!IsModified())
		{
			nFormatID = IDS_REOPEN_DOCUMENT_SAFE;
			nType = MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1;
		}
		else
		{
			nFormatID = IDS_REOPEN_DOCUMENT_UNSAFE;
			nType = MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2;
		}
		strMsg.Format(nFormatID, m_strPathNameWatch, AfxGetAppName());
		if ((nRet = AfxMessageBox(strMsg, nType)) == IDYES)
		{
			OnReopenDocument();
		}
		break;
	case FA_FILE_LOST:
		nFormatID = IDS_LOST_DOCUMENT;
		nType = MB_ICONINFORMATION | MB_OK;
		strMsg.Format(nFormatID, m_strPathNameWatch, AfxGetAppName());
		AfxMessageBox(strMsg, nType);
		break;
	default:
		{}
	}

	return nRet;
}

BOOL CGlDocument::SaveModified()
{
	BOOL bRet;

	if (IsModified())
		RemoveStarFromTitle();

	if (IsFileLost())
	{
		ASSERT(!m_strPathName.IsEmpty());

		// get name/title of document
		CString name;
		if (m_strPathName.IsEmpty())
		{
			// get name based on caption
			name = GetTitle();
			if (name.IsEmpty())
				VERIFY(name.LoadString(AFX_IDS_UNTITLED));
		}
		else
		{
			// get name based on file title of path name
			name = m_strPathName;
			BOOL bMarked4 = (GetProcessVersion(0) >= 0x00040000);
			if (bMarked4)
			{
				AfxGetFileTitle(m_strPathName, name.GetBuffer(_MAX_PATH), _MAX_PATH);
				name.ReleaseBuffer();
			}
		}

		CString prompt;
		AfxFormatString1(prompt, IDS_ASK_FILE_TO_SAVE, name);
		switch (AfxMessageBox(prompt, MB_YESNOCANCEL, IDS_ASK_FILE_TO_SAVE))
		{
		case IDCANCEL:
			bRet = FALSE;			// don't continue
			break;
		case IDYES:
			// If so, either Save or Update, as appropriate
			if (m_strPathName.IsEmpty())
			{
				if (!DoSave(NULL))
				{
					TRACE0("Warning: File save with new name failed.\n");
					bRet = FALSE;	// don't continue
				}
				else
					bRet = TRUE;	// keep going
			}
			else
			{
				if (!DoSave(m_strPathName))
				{
					TRACE0("Warning: File save failed.\n");
					bRet = FALSE;	// don't continue
				}
				else
					bRet = TRUE;	// keep going
			}
			break;
		case IDNO:
			// If not saving changes, revert the document
			bRet = TRUE;			// keep going
			break;
		default:
			bRet = FALSE;			// don't continue
			ASSERT(FALSE);
			break;
		}
	}
	else
	{
		bRet = CDocument::SaveModified();
	}

	if (IsModified())
		AddStarToTitle();

	return bRet;
}

BOOL CGlDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	if (IsModified())
		RemoveStarFromTitle();

	BOOL bRet = CDocument::DoSave(lpszPathName, bReplace);

	if (bRet)
		SetFileLostFlag(FALSE);

	if (IsModified())
		AddStarToTitle();

	return bRet;
}

void CGlDocument::SetModifiedFlag(BOOL bModified)
{
	if (bModified && !IsModified())
		AddStarToTitle();
	else if (!bModified && IsModified())
		RemoveStarFromTitle();

	CDocument::SetModifiedFlag(bModified);
}

void CGlDocument::CheckVersion(const CArchive& ar, const CString& strFileVersion)
{
	double dAppVersion, dFileVersion;
	CString strAppVersion, strInternalName;
	TCHAR szFullPath[_MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), szFullPath, _countof(szFullPath));
	GetVersionInfo(szFullPath, _T("FileVersion"), &strAppVersion);
	GetVersionInfo(szFullPath, _T("InternalName"), &strInternalName);

	dAppVersion = atof(strAppVersion);
	dFileVersion = atof(strFileVersion);

	if (dFileVersion > dAppVersion)
	{
		CString msg;
		msg.Format(IDS_MB_WARNING_VERSION, strInternalName, strAppVersion, 
			ar.GetFile()->GetFilePath(), 
			strInternalName, strFileVersion);
		if (AfxMessageBox(msg, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
			AfxThrowUserException();
	}
}

void CGlDocument::OnPrint(CDC* pDC, CPrintInfo* pInfo, CGlView* pCallingView) 
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();

	SIZE	res;			// Device resolution
	RECT	rect;			// Page rectangle
	HBRUSH	brush;			// Background brush for page
	POINT	PageSize;		// Size of printed image
	POINT	ImageOffset;	// Offset from edges for image
	int		nXRes, nYRes;
	HDC		hDC, hMemDC, hTmpDC;
	HGLRC	hRC, hMemRC;

	BITMAPINFO	bitmapInfo;
	HBITMAP		hDib;
	LPVOID		pBitmapBits;
	double		fac = 1.;		// try <printer device> DPI

	// Set our device context mapping mode.
	// Each logical unit is converted to 1 device pixel. Positive x is to
	// the right; positive y is down.
	pDC->SetMapMode(MM_TEXT);

	// Get the size of the page (in printer device points)
	rect.top	= 0;
	rect.left   = 0;
	rect.bottom	= 0;
	rect.right   = 0;

	if (pInfo->m_bPreview)
	{
		PRINTDLG PrtDlg;	// Printer dialog

		if (!pApp->GetPrinterDeviceDefaults(&PrtDlg))
		{
			TRACE("No default printer.\n");
			// Just create a display device context.
			// Note that the DIB image will NOT be 
			// positioned properly, but it will at least
			// be displayed.

			if (pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC)))
			{
				CPreviewDC *pPrevDC = (CPreviewDC *)pDC;
				//m_hAttribDC hold the print/fax DC so...
				rect.right =  GetDeviceCaps(pPrevDC->m_hAttribDC, HORZRES);
				rect.bottom = GetDeviceCaps(pPrevDC->m_hAttribDC, VERTRES);
				res.cx = GetDeviceCaps(pPrevDC->m_hAttribDC, LOGPIXELSX);
				res.cy = GetDeviceCaps(pPrevDC->m_hAttribDC, LOGPIXELSY);
			}
			else
			{
				rect.right = 0;
				rect.bottom = 0;
				res.cx = 0;
				res.cy = 0;
			}
		}
		else
		{
			// Don't display the dialog.  We create the CPrintDialog so that
			// we can create a printer device context.
			CPrintDialog dlg(FALSE);

			dlg.m_pd.hDevMode  = PrtDlg.hDevMode;
			dlg.m_pd.hDevNames = PrtDlg.hDevNames;

			HDC hdc = dlg.CreatePrinterDC();

			// Get the size of the default printer page
			rect.right  = GetDeviceCaps(hdc, HORZRES);
			rect.bottom = GetDeviceCaps(hdc, VERTRES);

			// Get the resolution of the printer device
			res.cx = GetDeviceCaps(hdc, LOGPIXELSX);
			res.cy = GetDeviceCaps(hdc, LOGPIXELSY);

			// Free the device context
			DeleteDC(hdc);
		}
	}
	else
	{
		// Get the size of the actual printed page
		rect.right  = GetDeviceCaps(pDC->GetSafeHdc(), HORZRES);
		rect.bottom = GetDeviceCaps(pDC->GetSafeHdc(), VERTRES);

		// Get the resolution of the printer device
		res.cx = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSX);
		res.cy = GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY);
	}
	nXRes = rect.right-rect.left;
	nYRes = rect.bottom-rect.top;

	// Reduce the Resolution if the Bitmap size is bigger than m_nMaxPrintMemSize*1024
	while (3*nXRes*nYRes > (pApp->m_nMaxPrintMemSize)*1024) 
	{
		nXRes /= 2;
		nYRes /= 2;
		fac /= 2.;
	}

	//nXRes = (nXRes + (sizeof(DWORD)-1)) & ~(sizeof(DWORD)-1);	// aligning width to 4 bytes (sizeof(DWORD)) avoids 
	nXRes = nXRes & ~(sizeof(DWORD)-1);							// aligning width to 4 bytes (sizeof(DWORD)) avoids 
																// pixel garbage at the upper line

	// arrange views
	CView *pView;
	POSITION pos;
	double printerAspect = (nYRes == 0) ? (double)nXRes : (double)nXRes/(double)nYRes;
	double averageAspect = 0.;

	// determine number of views to be printed
	int nNumberOfViewsToPrint = 0;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			nNumberOfViewsToPrint++;
			averageAspect += ((CGlView*)pView)->m_aspect;

			// check if pView is in a hidden row/column of an extended splitter view
			CWnd* pParent = pView->GetParent();
			if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
			{
				int nRow, nCol;

				VERIFY(((CSplitterWndEx*)pParent)->IsChildPaneEx(pView, &nRow, &nCol));
				if (nRow >= ((CSplitterWndEx*)pParent)->GetRowCount() || 
					nCol >= ((CSplitterWndEx*)pParent)->GetColumnCount())		// row/column is hidden
				{
					nNumberOfViewsToPrint--;
					averageAspect -= ((CGlView*)pView)->m_aspect;
				}
			}
		}
	}
	ASSERT(nNumberOfViewsToPrint > 0);
	averageAspect /= (double)nNumberOfViewsToPrint;
	ASSERT(averageAspect > 0.);
	ASSERT(printerAspect > 0.);
	if (nNumberOfViewsToPrint == 0 || averageAspect == 0. || printerAspect == 0.)
		return;

	// determine how many rows/columns are needed to arrange views optimal
	// nX - number of columns
	// nY - number of rows
	int nX, nY, nX1, nY1, nX2, nY2;
	nX1 = nX = (int)ceil(sqrt(nNumberOfViewsToPrint*printerAspect/averageAspect));
	nY2 = nY = (int)ceil(sqrt(nNumberOfViewsToPrint*averageAspect/printerAspect));
	nY1 = (int)ceil(double(nNumberOfViewsToPrint)/double(nX1));
	nX2 = (int)ceil(double(nNumberOfViewsToPrint)/double(nY2));
	if (nX1*nY1 < nX2*nY2)
	{
		nX = nX1;
		nY = nY1;
	}
	else
	{
		nX = nX2;
		nY = nY2;
	}

	// how does a view with averageAspect fill it's area?
	// (cut height if nessesary)
	int nXViewRes = nXRes/nX, nYViewRes = nYRes/nY;
	if (double(nXViewRes)/double(nYViewRes) < averageAspect)	// average view aspect is larger than reserved place, i.e. view is too wide
		nYViewRes = int(nXViewRes/averageAspect);

	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			((CGlView*)pView)->ScaleFont(double(nYViewRes)/double(pCallingView->m_DocPrintHeight));
		}
	}

	// First of all, initialize the bitmap header information...
	memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth		= nXRes;
	bitmapInfo.bmiHeader.biHeight		= nYRes;
	bitmapInfo.bmiHeader.biPlanes		= 1;
	bitmapInfo.bmiHeader.biBitCount		= 24;
	bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * 3;
	bitmapInfo.bmiHeader.biXPelsPerMeter = int(res.cx*fac/.0254);	// res.cx*fac DPI
	bitmapInfo.bmiHeader.biYPelsPerMeter = int(res.cy*fac/.0254);	// res.cy*fac DPI

	// create DIB
	hTmpDC = ::GetDC(pCallingView->m_hWnd);
	hDib = CreateDIBSection(hTmpDC, &bitmapInfo, DIB_RGB_COLORS, &pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(pCallingView->m_hWnd, hTmpDC);

	// create memory device context
	if ((hMemDC = CreateCompatibleDC(pCallingView->m_pDC == NULL ? NULL : pCallingView->m_pDC->GetSafeHdc())) == NULL)
	{
		DeleteObject(hDib);
		return;
	}
	HGDIOBJ hOldDib = SelectObject(hMemDC, hDib);

	// setup pixel format
	if (!pCallingView->SetMemDcPixelFormat(hMemDC) && !pCallingView->SetMemDcPixelFormat(hMemDC, TRUE))
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return;
	}

	// create memory rendering context
	if ((hMemRC = wglCreateContext(hMemDC)) == NULL)
	{
		if (hOldDib != NULL)
			SelectObject(hMemDC, hOldDib);
		DeleteObject(hDib);
		DeleteDC(hMemDC);
		return;
	}

	// Store current rendering and device contexts
	pCallingView->GetCurrent(hDC, hRC);

	// Make this hMemRC the current OpenGL rendering context.
	pCallingView->SetCurrent(hMemDC, hMemRC);

	pCallingView->SetOpenGLProperties();

	// must be done here, not in pView->OnDraw()
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// must be created once for hMemDC
	pCallingView->CreateFontBitmaps();

	// draw (loop over all views to be printed)
	BOOL bPrint;
	int i = -1;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			bPrint = TRUE;
			i++;

			// check if pView is in a hidden row/column of an extended splitter view
			CWnd* pParent = pView->GetParent();
			if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CSplitterWndEx)))
			{
				int nRow, nCol;

				VERIFY(((CSplitterWndEx*)pParent)->IsChildPaneEx(pView, &nRow, &nCol));
				if (nRow >= ((CSplitterWndEx*)pParent)->GetRowCount() || 
					nCol >= ((CSplitterWndEx*)pParent)->GetColumnCount())		// row/column is hidden
				{
					bPrint = FALSE;
					i--;
				}
			}

			if (bPrint)
			{
				nXViewRes = int(nYViewRes*((CGlView*)pView)->m_aspect);

				glViewport((nXRes/nX - nXViewRes)/2 + (i/nY) * (nXRes/nX), (nYRes/nY - nYViewRes)/2 + (nY-1-i%nY) * (nYRes/nY), nXViewRes, nYViewRes);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glEnable(GL_DEPTH_TEST);

				((CGlView*)pView)->m_bIsDocPrinting = TRUE;
				((CGlView*)pView)->OnDraw(pDC);
				((CGlView*)pView)->m_bIsDocPrinting = FALSE;
			}
		}
	}
	glFinish();	// Finish all OpenGL commands

	// the rendering context will be no longer needed
	pCallingView->SetCurrent(NULL, NULL);
	wglDeleteContext(hMemRC);

	// Restore last rendering and device contexts
	if (hDC != NULL && hRC != NULL)
		pCallingView->SetCurrent(hDC, hRC);

	// Restore the Views original font sizes
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CGlView)))
		{
			((CGlView*)pView)->UnScaleFont();
			((CGlView*)pView)->CreateFontBitmaps();
		}
	}

	// Get the size of our page and clear to White
	brush = CreateSolidBrush(0x00ffffff);		// White brush
	FillRect(pDC->GetSafeHdc(), &rect, brush);	// Clear the page

	PageSize.x = rect.right;
	PageSize.y = PageSize.x * bitmapInfo.bmiHeader.biHeight / bitmapInfo.bmiHeader.biWidth;
	if (PageSize.y > rect.bottom)
	{
		PageSize.y = rect.bottom;
		PageSize.x = PageSize.y * bitmapInfo.bmiHeader.biWidth / bitmapInfo.bmiHeader.biHeight;
	}
	ImageOffset.x = /*pApp->m_bPrintCenter ? (rect.right - PageSize.x) / 2 :*/ 0;
	ImageOffset.y = /*pApp->m_bPrintCenter ? (rect.bottom - PageSize.y) / 2 :*/ 0;

	// Does our device context support StretchDIBits?
	if (pCallingView->m_bStretchDIBits)
	{
		StretchDIBits(pDC->GetSafeHdc(),
								ImageOffset.x, ImageOffset.y,
								PageSize.x, PageSize.y,
								0, 0,
								bitmapInfo.bmiHeader.biWidth,
								bitmapInfo.bmiHeader.biHeight,
								(GLubyte*)pBitmapBits,
								&bitmapInfo,
								DIB_RGB_COLORS,
								SRCCOPY);
	}
	else if (pCallingView->m_bStretchBlt)
	{
		HDC hdc;
		HBITMAP bitmap;

		hdc	= CreateCompatibleDC(pDC->GetSafeHdc());
		bitmap = CreateDIBitmap(hdc,
								&(bitmapInfo.bmiHeader),
								CBM_INIT,
								(GLubyte*)pBitmapBits,
								&bitmapInfo,
								DIB_RGB_COLORS);

		HGDIOBJ oldbitmap = SelectObject(hdc, bitmap);
		StretchBlt(pDC->GetSafeHdc(),
								ImageOffset.x, ImageOffset.y,
								PageSize.x, PageSize.y,
								hdc,
								0, 0,
								bitmapInfo.bmiHeader.biWidth,
								bitmapInfo.bmiHeader.biHeight,
								SRCCOPY);
		if (oldbitmap != NULL)
			SelectObject(hdc, oldbitmap);

		// Free our bitmap and bitmap device context
		DeleteObject(bitmap);
		DeleteDC(hdc);
	}
	if (hOldDib != NULL)
		SelectObject(hMemDC, hOldDib);

	// Delete our brush, DIB and device context
	DeleteObject(brush);
	DeleteObject(hDib);
	DeleteDC(hMemDC);
}
