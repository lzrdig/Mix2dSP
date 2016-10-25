
//***************************************************************************
//
// Class CGlobalVar
//
// List of member functions:
//
//  * dSetGlobal(int array, double val)
//
//         Sets the value of the 'array' argument to the type double 'val'.
//
//      Example:  pGlobalVar->dSetGlobal(NOISEBANDWIDTH,14.2) sets the value
//                              of 'Noisebandwidth' = 14.2
//
//  * ulSetGlobal(int array, long val)
//
//         Same as above, only for type unsigned long 'val'.
//
//      Example:  pGlobalVar->lSetGlobal(NBUFFER,40) sets 'Nbuffer' = 40
//
//  
//
//  *  dGetGlobal(int array)
//
//          Returns the type double value for the variable 'array'
//
//       Example:  pGlobalVar->dGetGlobal(VFAST) returns the value for 'vfast'
//
//
//  *  ulGetGlobal(int array)
//
//          Same as above, but for type unsigned long.
//
//       Example:  pGlobalVar->lGetGlobal(NGRID) returns the value for 'ngrid'
//
//  (Note: for a complete list of the 'array' arguments, see 'GlobVar.h')
//
//***************************************************************************

// Pre-compiler instructions
#include "StdAfx.h"
#include "DecIncl.h"
#include "GlobVar.h"

#pragma warning(disable:4244)

extern "C" unsigned long *lvector(long nl, long nh);
extern "C" void free_lvector(unsigned long *v, long nl, long nh);
extern "C" double *dvector(long nl, long nh);
extern "C" void free_dvector(double *v, long nl, long nh);

//**********************************************************************
// Class constructor

CGlobalVar::CGlobalVar()
{
	m_nn = lvector(1,3);
}


//**********************************************************************
// Class destructor

CGlobalVar::~CGlobalVar()
{
	free_lvector(m_nn,1,3);
}


//***************************************************************************
// Function dSetGlobal
 
void CGlobalVar::dSetGlobal(int array, double val)
{

	switch (array)
	{
	case OMEGA_S:
		m_omega_s = val;
		break;
	case OMEGA_I:
		m_omega_i = val;
		break;
	case OMEGA_P:
		m_omega_p = val;
		break;
	case DX:
		m_dx = val;
		break;
	case DY:
		m_dy = val;
		break;
	case DT:
		m_dt = val;
		break;
	case DZ:
		m_dz = val;
		break;
	case I_TERM:
		m_I_term = val;
		break;
	case PEAKPOW_S:
		m_peakPow_s = val;
		break;
	case PEAKPOW_I:
		m_peakPow_i = val;
		break;
	case PEAKPOW_P:
		m_peakPow_p = val;
		break;
	case PEAKIR_S:
		m_peakIr_s = val;
		break;
	case PEAKIR_I:
		m_peakIr_i = val;
		break;
	case PEAKIR_P:
		m_peakIr_p = val;
		break;
	case PEAKFL_S:
		m_peakFl_s = val;
		break;
	case PEAKFL_I:
		m_peakFl_i = val;
		break;
	case PEAKFL_P:
		m_peakFl_p = val;
		break;
	case IROUT_S:
		m_IrOut_s = val;
		break;
	case IROUT_I:
		m_IrOut_i = val;
		break;
	case IROUT_P:
		m_IrOut_p = val;
		break;
	case EPULSE_S:
		m_Epulse_s = val;
		break;
	case EPULSE_I:
		m_Epulse_i = val;
		break;
	case EPULSE_P:
		m_Epulse_p = val;
		break;
	case NGRIDX:
		m_ngridx = val;
		break;
	case NGRIDY:
		m_ngridy = val;
		break;
	case PADVAL:
		m_padval = val;
		break;
	case DTHETAX:
		m_dthetax = val;
		break;
	case DTHETAY:
		m_dthetay = val;
		break;
	case ORIGINX_FL:
		m_originx_fl = val;
		break;
	case ORIGINY_FL:
		m_originy_fl = val;
		break;
	default:
		break;
	}	

}


//***************************************************************************
// Function lSetGlobal

void CGlobalVar::lSetGlobal(int array, long val)
{
	switch(array)
	{
	case NGRIDX:
		m_ngridx = val;
		break;
	case NGRIDY:
		m_ngridy = val;
		break;
	case PADVAL:
		m_padval = val;
		break;
	default:
		break;
	}
}	


//***************************************************************************
// Function ulSetGlobal

void CGlobalVar::ulSetGlobal(int array, int element, unsigned long val)
{
	switch(array)
	{
	case NN:
		switch(element)
		{
			case 1:
				m_nn[1] = val;
				break;
			case 2:
				m_nn[2] = val;
				break;
			case 3:
				m_nn[3] = val;
				break;
			default:
			break;
		}
		break;
	default:
		break;
	}

}


//***************************************************************************
// Function dGetGlobal 

double CGlobalVar::dGetGlobal(int array)
{

	switch (array)
	{
	case OMEGA_S:
		return m_omega_s;
		break;
	case OMEGA_I:
		return m_omega_i;
		break;
	case OMEGA_P:
		return m_omega_p;
		break;
	case DX:
		return m_dx;
		break;
	case DY:
		return m_dy;
		break;
	case DT:
		return m_dt;
		break;
	case DZ:
		return m_dz;
		break;
	case I_TERM:
		return m_I_term;
		break;
	case PEAKPOW_S:
		return m_peakPow_s;
		break;
	case PEAKPOW_I:
		return m_peakPow_i;
		break;
	case PEAKPOW_P:
		return m_peakPow_p;
		break;
	case PEAKIR_S:
		return m_peakIr_s;
		break;
	case PEAKIR_I:
		return m_peakIr_i;
		break;
	case PEAKIR_P:
		return m_peakIr_p;
		break;
	case PEAKFL_S:
		return m_peakFl_s;
		break;
	case PEAKFL_I:
		return m_peakFl_i;
		break;
	case PEAKFL_P:
		return m_peakFl_p;
		break;
	case IROUT_S:
		return m_IrOut_s;
		break;
	case IROUT_I:
		return m_IrOut_i;
		break;
	case IROUT_P:
		return m_IrOut_p;
		break;
	case EPULSE_S:
		return m_Epulse_s;
		break;
	case EPULSE_I:
		return m_Epulse_i;
		break;
	case EPULSE_P:
		return m_Epulse_p;
		break;
	case NGRIDX:
		return m_ngridx;
		break;
	case NGRIDY:
		return m_ngridy;
		break;
	case PADVAL:
		return m_padval;
		break;
	case DTHETAX:
		return m_dthetax;
		break;
	case DTHETAY:
		return m_dthetay;
		break;
	case ORIGINX_FL:
		return m_originx_fl;
		break;
	case ORIGINY_FL:
		return m_originy_fl;
		break;
	default:
		return 0.0;
		break;
	}	
}


//***************************************************************************
// Function lGetGlobal

long CGlobalVar::lGetGlobal(int array)
{
	switch(array)
	{
	case NGRIDX:
		return m_ngridx;
		break;
	case NGRIDY:
		return m_ngridy;
		break;
	case PADVAL:
		return m_padval;
		break;
	default:
		return 0;
		break;
	}
}	


//***************************************************************************
// Function ulGetGlobal

unsigned long CGlobalVar::ulGetGlobal(int array, int element)
{
	switch(array)
	{
	case NN:
		switch(element)
		{
			case 1:
				return m_nn[1];
				break;
			case 2:
				return m_nn[2];
				break;
			case 3:
				return m_nn[3];
				break;
			default:
				return 0;
				break;
		}
		break;
	default:
		return 0;
		break;
	}
}



