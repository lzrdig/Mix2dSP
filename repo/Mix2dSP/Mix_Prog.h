
//***************************************************************************
//
// Mix_Prog.h - Header file for the main functions found in BB_Prog.cpp
//
// This is the declaration file for the functions used to calculate
// the plane-wave broadband OPO.  This file must be #included for the
// pre-compiler in the 'Mix_Prog.cpp' file. 
//
//***************************************************************************

// Pre-compiler instructions
#ifndef MIX_PROG_H
#define MIX_PROG_H   

//#include "StdAfx.h"
#include "DecIncl.h"
#include "Fields.h"
#include "Inputs.h"
#include "GlobVar.h"
#include "Beams.h"
#include "Nrutil.h"

using namespace std;

typedef struct Fluences
{
	double* pXgrid;
	double* pYgrid;
	double* pFluence_s;
	double* pFluence_i;
	double* pFluence_p;
	int nx;
	int ny;
}Fluences;


// Numerical Recipies definitions
#define NR_END 1
#define FREE_ARG char*

//***************************************************************************
// Program functions

// Functions to get and set the stop variable 
void SetStop(bool Stop);
bool GetStop();

// Functions to find the value of the radio buttons in CGraphDlg
void SetRadio(int num);
int GetRadio();

// Functions to reset the rotation of the graph
void SetRotation(double numX, double numZ);
double GetRotX();
double GetRotZ();

// Function to check for windows messages
void CheckWinMessages(int numCycles);

// Functions for complex numbers
complex<double> cexp(double rval, double ival, double expval, int Isign);
complex<double> cmult(double r_a, double i_b, double r_c, double i_d,int c_conj);
void OuterProduct(float* F_x, float* F_y, float* F_t, float* XYZ, long nx, long ny, long nt);

// Main functions
void Reader(CInputs* pBt);
void SetInputForm(CInputs* pBt); 
void WriteOut(CInputs* pBt);
void ConvertToSI(CInputs* pBt);
void SetGrids(CInputs* pBt,CGlobalVar* pGV,CFields* pField);
void InitializeFields(CInputs* pBt, CGlobalVar* pGV, CFields* pField);
void InitializePhases(CInputs* pBt, CGlobalVar* pGV, CFields* pField);
void Integrate(CInputs* pBt, CGlobalVar* pGV, CFields* pField, 
			   CProgressCtrl* pProgS, CProgressCtrl* pProgT);
void PropagateField(CFields* pField,float* Efield,float* Phase,long nx, long ny, long nt);
void CalculateOutput(CInputs* pBt,CGlobalVar* pGV,CFields* pField,
					 double* pMinff,double* pMaxff,double* pSmax,double* pSmin,
					 double* pMaxS_s,double* pMaxS_i,double* pMaxS_p,
					 double* pMaxP_s,double* pMaxP_i,double* pMaxP_p,
					 CProgressCtrl* pProgS);

void Spectra(CFields* pField, float* EField, float* Spectra, 
			 int nx, int ny, int nt, long XFactor,double* maxnum);
void FarField(CFields* pField, float* EField, float* FField, int nx, int ny, int nt);


void AnalyzeBeams_t(float* Power,float* EField,CBeams* pBeams,
					double Lambda,double MaxP,long nx,long ny,long nt,
					double dx,double dy,double dt,CProgressCtrl* pProgS,
					CProgressCtrl* pProgT,int T,double* Mavg);

void AnalyzeBeams_w(float* Spectrum,float* EField,CBeams* pSBeam,
					double Lambda,double MaxSp,long nx,long ny,long nt,
					double dx,double dy,double dt,CProgressCtrl* pProgS,
					CProgressCtrl* pProgT,int T,double* Mavg);

void Msquared(float* Power,float* Field,CBeams* pBeams,double Lambda,
			  double MaxP,long nx,long ny,long nt,double dx,double dy,
			  CProgressCtrl* pProgS);

void GaussianAfterLens(double index, double wavelength, double dis,
					   double inBeamsize, double focus,
					   double* outBeamsize, double* radcurv);

// Numerical Recipies functions
extern "C" float *vector(long nl, long nh);
extern "C" void free_vector(float *v, long nl, long nh);
extern "C" unsigned long *lvector(long nl, long nh);
extern "C" void free_lvector(unsigned long *v, long nl, long nh);
extern "C" double *dvector(long nl, long nh);
extern "C" void free_dvector(double *v, long nl, long nh);


#endif
