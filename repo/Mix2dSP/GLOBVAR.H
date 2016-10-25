
//***************************************************************************
//
// GlobVar.h - Header for class CGlobalVar
//
//***************************************************************************

 
// Pre-compiler instructions
#ifndef GLOBALVAR_H


//***************************************************************************
// List of 'array' arguments for class CGlobalVar

const int NN = 11111;

const int OMEGA_S = 10101;
const int OMEGA_I = 10102;
const int OMEGA_P = 10103;
const int DX = 10104;
const int DY = 10105;
const int DT = 10106;
const int DZ = 10107;
const int I_TERM = 10108;
const int PEAKPOW_S = 10109;
const int PEAKPOW_I = 10110;
const int PEAKPOW_P = 10111;
const int PEAKIR_S = 10112;
const int PEAKIR_I = 10113;
const int PEAKIR_P = 10114;
const int PEAKFL_S = 10115;
const int PEAKFL_I = 10116;
const int PEAKFL_P = 10117;
const int IROUT_S = 10118;
const int IROUT_I = 10119;
const int IROUT_P = 10120;
const int EPULSE_S = 10121;
const int EPULSE_I = 10122;
const int EPULSE_P = 10123;
const int NGRIDX = 10124;
const int NGRIDY = 10125;
const int PADVAL = 10126;
const int DTHETAX = 10127;
const int DTHETAY = 10128;
const int ORIGINX_FL = 10129;
const int ORIGINY_FL = 10130;

extern "C" unsigned long *lvector(long nl, long nh);
extern "C" void free_lvector(unsigned long *v, long nl, long nh);

//***************************************************************************
// Definition of class CGlobalVar

class CGlobalVar
{
public:

// Class constructor 
	CGlobalVar();

// Class destructor
	~CGlobalVar();

// Member functions
	void dSetGlobal(int array, double val);
	void lSetGlobal(int array, long val);
	void ulSetGlobal(int array, int element, unsigned long val);
	double dGetGlobal(int array);
	long lGetGlobal(int array);
	unsigned long ulGetGlobal(int array, int element);

// Class variables
private:
	double m_omega_s,m_omega_i,m_omega_p;	// frequencies (rad/s)
	double m_dx,m_dy,m_dt,m_dz;	// step sizes in x,y,t,z
	double m_I_term;		// term for intensities
	double m_peakPow_s, m_peakPow_i, m_peakPow_p;	// peak power [W]
	double m_peakIr_s, m_peakIr_i, m_peakIr_p;		// peak irradiance [J/m^2]
	double m_peakFl_s, m_peakFl_i, m_peakFl_p;		// peak fluence [W/m^2]
	double m_IrOut_s, m_IrOut_i, m_IrOut_p;			// output peak irradiance [J/m^2]
	double m_Epulse_s, m_Epulse_i, m_Epulse_p;		// output pulse energy [J]
	
	long m_ngridx, m_ngridy, m_padval;	// variables for graphing
	double m_dthetax, m_dthetay;
	double m_originx_fl, m_originy_fl;

	unsigned long *m_nn;	// grid sizes in x,y,t
};



#define GLOBALVAR_H
#endif