/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 1999
//
/////////////////////////////////////////////////////////////////////////////
// GlWinApp.cpp : Implementierungsdatei
//

#include "StdAfx.h"
#include "GlWinApp.h"
#include "GlView.h"
#include "GlDocument.h"
#include "Helper.h"
#include "DlgAbout.h"
#include "DlgOptions.h"
#include "DlgListCtrl.h"
#include "DlgFilePrintOptions.h"
#include "DocMgrEx.h"
#include "WatchInThreadExtra.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF CGlWinApp::m_custColors[] = { RGB(0,0,0), RGB(128,0,0), RGB(0,128,0), RGB(128,128,0),
		RGB(0,0,128), RGB(128,0,128), RGB(0,128,128), RGB(128,128,128),
		RGB(192,192,192), RGB(255,0,0), RGB(0,255,0), RGB(255,255,0),
		RGB(0,0,255), RGB(255,0,255), RGB(0,255,255), RGB(255,255,255) };

/////////////////////////////////////////////////////////////////////////////
// CGlWinApp

IMPLEMENT_DYNAMIC(CGlWinApp, CWinApp)

BEGIN_MESSAGE_MAP(CGlWinApp, CWinApp)
	ON_THREAD_MESSAGE(WM_FILE_ALARM, OnFileAlarm)
	//{{AFX_MSG_MAP(CGlWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_HELP_SHORTCUTS, OnHelpShortcuts)
	ON_COMMAND(ID_FILE_PRINT_SETUP_EX, OnFilePrintSetupEx)
	ON_COMMAND(ID_CHANGE_MOUSE_POLICY, OnChangeMousePolicy)
	ON_COMMAND(ID_CHANGE_HARDWARE_ACCELERATION, OnChangeHardwareAcceleration)
	ON_COMMAND(ID_CHANGE_PROP_POLICY, OnChangePropPolicy)
	ON_COMMAND(ID_CHANGE_REOPEN_POLICY, OnChangeReopenPolicy)
	ON_COMMAND(ID_SERIALIZE_RESULT_ONE_BYTE, OnSerializeResultOneByte)
	ON_COMMAND(ID_SERIALIZE_RESULT_TWO_BYTE, OnSerializeResultTwoBytes)
	ON_COMMAND(ID_SERIALIZE_RESULT_FOUR_BYTE, OnSerializeResultFourBytes)
	ON_COMMAND(ID_SERIALIZE_RESULT_EIGHT_BYTE, OnSerializeResultEightBytes)
	ON_COMMAND(ID_EDIT_OPTIONS, OnEditOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlWinApp construction

CGlWinApp::CGlWinApp()
{
	// Options
	m_OptionColorFont = RGB(255, 255, 255);
	m_OptionColorFore = RGB(200, 160, 80);
	m_OptionColorBack = RGB(96, 96, 96);
	m_OptionColorLightAmbient = RGB(128, 128, 128);
	m_OptionColorLightDiffuse = RGB(128, 128, 128);
	m_OptionColorLightSpecular = RGB(0, 0, 0);

	m_LogFont.lfHeight = -24;
	m_LogFont.lfWidth = 0;
	m_LogFont.lfEscapement = 0;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfWeight = 0;
	m_LogFont.lfItalic = 0;
	m_LogFont.lfUnderline = 0;
	m_LogFont.lfStrikeOut = 0;
	m_LogFont.lfCharSet = 0;
	m_LogFont.lfOutPrecision = 0;
	m_LogFont.lfClipPrecision = 0;
	m_LogFont.lfQuality = 0;
	m_LogFont.lfPitchAndFamily = 0;
	sprintf(m_LogFont.lfFaceName, "Times New Roman");

	m_bAntialias = FALSE;
	m_bSmooth = TRUE;
	m_bLighting = TRUE;
	m_bMonochromeMode = FALSE;
	m_nPolygonMode = 2;
	m_nMaxPrintMemSize = 8*1024;
	m_nPrintObject = 0;
	m_bPrintHighestQuality = FALSE;
	m_bPrintCenter = FALSE;
	m_bPrintBkgndWhite = TRUE;
	m_nMousePolicy = 1;						// 0 = DblClk/Clk
											// 1 = Clk/Clk
											// 2 = Clk/hold
	m_bNoHardwareAcceleration = FALSE;		// TRUE  = support pixel formats without hardware acceleration only
	m_bPropPolicy = TRUE;					// FALSE = properties only for current view
											// TRUE  = properties for all views
	m_bBlackLinesOrRemoveHidden = FALSE;	// TRUE  = draw black lines in case of GL_FILL, 
											//         remove hidden lines/points in case of GL_POINT/GL_LINE
	m_bScaleShowFrame = TRUE;				// TRUE  = show coordinate system (frame) if any 
	m_bScaleShowLabels = TRUE;				// TRUE  = show coordinate system (lables) if any 
	m_nScaleShowGrid = 2;					// 1/2   = show coordinate system (grid) if any (1 = coarse, 2 = fine)
	m_bScaleShowAxis = FALSE;				// TRUE  = show coordinate system (axis) if any 
	m_bScaleShowAxisLabels = TRUE;			// TRUE  = show coordinate system (axis labels) if any 
	m_bReopenPolicy = FALSE;				// TRUE  = reopen first document of recent file list at startup
	m_nCopyResolution = 300;				// 300 DPI
	m_nSerializeResultBytes = 2;

	m_nOsRunning = GetOSRunning();
}

/////////////////////////////////////////////////////////////////////////////
// CGlWinApp commands

BOOL CGlWinApp::InitInstance() 
{
	LoadOptions();
	return CWinApp::InitInstance();
}

int CGlWinApp::ExitInstance() 
{
	SaveOptions();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// LoadOptions
// Open from registry
/////////////////////////////////////////////////////////////////////////////
BOOL CGlWinApp::LoadOptions()
{
	unsigned int red,green,blue;
	CString facename;

	// Gl text color
	red = GetProfileInt("OpenGL text color","Red",GetRValue(m_OptionColorFont));
	green = GetProfileInt("OpenGL text color","Green",GetGValue(m_OptionColorFont));
	blue = GetProfileInt("OpenGL text color","Blue",GetBValue(m_OptionColorFont));
	m_OptionColorFont = RGB(red,green,blue);

	// Gl fore color
	red = GetProfileInt("OpenGL fore color","Red",GetRValue(m_OptionColorFore));
	green = GetProfileInt("OpenGL fore color","Green",GetGValue(m_OptionColorFore));
	blue = GetProfileInt("OpenGL fore color","Blue",GetBValue(m_OptionColorFore));
	m_OptionColorFore = RGB(red,green,blue);

	// Gl back color
	red = GetProfileInt("OpenGL back color","Red",GetRValue(m_OptionColorBack));
	green = GetProfileInt("OpenGL back color","Green",GetGValue(m_OptionColorBack));
	blue = GetProfileInt("OpenGL back color","Blue",GetBValue(m_OptionColorBack));
	m_OptionColorBack = RGB(red,green,blue);

	// Gl light ambient color
	red = GetProfileInt("OpenGL light ambient color","Red",GetRValue(m_OptionColorLightAmbient));
	green = GetProfileInt("OpenGL light ambient color","Green",GetGValue(m_OptionColorLightAmbient));
	blue = GetProfileInt("OpenGL light ambient color","Blue",GetBValue(m_OptionColorLightAmbient));
	m_OptionColorLightAmbient = RGB(red,green,blue);

	// Gl light diffuse color
	red = GetProfileInt("OpenGL light diffuse color","Red",GetRValue(m_OptionColorLightDiffuse));
	green = GetProfileInt("OpenGL light diffuse color","Green",GetGValue(m_OptionColorLightDiffuse));
	blue = GetProfileInt("OpenGL light diffuse color","Blue",GetBValue(m_OptionColorLightDiffuse));
	m_OptionColorLightDiffuse = RGB(red,green,blue);

	// Gl light specular color
	red = GetProfileInt("OpenGL light specular color","Red",GetRValue(m_OptionColorLightSpecular));
	green = GetProfileInt("OpenGL light specular color","Green",GetGValue(m_OptionColorLightSpecular));
	blue = GetProfileInt("OpenGL light specular color","Blue",GetBValue(m_OptionColorLightSpecular));
	m_OptionColorLightSpecular = RGB(red,green,blue);

	// LOGFONT
	m_LogFont.lfHeight = GetProfileInt("OpenGL text font","Height", m_LogFont.lfHeight);
	m_LogFont.lfWidth = GetProfileInt("OpenGL text font","Width", m_LogFont.lfWidth);
	m_LogFont.lfEscapement = GetProfileInt("OpenGL text font","Escapement", m_LogFont.lfEscapement);
	m_LogFont.lfOrientation = GetProfileInt("OpenGL text font","Orientation", m_LogFont.lfOrientation);
	m_LogFont.lfWeight = GetProfileInt("OpenGL text font","Weight", m_LogFont.lfWeight);
	m_LogFont.lfItalic = (BYTE)GetProfileInt("OpenGL text font","Italic", m_LogFont.lfItalic);
	m_LogFont.lfUnderline = (BYTE)GetProfileInt("OpenGL text font","Underline", m_LogFont.lfUnderline);
	m_LogFont.lfStrikeOut = (BYTE)GetProfileInt("OpenGL text font","StrikeOut", m_LogFont.lfStrikeOut);
	m_LogFont.lfCharSet = (BYTE)GetProfileInt("OpenGL text font","CharSet", m_LogFont.lfCharSet);
	m_LogFont.lfOutPrecision = (BYTE)GetProfileInt("OpenGL text font","OutPrecision", m_LogFont.lfOutPrecision);
	m_LogFont.lfClipPrecision = (BYTE)GetProfileInt("OpenGL text font","ClipPrecision", m_LogFont.lfClipPrecision);
	m_LogFont.lfQuality = (BYTE)GetProfileInt("OpenGL text font","Quality", m_LogFont.lfQuality);
	m_LogFont.lfPitchAndFamily = (BYTE)GetProfileInt("OpenGL text font","PitchAndFamily", m_LogFont.lfPitchAndFamily);
	facename = GetProfileString("OpenGL text font","FaceName", m_LogFont.lfFaceName);
	sprintf(m_LogFont.lfFaceName, facename);

	// Gl appearance
	m_bAntialias = GetProfileInt("OpenGL appearance","Antialias",m_bAntialias);
	m_bSmooth = GetProfileInt("OpenGL appearance","Smooth",m_bSmooth);
	m_bLighting = GetProfileInt("OpenGL appearance","Lighting",m_bLighting);
	m_bBlackLinesOrRemoveHidden = GetProfileInt("OpenGL appearance","BlackLines/RemoveHidden",m_bBlackLinesOrRemoveHidden);
	m_bMonochromeMode = GetProfileInt("OpenGL appearance","MonochromeMode",m_bMonochromeMode);
	m_nPolygonMode = GetProfileInt("OpenGL appearance","PolygonMode",m_nPolygonMode);

	// Gl coordinates
	m_bScaleShowFrame = GetProfileInt("OpenGL coordinates","Frame",m_bScaleShowFrame);
	m_bScaleShowLabels = GetProfileInt("OpenGL coordinates","Labels",m_bScaleShowLabels);
	m_nScaleShowGrid = GetProfileInt("OpenGL coordinates","Grid",m_nScaleShowGrid);
	m_bScaleShowAxis = GetProfileInt("OpenGL coordinates","Axis",m_bScaleShowAxis);
	m_bScaleShowAxisLabels = GetProfileInt("OpenGL coordinates","AxisLabels",m_bScaleShowAxisLabels);

	// Printing
	m_nMaxPrintMemSize = GetProfileInt("Printing","MaxPrintMemSize",m_nMaxPrintMemSize);
	m_nPrintObject = GetProfileInt("Printing","Object",m_nPrintObject);
	m_bPrintHighestQuality = GetProfileInt("Printing","HighestQuality",m_bPrintHighestQuality);
	m_bPrintCenter = GetProfileInt("Printing","Center",m_bPrintCenter);
	m_bPrintBkgndWhite = GetProfileInt("Printing","BkgndWhite",m_bPrintBkgndWhite);

	// Mouse, prop, save graph and reopen last document policy
	m_nMousePolicy = GetProfileInt("Properties","MousePolicy",m_nMousePolicy);
	m_bNoHardwareAcceleration = GetProfileInt("Properties","NoHardwareAcceleration",m_bNoHardwareAcceleration);
	m_bPropPolicy = GetProfileInt("Properties","PropPolicy",m_bPropPolicy);
	m_bReopenPolicy = GetProfileInt("Properties","ReopenLastDoc",m_bReopenPolicy);
	m_nCopyResolution = GetProfileInt("Properties","CopyResolution",m_nCopyResolution);
	m_nSerializeResultBytes = GetProfileInt("Properties","SaveGraphPolicy",m_nSerializeResultBytes);

	// MainFrame state
	m_nCmdShow = GetProfileInt("Window","ShowCmd",SW_SHOWNORMAL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// SaveOptions
// Save to registry
/////////////////////////////////////////////////////////////////////////////
BOOL CGlWinApp::SaveOptions()
{
	unsigned char red,green,blue;

	// Gl text color
	red = GetRValue(m_OptionColorFont);
	green = GetGValue(m_OptionColorFont);
	blue = GetBValue(m_OptionColorFont);
	WriteProfileInt("OpenGL text color","Red",red);
	WriteProfileInt("OpenGL text color","Green",green);
	WriteProfileInt("OpenGL text color","Blue",blue);

	// Gl fore color
	red = GetRValue(m_OptionColorFore);
	green = GetGValue(m_OptionColorFore);
	blue = GetBValue(m_OptionColorFore);
	WriteProfileInt("OpenGL fore color","Red",red);
	WriteProfileInt("OpenGL fore color","Green",green);
	WriteProfileInt("OpenGL fore color","Blue",blue);

	// Gl back color
	red = GetRValue(m_OptionColorBack);
	green = GetGValue(m_OptionColorBack);
	blue = GetBValue(m_OptionColorBack);
	WriteProfileInt("OpenGL back color","Red",red);
	WriteProfileInt("OpenGL back color","Green",green);
	WriteProfileInt("OpenGL back color","Blue",blue);

	// Gl light ambient color
	red = GetRValue(m_OptionColorLightAmbient);
	green = GetGValue(m_OptionColorLightAmbient);
	blue = GetBValue(m_OptionColorLightAmbient);
	WriteProfileInt("OpenGL light ambient color","Red",red);
	WriteProfileInt("OpenGL light ambient color","Green",green);
	WriteProfileInt("OpenGL light ambient color","Blue",blue);

	// Gl light diffuse color
	red = GetRValue(m_OptionColorLightDiffuse);
	green = GetGValue(m_OptionColorLightDiffuse);
	blue = GetBValue(m_OptionColorLightDiffuse);
	WriteProfileInt("OpenGL light diffuse color","Red",red);
	WriteProfileInt("OpenGL light diffuse color","Green",green);
	WriteProfileInt("OpenGL light diffuse color","Blue",blue);

	// Gl light specular color
	red = GetRValue(m_OptionColorLightSpecular);
	green = GetGValue(m_OptionColorLightSpecular);
	blue = GetBValue(m_OptionColorLightSpecular);
	WriteProfileInt("OpenGL light specular color","Red",red);
	WriteProfileInt("OpenGL light specular color","Green",green);
	WriteProfileInt("OpenGL light specular color","Blue",blue);

	// LOGFONT
	WriteProfileInt("OpenGL text font","Height", m_LogFont.lfHeight);
	WriteProfileInt("OpenGL text font","Width", m_LogFont.lfWidth);
	WriteProfileInt("OpenGL text font","Escapement", m_LogFont.lfEscapement);
	WriteProfileInt("OpenGL text font","Orientation", m_LogFont.lfOrientation);
	WriteProfileInt("OpenGL text font","Weight", m_LogFont.lfWeight);
	WriteProfileInt("OpenGL text font","Italic", m_LogFont.lfItalic);
	WriteProfileInt("OpenGL text font","Underline", m_LogFont.lfUnderline);
	WriteProfileInt("OpenGL text font","StrikeOut", m_LogFont.lfStrikeOut);
	WriteProfileInt("OpenGL text font","CharSet", m_LogFont.lfCharSet);
	WriteProfileInt("OpenGL text font","OutPrecision", m_LogFont.lfOutPrecision);
	WriteProfileInt("OpenGL text font","ClipPrecision", m_LogFont.lfClipPrecision);
	WriteProfileInt("OpenGL text font","Quality", m_LogFont.lfQuality);
	WriteProfileInt("OpenGL text font","PitchAndFamily", m_LogFont.lfPitchAndFamily);
	WriteProfileString("OpenGL text font","FaceName", m_LogFont.lfFaceName);

	// Gl appearance
	WriteProfileInt("OpenGL appearance","Antialias",m_bAntialias);
	WriteProfileInt("OpenGL appearance","Smooth",m_bSmooth);
	WriteProfileInt("OpenGL appearance","Lighting",m_bLighting);
	WriteProfileInt("OpenGL appearance","BlackLines/RemoveHidden",m_bBlackLinesOrRemoveHidden);
	WriteProfileInt("OpenGL appearance","MonochromeMode",m_bMonochromeMode);
	WriteProfileInt("OpenGL appearance","PolygonMode",m_nPolygonMode);

	// Gl coordinates
	WriteProfileInt("OpenGL coordinates","Frame",m_bScaleShowFrame);
	WriteProfileInt("OpenGL coordinates","Labels",m_bScaleShowLabels);
	WriteProfileInt("OpenGL coordinates","Grid",m_nScaleShowGrid);
	WriteProfileInt("OpenGL coordinates","Axis",m_bScaleShowAxis);
	WriteProfileInt("OpenGL coordinates","AxisLabels",m_bScaleShowAxisLabels);

	// Printing
	WriteProfileInt("Printing","MaxPrintMemSize",m_nMaxPrintMemSize);
	WriteProfileInt("Printing","Object",m_nPrintObject);
	WriteProfileInt("Printing","HighestQuality",m_bPrintHighestQuality);
	WriteProfileInt("Printing","Center",m_bPrintCenter);
	WriteProfileInt("Printing","BkgndWhite",m_bPrintBkgndWhite);

	// Mouse, prop, save graph and reopen last document policy
	WriteProfileInt("Properties","MousePolicy",m_nMousePolicy);
	WriteProfileInt("Properties","NoHardwareAcceleration",m_bNoHardwareAcceleration);
	WriteProfileInt("Properties","PropPolicy",m_bPropPolicy);
	WriteProfileInt("Properties","ReopenLastDoc",m_bReopenPolicy);
	WriteProfileInt("Properties","CopyResolution",m_nCopyResolution);
	WriteProfileInt("Properties","SaveGraphPolicy",m_nSerializeResultBytes);

	return TRUE;
}

// this is a copy from DOCMGR.CPP 
//in order to override CGlWinApp::RegisterShellFileTypes

static const TCHAR szShellOpenFmt[] = _T("%s\\shell\\open\\%s");
static const TCHAR szShellPrintFmt[] = _T("%s\\shell\\print\\%s");
static const TCHAR szShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
static const TCHAR szDefaultIconFmt[] = _T("%s\\DefaultIcon");
static const TCHAR szShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

static const TCHAR szIconIndexFmt[] = _T(",%d");
static const TCHAR szCommand[] = _T("command");
static const TCHAR szOpenArg[] = _T(" \"%1\"");
static const TCHAR szPrintArg[] = _T(" /p \"%1\"");
static const TCHAR szPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
static const TCHAR szDDEArg[] = _T(" /dde");

static const TCHAR szDDEExec[] = _T("ddeexec");
static const TCHAR szDDEOpen[] = _T("[open(\"%1\")]");
static const TCHAR szDDEPrint[] = _T("[print(\"%1\")]");
static const TCHAR szDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

static const TCHAR szShellNewValueName[] = _T("NullFile");
static const TCHAR szShellNewValue[] = _T("");

static BOOL AFXAPI AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
				lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if (::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if (::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

void CGlWinApp::RegisterShellFileTypes(BOOL bCompat, BOOL bOmitPrintCmd)
{
	ASSERT(m_pDocManager != NULL);
	if (!bCompat || !bOmitPrintCmd)
	{
		m_pDocManager->RegisterShellFileTypes(bCompat);
	}
	else
	{
		// similar to m_pDocManager->RegisterShellFileTypes(TRUE)
		// only the "Print" and "PrintTo" keys are omited
		ASSERT(GetFirstDocTemplatePosition() != NULL);  // must have some doc templates

		CString strPathName, strTemp;

		AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

		POSITION pos = GetFirstDocTemplatePosition();
		for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
		{
			CDocTemplate* pTemplate = GetNextDocTemplate(pos);

			CString strOpenCommandLine = strPathName;
			CString strDefaultIconCommandLine = strPathName;

			if (bCompat)
			{
				CString strIconIndex;
				HICON hIcon = ExtractIcon(AfxGetInstanceHandle(), strPathName, nTemplateIndex);
				if (hIcon != NULL)
				{
					strIconIndex.Format(szIconIndexFmt, nTemplateIndex);
					DestroyIcon(hIcon);
				}
				else
				{
					strIconIndex.Format(szIconIndexFmt, DEFAULT_ICON_INDEX);
				}
				strDefaultIconCommandLine += strIconIndex;
			}

			CString strFilterExt, strFileTypeId, strFileTypeName;
			if (pTemplate->GetDocString(strFileTypeId,
				CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
			{
				// enough info to register it
				if (!pTemplate->GetDocString(strFileTypeName,
					CDocTemplate::regFileTypeName))
					strFileTypeName = strFileTypeId;    // use id name

				ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

				// first register the type ID of our server
				if (!AfxSetRegKey(strFileTypeId, strFileTypeName))
					continue;       // just skip it

				if (bCompat)
				{
					// path\DefaultIcon = path,1
					strTemp.Format(szDefaultIconFmt, (LPCTSTR)strFileTypeId);
					if (!AfxSetRegKey(strTemp, strDefaultIconCommandLine))
						continue;       // just skip it
				}

				// If MDI Application
				if (!pTemplate->GetDocString(strTemp, CDocTemplate::windowTitle) ||
					strTemp.IsEmpty())
				{
					// path\shell\open\ddeexec = [open("%1")]
					strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
						(LPCTSTR)szDDEExec);
					if (!AfxSetRegKey(strTemp, szDDEOpen))
						continue;       // just skip it

					if (bCompat)
					{
						// path\shell\open\command = path /dde
						strOpenCommandLine += szDDEArg;
					}
					else
					{
						strOpenCommandLine += szOpenArg;
					}
				}
				else
				{
					// path\shell\open\command = path filename
					strOpenCommandLine += szOpenArg;
				}

				// path\shell\open\command = path filename
				strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)szCommand);
				if (!AfxSetRegKey(strTemp, strOpenCommandLine))
					continue;       // just skip it

				pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
				if (!strFilterExt.IsEmpty())
				{
					ASSERT(strFilterExt[0] == '.');

					LONG lSize = _MAX_PATH * 2;
					LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
						strTemp.GetBuffer(lSize), &lSize);
					strTemp.ReleaseBuffer();

					if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
						strTemp == strFileTypeId)
					{
						// no association for that suffix
						if (!AfxSetRegKey(strFilterExt, strFileTypeId))
							continue;

						if (bCompat)
						{
							strTemp.Format(szShellNewFmt, (LPCTSTR)strFilterExt);
							(void)AfxSetRegKey(strTemp, szShellNewValue, szShellNewValueName);
						}
					}
				}
			}
		}
	}
}

// App command to run the dialog
void CGlWinApp::OnAppAbout()
{
	CDlgAbout dlgAbout;
	dlgAbout.DoModal();
}

void CGlWinApp::OnHelpContents()
{
	WinHelp(0L, HELP_CONTENTS);
}

void CGlWinApp::OnHelpShortcuts()
{
	CDlgListCtrl dlg;
	CString strAccel, strShortcut, strStatement, strDescription, strPrefix;
	int nAccel, nPos;

	HACCEL hAccel = NULL;
	CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();

	if (pMainFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		CFrameWnd* pActiveFrame = pMainFrame->GetActiveFrame();
		hAccel = pActiveFrame->GetDefaultAccelerator();
		if (hAccel == NULL && pActiveFrame != pMainFrame)
			hAccel = pMainFrame->GetDefaultAccelerator();
	}

	if ((nAccel = ::CopyAcceleratorTable(hAccel, NULL, 0)) > 0)
	{
		LPACCEL lpAccel = new ACCEL[nAccel];
		if (lpAccel)
		{
			::CopyAcceleratorTable(hAccel, lpAccel, nAccel);
			for (int k = 0; k < nAccel; k++)
			{
				strAccel.LoadString(lpAccel[k].cmd);
				if (strAccel.IsEmpty())
					continue;			// no description for this feature 

				nPos = strAccel.Find(_T('\n'));
				strStatement = strAccel.Right(strAccel.GetLength()-nPos-1);
				strDescription = strAccel.Left(nPos);
				if ((lpAccel[k].key >= 0x30 && lpAccel[k].key <= 0x39) || 
					(lpAccel[k].key >= 0x41 && lpAccel[k].key <= 0x5A))
				{
					strShortcut = (char)lpAccel[k].key;
				}
				else
				{
#ifdef _DEBUG
					LoadString(_T("General32d.dll"), lpAccel[k].key, strShortcut);
#else
					LoadString(_T("General32.dll"), lpAccel[k].key, strShortcut);
#endif //_DEBUG
					if (strShortcut.IsEmpty())
						continue;		// no virtkey symbol string found
				}
				if (lpAccel[k].fVirt & FVIRTKEY && lpAccel[k].fVirt & FSHIFT)
				{
					strPrefix.LoadString(IDS_STR_SHIFT);
					strShortcut = strPrefix + _T("+") + strShortcut;
				}
				if (lpAccel[k].fVirt & FALT)
				{
					strPrefix.LoadString(IDS_STR_ALT);
					strShortcut = strPrefix + _T("+") + strShortcut;
				}
				if (lpAccel[k].fVirt & FVIRTKEY && lpAccel[k].fVirt & FCONTROL)
				{
					strPrefix.LoadString(IDS_STR_CTRL);
					strShortcut = strPrefix + _T("+") + strShortcut;
				}
				dlg.AddDataString(strShortcut);
				dlg.AddDataString(strStatement);
				dlg.AddDataString(strDescription);
			}
			delete [] lpAccel;
		}
	}

	CString strDetails;
	strDetails.LoadString(IDS_DETAIL_SHORTCUTS_TITLE);
	dlg.SetWindowTitleString(strDetails);
	if (dlg.GetDataCount() > 0)
	{
		dlg.SetListCtrlColumnCount(3);
		strDetails.LoadString(IDS_STR_SHORTCUT);
		dlg.SetListCtrlColumnLabel(0, strDetails);
		strDetails.LoadString(IDS_STR_STAT);
		dlg.SetListCtrlColumnLabel(1, strDetails);
		strDetails.LoadString(IDS_STR_DESC);
		dlg.SetListCtrlColumnLabel(2, strDetails);
	}

	dlg.m_ListCtrl.SetReadOnly();
	dlg.DoModal();
}

void CGlWinApp::OnFilePrintSetupEx() 
{
	// TODO: Add your command handler code here
	CDlgFilePrintOptions dlg;
	int tmpInt;
	BOOL tmpBOOL;

	dlg.m_nMaxPrintMemSize = m_nMaxPrintMemSize;
	dlg.m_nPrintObject = m_nPrintObject;
	dlg.m_bPrintHighestQuality = m_bPrintHighestQuality;
	dlg.m_bPrintCenter = m_bPrintCenter;
	dlg.m_bPrintBkgndWhite = m_bPrintBkgndWhite;
	if (dlg.DoModal() == IDOK)
	{
		tmpInt = dlg.m_nMaxPrintMemSize;
		if (m_nMaxPrintMemSize != tmpInt)
		{
			m_nMaxPrintMemSize = tmpInt;
		}
		tmpInt = dlg.m_nPrintObject;
		if (m_nPrintObject != tmpInt)
		{
			m_nPrintObject = tmpInt;
		}
		tmpBOOL = dlg.m_bPrintHighestQuality;
		if (m_bPrintHighestQuality != tmpBOOL)
		{
			m_bPrintHighestQuality = tmpBOOL;
		}
		tmpBOOL = dlg.m_bPrintCenter;
		if (m_bPrintCenter != tmpBOOL)
		{
			m_bPrintCenter = tmpBOOL;
		}
		tmpBOOL = dlg.m_bPrintBkgndWhite;
		if (m_bPrintBkgndWhite != tmpBOOL)
		{
			m_bPrintBkgndWhite = tmpBOOL;
		}
	}
}

void CGlWinApp::OnChangeMousePolicy()
{
	m_nMousePolicy = ++m_nMousePolicy - 3*(m_nMousePolicy/3);
	SetStatusBarInfo(POLICY_MOUSE, m_nMousePolicy);
}

void CGlWinApp::OnChangeHardwareAcceleration() 
{
	m_bNoHardwareAcceleration = !m_bNoHardwareAcceleration;
	SetStatusBarInfo(POLICY_HARDWARE_ACCELERATION, m_bNoHardwareAcceleration);

	CString strSection;
	strSection.LoadString(IDS_SETTINGCHANGE_LPARAM_FORCE);
	AfxGetMainWnd()->SendMessage(WM_SETTINGCHANGE, 0, (LPARAM)(LPCTSTR)strSection);
}

void CGlWinApp::OnChangePropPolicy() 
{
	m_bPropPolicy = !m_bPropPolicy;
	SetStatusBarInfo(POLICY_PROP, m_bPropPolicy);
}

void CGlWinApp::OnChangeReopenPolicy() 
{
	m_bReopenPolicy = !m_bReopenPolicy;
	SetStatusBarInfo(POLICY_REOPEN, m_bReopenPolicy);
}

void CGlWinApp::OnSerializeResultOneByte() 
{
	m_nSerializeResultBytes = 1;
	SetStatusBarInfo(POLICY_SAVE_RESULT, m_nSerializeResultBytes);
}

void CGlWinApp::OnSerializeResultTwoBytes() 
{
	m_nSerializeResultBytes = 2;
	SetStatusBarInfo(POLICY_SAVE_RESULT, m_nSerializeResultBytes);
}

void CGlWinApp::OnSerializeResultFourBytes() 
{
	m_nSerializeResultBytes = 4;
	SetStatusBarInfo(POLICY_SAVE_RESULT, m_nSerializeResultBytes);
}

void CGlWinApp::OnSerializeResultEightBytes() 
{
	m_nSerializeResultBytes = 8;
	SetStatusBarInfo(POLICY_SAVE_RESULT, m_nSerializeResultBytes);
}

void CGlWinApp::OnEditOptions() 
{
	CDlgOptions dlg;

	dlg.m_nRadioMousePolicy = m_nMousePolicy;
	dlg.m_bCheckNoHardwareAcceleration = m_bNoHardwareAcceleration;
	dlg.m_bCheckPropPolicy = m_bPropPolicy;
	dlg.m_nCopyResolution = m_nCopyResolution;
	dlg.m_bCheckReopenPolicy = m_bReopenPolicy;
	if (m_nSerializeResultBytes == 1)
		dlg.m_nRadioSerializeResultBytes = 0;
	else if (m_nSerializeResultBytes == 2)
		dlg.m_nRadioSerializeResultBytes = 1;
	else if (m_nSerializeResultBytes == 4)
		dlg.m_nRadioSerializeResultBytes = 2;
	else if (m_nSerializeResultBytes == 8)
		dlg.m_nRadioSerializeResultBytes = 3;
	else
		dlg.m_nRadioSerializeResultBytes = -1;

	if (dlg.DoModal() == IDOK)
	{
		int tmpInt;
		BOOL tmpBOOL;

		tmpInt = dlg.m_nRadioMousePolicy;
		if (m_nMousePolicy != tmpInt)
		{
			m_nMousePolicy = tmpInt;
		}
		tmpBOOL = dlg.m_bCheckNoHardwareAcceleration;
		if (m_bNoHardwareAcceleration != tmpBOOL)
		{
			m_bNoHardwareAcceleration = tmpBOOL;

			CString strSection;
			strSection.LoadString(IDS_SETTINGCHANGE_LPARAM_FORCE);
			AfxGetMainWnd()->SendMessage(WM_SETTINGCHANGE, 0, (LPARAM)(LPCTSTR)strSection);
		}
		tmpBOOL = dlg.m_bCheckPropPolicy;
		if (m_bPropPolicy != tmpBOOL)
		{
			m_bPropPolicy = tmpBOOL;
		}
		tmpInt = dlg.m_nCopyResolution;
		if (m_nCopyResolution != tmpInt)
		{
			m_nCopyResolution = tmpInt;
		}
		tmpBOOL = dlg.m_bCheckReopenPolicy;
		if (m_bReopenPolicy != tmpBOOL)
		{
			m_bReopenPolicy = tmpBOOL;
		}
		tmpInt = dlg.m_nRadioSerializeResultBytes;
		if (tmpInt == -1)
		{
			m_nSerializeResultBytes = 0;
		}
		else if (tmpInt == 0)
		{
			m_nSerializeResultBytes = 1;
		}
		else if (tmpInt == 1)
		{
			m_nSerializeResultBytes = 2;
		}
		else if (tmpInt == 2)
		{
			m_nSerializeResultBytes = 4;
		}
		else if (tmpInt == 3)
		{
			m_nSerializeResultBytes = 8;
		}
	}
}

void CGlWinApp::SetStatusBarInfo(int nInfoMajor, int nInfoMinor) 
{
	CFrameWnd* pMainFrame;
	if (m_pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		pMainFrame = (CFrameWnd *)m_pMainWnd;
	else
		return;

	if (nInfoMajor == POLICY_NO_MORE_ZOOM)
	{
		pMainFrame->SetMessageText(IDS_PROP_NO_MORE_ZOOM);
	}
	else if (nInfoMajor == POLICY_MOUSE)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_PROP_MOUSE_DBLCLK_CLK);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_PROP_MOUSE_CLK_CLK);
		}
		else if (nInfoMinor == 2)
		{
			pMainFrame->SetMessageText(IDS_PROP_MOUSE_CLK_DROP);
		}
	}
	else if (nInfoMajor == POLICY_HARDWARE_ACCELERATION)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_PROP_ACCEL_PERMIT);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_PROP_ACCEL_NO);
		}
	}
	else if (nInfoMajor == POLICY_PROP)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_PROP_WND_CUR);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_PROP_WND_ALL);
		}
	}
	else if (nInfoMajor == POLICY_REOPEN)
	{
		if (nInfoMinor == 0)
		{
			pMainFrame->SetMessageText(IDS_PROP_RECENT_NO);
		}
		else if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_PROP_RECENT_YES);
		}
	}
	else if (nInfoMajor == POLICY_SAVE_RESULT)
	{
		if (nInfoMinor == 1)
		{
			pMainFrame->SetMessageText(IDS_PROP_SAVE_ONE);
		}
		else if (nInfoMinor == 2)
		{
			pMainFrame->SetMessageText(IDS_PROP_SAVE_TWO);
		}
		else if (nInfoMinor == 4)
		{
			pMainFrame->SetMessageText(IDS_PROP_SAVE_FOUR);
		}
		else if (nInfoMinor == 8)
		{
			pMainFrame->SetMessageText(IDS_PROP_SAVE_EIGHT);
		}
	}
	else if (nInfoMajor == POLICY_RESOLUTION_REDUCED)
	{
		CString strMsg, strFormat;
		strFormat.LoadString(IDS_CHANGED_RESOLUTION);
		strMsg.Format(strFormat, nInfoMinor);
		pMainFrame->SetMessageText(strMsg);
	}
	pMainFrame->SetTimer(AFX_IDS_IDLEMESSAGE, 2000, NULL);
}

CRecentFileList* CGlWinApp::GetRecentFileList()
{
	return m_pRecentFileList;
}

void CGlWinApp::OnFilePrintEx() 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
				}
			}
		}
		break;
	case 1:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView || !pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					pFirstGlView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
			}
		}
		break;
	case 2:
		DoFilePrintAll();
		break;
	default:
		{}
	}
}

void CGlWinApp::OnFilePrintPreviewEx() 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pView->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
				}
			}
		}
		break;
	case 1:
	case 2:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView || !pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && (m_nPrintObject == 2 || pView->IsWindowVisible()))
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					pFirstGlView->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
			}
		}
		break;
	default:
		{}
	}
}

void CGlWinApp::OnUpdateFilePrintEx(CCmdUI* pCmdUI) 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;
	BOOL bEnable = FALSE;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					bEnable = TRUE;
				}
			}
		}
		break;
	case 1:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView || !pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && pView->IsWindowVisible())
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					bEnable = TRUE;
			}
		}
		break;
	case 2:
		bEnable = TRUE;
		break;
	default:
		{}
	}
	pCmdUI->Enable(bEnable);
}

void CGlWinApp::OnUpdateFilePrintPreviewEx(CCmdUI* pCmdUI) 
{
	CFrameWnd *pFrame, *pMainFrame;
	CView *pView;
	CDocument *pDoc;
	BOOL bEnable = FALSE;

	switch (m_nPrintObject)
	{
	case 0:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				pView = pFrame->GetActiveView();
				if (pView && pView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					bEnable = TRUE;
				}
			}
		}
		break;
	case 1:
	case 2:
		pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		if (pMainFrame)
		{
			pFrame = pMainFrame->GetActiveFrame();
			if (pFrame)
			{
				CView* pFirstGlView = pFrame->GetActiveView();
				if (!pFirstGlView || !pFirstGlView->IsKindOf(RUNTIME_CLASS(CGlView)))
				{
					pFirstGlView = NULL;
					pDoc = pFrame->GetActiveDocument();
					if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
					{
						// get some GlView
						POSITION pos = pDoc->GetFirstViewPosition();
						while (pos != NULL)
						{
							pView = pDoc->GetNextView(pos);
							if (pView->IsKindOf(RUNTIME_CLASS(CGlView)) && (m_nPrintObject == 2 || pView->IsWindowVisible()))
							{
								pFirstGlView = pView;
								break;
							}
						}
					}
				}

				// start printing
				if (pFirstGlView)
					bEnable = TRUE;
			}
		}
		break;
	default:
		{}
	}
	pCmdUI->Enable(bEnable);
}

BOOL CGlWinApp::BeginPrinting(CDC* pDC) 
{
	// get the screen dc
	HDC hScreenDC = NULL;
	hScreenDC = ::GetDC(NULL);
	ASSERT(hScreenDC != NULL);

	CDC ScreenDC;
	VERIFY(ScreenDC.Attach(hScreenDC));

	// create screen device dependent bitmap (DDB)
	int nScreenWidth = 0;
	nScreenWidth = ScreenDC.GetDeviceCaps(HORZRES);

	int nScreenHeight = 0;
	nScreenHeight = ScreenDC.GetDeviceCaps(VERTRES);

	CRect& mRect = m_Rect;
	AfxGetMainWnd()->GetWindowRect(&mRect);
	if (mRect.left < 0)
		mRect.left = 0;
	if (mRect.right > nScreenWidth)
		mRect.right = nScreenWidth;
	if (mRect.top < 0)
		mRect.top = 0;
	if (mRect.bottom > nScreenHeight)
		mRect.bottom = nScreenHeight;

	CBitmap ScreenBitmap;
	VERIFY(ScreenBitmap.CreateCompatibleBitmap(&ScreenDC, 
												/*nScreenWidth*/mRect.Width(), 
												/*nScreenHeight*/mRect.Height()));

	// copy image to screen DDB
	CDC MemDC;
	VERIFY(MemDC.CreateCompatibleDC(&ScreenDC));

	CBitmap* pOldBitmap = NULL;
	pOldBitmap = MemDC.SelectObject(&ScreenBitmap);

	// else could not select bitmap
	if (pOldBitmap == NULL)
	{
		// abort job
		VERIFY(pDC->AbortDoc() >= 0);
		return FALSE;
	}

	VERIFY(MemDC.BitBlt(0, 
						0, 
						/*nScreenWidth*/mRect.Width(), 
						/*nScreenHeight*/mRect.Height(),
						&ScreenDC, 
						/*0*/mRect.left, 
						/*0*/mRect.top, 
						SRCCOPY));
	ASSERT(pOldBitmap != NULL);
	MemDC.SelectObject(pOldBitmap);

	// create device independent bitmap (DIB)
	const int nBitCount = 24;
	long nImageSize = 0;
	nImageSize = ((((/*nScreenWidth*/mRect.Width() * nBitCount) + 31) & ~31) >> 3) * /*nScreenHeight*/mRect.Height();

	BYTE*& pBitmapBits = m_pBitmapBits;
	pBitmapBits = NULL;
	pBitmapBits = new BYTE[nImageSize];
	ASSERT(pBitmapBits != NULL);

	// copy image from screen DDB to DIB
	BITMAPINFO& bitmapInfo = m_bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = /*nScreenWidth*/mRect.Width();
	bitmapInfo.bmiHeader.biHeight = /*nScreenHeight*/mRect.Height();
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = nBitCount;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage = nImageSize;
	bitmapInfo.bmiHeader.biXPelsPerMeter = int(72./.0254);	// 72 DPI 
	bitmapInfo.bmiHeader.biYPelsPerMeter = int(72./.0254);	// 72 DPI 
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	memset(&bitmapInfo.bmiColors[0], 0, sizeof(RGBQUAD));

	VERIFY(GetDIBits(ScreenDC.GetSafeHdc(),
					(HBITMAP)ScreenBitmap,
					0,
					/*nScreenHeight*/mRect.Height(),
					(LPVOID)pBitmapBits,
					&bitmapInfo,
					DIB_RGB_COLORS));

	VERIFY(ScreenBitmap.DeleteObject());
	VERIFY(MemDC.DeleteDC());
	ReleaseDC(NULL, ScreenDC.Detach());

	return TRUE;
}

BOOL CGlWinApp::Print(CDC* pDC) 
{
	CRect& mRect = m_Rect;
	BITMAPINFO& bitmapInfo = m_bitmapInfo;
	BYTE*& pBitmapBits = m_pBitmapBits;

	// copy DIB to printer
	int nPrinterWidth = 0;
	nPrinterWidth = pDC->GetDeviceCaps(HORZRES);

	int nPrinterHeight = 0;
	nPrinterHeight = pDC->GetDeviceCaps(VERTRES);

	if (m_bPrintHighestQuality)
	{
		double fac = 1.;
		if (mRect.Width() <= nPrinterWidth && mRect.Height() <= nPrinterHeight)
		{
			// good condition
			while ((int)fac*mRect.Width() <= nPrinterWidth && (int)fac*mRect.Height() <= nPrinterHeight)
			{
				fac *= 2.;
			}
			fac /= 2.;
			nPrinterWidth = int(mRect.Width()*fac);
			nPrinterHeight = int(mRect.Height()*fac);
		}
		else
		{
			// bad condition
			while (mRect.Width() > (int)fac*nPrinterWidth || mRect.Height() > (int)fac*nPrinterHeight)
			{
				fac *= 2.;
			}
			nPrinterWidth = int(mRect.Width()/fac);
			nPrinterHeight = int(mRect.Height()/fac);
		}
	}
	else
	{
		if (nPrinterWidth * bitmapInfo.bmiHeader.biHeight / bitmapInfo.bmiHeader.biWidth < nPrinterHeight)
			nPrinterHeight = nPrinterWidth * bitmapInfo.bmiHeader.biHeight / bitmapInfo.bmiHeader.biWidth;
		else
			nPrinterWidth = nPrinterHeight * bitmapInfo.bmiHeader.biWidth / bitmapInfo.bmiHeader.biHeight;
	}
	int nImageOffsetHorz = m_bPrintCenter ? (pDC->GetDeviceCaps(HORZRES) - nPrinterWidth) / 2 : 0;
	int nImageOffsetVert = m_bPrintCenter ? (pDC->GetDeviceCaps(VERTRES) - nPrinterHeight) / 2 : 0;

	BOOL bStretchDIBits, bStretchBlt;
	int nRasterCaps	= pDC->GetDeviceCaps(RASTERCAPS);

	// Does the device support raster operations?
	if (nRasterCaps & RC_STRETCHDIB)
		bStretchDIBits = TRUE;
	else
		bStretchDIBits = FALSE;

	if (nRasterCaps & RC_STRETCHBLT)
		bStretchBlt = TRUE;
	else
		bStretchBlt = FALSE;

	// Does our device context support StretchDIBits?
	if (bStretchDIBits)
	{
		VERIFY(StretchDIBits(pDC->GetSafeHdc(),
							nImageOffsetHorz,
							nImageOffsetVert,
							nPrinterWidth,
							nPrinterHeight,
							0,
							0,
							bitmapInfo.bmiHeader.biWidth,
							bitmapInfo.bmiHeader.biHeight,
							(LPVOID)pBitmapBits,
							&bitmapInfo,
							DIB_RGB_COLORS,
							SRCCOPY) != GDI_ERROR);
	}
	else if (bStretchBlt)
	{
		HDC hdc;
		HBITMAP bitmap;

		hdc	= CreateCompatibleDC(pDC->GetSafeHdc());
		bitmap = CreateDIBitmap(hdc,
							&(bitmapInfo.bmiHeader),
							CBM_INIT,
							(LPVOID)pBitmapBits,
							&bitmapInfo,
							DIB_RGB_COLORS);

		HGDIOBJ oldbitmap = SelectObject(hdc, bitmap);
		VERIFY(StretchBlt(pDC->GetSafeHdc(),
							nImageOffsetHorz,
							nImageOffsetVert,
							nPrinterWidth,
							nPrinterHeight,
							hdc,
							0,
							0,
							bitmapInfo.bmiHeader.biWidth,
							bitmapInfo.bmiHeader.biHeight,
							SRCCOPY) != GDI_ERROR);
		if (oldbitmap != NULL)
			SelectObject(hdc, oldbitmap);

		DeleteObject(bitmap);
		DeleteDC(hdc);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGlWinApp::EndPrinting(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);

	BYTE*& pBitmapBits = m_pBitmapBits;

	// clean up
	if (pBitmapBits)
	{
		delete pBitmapBits;
		pBitmapBits = NULL;
		return TRUE;
	}
	return FALSE;
}

void CGlWinApp::DoFilePrintAll() 
{
	// show options
	CPrintDialog PrintDlg(FALSE, 
						PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_NOSELECTION |
						PD_DISABLEPRINTTOFILE, 
						AfxGetMainWnd()/*this*/);

	if (DoPrintDialog(&PrintDlg) == IDOK)
	{
		CString mMsg;
		BeginWaitCursor();
		AfxGetMainWnd()->SendMessageToDescendants(WM_PAINT);

		// get printer dc
		HDC hPrinterDC = NULL;
		hPrinterDC = PrintDlg.m_pd.hDC;
		ASSERT(hPrinterDC != NULL);

		CDC PrinterDC;
		BOOL bIsAttached = FALSE;
		bIsAttached = PrinterDC.Attach(hPrinterDC);
		ASSERT(bIsAttached);

		// attempt to set abort procedure
		int nSetAbortProcStatus = 0;
		nSetAbortProcStatus = PrinterDC.SetAbortProc(&CGlWinApp::AbortProc);

		// attempt to start print job
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof (DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszOutput = NULL;
		docInfo.lpszDocName = AfxGetAppName();

		int nStartDocStatus = 0;
		nStartDocStatus = PrinterDC.StartDoc(&docInfo);

		// if abort procedure set and print job started
		if (nSetAbortProcStatus >= 0 && nStartDocStatus >= 0)
		{
			// start page
			VERIFY(PrinterDC.StartPage() > 0);

			if (BeginPrinting(&PrinterDC))
			{
				if (!Print(&PrinterDC))
				{
					mMsg.Format(IDS_ERROR_PRINTER_NO_SUPPORT_MSG, "", "", "");
					AfxMessageBox(mMsg, MB_OK | MB_ICONERROR);
				}
				EndPrinting(&PrinterDC);
			}
			else
			{
				AfxMessageBox(IDS_ERROR_PRINTER_INSUF_MEM, MB_OK | MB_ICONERROR);
			}

			// finish the print job
			VERIFY(PrinterDC.EndPage() >= 0);
		}

		// else job could not be started
		else
		{
			// attempt to abort job
			PrinterDC.AbortDoc();
			AfxMessageBox(IDS_ERROR_PRINTER_NO_START, MB_OK | MB_ICONERROR);
		}
		VERIFY(PrinterDC.EndDoc() >= 0);

		// clean up
		PrinterDC.Detach();
		VERIFY(DeleteDC(hPrinterDC));
		EndWaitCursor();
	}
}

BOOL CALLBACK CGlWinApp::AbortProc(HDC hdc, int iCode)
{
	UNREFERENCED_PARAMETER(hdc);

	if (!RunExtraLoop())
		return FALSE;

	return (iCode == 0);
}

void CGlWinApp::OnFileAlarm(UINT wParam, LONG lParam)
{
	CDocument* pDoc = (CDocument*)wParam;
	CWatchInThreadExtra* pThreadExtra = (CWatchInThreadExtra*)wParam;
	if (pThreadExtra && pThreadExtra->IsKindOf(RUNTIME_CLASS(CWatchInThreadExtra)))
		pDoc = ((CWatchInThreadExtra*)pThreadExtra)->GetDocument();
	else
		pThreadExtra = NULL;
	ASSERT_VALID(pDoc);
	ASSERT(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CDocument)));

	if (pDoc->IsKindOf(RUNTIME_CLASS(CGlDocument)))
		((CGlDocument*)pDoc)->OnFileAlarm((int)lParam, pThreadExtra);
}

BOOL CGlWinApp::ExistShortcut(LPCTSTR szShortcut, BOOL bShift, BOOL bAlt, BOOL bCtrl, BOOL bIncludeHidden)
{
	CString strAccel, strShortcut;
	int nAccel, nRet = 0;

	HACCEL hAccel = NULL;
	CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();

	if (pMainFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		CFrameWnd* pActiveFrame = pMainFrame->GetActiveFrame();
		hAccel = pActiveFrame->GetDefaultAccelerator();
		if (hAccel == NULL && pActiveFrame != pMainFrame)
			hAccel = pMainFrame->GetDefaultAccelerator();
	}

	if ((nAccel = ::CopyAcceleratorTable(hAccel, NULL, 0)) > 0)
	{
		LPACCEL lpAccel = new ACCEL[nAccel];
		if (lpAccel)
		{
			::CopyAcceleratorTable(hAccel, lpAccel, nAccel);
			for (int k = 0; k < nAccel; k++)
			{
				nRet = 0;

				strAccel.LoadString(lpAccel[k].cmd);
				if (strAccel.IsEmpty() && !bIncludeHidden)
					continue;			// no description for this feature 

				if ((lpAccel[k].key >= 0x30 && lpAccel[k].key <= 0x39) || 
					(lpAccel[k].key >= 0x41 && lpAccel[k].key <= 0x5A))
				{
					strShortcut = (char)lpAccel[k].key;
				}
				else
				{
#ifdef _DEBUG
					LoadString(_T("General32d.dll"), lpAccel[k].key, strShortcut);
#else
					LoadString(_T("General32.dll"), lpAccel[k].key, strShortcut);
#endif //_DEBUG
					if (strShortcut.IsEmpty())
						continue;		// no virtkey symbol string found
				}

				nRet += (!!(lpAccel[k].fVirt & FVIRTKEY && lpAccel[k].fVirt & FSHIFT) == !!bShift);
				nRet += (!!(lpAccel[k].fVirt & FVIRTKEY && lpAccel[k].fVirt & FALT) == !!bAlt);
				nRet += (!!(lpAccel[k].fVirt & FVIRTKEY && lpAccel[k].fVirt & FCONTROL) == !!bCtrl);
				nRet += (strShortcut == szShortcut);

				if (nRet >= 4)
					break;
			}
			delete [] lpAccel;
		}
	}
	return (nRet >= 4);
}

int CGlWinApp::DoTimedMessageBox(UINT nTimeout, LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
	// disable windows for modal dialog
	EnableModeless(FALSE);
	HWND hWndTop;
	HWND hWnd = CWnd::GetSafeOwner(NULL, &hWndTop)->GetSafeHwnd();

	// set help context if possible
	DWORD* pdwContext = NULL;
#if _MSC_VER >= 1200
	if (hWnd != NULL)
	{
		// use app-level context or frame level context
		LRESULT lResult = ::SendMessage(hWndTop, WM_HELPPROMPTADDR, 0, 0);
		if (lResult != 0)
			pdwContext = (DWORD*)lResult;
	}
#endif // _MSC_VER >= 1200
	// for backward compatibility use app context if possible
	if (pdwContext == NULL && this != NULL)
		pdwContext = &m_dwPromptContext;

	DWORD dwOldPromptContext = 0;
	if (pdwContext != NULL)
	{
		// save old prompt context for restoration later
		dwOldPromptContext = *pdwContext;
		if (nIDPrompt != 0)
			*pdwContext = HID_BASE_PROMPT+nIDPrompt;
	}

	// determine icon based on type specified
	if ((nType & MB_ICONMASK) == 0)
	{
		switch (nType & MB_TYPEMASK)
		{
		case MB_OK:
		case MB_OKCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_YESNO:
		case MB_YESNOCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_ABORTRETRYIGNORE:
		case MB_RETRYCANCEL:
			// No default icon for these types, since they are rarely used.
			// The caller should specify the icon.
			break;
		}
	}

#ifdef _DEBUG
	if ((nType & MB_ICONMASK) == 0)
		TRACE0("Warning: no icon specified for message box.\n");
#endif

	TCHAR szAppName[_MAX_PATH];
	LPCTSTR pszAppName;
	if (this != NULL)
		pszAppName = m_pszAppName;
	else
	{
		pszAppName = szAppName;
		GetModuleFileName(NULL, szAppName, _MAX_PATH);
	}

	int nResult =
		::TimedMessageBox(nTimeout, hWnd, lpszPrompt, pszAppName, nType);

	// restore prompt context if possible
	if (pdwContext != NULL)
		*pdwContext = dwOldPromptContext;

	// re-enable windows
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	EnableModeless(TRUE);

	return nResult;
}

COLORREF* CGlWinApp::GetCustColors() const
{
	return m_custColors;
}

int CGlWinApp::GetCustColorsCount() const
{
	return _countof(m_custColors);
}
