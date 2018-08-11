/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// StdAfx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__06A3562B_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_)
#define AFX_STDAFX_H__06A3562B_72E5_11D1_A6C2_00A0242C0A32__INCLUDED_

#define WINVER 0x0A01
#define _WIN32_IE 0xA01




#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#define _CRT_SECURE_NO_WARNINGS

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxtempl.h>       // Template support
#include <afxpriv.h>		// to see the CPreviewDC structure
#include <afxcview.h>		// to see the CListView class
#include <float.h>			// to see FLT_DIG/DBL_DIG
#include <process.h>		// to see _beginthreadex

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>        // MFC OLE-Automatisierungsklassen
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

/////////////////////////////////////////////////////////////////////////////

#ifndef AFX_EXT_CLASS_GENERAL
	#ifdef _AFXEXT
		#ifdef _GENERALDLL
			#define AFX_EXT_CLASS_GENERAL	AFX_CLASS_EXPORT
		#else
			#define AFX_EXT_CLASS_GENERAL	AFX_CLASS_IMPORT
		#endif
	#else
		#define AFX_EXT_CLASS_GENERAL		AFX_CLASS_IMPORT
	#endif
#endif
#ifndef AFX_EXT_API_GENERAL
	#ifdef _AFXEXT
		#ifdef _GENERALDLL
			#define AFX_EXT_API_GENERAL		AFX_API_EXPORT
		#else
			#define AFX_EXT_API_GENERAL		AFX_API_IMPORT
		#endif
	#else
		#define AFX_EXT_API_GENERAL			AFX_API_IMPORT
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////

//#include <math.h>

#include <gl\gl.h>
#include <gl\glu.h>

/////////////////////////////////////////////////////////////////////////////
// This block is a copy from the MFC source. It contains
//	special exception handling just for MFC library implementation
#ifndef _AFX_OLD_EXCEPTIONS

// MFC does not rely on auto-delete semantics of the TRY..CATCH macros,
//  therefore those macros are mapped to something closer to the native
//  C++ exception handling mechanism when building MFC itself.

#undef TRY
#define TRY { try {

#undef CATCH
#define CATCH(class, e) } catch (class* e) \
	{ ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); UNUSED(e);

#undef AND_CATCH
#define AND_CATCH(class, e) } catch (class* e) \
	{ ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); UNUSED(e);

#undef CATCH_ALL
#define CATCH_ALL(e) } catch (CException* e) \
	{ { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); UNUSED(e);

#undef AND_CATCH_ALL
#define AND_CATCH_ALL(e) } catch (CException* e) \
	{ { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); UNUSED(e);

#undef END_TRY
#define END_TRY } catch (CException* e) \
	{ ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); e->Delete(); } }

#undef THROW_LAST
#define THROW_LAST() throw

// Because of the above definitions of TRY...CATCH it is necessary to
//  explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)
#define NO_CPP_EXCEPTION(expr)

#else   //!_AFX_OLD_EXCEPTIONS

// In this case, the TRY..CATCH macros provide auto-delete semantics, so
//  it is not necessary to explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e)
#define NO_CPP_EXCEPTION(expr) expr

#endif  //_AFX_OLD_EXCEPTIONS


#ifndef _countof

// _countof is defined in <afximpl.h> which is not included in the WinNT
//  MSVC installation. So we define it here.

// determine number of elements in an array (not bytes)
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#endif  //_countof

// _osfile is defined in <internal.h> which is not included in the WinNT
//  MSVC installation. So we define it here.

#ifndef FTEXT
#define FTEXT				0x80	// file handle is in text mode
#endif  //FTEXT

#ifndef IOINFO_L2E
#define IOINFO_L2E			5
#endif  //IOINFO_L2E

#ifndef IOINFO_ARRAY_ELTS
#define IOINFO_ARRAY_ELTS	(1 << IOINFO_L2E)
#endif  //IOINFO_ARRAY_ELTS

/**/
#ifndef ioinfo
typedef struct {
        long osfhnd;	// underlying OS file HANDLE
        char osfile;	// attributes of file (e.g., open in text mode?)
        char pipech;	// one char buffer for handles opened on pipes
#ifdef _MT
        int lockinitflag;
        CRITICAL_SECTION lock;
#endif  /* _MT */
    }   ioinfo;
#endif  //ioinfo

#ifndef _pioinfo
//ioinfo* __pioinfo[];
#define _pioinfo(i) ( __pioinfo[(i) >> IOINFO_L2E] + ((i) & (IOINFO_ARRAY_ELTS - 1)) )
extern "C" AFX_DATA_IMPORT ioinfo* __pioinfo[];
#endif  //_pioinfo

#ifndef _osfile
#define _osfile(i)  ( _pioinfo(i)->osfile )
#endif  //_osfile


#ifndef DLGTEMPLATEEX

// DLGTEMPLATEEX is defined in <afximpl.h> which is not included in the WinNT
//  MSVC installation. So we define it here.
typedef struct
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;
typedef DLGTEMPLATEEX *LPDLGTEMPLATEEXA;
typedef DLGTEMPLATEEX *LPDLGTEMPLATEEXW;
#ifdef UNICODE
typedef LPDLGTEMPLATEEXW LPDLGTEMPLATEEX;
#else
typedef LPDLGTEMPLATEEXA LPDLGTEMPLATEEX;
#endif // UNICODE

#endif  //DLGTEMPLATEEX

#ifndef DLGITEMTEMPLATEEX

// DLGITEMTEMPLATEEX is defined in <afximpl.h> which is not included in the WinNT
//  MSVC installation. So we define it here.
typedef struct
{
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	short x;
	short y;
	short cx;
	short cy;
	DWORD id;
} DLGITEMTEMPLATEEX;
typedef DLGITEMTEMPLATEEX *LPDLGITEMTEMPLATEEXA;
typedef DLGITEMTEMPLATEEX *LPDLGITEMTEMPLATEEXW;
#ifdef UNICODE
typedef LPDLGITEMTEMPLATEEXW LPDLGITEMTEMPLATEEX;
#else
typedef LPDLGITEMTEMPLATEEXA LPDLGITEMTEMPLATEEX;
#endif // UNICODE

#endif  //DLGITEMTEMPLATEEX

/////////////////////////////////////////////////////////////////////////////

#include "MathAdd.h"		// epsilon() cannot be declared here
#include "StringEx.h"		// add mprintf to global namespace

/////////////////////////////////////////////////////////////////////////////
// Globale Variablen

const double QNAN = fmod(0., 0.);
const double pi = 4. * atan(1.);
const double eps = mymathfuncs::epsilon();

const double eps0 = sqrt(eps);
const double eps1 = 100. * eps0;
const double eps2 = 5000. * eps0;
const double eps3 = 10000. * eps0;
const double eps4 = 15000. * eps0;
const double eps5 = 20000. * eps0;
const double eps6 = 25000. * eps0;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STDAFX_H__37EC43EF_7E06_11D2_8A55_00E0290F649D__INCLUDED_)
