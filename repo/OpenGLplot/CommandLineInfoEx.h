/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: June 2001
//
/////////////////////////////////////////////////////////////////////////////
// CommandLineInfoEx.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDLINEINFOEX_H__A51AA2D3_6553_11D5_8D45_00001CD5E4D1__INCLUDED_)
#define AFX_COMMANDLINEINFOEX_H__A51AA2D3_6553_11D5_8D45_00001CD5E4D1__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif



/////////////////////////////////////////////////////////////////////////////
// CCommandLineInfoEx 

class AFX_EXT_CLASS_GENERAL CCommandLineInfoEx : public CCommandLineInfo
{
public:
	CCommandLineInfoEx();

	//plain char* version on UNICODE for source-code backwards compatibility
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
#ifdef _UNICODE
	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);
#endif

	virtual ~CCommandLineInfoEx();

// Implementation
protected:
	void ParseParamFlagEx(const char* pszParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_COMMANDLINEINFOEX_H__A51AA2D3_6553_11D5_8D45_00001CD5E4D1__INCLUDED_
