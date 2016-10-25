/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// MathAdd.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHADD_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_MATHADD_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Mathematische Funktionen

AFX_EXT_API_GENERAL double epsilon();
AFX_EXT_API_GENERAL double round(const double& inValue);
AFX_EXT_API_GENERAL double sign(const double& inValue);
AFX_EXT_API_GENERAL double fix(const double& inValue);
AFX_EXT_API_GENERAL double frem(const double& inValue1, const double& inValue2);
AFX_EXT_API_GENERAL double arcreset(const double& inValue1, const double& inValue2);
AFX_EXT_API_GENERAL int _isinf(double inValue);
AFX_EXT_API_GENERAL void roundres(double& inValue1, double& inValue2);
AFX_EXT_API_GENERAL void roundres(double& inValue);
AFX_EXT_API_GENERAL void crossprod(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z, double& c_x, double& c_y, double& c_z);
AFX_EXT_API_GENERAL double scalprod3(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_MATHADD_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
