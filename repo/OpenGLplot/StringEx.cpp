/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 1998
//
/////////////////////////////////////////////////////////////////////////////
// StringEx.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _MSC_VER < 1200

/////////////////////////////////////////////////////////////////////////////
// CStringEx

CStringEx::CStringEx()
{
}

CStringEx::~CStringEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// CStringEx Kommandos
void CStringEx::FormatV(LPCTSTR lpszFormat, va_list argList)
{
	CString::FormatV(lpszFormat, argList);
}

int CStringEx::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;

	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		LPTSTR psz = m_pchData;
		LPTSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = _tcsinc(psz);
		}
	}
	return nCount;
}

int CStringEx::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	// can't have empty or NULL lpszOld

	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	LPTSTR lpszStart = m_pchData;
	LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPTSTR lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += lstrlen(lpszStart) + 1;
	}

	// if any changes were made, make them
	if (nCount > 0)
	{
		CopyBeforeWrite();

		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen-nSourceLen)*nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, pOldData->nDataLength*sizeof(TCHAR));
			CString::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;

		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
					nBalance * sizeof(TCHAR));
				memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}
		ASSERT(m_pchData[nNewLength] == '\0');
		GetData()->nDataLength = nNewLength;
	}

	return nCount;
}
#endif // _MSC_VER < 1200

/////////////////////////////////////////////////////////////////////////////
// Globale Funktionen
void AFX_CDECL mprintf(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
#if _MSC_VER < 1200
	CStringEx msg;
#else
	CString msg;
#endif // _MSC_VER < 1200

	va_list argList;
	va_start(argList, lpszFormat);
	msg.FormatV(lpszFormat, argList);
	va_end(argList);

	AfxMessageBox(msg);
}

UINT CharCount(LPCTSTR lpsz, TCHAR ch)
{
	UINT nCount = 0;

	while (*lpsz != '\0')
	{
		if (*lpsz == ch)
			nCount++;
		lpsz = _tcsinc(lpsz);
	}

	return nCount;
}
