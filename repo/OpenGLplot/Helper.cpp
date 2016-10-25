/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
// based on a sample of
// Adrian Roman <aroman@medanet.ro>
//
/////////////////////////////////////////////////////////////////////////////
// Helper.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Helper.h"

static unsigned char three2eight[8] = {
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

static unsigned char two2eight[4] = {
	0, 0x55, 0xaa, 0xff
};

static unsigned char one2eight[2] = {
	0, 255
};

unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift)
{
	unsigned char val;

	val = (unsigned char) (i >> shift);

	switch (nbits)
	{
		case 1:
			val &= 0x1;
			return one2eight[val];
		case 2:
			val &= 0x3;
			return two2eight[val];
		case 3:
			val &= 0x7;
			return three2eight[val];
		default:
			return 0;
	}
}

static int defaultOverride[13] = {
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] = {
	{ 0,   0,   0,   0 },
	{ 0x80,0,   0,   0 },
	{ 0,   0x80,0,   0 },
	{ 0x80,0x80,0,   0 },
	{ 0,   0,   0x80,0 },
	{ 0x80,0,   0x80,0 },
	{ 0,   0x80,0x80,0 },
	{ 0xC0,0xC0,0xC0,0 },

	{ 192, 220, 192, 0 },
	{ 166, 202, 240, 0 },
	{ 255, 251, 240, 0 },
	{ 160, 160, 164, 0 },

	{ 0x80,0x80,0x80,0 },
	{ 0xFF,0,   0,   0 },
	{ 0,   0xFF,0,   0 },
	{ 0xFF,0xFF,0,   0 },
	{ 0,   0,   0xFF,0 },
	{ 0xFF,0,   0xFF,0 },
	{ 0,   0xFF,0xFF,0 },
	{ 0xFF,0xFF,0xFF,0 }
};

void DefaultPaletteEntries(LOGPALETTE  *pPal)
{
	ASSERT(pPal);

	for (int i = 1 ; i <= 12; i++)
		pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
}

COLORREF BlendColors(COLORREF colorInput, COLORREF colorOldFore, COLORREF colorNewFore, COLORREF colorOldBack, COLORREF colorNewBack)
{
	int redInput, greenInput, blueInput, 
		redOldFore, greenOldFore, blueOldFore, 
		redNewFore, greenNewFore, blueNewFore, 
		redOldBack, greenOldBack, blueOldBack, 
		redNewBack, greenNewBack, blueNewBack, 
		redOutput, greenOutput, blueOutput;
	double redLambda, greenLambda, blueLambda;

	redInput = GetRValue(colorInput);
	greenInput = GetGValue(colorInput);
	blueInput = GetBValue(colorInput);

	redOldFore = GetRValue(colorOldFore);
	greenOldFore = GetGValue(colorOldFore);
	blueOldFore = GetBValue(colorOldFore);

	redNewFore = GetRValue(colorNewFore);
	greenNewFore = GetGValue(colorNewFore);
	blueNewFore = GetBValue(colorNewFore);

	redOldBack = GetRValue(colorOldBack);
	greenOldBack = GetGValue(colorOldBack);
	blueOldBack = GetBValue(colorOldBack);

	redNewBack = GetRValue(colorNewBack);
	greenNewBack = GetGValue(colorNewBack);
	blueNewBack = GetBValue(colorNewBack);

	if (redOldBack != redOldFore)
	{
		redLambda = double(redNewBack-redNewFore)/double(redOldBack-redOldFore);
		redOutput = (int)round(redNewFore + redLambda*(redInput-redOldFore));
		if (redOutput < 0)
			redOutput = 0;
		else if (redOutput > 255)
			redOutput = 255;
	}
	else
	{
		redOutput = redOldBack;
	}

	if (greenOldBack != greenOldFore)
	{
		greenLambda = double(greenNewBack-greenNewFore)/double(greenOldBack-greenOldFore);
		greenOutput = (int)round(greenNewFore + greenLambda*(greenInput-greenOldFore));
		if (greenOutput < 0)
			greenOutput = 0;
		else if (greenOutput > 255)
			greenOutput = 255;
	}
	else
	{
		greenOutput = greenOldBack;
	}

	if (blueOldBack != blueOldFore)
	{
		blueLambda = double(blueNewBack-blueNewFore)/double(blueOldBack-blueOldFore);
		blueOutput = (int)round(blueNewFore + blueLambda*(blueInput-blueOldFore));
		if (blueOutput < 0)
			blueOutput = 0;
		else if (blueOutput > 255)
			blueOutput = 255;
	}
	else
	{
		blueOutput = blueOldBack;
	}

	return RGB(redOutput,greenOutput,blueOutput);
}

#define  HLSMAX   360	// hue, lum, sat vary over 0-HLSMAX
#define  RGBMAX   255	// red, green, blue vary over 0-RGBMAX
						// HLSMAX BEST IF DIVISIBLE BY 6
						// RGBMAX, HLSMAX must each fit in a byte.

// Hue is undefined if Saturation is 0 (grey-scale)
// This value determines where the Hue scrollbar is
// initially set for achromatic colors
#define UNDEFINED (HLSMAX*2/3)

void RGB2HLS(BYTE red, BYTE green, BYTE blue, short& hue, short& lum, short& sat)
{ 
	BYTE cMax, cMin;				// max and min RGB values
	short rDelta, gDelta, bDelta;	// intermediate value: % of spread from max

	// calculate lightness
	cMax = max(max(red, green), blue);
	cMin = min(min(red, green), blue);
	lum = short((((cMax+cMin)*HLSMAX) + RGBMAX)/(2*RGBMAX));

	if (cMax == cMin)
	{
		// achromatic case (r == g == b)

		sat = 0;			// saturation
		hue = UNDEFINED;	// hue
	}
	else
	{
		// chromatic case

		// saturation
		if (lum <= (HLSMAX/2))
			sat = short((((cMax-cMin)*HLSMAX) + ((cMax+cMin)/2)) / (cMax+cMin));
		else
			sat = short((((cMax-cMin)*HLSMAX) + ((2*RGBMAX-cMax-cMin)/2)) / (2*RGBMAX-cMax-cMin));

		// hue
		rDelta = short((((cMax-red)*(HLSMAX/6)) + ((cMax-cMin)/2)) / (cMax-cMin));
		gDelta = short((((cMax-green)*(HLSMAX/6)) + ((cMax-cMin)/2)) / (cMax-cMin));
		bDelta = short((((cMax-blue)*(HLSMAX/6)) + ((cMax-cMin)/2)) / (cMax-cMin));

		if (red == cMax)
			hue = short(bDelta - gDelta);
		else if (green == cMax)
			hue = short((HLSMAX/3) + rDelta - bDelta);
		else if (blue == cMax)
			hue = short(((2*HLSMAX)/3) + gDelta - rDelta);

		if (hue < 0)
			hue += HLSMAX;
		if (hue > HLSMAX)
			hue -= HLSMAX;
	}
}

// utility routine for HLS2RGB
short Hue2RGB(short n1, short n2, short hue)
{ 
	// range check: note values passed add/subtract thirds of range
	if (hue < 0)
		hue += HLSMAX;

	if (hue > HLSMAX)
		hue -= HLSMAX;

	// return r,g, or b value from this tridrant
	if (hue < (HLSMAX/6))
		return short(n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)));
	if (hue < (HLSMAX/2))
		return n2;
	if (hue < ((HLSMAX*2)/3))
		return short(n1 + (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6)));
	else
		return n1;
}

void HLS2RGB(short hue, short lum, short sat, BYTE& red, BYTE& green, BYTE& blue)
{ 
	short magic1, magic2;	// calculated magic numbers

	if (sat == 0)
	{
		// achromatic case

		red = green = blue = (BYTE)MulDiv(lum, RGBMAX, HLSMAX);
		if (hue != UNDEFINED)
		{
			// ERROR
			ASSERT(FALSE);
		}
	}
	else
	{
		// chromatic case

		// set up magic numbers
		if (lum <= (HLSMAX/2))
			magic2 = short((lum*(HLSMAX + sat) + (HLSMAX/2))/HLSMAX);
		else
			magic2 = short(lum + sat - ((lum*sat) + (HLSMAX/2))/HLSMAX);
		magic1 = short(2*lum-magic2);

		// get RGB, change units from HLSMAX to RGBMAX
		red = BYTE((Hue2RGB(magic1,magic2,short(hue+(HLSMAX/3)))*RGBMAX + (HLSMAX/2)) / HLSMAX);
		green = BYTE((Hue2RGB(magic1,magic2,hue)*RGBMAX + (HLSMAX/2)) / HLSMAX);
		blue = BYTE((Hue2RGB(magic1,magic2,short(hue-(HLSMAX/3)))*RGBMAX + (HLSMAX/2)) / HLSMAX);
	}
}

void inverse_bytes(void* buffer, size_t size, size_t count)
{
	BYTE tmpByte, *pByteLower, *pByteHigher;

	for (size_t i = 0; i < count; i++)
	{
		pByteLower = (BYTE*)buffer + i*size;
		pByteHigher = (BYTE*)buffer + (i+1)*size - 1;
		for ( ; pByteLower < pByteHigher; pByteLower++, pByteHigher--)
		{
			tmpByte = *pByteLower;
			*pByteLower = *pByteHigher;
			*pByteHigher = tmpByte;
		}
	}
}

void inverse_bits(void* buffer, size_t size, size_t count)
{
	BYTE tmpByte, *pByte;
	inverse_bytes(buffer, size, count);

	for (size_t i = 0; i < size*count; i++)
	{
		pByte = (BYTE*)buffer + i;
		tmpByte = BYTE(((*pByte << 7) & 0x80) | 
			((*pByte << 5) & 0x40) | 
			((*pByte << 3) & 0x20) | 
			((*pByte << 1) & 0x10) | 
			((*pByte >> 1) & 0x08) | 
			((*pByte >> 3) & 0x04) | 
			((*pByte >> 5) & 0x02) | 
			((*pByte >> 7) & 0x01));
		*pByte = tmpByte;
	}
}

BOOL RunExtraLoop()
{
	CWinThread* pWinThread = AfxGetThread();
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
#endif

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!pWinThread->PumpMessage())
		{
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
			ASSERT(pState->m_msgCur.message == WM_QUIT);
			AfxPostQuitMessage(pState->m_msgCur.wParam);
#else
			ASSERT(pWinThread->m_msgCur.message == WM_QUIT);
			AfxPostQuitMessage(pWinThread->m_msgCur.wParam);
#endif
			return FALSE;
		}
	}

	// let MFC do its idle processing
	LONG lIdle = 0;
	while (pWinThread->OnIdle(lIdle++));

	return TRUE;
}

LRESULT PostMessageTimeout(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, 
							UINT nFlags, UINT nTimeout, LPDWORD lpdwResult, 
							BOOL bRetry)
{
	CGlWinApp *pApp = (CGlWinApp *)AfxGetApp();
	LRESULT ret;

	if (pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)) && pApp->m_nOsRunning >= OSWinNT)
	{
		ret = SendMessageTimeout(hWnd, Msg, wParam, lParam, nFlags, nTimeout, lpdwResult);
		if (bRetry && ret == 0 && GetLastError() == ERROR_TIMEOUT)
		{
			// retry it
			ret = PostMessage(hWnd, Msg, wParam, lParam);
		}
	}
	else
	{
		// It seems that Win95/Win98 returns GetLastError() == ERROR_NO_ERROR after timeout.
		// so we ignore SendMessageTimeout in this case.
		ret = PostMessage(hWnd, Msg, wParam, lParam);
	}
	return ret;
}

/*static void CALLBACK MessageBoxTimerProc(HWND hwnd, UINT uiMsg, UINT idEvent, DWORD dwTime)
{
	UNREFERENCED_PARAMETER(hwnd);
	UNREFERENCED_PARAMETER(uiMsg);
	UNREFERENCED_PARAMETER(idEvent);
	UNREFERENCED_PARAMETER(dwTime);

	AfxPostQuitMessage(0);
}

int TimedMessageBox(UINT nTimeout, HWND hWnd, LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	UINT idTimer, uiResult;
	MSG msg;

	// if called from a modal dialog we have to restore the z-order and to reenable the active window
	BOOL bIsMainWndEnabled = FALSE, bIsForegroundWndEnabled = FALSE, bIsActiveWndEnabled = FALSE;
	HWND hMainWnd = AfxGetMainWnd()->GetSafeHwnd();
	HWND hActiveWnd = GetActiveWindow();
	HWND hForegroundWnd = GetForegroundWindow();

	if (hMainWnd)
	{
		bIsMainWndEnabled = IsWindowEnabled(hMainWnd);
	}

	if (hActiveWnd)
	{
		bIsActiveWndEnabled = IsWindowEnabled(hActiveWnd);
	}

	if (hForegroundWnd)
	{
		bIsForegroundWndEnabled = IsWindowEnabled(hForegroundWnd);
	}

	idTimer = SetTimer(NULL, 0, nTimeout, (TIMERPROC)MessageBoxTimerProc);

	uiResult = MessageBox(hWnd, lpszText, lpszCaption, nType);

	KillTimer(NULL, idTimer);

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			if (hMainWnd)
			{
				EnableWindow(hMainWnd, bIsMainWndEnabled);
			}

			if (hActiveWnd && hActiveWnd != hForegroundWnd)
			{
				EnableWindow(hActiveWnd, bIsActiveWndEnabled);
				SetActiveWindow(hActiveWnd);
			}

			if (hForegroundWnd)
			{
				EnableWindow(hForegroundWnd, bIsForegroundWndEnabled);
				SetForegroundWindow(hForegroundWnd);
			}

			if (!bIsMainWndEnabled && !bIsActiveWndEnabled)
				EnableWindow(hMainWnd, TRUE);

			uiResult = IDBACK;
			continue;
		}

		if (msg.message != WM_KICKIDLE && !AfxGetThread()->PreTranslateMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return uiResult;
}*/

typedef struct _MessageBoxInfo
{
	DWORD	dwCurrentThreadID;
	HANDLE	hEventHandle;
	UINT	nTimeout;
	HWND	hWnd;
	LPCTSTR	lpszCaption;
	LPCTSTR	lpszText;
} 
MessageBoxInfo;

static BOOL CALLBACK FindMessageBox(HWND hWnd, LPARAM lParam)
{
	MessageBoxInfo *pmbInfo = (MessageBoxInfo*)lParam;
	TCHAR szClassName[256];
	BOOL bRetVal = TRUE;

	GetClassName(hWnd, szClassName, _countof(szClassName));
	if (!_tcsicmp(szClassName, _T("#32770")))
	{
		// get caption
		int nLengthCaption = GetWindowTextLength(hWnd);
		TCHAR* pszCaption = new TCHAR[nLengthCaption+1];
		GetWindowText(hWnd, pszCaption, nLengthCaption+1);

		// check cation
		if (_tcscmp(pszCaption, pmbInfo->lpszCaption) == 0)
		{
			for (HWND hChild = ::GetWindow(hWnd, GW_CHILD); hChild != NULL; hChild = ::GetNextWindow(hChild, GW_HWNDNEXT))
			{
				GetClassName(hChild, szClassName, _countof(szClassName));
				if (_tcsicmp(szClassName, _T("Static")) == 0)
				{
					// get text
					int nLengthText = GetWindowTextLength(hChild);
					TCHAR* pszText = new TCHAR[nLengthText+1];
					GetWindowText(hChild, pszText, nLengthText+1);

					// check text
					if (_tcscmp(pszText, pmbInfo->lpszText) == 0)
					{
						pmbInfo->hWnd = hWnd;
						bRetVal = FALSE;
					}

					if (pszText != NULL)
						delete pszText;

					if (!bRetVal)
						break;
				}
			}
		}

		if (pszCaption != NULL)
			delete pszCaption;
	}
	return bRetVal;
}

static unsigned int AFXAPI TimedMessageBoxThreadProc(LPVOID pParam)
{
	MessageBoxInfo *pmbInfo = (MessageBoxInfo*)pParam;
	UINT nTime;
	UINT nTimeout;
	DWORD dwWaitRetVal;
	DWORD dwCheckInterval = 100;
	TCHAR szClassName[256];
	TCHAR szWindowName[256];
	HFONT hFont = NULL;
	HWND hChild;

	AttachThreadInput(GetCurrentThreadId(), pmbInfo->dwCurrentThreadID, TRUE);

	// try to find the handle of the MessageBox (at most 10 times)
	HWND hWndMessageBox = NULL;
	for (int k = 0; k < 10; k++)
	{
		if (!EnumThreadWindows(pmbInfo->dwCurrentThreadID, FindMessageBox, (LPARAM)pmbInfo))
		{
			hWndMessageBox = pmbInfo->hWnd;
			break;
		}
	}

	if (hWndMessageBox == NULL)
		return 0;

	RECT rect, rectAll;

	// prepare check box
	rectAll.left = -1;
	rectAll.right = -1;
	rectAll.top = -1;
	rectAll.bottom = -1;
	int nButtonCount = 0;
	for (hChild = ::GetWindow(hWndMessageBox, GW_CHILD); hChild != NULL; hChild = ::GetNextWindow(hChild, GW_HWNDNEXT))
	{
		GetClassName(hChild, szClassName, _countof(szClassName));
		if (_tcsicmp(szClassName, _T("Button")) == 0)
		{
			// get button rects
			GetWindowRect(hChild, &rect);
			ScreenToClient(hWndMessageBox, (LPPOINT)&rect);
			ScreenToClient(hWndMessageBox, ((LPPOINT)&rect)+1);

			if (nButtonCount++ == 0)
			{
				rectAll = rect;
			}
			else
			{
				rectAll.left = min(rectAll.left, rect.left);
				rectAll.right = max(rectAll.right, rect.right);
				rectAll.top = min(rectAll.top, rect.top);
				rectAll.bottom = max(rectAll.bottom, rect.bottom);
			}
		}
		else if (_tcsicmp(szClassName, _T("Static")) == 0)
		{
			// get text
			int nLengthText = GetWindowTextLength(hChild);
			TCHAR* pszText = new TCHAR[nLengthText+1];
			GetWindowText(hChild, pszText, nLengthText+1);

			// get font
			if (nLengthText > 0)
			{
				hFont = (HFONT)SendMessage(hChild, WM_GETFONT, 0, 0);
			}

			if (pszText != NULL)
				delete pszText;
		}
	}
	GetWindowRect(hWndMessageBox, &rect);

	// enlarge message box
	int nButtonHeight = rectAll.bottom - rectAll.top;
	rect.bottom += nButtonHeight;
	SetWindowPos(hWndMessageBox, NULL, -1, -1, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	UpdateWindow(hWndMessageBox); 

	// continue prepare check box
	HDC hDC;
	hDC = GetDC(hWndMessageBox);
	if (hDC != NULL)
	{
		// get maximal size
		SIZE size;
		HGDIOBJ hOldFont = NULL;
		hOldFont = SelectObject(hDC, hFont);
		GetTextExtentPoint32(hDC, _T("hh:mm:ss"), _tcslen(_T("hh:mm:ss")), &size);
		if (hOldFont != NULL)
			SelectObject(hDC, hOldFont);
		ReleaseDC(hWndMessageBox, hDC);

		rectAll.left = ((rect.right-rect.left) - (size.cx+size.cy))/2;
	}

	// move check box down
	rectAll.top += -nButtonHeight/3;
	rectAll.bottom += -nButtonHeight/3;

	// move buttons down
	for (hChild = ::GetWindow(hWndMessageBox, GW_CHILD); hChild != NULL; hChild = ::GetNextWindow(hChild, GW_HWNDNEXT))
	{
		GetClassName(hChild, szClassName, _countof(szClassName));
		if (_tcsicmp(szClassName, _T("Button")) == 0)
		{
			// get button rects
			GetWindowRect(hChild, &rect);
			ScreenToClient(hWndMessageBox, (LPPOINT)&rect);
			ScreenToClient(hWndMessageBox, ((LPPOINT)&rect)+1);
			rect.top += nButtonHeight;
			rect.bottom += nButtonHeight;
			SetWindowPos(hChild, NULL, rect.left, rect.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	// create check box
	HWND hWndExtra = ::CreateWindowEx(0, _T("Button"), _T(""), 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 
		rectAll.left, rectAll.top,
		rectAll.right - rectAll.left, rectAll.bottom - rectAll.top, 
		hWndMessageBox, (HMENU)-1, AfxGetInstanceHandle(), NULL);

	// set it checked
	SendMessage(hWndExtra, BM_SETCHECK, 1, 0);
	if (hFont != NULL)
		SendMessage(hWndExtra, WM_SETFONT, (WPARAM)hFont, FALSE);

	// show it
	UpdateWindow(hWndExtra); 

	// count down
	dwWaitRetVal = WaitForSingleObject(pmbInfo->hEventHandle, 0);
	nTimeout = pmbInfo->nTimeout;
	nTime = nTimeout;
	while ((int)nTime > 0)
	{
		dwWaitRetVal = WaitForSingleObject(pmbInfo->hEventHandle, dwCheckInterval);
		if (dwWaitRetVal != WAIT_TIMEOUT)
			break;

		if (SendMessage(hWndExtra, BM_GETCHECK, 0, 0) == BST_CHECKED)
			nTime -= dwCheckInterval;

		// format time string
		if (nTime >= 3600000)
			_stprintf(szWindowName, _T("%d:%02d:%02d"), (nTime/1000)/3600, ((nTime/1000)%3600)/60, ((nTime/1000)%3600)%60);
		else if (nTime >= 60000)
			_stprintf(szWindowName, _T("%d:%02d"), (nTime/1000)/60, (nTime/1000)%60);
		else
			_stprintf(szWindowName, _T("%d s"), nTime/1000);
		SetWindowText(hWndExtra, szWindowName);

		// process messages
		if (hWndExtra != NULL)
		{
			MSG msg;
			while (PeekMessage(&msg, hWndExtra, 0, 0, PM_REMOVE))
			{
				//PostThreadMessage(pmbInfo->dwCurrentThreadID, msg.message, msg.wParam, msg.lParam);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	if (dwWaitRetVal == WAIT_TIMEOUT)
		EndDialog(hWndMessageBox, IDBACK);

	return 0;
}

int TimedMessageBox(UINT nTimeout, HWND hWnd, LPCTSTR lpszText, LPCTSTR lpszCaption, UINT uType)
{
	MessageBoxInfo mbInfo;
	mbInfo.dwCurrentThreadID = GetCurrentThreadId();
	mbInfo.nTimeout = nTimeout;
	mbInfo.hEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	mbInfo.hWnd = NULL;
	mbInfo.lpszText = lpszText;
	mbInfo.lpszCaption = lpszCaption;

	UINT uThreadID;
	HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, TimedMessageBoxThreadProc, (void*)&mbInfo, 0, &uThreadID);

	int nRet = MessageBox(hWnd, lpszText, lpszCaption, uType);

	SetEvent(mbInfo.hEventHandle);
	CloseHandle(mbInfo.hEventHandle);

	if (hThreadHandle != NULL)
	{
		WaitForSingleObject(hThreadHandle, INFINITE);
		CloseHandle(hThreadHandle);
	}

	return nRet;
}

int AFXAPI AfxTimedMessageBox(UINT nTimeout, LPCTSTR lpszText, UINT nType, UINT nIDHelp)
{
	CGlWinApp* pApp = (CGlWinApp*)AfxGetApp();
	ASSERT(!pApp || pApp->IsKindOf(RUNTIME_CLASS(CGlWinApp)));
	if (pApp != NULL)
		return pApp->DoTimedMessageBox(nTimeout, lpszText, nType, nIDHelp);
	else
		return pApp->CGlWinApp::DoTimedMessageBox(nTimeout, lpszText, nType, nIDHelp);
}

int AFXAPI AfxTimedMessageBox(UINT nTimeout, UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
	CString string;
	if (!string.LoadString(nIDPrompt))
	{
		TRACE1("Error: failed to load message box prompt string 0x%04x.\n",
			nIDPrompt);
		ASSERT(FALSE);
	}
	if (nIDHelp == (UINT)-1)
		nIDHelp = nIDPrompt;
	return AfxTimedMessageBox(nTimeout, string, nType, nIDHelp);
}

static BOOL CALLBACK GetSafeMainWndProc(HWND hWnd, LPARAM lParam)
{
	if (GetParent(hWnd) == NULL)
	{
		*(HWND*)lParam = hWnd;
		return FALSE;
	}

	return TRUE;
}

CWnd* AFXAPI AfxGetSafeMainWnd()
{
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd == NULL)
	{
		HWND hWnd = NULL;
		if (!EnumThreadWindows(GetCurrentThreadId(), GetSafeMainWndProc, (LPARAM)&hWnd))
			pWnd = CWnd::FromHandle(hWnd);
	}
	return pWnd;
}

OSRUNNING GetOSRunning()
{
	int nRet = 0;
	OSVERSIONINFOEX OSInfoEx;
	LPOSVERSIONINFO pOSInfo = (LPOSVERSIONINFO)&OSInfoEx;
	LPOSVERSIONINFOEX pOSInfoEx = &OSInfoEx;

	if (!nRet)
	{
		ZeroMemory(pOSInfo, sizeof(OSVERSIONINFO));
		pOSInfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		nRet = ::GetVersionEx(pOSInfo);
	}
	if (!nRet)
	{
		ZeroMemory(pOSInfoEx, sizeof(OSVERSIONINFOEX));
		pOSInfoEx->dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		nRet = ::GetVersionEx(pOSInfo);
	}

	OSRUNNING retOS = OSUnknown;
	if (!nRet)
	{
		return retOS;
	}

	if (pOSInfo->dwPlatformId == VER_PLATFORM_WIN32s)
	{
		retOS = OSWin3;
	}
	else if ((pOSInfo->dwPlatformId == VER_PLATFORM_WIN32_WINDOWS))
	{
		ASSERT(pOSInfo->dwMajorVersion == 4);
		ASSERT(pOSInfo->dwMinorVersion == 0 || pOSInfo->dwMinorVersion == 10 || pOSInfo->dwMinorVersion == 90);
		if (pOSInfo->dwMinorVersion == 0)
			retOS = OSWin95;
		else if (pOSInfo->dwMinorVersion == 10)
			retOS = OSWin98;
		else/* if (pOSInfo->dwMinorVersion == 90)*/
			retOS = OSWinME;
	}
	else if (pOSInfo->dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		ASSERT((pOSInfo->dwMajorVersion == 4 && pOSInfo->dwMinorVersion == 0) || 
			(pOSInfo->dwMajorVersion == 5 && pOSInfo->dwMinorVersion == 0) || 
			(pOSInfo->dwMajorVersion == 5 && pOSInfo->dwMinorVersion == 1) || 
			(pOSInfo->dwMajorVersion == 5 && pOSInfo->dwMinorVersion == 2) || 
			(pOSInfo->dwMajorVersion == 6 && pOSInfo->dwMinorVersion == 0) ||
			(pOSInfo->dwMajorVersion == 6 && pOSInfo->dwMinorVersion == 1));
		retOS = ((pOSInfo->dwMajorVersion == 4) ? OSWinNT : ((pOSInfo->dwMinorVersion == 0) ? OSWin2K : ((pOSInfo->dwMinorVersion == 1) ? OSWinXP : OSWin2K3)));
		if (pOSInfo->dwMajorVersion == 4)
			retOS = OSWinNT;
		else if (pOSInfo->dwMajorVersion == 5 && pOSInfo->dwMinorVersion == 0)
			retOS = OSWin2K;
		else if (pOSInfo->dwMajorVersion == 5 && pOSInfo->dwMinorVersion == 1)
			retOS = OSWinXP;
		else if (pOSInfo->dwMajorVersion == 5 && pOSInfo->dwMinorVersion == 2)
			retOS = OSWin2K3;
		else/* if (pOSInfo->dwMajorVersion == 6 && pOSInfo->dwMinorVersion == 0)*/
			retOS = OSWinVista;
	}

	return retOS;
}

// LoadString must not only check for the appropriate string segment in
//   the specified modules resource file, but also that the string is non-zero
int AFXAPI LoadString(LPCTSTR lpszModuleName, UINT nID, LPTSTR lpszBuf, UINT nMaxBuf)
{
	ASSERT(AfxIsValidAddress(lpszBuf, nMaxBuf*sizeof(TCHAR)));

	LPCTSTR lpszName = MAKEINTRESOURCE((nID>>4)+1);
	HINSTANCE hInstance;
	int nLen;

	hInstance = AfxLoadLibrary(lpszModuleName);
	if (hInstance && ::FindResource(hInstance, lpszName, RT_STRING) != NULL &&
		(nLen = ::LoadString(hInstance, nID, lpszBuf, nMaxBuf)) != 0)
	{
		// found a non-zero string in the module
		AfxFreeLibrary(hInstance);
		return nLen;
	}

	// did not find it
	AfxFreeLibrary(hInstance);
	lpszBuf[0] = '\0';
	return 0;
}

#ifndef CHAR_FUDGE
#ifdef _UNICODE
#define CHAR_FUDGE 1    // one TCHAR unused is good enough
#else
#define CHAR_FUDGE 2    // two BYTES unused for case of DBC last char
#endif //_UNICODE
#endif //CHAR_FUDGE

BOOL LoadString(LPCTSTR lpszModuleName, UINT nID, CString& rString)
{
	// try fixed buffer first (to avoid wasting space in the heap)
	TCHAR szTemp[256];
	int nLen = LoadString(lpszModuleName, nID, szTemp, _countof(szTemp));
	if (_countof(szTemp) - nLen > CHAR_FUDGE)
	{
		rString = szTemp;
		return nLen > 0;
	}

	// try buffer size of 512, then larger size until entire string is retrieved
	int nSize = 256;
	do
	{
		nSize += 256;
		nLen = LoadString(lpszModuleName, nID, rString.GetBuffer(nSize-1), nSize);
	}
	while (nSize - nLen <= CHAR_FUDGE);
	rString.ReleaseBuffer();

	return nLen > 0;
}

static inline BOOL IsDialogEx(const DLGTEMPLATE* pTemplate)
{
	return ((DLGTEMPLATEEX*)pTemplate)->signature == 0xFFFF;
}

static DLGITEMTEMPLATE* AFXAPI FindFirstDlgItem(const DLGTEMPLATE* pTemplate)
{
	DWORD dwStyle = pTemplate->style;
	BOOL bDialogEx = IsDialogEx(pTemplate);

	WORD* pw;
	if (bDialogEx)
	{
		pw = (WORD*)((DLGTEMPLATEEX*)pTemplate + 1);
		dwStyle = ((DLGTEMPLATEEX*)pTemplate)->style;
	}
	else
	{
		pw = (WORD*)(pTemplate + 1);
	}

	if (*pw == (WORD)-1)        // Skip menu name ordinal or string
		pw += 2; // WORDs
	else
		while (*pw++);

	if (*pw == (WORD)-1)        // Skip class name ordinal or string
		pw += 2; // WORDs
	else
		while (*pw++);

	while (*pw++);              // Skip caption string

	if (dwStyle & DS_SETFONT)
	{
		pw += bDialogEx ? 3 : 1;    // Skip font size, weight, (italic, charset)
		while (*pw++);              // Skip font name
	}

	// Dword-align and return
	return (DLGITEMTEMPLATE*)(((DWORD)pw + 3) & ~3);
}

static DLGITEMTEMPLATE* AFXAPI FindNextDlgItem(DLGITEMTEMPLATE* pItem, BOOL bDialogEx)
{
	WORD* pw;

	if (bDialogEx)
		pw = (WORD*)((DLGITEMTEMPLATEEX*)pItem + 1);
	else
		pw = (WORD*)(pItem + 1);

	if (*pw == (WORD)-1)            // Skip class name ordinal or string
		pw += 2; // WORDs
	else
		while (*pw++);

	if (*pw == (WORD)-1)            // Skip text ordinal or string
		pw += 2; // WORDs
	else
		while (*pw++);

	WORD cbExtra = *pw++;           // Skip extra data

	// Dword-align and return
	return (DLGITEMTEMPLATE*)(((DWORD)pw + cbExtra + 3) & ~3);
}

CRect GetItemTemplateRect(UINT nIDTemplate, UINT nIDCtrl) 
{
	return GetItemTemplateRect(MAKEINTRESOURCE(nIDTemplate), nIDCtrl);
}

CRect GetItemTemplateRect(LPCTSTR lpszTemplateName, UINT nIDCtrl) 
{
	CRect rectItemTemplateDLU(0, 0, 0, 0);

	HINSTANCE hInstance = AfxFindResourceHandle(lpszTemplateName, RT_DIALOG);
	HRSRC hDlg = FindResource(hInstance, lpszTemplateName, RT_DIALOG);

	if (hDlg)
	{
		HGLOBAL hResource = LoadResource(hInstance, hDlg);
		if (hResource)
		{
			LPDLGTEMPLATE lpDialogTemplate = (LPDLGTEMPLATE)LockResource(hResource);

			if (lpDialogTemplate)
			{
				BOOL bIsDialogEx = IsDialogEx(lpDialogTemplate);
				LPDLGITEMTEMPLATE lpDialogItemTemplate = FindFirstDlgItem(lpDialogTemplate);
				LPDLGITEMTEMPLATEEX lpDialogItemTemplateEx = (LPDLGITEMTEMPLATEEX)lpDialogItemTemplate;

				if (bIsDialogEx)
				{
					// DIALOGEX resource
					while (lpDialogItemTemplateEx)
					{
						if (lpDialogItemTemplateEx->id == nIDCtrl)
							break;
						lpDialogItemTemplate = FindNextDlgItem(lpDialogItemTemplate, bIsDialogEx);
						lpDialogItemTemplateEx = (LPDLGITEMTEMPLATEEX)lpDialogItemTemplate;
					}
					ASSERT(lpDialogItemTemplateEx->id == nIDCtrl);

					rectItemTemplateDLU.SetRect(lpDialogItemTemplateEx->x, lpDialogItemTemplateEx->y, 
						lpDialogItemTemplateEx->x + lpDialogItemTemplateEx->cx, lpDialogItemTemplateEx->y + lpDialogItemTemplateEx->cy);
				}
				else
				{
					// DIALOG resource
					while (lpDialogItemTemplate)
					{
						if (lpDialogItemTemplate->id == nIDCtrl)
							break;
						lpDialogItemTemplate = FindNextDlgItem(lpDialogItemTemplate, bIsDialogEx);
					}
					ASSERT(lpDialogItemTemplate->id == nIDCtrl);

					rectItemTemplateDLU.SetRect(lpDialogItemTemplate->x, lpDialogItemTemplate->y, 
						lpDialogItemTemplate->x + lpDialogItemTemplate->cx, lpDialogItemTemplate->y + lpDialogItemTemplate->cy);
				}

				UnlockResource(hResource);
			}
			FreeResource(hResource);
		}
	}

	return rectItemTemplateDLU;
}

CRect GetTemplateRect(UINT nIDTemplate) 
{
	return GetTemplateRect(MAKEINTRESOURCE(nIDTemplate));
}

CRect GetTemplateRect(LPCTSTR lpszTemplateName) 
{
	CRect rectTemplateDLU(0, 0, 0, 0);

	HINSTANCE hInstance = AfxFindResourceHandle(lpszTemplateName, RT_DIALOG);
	HRSRC hDlg = FindResource(hInstance, lpszTemplateName, RT_DIALOG);

	if (hDlg)
	{
		HGLOBAL hResource = LoadResource(hInstance, hDlg);
		if (hResource)
		{
			LPDLGTEMPLATE lpDialogTemplate = (LPDLGTEMPLATE)LockResource(hResource);
			LPDLGTEMPLATEEX lpDialogTemplateEx = (LPDLGTEMPLATEEX)lpDialogTemplate;

			if (lpDialogTemplate)
			{
				BOOL bIsDialogEx = IsDialogEx(lpDialogTemplate);

				// Support for DIALOGEX PropertyPages, although those aren't very well supported either
				if (bIsDialogEx)
				{
					// DIALOGEX resource
					rectTemplateDLU.SetRect(lpDialogTemplateEx->x, lpDialogTemplateEx->y, 
						lpDialogTemplateEx->x + lpDialogTemplateEx->cx, lpDialogTemplateEx->y + lpDialogTemplateEx->cy);
				}
				else
				{
					// DIALOG resource
					rectTemplateDLU.SetRect(lpDialogTemplate->x, lpDialogTemplate->y, 
						lpDialogTemplate->x+lpDialogTemplate->cx, lpDialogTemplate->y + lpDialogTemplate->cy);
				}
				UnlockResource(hResource);
			}
			FreeResource(hResource);
		}
	}

	return rectTemplateDLU;
}

int MapDialogUnitX(int nX, CWnd* pWnd)
{
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
	CRect rect(0, 0, nX, 0);
	if (MapDialogRect(pWnd->GetSafeHwnd(), &rect))
		nX = rect.right;
	else
	{
		TRACE("MapDialogUnitX: GetLastError returned %d.\n", GetLastError());

		HDC hDC = GetDC(pWnd->GetSafeHwnd());
		nX = nX*5/3*GetDeviceCaps(hDC, LOGPIXELSX)/96;
		ReleaseDC(pWnd->GetSafeHwnd(), hDC);
	}
	return nX;
}

int MapDialogUnitY(int nY, CWnd* pWnd)
{
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
	CRect rect(0, 0, 0, nY);
	if (MapDialogRect(pWnd->GetSafeHwnd(), &rect))
		nY = rect.bottom;
	else
	{
		TRACE("MapDialogUnitY: GetLastError returned %d.\n", GetLastError());

		HDC hDC = GetDC(pWnd->GetSafeHwnd());
		nY = nY*5/3*GetDeviceCaps(hDC, LOGPIXELSY)/96;
		ReleaseDC(pWnd->GetSafeHwnd(), hDC);
	}
	return nY;
}

CWnd* GetDefaultPushButton(CWnd* pParent)
{
	ASSERT_VALID(pParent);
	CWnd* pChild;
	for (pChild = pParent->GetWindow(GW_CHILD); pChild; pChild = pChild->GetNextWindow(GW_HWNDNEXT))
	{
		if (IsDefaultPushButton(pChild))
		{
			break;
		}
	}
	return pChild;
}

void SetDefaultPushButton(CWnd* pParent, CWnd* pButton, BOOL bForce)
{
	ASSERT_VALID(pParent);
	ASSERT_VALID(pButton);

	if (!pParent->IsChild(pButton) || (!IsPushButton(pButton) && !bForce))
		return;

	for (CWnd* pChild = pParent->GetWindow(GW_CHILD); pChild; pChild = pChild->GetNextWindow(GW_HWNDNEXT))
	{
		if (IsPushButton(pChild))
		{
			int nButtonStyle;

			if (pChild == pButton)
			{
				nButtonStyle = ((CButton*)pChild)->GetButtonStyle();
				((CButton*)pChild)->SetButtonStyle(nButtonStyle | BS_DEFPUSHBUTTON);
			}
			else
			{
				nButtonStyle = ((CButton*)pChild)->GetButtonStyle();
				((CButton*)pChild)->SetButtonStyle(nButtonStyle & ~BS_DEFPUSHBUTTON);
			}
		}
	}
}

void UnsetDefaultPushButton(CWnd* pParent, CWnd* pButton)
{
	ASSERT_VALID(pParent);
	ASSERT_VALID(pButton);

	if (!pParent->IsChild(pButton) || !IsPushButton(pButton))
		return;

	int nButtonStyle;

	nButtonStyle = ((CButton*)pButton)->GetButtonStyle();
	((CButton*)pButton)->SetButtonStyle(nButtonStyle & ~BS_DEFPUSHBUTTON);
}

BOOL IsDefaultPushButton(CWnd* pButton)
{
	ASSERT_VALID(pButton);

	return (pButton->SendMessage(WM_GETDLGCODE) & DLGC_DEFPUSHBUTTON);
}

BOOL IsPushButton(CWnd* pButton)
{
	ASSERT_VALID(pButton);

	return (pButton->SendMessage(WM_GETDLGCODE) & (DLGC_DEFPUSHBUTTON | DLGC_UNDEFPUSHBUTTON));
}

BOOL IsEdit(CWnd* pWnd)
{
	// IsEdit: a helper function to determine if a given CWnd pointer
	// points to a CEdit control.
	// Use the SDK ::GetClassName() function because MFC IsKindOf
	// fails if no CEdit variable has been created for the control you're
	// trying to test.

	ASSERT(pWnd != NULL);
	HWND hWnd = pWnd->GetSafeHwnd();
	if (hWnd == NULL)
		return FALSE;

	TCHAR szClassName[6];
	return (GetClassName(hWnd, szClassName, _countof(szClassName)) && _tcsicmp(szClassName, _T("Edit")) == 0);
}

BOOL IsEnabled(CCmdUI* pCmdUI)
{
	ASSERT(pCmdUI != NULL);
	BOOL bRet = FALSE;

	if (pCmdUI->m_pMenu != NULL)
	{
		bRet = !!(pCmdUI->m_pMenu->GetMenuState(pCmdUI->m_nIndex, MF_BYPOSITION) & MF_ENABLED);
	}
	else if (pCmdUI->m_pOther != NULL)
	{
		CToolBar* pToolBar = NULL;
		if (pCmdUI->m_pOther->IsKindOf(RUNTIME_CLASS(CToolBar)))
			pToolBar = (CToolBar*)pCmdUI->m_pOther;

		bRet = (pToolBar != NULL && pToolBar->GetToolBarCtrl().IsButtonEnabled(pToolBar->GetItemID(pCmdUI->m_nIndex)));
	}
	return bRet;
}

void UpdateDialogControl(UINT nID, CWnd* pParent, BOOL bDisableIfNoHndler)
{
	// This is derived from CWnd::UpdateDialogControls.
	// We only update controls matching nID.
	//
	if (pParent == NULL)
		return;

	ASSERT_VALID(pParent);

	CCmdUI state;
	CWnd wndTemp;       // very temporary window just for CmdUI update
	HWND hWnd = pParent->GetSafeHwnd();

	// walk all the kids - assume the IDs are for buttons
	for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		// send to buttons
		wndTemp.m_hWnd = hWndChild; // quick and dirty attach
		state.m_nID = (UINT)(WORD)GetDlgCtrlID(hWndChild);
		state.m_pOther = &wndTemp;

		// test if nID matches the control
		if (state.m_nID != nID)
			continue;

		// check for reflect handlers in the child window
		CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
		if (pWnd != NULL)
		{
			// call it directly to disable any routing
			if (pWnd->CWnd::OnCmdMsg(0, MAKELONG(-1,
				WM_COMMAND+WM_REFLECT_BASE), &state, NULL))
				continue;
		}

		// check for handlers in the parent window
		if (pParent->OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
			continue;

		// determine whether to disable when no handler exists
		BOOL bDisableTemp = bDisableIfNoHndler;
		if (bDisableTemp)
		{
			if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
			{
				// non-button controls don't get automagically disabled
				bDisableTemp = FALSE;
			}
			else
			{
				// only certain button controls get automagically disabled
				UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
				if (nStyle == (UINT)BS_AUTOCHECKBOX ||
					nStyle == (UINT)BS_AUTO3STATE ||
					nStyle == (UINT)BS_GROUPBOX ||
					nStyle == (UINT)BS_AUTORADIOBUTTON)
				{
					bDisableTemp = FALSE;
				}
			}
		}
		// check for handlers in the target (owner)
		state.DoUpdate(pParent, bDisableTemp);
	}
	wndTemp.m_hWnd = NULL;      // quick and dirty detach
}

void CheckButtonSpecial(CButton& wndButton, int& value, int strID, BOOL bSaveAndValidate) 
{
	CString strText;

	if (bSaveAndValidate)
	{
		CString strCompare;

		wndButton.GetWindowText(strText);
		for (value = 0; value < 2; value++)
		{
			strCompare.LoadString(strID + value);
			if (strText == strCompare)
				break;
		}
		if (value > 2)
		{
			TRACE1("Warning: dialog data checkbox value (%d) out of range.\n",
				 value);
			value = 0;  // default to off
		}
	}
	else
	{
		if (value < 0 || value > 2)
		{
			TRACE1("Warning: dialog data checkbox value (%d) out of range.\n",
				 value);
			value = 0;  // default to off
		}

		if (value == 2)
			wndButton.SetCheck(1);
		else if (value == 1)
			wndButton.SetCheck(1);
		else
			wndButton.SetCheck(0);
		strText.LoadString(strID + value);
		wndButton.SetWindowText(strText);
	}
}

BOOL TestDockState(LPCTSTR lpszProfileName, CFrameWnd* pFrame)
{
	CDockState state;
	state.LoadState(lpszProfileName);

	int k, size = state.m_arrBarInfo.GetSize();
	for (k = 0; k < size; k++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[k];
		ASSERT(pInfo != NULL);

		if (pFrame->GetControlBar(pInfo->m_nBarID) == NULL) //toolbar id's probably changed
			return FALSE;
	}
	return TRUE;
}

/*BOOL ReadRecordFromStream(FILE*& stream, CString& str)
{
 	char ch, item[256];

	for (int k = 0; (ch = (char)fgetc(stream)) != EOF && k < 256; k++)
	{
		item[k] = ch;
		if (k > 0 && item[k] == '\n' && item[k-1] == '\r')
		{
			k--;
			break;
		}
	}

	LPTSTR pBuffer = str.GetBuffer(k);
	memcpy(pBuffer, item, k);
	str.ReleaseBuffer(k);

	return (k > 0);
}*/

BOOL ReadRecordFromStream(FILE*& stream, CString& str)
{
	// The file must be opened in binary mode.
	// start delimiter: none
	// end delimiters: _T("\r\n") _TEOF

 	TCHAR cCurr, cPrev = _T('\0');
	int size = 0;

	fpos_t pos = 0;
	VERIFY(fgetpos(stream, &pos) == 0);

	while ((cCurr = (TCHAR)_fgettc(stream)) != _TEOF)
	{
		if (cCurr == _T('\n') && cPrev == _T('\r'))
		{
			size--;
			break;
		}
		size++;
		cPrev = cCurr;
	}

	VERIFY(fsetpos(stream, &pos) == 0);

	LPTSTR pBuffer = str.GetBuffer(size);
	for (int k = 0; k < size; k++)
	{
		pBuffer[k] = (TCHAR)_fgettc(stream);
	}
	str.ReleaseBuffer(size);

	cCurr = (TCHAR)_fgettc(stream);
	ASSERT(cCurr == _T('\r') || cCurr == _TEOF);
	cCurr = (TCHAR)_fgettc(stream);
	ASSERT(cCurr == _T('\n') || cCurr == _TEOF);

	return (size > 0);
}

/*BOOL ReadQuotedStringFromStream(FILE*& stream, CString& str)
{
	char ch, item[256];
	int k = 0;
	item[k] = '\0';

	while ((ch = (char)fgetc(stream)) != EOF)
	{
		if (ch == '\"')
		{
			break;
		}
	}
	for (k = 0; (ch = (char)fgetc(stream)) != EOF && k < 256; k++)
	{
		if (ch == '\"' || k == 255)
		{
			item[k] = '\0';
			break;
		}
		item[k] = ch;
	}
	str = item;
	return (ch != EOF);
}*/

/*BOOL ReadQuotedStringFromStream(FILE*& stream, CString& str)
{
	// Note: This works not properly.
	//	It is only correct if there are nonempty strings
	//	before and between the quotes.

	int nRet = fscanf(stream, "%*[^\"]%*[\"]%[^\"]%*[\"]", str.GetBuffer(256));
	str.ReleaseBuffer();

	return (nRet != 0);
}*/

BOOL ReadQuotedStringFromStream(FILE*& stream, CString& str)
{
	// The file can be opened in text or in binary mode.
	// start delimiter: _T("\"")
	// end delimites: _T("\"") _T("\0") _TEOF

	TCHAR cCurr;
	int size = 0;

	while ((cCurr = (TCHAR)_fgettc(stream)) != _TEOF)
	{
		if (cCurr == _T('\"'))
		{
			break;
		}
	}

	fpos_t pos = 0;
	VERIFY(fgetpos(stream, &pos) == 0);

	while ((cCurr = (TCHAR)_fgettc(stream)) != _TEOF)
	{
		if (cCurr == _T('\"') || cCurr == _T('\0'))
		{
			break;
		}
		size++;
	}

	VERIFY(fsetpos(stream, &pos) == 0);

	LPTSTR pBuffer = str.GetBuffer(size);
	for (int k = 0; k < size; k++)
	{
		pBuffer[k] = (TCHAR)_fgettc(stream);
	}
	str.ReleaseBuffer(size);

	cCurr = (TCHAR)_fgettc(stream);
	ASSERT(cCurr == _T('\"') || cCurr == _T('\0') || cCurr == _TEOF);

	return (size > 0);
}

/*BOOL ReadLineFromStream(FILE*& stream, CString& str)
{
	char *ch, item[256];

	if ((ch = fgets(item, 256, stream)) != NULL)
	{
		// remove '\n' (last character) from "item"
		if (item[strlen(item)-1] == '\n')
			item[strlen(item)-1] = '\0';
		str = item;
	}
	return (ch != NULL);
}*/

/*BOOL ReadLineFromStream(FILE*& stream, CString& str)
{
	// Note: This works not properly.
	//	It is only correct if there is a nonempty string
	//	before the newline.

	int nRet = fscanf(stream, "%[^\n]%*[\n]", str.GetBuffer(256));
	str.ReleaseBuffer();

	return (nRet != 0);
}*/

BOOL ReadLineFromStream(FILE*& stream, CString& str)
{
	// The file may be opened in text mode or in binary mode now.
	// start delimiter: none
	// end delimiters: _T("\n") _T("\0") _TEOF

 	TCHAR cCurr = _T('\0');
	int size = 0;
	int fd = _fileno(stream);

	fpos_t pos = 0;
	VERIFY(fgetpos(stream, &pos) == 0);

	while ((cCurr = (TCHAR)_fgettc(stream)) != _TEOF)
	{
		if (cCurr == _T('\n') || cCurr == _T('\0'))
		{
			break;
		}
		size++;
	}

	VERIFY(fsetpos(stream, &pos) == 0);

	LPTSTR pBuffer = str.GetBuffer(size);
	for (int k = 0; k < size; k++)
	{
		pBuffer[k] = (TCHAR)_fgettc(stream);
	}
	if (!(_osfile(fd) & FTEXT) && size > 0 && pBuffer[size-1] == _T('\r'))
	{
		// skip '\r' when file is opened in text mode
		pBuffer[size-1] = _T('\0');
		size--;
	}
	str.ReleaseBuffer(size);

	cCurr = (TCHAR)_fgettc(stream);
	ASSERT(cCurr == _T('\n') || cCurr == _T('\0') || cCurr == _TEOF);

	return (size > 0);
}

int AFX_CDECL fprintf_CommaInsteadOfDot(FILE* stream, LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
#if _MSC_VER < 1200
	CStringEx formatStr;
#else
	CString formatStr;
#endif // _MSC_VER < 1200

	va_list argList;
	va_start(argList, lpszFormat);
	formatStr.FormatV(lpszFormat, argList);
	va_end(argList);

	formatStr.Replace('.', ',');

	return fprintf(stream, formatStr);
}

/////////////////////////////////////////////////////////////////////////////
// To be used by Installshield Free Edition
/////////////////////////////////////////////////////////////////////////////
LONG APIENTRY LaunchApp(HWND hWnd, LPLONG lpnValue, LPSTR lpszValue)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lpnValue);

	BOOL bRet;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	bRet = CreateProcess(NULL, lpszValue,
		NULL, NULL,
		FALSE, 0,
		NULL, NULL,
		&si, &pi);
	if (bRet)
	{
		VERIFY(CloseHandle(pi.hProcess));
		VERIFY(CloseHandle(pi.hThread));
	}

	return (LONG)bRet;
}
