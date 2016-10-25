/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// DdxDdvSupport.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDXDDVSUPPORT_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_DDXDDVSUPPORT_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Helper functions

#ifdef _DEBUG

AFX_EXT_API_GENERAL void AFXAPI AfxTrace(UINT nIDPrompt);

#else   // _DEBUG

inline AFX_EXT_API_GENERAL void AFXAPI AfxTrace(UINT nIDPrompt) { UNREFERENCED_PARAMETER(nIDPrompt); }

#endif // !_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DDX/DDV support

// simple text operations (NoMessageBox)
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, BYTE& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, short& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, int& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, UINT& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, long& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, DWORD& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, CString& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, float& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, double& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, COleCurrency& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextNMB(CDataExchange* pDX, int nIDC, COleDateTime& value);

// simple text operations (Comma)
AFX_EXT_API_GENERAL void AFXAPI DDX_TextComma(CDataExchange* pDX, int nIDC, float& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextComma(CDataExchange* pDX, int nIDC, double& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextComma2(CDataExchange* pDX, int nIDC, double& value);

// simple text operations (Comma,NoMessageBox)
AFX_EXT_API_GENERAL void AFXAPI DDX_TextCommaNMB(CDataExchange* pDX, int nIDC, float& value);
AFX_EXT_API_GENERAL void AFXAPI DDX_TextCommaNMB(CDataExchange* pDX, int nIDC, double& value);

// valid range including border
AFX_EXT_API_GENERAL void AFXAPI DDV_MinByte(CDataExchange* pDX, BYTE value, BYTE minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxByte(CDataExchange* pDX, BYTE value, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinShort(CDataExchange* pDX, short value, short minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxShort(CDataExchange* pDX, short value, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinInt(CDataExchange* pDX, int value, int minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxInt(CDataExchange* pDX, int value, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinLong(CDataExchange* pDX, long value, long minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxLong(CDataExchange* pDX, long value, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinUInt(CDataExchange* pDX, UINT value, UINT minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxUInt(CDataExchange* pDX, UINT value, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinDWord(CDataExchange* pDX, DWORD value, DWORD minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxDWord(CDataExchange* pDX, DWORD value, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinFloat(CDataExchange* pDX, float const& value, float minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxFloat(CDataExchange* pDX, float const& value, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinDouble(CDataExchange* pDX, double const& value, double minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxDouble(CDataExchange* pDX, double const& value, double maxVal);

// valid range excluding border (NoBorder)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinByteNB(CDataExchange* pDX, BYTE value, BYTE minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxByteNB(CDataExchange* pDX, BYTE value, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinShortNB(CDataExchange* pDX, short value, short minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxShortNB(CDataExchange* pDX, short value, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinIntNB(CDataExchange* pDX, int value, int minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxIntNB(CDataExchange* pDX, int value, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinLongNB(CDataExchange* pDX, long value, long minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxLongNB(CDataExchange* pDX, long value, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinUIntNB(CDataExchange* pDX, UINT value, UINT minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxUIntNB(CDataExchange* pDX, UINT value, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinDWordNB(CDataExchange* pDX, DWORD value, DWORD minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxDWordNB(CDataExchange* pDX, DWORD value, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinFloatNB(CDataExchange* pDX, float const& value, float minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxFloatNB(CDataExchange* pDX, float const& value, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinDoubleNB(CDataExchange* pDX, double const& value, double minVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MaxDoubleNB(CDataExchange* pDX, double const& value, double maxVal);

AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNB(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNB(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNB(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NoLeftBorder)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNLB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNLB(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNLB(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNLB(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNLB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNLB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNLB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNLB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NoRightBorder)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNRB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNRB(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNRB(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNRB(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNRB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNRB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNRB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNRB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding single value (NotEqual)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNE(CDataExchange* pDX, BYTE value, BYTE val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNE(CDataExchange* pDX, short value, short val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNE(CDataExchange* pDX, int value, int val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNE(CDataExchange* pDX, long value, long val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNE(CDataExchange* pDX, UINT value, UINT val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNE(CDataExchange* pDX, DWORD value, DWORD val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNE(CDataExchange* pDX, float const& value, float val);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNE(CDataExchange* pDX, double const& value, double val);

// valid range excluding border (NotIn)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNI(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNI(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNI(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNI(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNI(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNI(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNI(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNI(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NotIn) (NoBorder)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNINB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNINB(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNINB(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNINB(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNINB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNINB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNINB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNINB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NotIn) (NoLeftBorder)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNINLB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNINLB(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNINLB(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNINLB(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNINLB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNINLB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNINLB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNINLB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// valid range excluding border (NotIn) (NoRightBorder)
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxByteNINRB(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxShortNINRB(CDataExchange* pDX, short value, short minVal, short maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxIntNINRB(CDataExchange* pDX, int value, int minVal, int maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxLongNINRB(CDataExchange* pDX, long value, long minVal, long maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxUIntNINRB(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDWordNINRB(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxFloatNINRB(CDataExchange* pDX, float const& value, float minVal, float maxVal);
AFX_EXT_API_GENERAL void AFXAPI DDV_MinMaxDoubleNINRB(CDataExchange* pDX, double const& value, double minVal, double maxVal);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DDXDDVSUPPORT_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
