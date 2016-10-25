/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: August 1998
//
/////////////////////////////////////////////////////////////////////////////
// StringEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGEX_H__E222F1E1_2D45_11D2_8A55_00E0290F649D__INCLUDED_)
#define AFX_STRINGEX_H__E222F1E1_2D45_11D2_8A55_00E0290F649D__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if _MSC_VER < 1200
class AFX_EXT_CLASS_GENERAL CStringEx : public CString
{
// Konstruktion
public:
	CStringEx();

// Attribute
public:

	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);

// Operationen
public:
	void FormatV(LPCTSTR lpszFormat, va_list argList);

// Implementierung
public:
	virtual ~CStringEx();
};
#endif // _MSC_VER < 1200

/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API_GENERAL void AFX_CDECL mprintf(LPCTSTR lpszFormat, ...);
AFX_EXT_API_GENERAL UINT CharCount(LPCTSTR lpsz, TCHAR ch);
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STRINGEX_H__E222F1E1_2D45_11D2_8A55_00E0290F649D__INCLUDED_)
