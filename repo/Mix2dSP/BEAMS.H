//*********************************************************************
//
// Beams.h - Header file for class CBeams
//
//*********************************************************************

// Pre-compiler instructions
#ifndef BEAMS_H


class CBeams
{
public:

// Class constructors
	CBeams();
	CBeams(long size);

// Class destructor
	~CBeams();

// Member functions
void SetBeamx(long array, double tilt,double msqr, double zo, double wavofcurv,
			   double sigsqx, double sigsqs, double asubx, double xbar);

void SetBeamy(long array, double tilt,double msqr, double zo, double wavofcurv,
			   double sigsqy, double sigsqs, double asuby, double ybar);

void SetMxavg(double value);
void SetMyavg(double value);

double GetTiltx(long array);
double GetTilty(long array);
double GetMsqrx(long array);
double GetMsqry(long array);
double GetZOx(long array);
double GetZOy(long array);
double GetWavofCurvx(long array);
double GetWavofCurvy(long array);
double GetSigsqx(long array);
double GetSigsqy(long array);
double GetSigsqs_x(long array);
double GetSigsqs_y(long array);
double GetAsubx(long array);
double GetAsuby(long array);
double GetXbar(long array);
double GetYbar(long array);
double GetMxavg();
double GetMyavg();

//Class variables
private:

	double *pTiltx, *pTilty;	// Beam tilt in x and y
	double *pMsqrx, *pMsqry;	// Msquarted in x and y
	double *pzox, *pzoy;		// dist. to waist in x and y
	double *pWavx, *pWavy;		// waves of curvature in x and y
	double *pSigsqx, *pSigsqy;	// Sigma squared in x and y
	double *pSigsqs_x, *pSigsqs_y;	// Sigma squared s in x and y
	double *pAsubx, *pAsuby;	// Asubx/y
	double *pXbar, *pYbar;		// Xbar and Ybar

	double m_mxavg, m_myavg;	// average msquared in x and y

	long nt;
};

#define BEAMS_H
#endif


