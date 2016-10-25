//*********************************************************************
//
// Beams.cpp - source file for CBeams 
//
//*********************************************************************

#include "StdAfx.h"
#include "DecIncl.h"
#include "Beams.h"

extern "C" double *dvector(long nl, long nh);
extern "C" void free_dvector(double *v, long nl, long nh);


//*********************************************************************
// Class constructors

CBeams::CBeams()
{
	// Default setting to 1
	nt = 1;

	pTiltx = dvector(1,nt);
	pTilty = dvector(1,nt);
	pMsqrx = dvector(1,nt);
	pMsqry = dvector(1,nt);
	pzox   = dvector(1,nt);
	pzoy   = dvector(1,nt);
	pWavx  = dvector(1,nt);
	pWavy  = dvector(1,nt);
	pSigsqx = dvector(1,nt);
	pSigsqy = dvector(1,nt);
	pSigsqs_x = dvector(1,nt);
	pSigsqs_y = dvector(1,nt);
	pAsubx = dvector(1,nt);
	pAsuby = dvector(1,nt);
	pXbar  = dvector(1,nt);
	pYbar  = dvector(1,nt);

	m_mxavg = 0.0;
	m_myavg = 0.0;

}

CBeams::CBeams(long size)
{
	// Set the arrays according to the size of the time dimension
	nt = size;

	pTiltx = dvector(1,nt);
	pTilty = dvector(1,nt);
	pMsqrx = dvector(1,nt);
	pMsqry = dvector(1,nt);
	pzox   = dvector(1,nt);
	pzoy   = dvector(1,nt);
	pWavx  = dvector(1,nt);
	pWavy  = dvector(1,nt);
	pSigsqx = dvector(1,nt);
	pSigsqy = dvector(1,nt);
	pSigsqs_x = dvector(1,nt);
	pSigsqs_y = dvector(1,nt);
	pAsubx = dvector(1,nt);
	pAsuby = dvector(1,nt);
	pXbar  = dvector(1,nt);
	pYbar  = dvector(1,nt);

	m_mxavg = 0.0;
	m_myavg = 0.0;
}

//*********************************************************************
// Class destructor

CBeams::~CBeams()
{
	free_dvector(pTiltx,1,nt);
	free_dvector(pTilty,1,nt);
	free_dvector(pMsqrx,1,nt);
	free_dvector(pMsqry,1,nt);
	free_dvector(pzox,1,nt);
	free_dvector(pzoy,1,nt);
	free_dvector(pWavx,1,nt);
	free_dvector(pWavy,1,nt);
	free_dvector(pSigsqx,1,nt);
	free_dvector(pSigsqy,1,nt);
	free_dvector(pSigsqs_x,1,nt);
	free_dvector(pSigsqs_y,1,nt);
	free_dvector(pAsubx,1,nt);
	free_dvector(pAsuby,1,nt);
	free_dvector(pXbar,1,nt);
	free_dvector(pYbar,1,nt);
}


void CBeams::SetBeamx(long array, double tilt,double msqr, double zo, double wavofcurv,
			   double sigsqx, double sigsqs, double asubx, double xbar)
{
	pTiltx[array] = tilt;
	pMsqrx[array] = msqr;
	pzox[array] = zo;
	pWavx[array] = wavofcurv;
	pSigsqx[array] = sigsqx;
	pSigsqs_x[array] = sigsqs;
	pAsubx[array] = asubx;
	pXbar[array] = xbar;
}

void CBeams::SetBeamy(long array, double tilt,double msqr, double zo, double wavofcurv,
			   double sigsqy, double sigsqs, double asuby, double ybar)
{
	pTilty[array] = tilt;
	pMsqry[array] = msqr;
	pzoy[array] = zo;
	pWavy[array] = wavofcurv;
	pSigsqy[array] = sigsqy;
	pSigsqs_y[array] = sigsqs;
	pAsuby[array] = asuby;
	pYbar[array] = ybar;
}

void CBeams::SetMxavg(double value)
{
	m_mxavg = value;
}

void CBeams::SetMyavg(double value)
{
	m_myavg = value;
}

double CBeams::GetTiltx(long array)
{
	return pTiltx[array];
}


double CBeams::GetTilty(long array)
{
	return pTilty[array];
}


double CBeams::GetMsqrx(long array)
{
	return pMsqrx[array];
}


double CBeams::GetMsqry(long array)
{
	return pMsqry[array];
}


double CBeams::GetZOx(long array)
{
	return pzox[array];
}


double CBeams::GetZOy(long array)
{
	return pzoy[array];
}


double CBeams::GetWavofCurvx(long array)
{
	return pWavx[array];
}


double CBeams::GetWavofCurvy(long array)
{
	return pWavy[array];
}


double CBeams::GetSigsqx(long array)
{
	return pSigsqx[array];
}


double CBeams::GetSigsqy(long array)
{
	return pSigsqy[array];
}


double CBeams::GetSigsqs_x(long array)
{
	return pSigsqs_x[array];
}


double CBeams::GetSigsqs_y(long array)
{
	return pSigsqs_y[array];
}


double CBeams::GetAsubx(long array)
{
	return pAsubx[array];
}


double CBeams::GetAsuby(long array)
{
	return pAsuby[array];
}


double CBeams::GetXbar(long array)
{
	return pXbar[array];
}


double CBeams::GetYbar(long array)
{
	return pYbar[array];
}

double CBeams::GetMxavg()
{
	return m_mxavg;
}

double CBeams::GetMyavg()
{
	return m_myavg;
}
