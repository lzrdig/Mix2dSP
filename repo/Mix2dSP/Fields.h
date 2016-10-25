
//***************************************************************************
//
// Fields.h - Header file for class CFields
//
//***************************************************************************


//Pre-compiler instructions
#ifndef FIELDS_H


//***************************************************************************
// List of 'type' arguments for CFields

// 3d arrays
const int EFIELD_S = 10078;
const int EFIELD_I = 10079;
const int EFIELD_P = 10080;
const int PHASE_S = 50000;
const int PHASE_I = 50001;
const int PHASE_P = 50002;
const int PHASETERM_S = 50003;
const int PHASETERM_I = 50004;
const int PHASETERM_P = 50005;
const int XGRID = 50006;
const int YGRID = 50007;
const int TGRID = 50008;
const int SGCOEFF = 50009;

#include "..\fftw2dll\fftw\Fftw.h"

extern "C" float *vector(long nl, long nh);
extern "C" void free_vector(float *v, long nl, long nh);

//***************************************************************************
// Definition of class CFields

class CFields 
{
public:

// Class constructor
	CFields(long gridtot, long xtot, long ytot, long ttot); 

// Class destructor
	~CFields();

// Member functions
	void fSetField(int type, int num, float val);
	float fGetField(int type, int num);
	void dSetField(int type, int num, double val);
	double dGetField(int type, int num);
		

// Class variables
private:
	
	long maxx;	// total number of x points
	long maxy;	// total number of y points
	long maxt;	// total number of t points
	long maxnn;	// total 3d array size
	
public:
	// 3d arrays
	float *pEField_s;
	float *pEField_i;
	float *pEField_p;

	float *pPhase_s;
	float *pPhase_i;
	float *pPhase_p;

	float *pPhaseTerm_s;
	float *pPhaseTerm_i;
	float *pPhaseTerm_p;

	float *pXgrid;
	float *pYgrid;
	float *pTgrid;

	float *pSgcoeff;

	int XFactor;

	fftw_complex *pFFTW;

	fftw_complex *pSpecFFT;

	fftw_complex *pFarFFT_x;
	fftw_complex *pFarFFT_y;
};	


#define FIELDS_H
#endif