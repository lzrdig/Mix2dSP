//***************************************************************************
//
// Class CFields
//
// List of member functions:
//
//  * fSetField(int type, int num, float val) 
//
//        This function will set the array element 'num' for the variable 
//        specified in the 'type' argument to a 'val' value of type float.
//
//                                           
//     Example:  pField->SetField(FREQ,5,0.79) will set the 5th array
//                         element of the 'Freq' array to the value 0.79  
//
//
//  * dSetField(int type, int num, double val) 
//
//         Same as above, only for type double 'val' (only used by the
//         Noisefield array)
//
//
//     Example:  pField->SetField(NOISEFIELD,jj,12.4) sets the jjth 
//                               element of the 'Noisefield' array to 12.4
//
//
//  * dGetField(int type, int num) 
//
//         This function returns the value of the 'type' array for the 
//         'num' array element (only used by the Noisefield array)
//
//
//     Example:  pField->dGetField(NOISEFIELD,m) returns the mth element
//                                of the 'Noisefield' array
//
//
//  * fGetField(int type, int num) 
//
//         Same as above, only for type float 'val'
//
//
//     Example:  pField->fGetField(TERMPUMP,kk) returns the kkth element
//                         of the 'Termpump' array
//
//
//	(Note: for a complete list of the 'type' arguments, see 'Fields.h')
//
//***************************************************************************       


// Pre-compiler instructions
#include "StdAfx.h"
#include "DecIncl.h"
#include "Fields.h"

#pragma warning(disable:4305)
#pragma warning(disable:4100)


extern "C" float *vector(long nl, long nh);
extern "C" void free_vector(float *v, long nl, long nh);
extern "C" double *dvector(long nl, long nh);
extern "C" void free_dvector(double *v, long nl, long nh);

//***************************************************************************
// Class constructor (initializes all arrays with zero elements)

CFields::CFields(long gridtot, long xtot, long ytot, long ttot)
{

	// Supergaussian coefficients
	pSgcoeff = vector(1,5);

	pSgcoeff[1] = 0.5;
	pSgcoeff[2] = 0.4431;
	pSgcoeff[3] = 0.4465;
	pSgcoeff[4] = 0.4532;
	pSgcoeff[5] = 0.4591;

	// 3d arrays
	maxnn = gridtot;

	// complex arrays
	pEField_s = vector(1,2*xtot*(ytot/2)*ttot);
	pEField_i = vector(1,2*xtot*(ytot/2)*ttot);
	pEField_p = vector(1,2*xtot*(ytot/2)*ttot);

	pPhase_s = vector(1,2*maxnn);
	pPhase_i = vector(1,2*maxnn);
	pPhase_p = vector(1,2*maxnn);

	// real arrays
	pPhaseTerm_s = vector(1,maxnn);
	pPhaseTerm_i = vector(1,maxnn);
	pPhaseTerm_p = vector(1,maxnn);

	// x arrays
	maxx = xtot;
	pXgrid = vector(1,maxx);

	// y arrays
	maxy = ytot;
	pYgrid = vector(1,maxy);

	// t arrays
	maxt = ttot;
	pTgrid = vector(1,maxt);

	// FFT arrays
	pFFTW = (fftw_complex *) malloc(xtot*ytot*ttot*sizeof(fftw_complex));
	pFarFFT_x = (fftw_complex *) malloc(xtot*sizeof(fftw_complex));
	pFarFFT_y = (fftw_complex *) malloc(ytot*sizeof(fftw_complex));

	// Expansion factor for smoothing the spectra by 0 buffering
	XFactor = 4;
	pSpecFFT = (fftw_complex *) malloc(XFactor*ttot*sizeof(fftw_complex));
	
}


//***************************************************************************
// Class destructor (deletes all of the arrays and sets them to zero)

CFields::~CFields()
{

	free_vector(pSgcoeff,1,5);
	
	// 3d fields
	free_vector(pEField_s,1,2*maxx*(maxy/2)*maxt);
	free_vector(pEField_i,1,2*maxx*(maxy/2)*maxt);
	free_vector(pEField_p,1,2*maxx*(maxy/2)*maxt);

	free_vector(pPhase_s,1,2*maxnn);
	free_vector(pPhase_i,1,2*maxnn);
	free_vector(pPhase_p,1,2*maxnn);

	free_vector(pPhaseTerm_s,1,maxnn);
	free_vector(pPhaseTerm_i,1,maxnn);
	free_vector(pPhaseTerm_p,1,maxnn);

	free_vector(pXgrid,1,maxx);
	free_vector(pYgrid,1,maxy);
	free_vector(pTgrid,1,maxt);

	free(pFFTW);
	free(pSpecFFT);
	free(pFarFFT_x);
	free(pFarFFT_y);

	
}


//**********************************************************************
// Function fSetField

void CFields::fSetField(int type, int num, float val)
{
	switch(type)
	{
	case SGCOEFF:
		pSgcoeff[num] = val;
		break;
	case PHASETERM_S:
		pPhaseTerm_s[num] = val;
		break;
	case PHASETERM_I:
		pPhaseTerm_i[num] = val;
		break;
	case PHASETERM_P:
		pPhaseTerm_p[num] = val;
		break;
	case XGRID:
		pXgrid[num] = val;
		break;
	case YGRID:
		pYgrid[num] = val;
		break;
	case TGRID:
		pTgrid[num] = val;
		break;
	default:
		break;
	}

}


//**********************************************************************
// Function fGetField

float CFields::fGetField(int type, int num)
{
	switch(type)
	{
	case SGCOEFF:
		return pSgcoeff[num];
		break;
	case PHASETERM_S:
		return pPhaseTerm_s[num];
		break;
	case PHASETERM_I:
		return pPhaseTerm_i[num];
		break;
	case PHASETERM_P:
		return pPhaseTerm_p[num];
		break;
	case XGRID:
		return pXgrid[num];
		break;
	case YGRID:
		return pYgrid[num];
		break;
	case TGRID:
		return pTgrid[num];
		break;
	default:
		return 0.0;
		break;
	}

}


//**********************************************************************
// Function dSetField

void CFields::dSetField(int type, int num, double val)
{
/*	switch(type)
	{
	case EFIELD_S:
		pEField_s[num] = val;
		break;
	case EFIELD_I:
		pEField_i[num] = val;
		break;
	case EFIELD_P:
		pEField_p[num] = val;
		break;
	case PHASE_S:
		pPhase_s[num] = val;
		break;
	case PHASE_I:
		pPhase_i[num] = val;
		break;
	case PHASE_P:
		pPhase_p[num] = val;
		break;
	default:
		break;
	}
*/
}

//**********************************************************************
// Function dGetField

double CFields::dGetField(int type, int num)
{
/*	switch(type)
	{
	case EFIELD_S:
		return pEField_s[num];
		break;
	case EFIELD_I:
		return pEField_i[num];
		break;
	case EFIELD_P:
		return pEField_p[num];
		break;
	case PHASE_S:
		return pPhase_s[num];
		break;
	case PHASE_I:
		return pPhase_i[num];
		break;
	case PHASE_P:
		return pPhase_p[num];
		break;
	default:
		return 0.0;
		break;
	}
*/
	return 0.0;
}

