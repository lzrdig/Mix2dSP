/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// MathAdd.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "MathAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
namespace mymathfuncs
{
	/////////////////////////////////////////////////////////////////////////////
	// Mathematische Funktionen
	double epsilon()
	{
		double ret = 1.;
		while (ret+1. != 1.)
			ret = ret/2.;
		return 2.*ret;
	}

	double round(const double& inValue)
	{
		double xShift = inValue - .5;
		double nShift = fix(xShift);
		if (xShift == nShift)
		{
			if (nShift == 2.*fix(nShift/2.))
				xShift -= .25;		// nShift even, round off
			else
				xShift += .25;		// nShift odd, round up
		}
		return ceil(xShift);
	}

	double sign(const double& inValue)
	{
		if (_isnan(inValue))
			return 1.;
		else if (inValue > 0.)
			return 1.;
		else if (inValue < 0.)
			return -1.;
		else
			return 0.;
	}

	double fix(const double& inValue)
	{
		return (inValue >= 0.) ? floor(inValue) : ceil(inValue);
	}

	double frem(const double& inValue1, const double& inValue2)
	{
		if (inValue2 == 0.)
			return QNAN;
		else
			return inValue1-floor(inValue1/inValue2)*inValue2;
	}

	double arcreset(const double& inValue1, const double& inValue2)
	{
		//arcreset   reset "inValue1" modulo "inValue2" to the intervall 
		//           [-inValue2/2.,inValue2/2.].
		//           f.e. is  cos(inValue1) == cos(arcreset(inValue1, 2.*pi)) .
		if (inValue1 == 0.)
			return inValue1;
		double retValue = fabs(inValue1) + inValue2/2.;
		retValue -= ceil(retValue/inValue2)*inValue2;
		return sign(inValue1)*(retValue + inValue2/2.);
	}

	int _isinf(double inValue)
	{
		return (!_finite(inValue) && !_isnan(inValue));
	}

	void roundres(double& inValue1, double& inValue2)
	{
		// "round" inValue1 and inValue2 such that at most two 
		// decimal places of the differenz of precision (max-min) keep.
		// "100." is related to 10^2 == 100, i.e. 2 decimal places 
		double delta = min(1., fabs(inValue1-inValue2));
		delta = fabs(delta) + 1. - sign(delta);
		double fac = pow(10., floor(log10(delta)))/100.;

		inValue1 = fac*round(inValue1/fac);
		inValue2 = fac*round(inValue2/fac);
	}

	void roundres(double& inValue)
	{
		// "round" inValue such that at most two 
		// decimal places of the precision keep.
		// "100." is related to 10^2 == 100, i.e. 2 decimal places 
		double delta = min(1., fabs(inValue));
		delta = fabs(delta) + 1. - sign(delta);
		double fac = pow(10., floor(log10(delta)))/100.;

		inValue = fac*round(inValue/fac);
	}

	void crossprod(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z, double& c_x, double& c_y, double& c_z)
	{
		c_x = a_y * b_z - a_z * b_y;
		c_y = a_z * b_x - a_x * b_z;
		c_z = a_x * b_y - a_y * b_x;
	}

	double scalprod3(double a_x, double a_y, double a_z, double b_x, double b_y, double b_z)
	{
		return a_x * b_x + a_y * b_y + a_z * b_z;
	}

}
