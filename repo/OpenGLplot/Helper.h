/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: April 1999
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// Helper.h :  Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(__HELPER_H_)
#define __HELPER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "GlWinApp.h"
#include "Matrix.h"
#include "IntMatrix.h"

typedef struct _GlInfoStruct
{
	CString strGlVendor;
	CString strGlRenderer;
	CString strGlVersion;
	CString strGlAcceleration;
	CString strGlExtensions;
	CString strGluVersion;
	CString strGluExtensions;
	int nMaxLights;
	int nMaxClipPlanes;
	int nMaxModelStackDepth;
	int nMaxProjectionStackDepth;
	int nMaxTextureStackDepth;
	int nMaxNameStackDepth;
	int nMaxAttributeStackDepth;
	int nMaxTextureSize;
	int nMaxListNesting;
	int nMaxEvalOrder;
	CSize maxViewportDims;
	int nAuxiliaryBuffers;
	float minSmoothPointSize;
	float maxSmoothPointSize;
	float smoothPointGranularity;
	float minSmoothLineSize;
	float maxSmoothLineSize;
	float smoothLineGranularity;
	int nRedBits;
	int nGreenBits;
	int nBlueBits;
	int nAlphaBits;
	int nDepthBits;
	int nStencilBits;
}
GlInfoStruct;

/////////////////////////////////////////////////////////////////////////////
// Helper functions

unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
void DefaultPaletteEntries(LOGPALETTE  *pPal);
AFX_EXT_API_GENERAL COLORREF BlendColors(COLORREF colorInput, COLORREF colorOldFore, COLORREF colorNewFore, COLORREF colorOldBack, COLORREF colorNewBack);
AFX_EXT_API_GENERAL void RGB2HLS(BYTE red, BYTE green, BYTE blue, short& hue, short& lum, short& sat);
AFX_EXT_API_GENERAL void HLS2RGB(short hue, short lum, short sat, BYTE& red, BYTE& green, BYTE& blue);

AFX_EXT_API_GENERAL void inverse_bytes(void *buffer, size_t size, size_t count = 1);
AFX_EXT_API_GENERAL void inverse_bits(void *buffer, size_t size, size_t count = 1);
AFX_EXT_API_GENERAL BOOL RunExtraLoop();
AFX_EXT_API_GENERAL LRESULT PostMessageTimeout(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, 
							UINT nFlags, UINT nTimeout, LPDWORD lpdwResult, 
							BOOL bRetry = FALSE);
AFX_EXT_API_GENERAL int TimedMessageBox(UINT nTimeout, HWND hWnd, LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);
AFX_EXT_API_GENERAL int AFXAPI AfxTimedMessageBox(UINT nTimeout, LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);
AFX_EXT_API_GENERAL int AFXAPI AfxTimedMessageBox(UINT nTimeout, UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT)-1);
AFX_EXT_API_GENERAL CWnd* AFXAPI AfxGetSafeMainWnd();
AFX_EXT_API_GENERAL OSRUNNING GetOSRunning();
int AFXAPI LoadString(LPCTSTR lpszModuleName, UINT nID, LPTSTR lpszBuf, UINT nMaxBuf);
BOOL LoadString(LPCTSTR lpszModuleName, UINT nID, CString& rString);
AFX_EXT_API_GENERAL CRect GetItemTemplateRect(UINT nIDTemplate, UINT nIDCtrl);
AFX_EXT_API_GENERAL CRect GetItemTemplateRect(LPCTSTR lpszTemplateName, UINT nIDCtrl);
AFX_EXT_API_GENERAL CRect GetTemplateRect(UINT nIDTemplate);
AFX_EXT_API_GENERAL CRect GetTemplateRect(LPCTSTR lpszTemplateName);

AFX_EXT_API_GENERAL int MapDialogUnitX(int nX, CWnd* pWnd);
AFX_EXT_API_GENERAL int MapDialogUnitY(int nY, CWnd* pWnd);

AFX_EXT_API_GENERAL CWnd* GetDefaultPushButton(CWnd* pParent);
AFX_EXT_API_GENERAL void SetDefaultPushButton(CWnd* pParent, CWnd* pButton, BOOL bForce = FALSE);
AFX_EXT_API_GENERAL void UnsetDefaultPushButton(CWnd* pParent, CWnd* pButton);
AFX_EXT_API_GENERAL BOOL IsDefaultPushButton(CWnd* pButton);
AFX_EXT_API_GENERAL BOOL IsPushButton(CWnd* pButton);
AFX_EXT_API_GENERAL BOOL IsEdit(CWnd* pWnd);
AFX_EXT_API_GENERAL BOOL IsEnabled(CCmdUI* pCmdUI);
AFX_EXT_API_GENERAL void UpdateDialogControl(UINT nID, CWnd* pParent, BOOL bDisableIfNoHndler);

void CheckButtonSpecial(CButton& wndButton, int& value, int strID,  BOOL bSaveAndValidate);

AFX_EXT_API_GENERAL BOOL TestDockState(LPCTSTR lpszProfileName, CFrameWnd* pFrame);

AFX_EXT_API_GENERAL BOOL ReadRecordFromStream(FILE*& stream, CString& str);
AFX_EXT_API_GENERAL BOOL ReadQuotedStringFromStream(FILE*& stream, CString& str);
AFX_EXT_API_GENERAL BOOL ReadLineFromStream(FILE*& stream, CString& str);
AFX_EXT_API_GENERAL int AFX_CDECL fprintf_CommaInsteadOfDot(FILE* stream, LPCTSTR lpszFormat, ...);

extern "C" LONG APIENTRY LaunchApp(HWND hWnd, LPLONG lpnValue, LPSTR lpszValue);	// to be used by installshield

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1200
typedef struct _OSVERSIONINFOEXA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
    WORD wServicePackMajor;
    WORD wServicePackMinor;
    BYTE wProductType;
    BYTE wReserved;
} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;
typedef struct _OSVERSIONINFOEXW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR szCSDVersion[ 128 ];     // Maintenance string for PSS usage
    WORD wServicePackMajor;
    WORD wServicePackMinor;
    BYTE wProductType;
    BYTE wReserved;
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW;
#ifdef UNICODE
typedef OSVERSIONINFOEXW OSVERSIONINFOEX;
typedef POSVERSIONINFOEXW POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXW LPOSVERSIONINFOEX;
#else
typedef OSVERSIONINFOEXA OSVERSIONINFOEX;
typedef POSVERSIONINFOEXA POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXA LPOSVERSIONINFOEX;
#endif // UNICODE
#endif // _MSC_VER < 1200

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(__HELPER_H_)
