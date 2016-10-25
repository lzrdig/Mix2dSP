
//***************************************************************************
//
// Inputs.h - Header file for class CInputs
//
//***************************************************************************
 
// Pre-compiler instructions
#ifndef INPUTS_H


//***************************************************************************
// List of 'array' arguments for CInputs
const int DEFF = 10000;					
const int DELTAK = 10001;				
const int ENERGY_P = 10002;				
const int ENERGY_S = 10003;			
const int ENERGY_I = 10004;
const int BEAMDIAM_S = 10005;			
const int BEAMDIAM_I = 10006;
const int BEAMDIAM_P = 10007;
const int DURATION_S = 10008;			
const int DURATION_I = 10009;
const int DURATION_P = 10010;
const int LAMBDA_S = 10011;				
const int LAMBDA_I = 10012;
const int LAMBDA_P = 10013;
const int INDEX_S = 10014;				
const int INDEX_I = 10015;
const int INDEX_P = 10016;
const int GVI_S = 10017;				
const int GVI_I = 10018;
const int GVI_P = 10019;
const int GVD_S = 10020;				
const int GVD_I = 10021;
const int GVD_P = 10022;
const int ALPHA_S = 10023;				
const int ALPHA_I = 10024;
const int ALPHA_P = 10025;
const int RC_IN_S = 10026;				
const int RC_IN_I = 10027;
const int RC_IN_P = 10028;
const int RC_OUT_S = 10029;				
const int RC_OUT_I = 10030;
const int RC_OUT_P = 10031;
const int CRYSTLENGTH = 10032;			

const int N2_S1 = 10033;
const int N2_S2 = 10034;
const int N2_S3 = 10035;
const int N2_I1 = 10036;
const int N2_I2 = 10037;
const int N2_I3 = 10038;
const int N2_P1 = 10039;
const int N2_P2 = 10040;
const int N2_P3 = 10041;
const int BETA_S1 = 10042;
const int BETA_S2 = 10043;
const int BETA_S3 = 10044;
const int BETA_I1 = 10045;
const int BETA_I2 = 10046;
const int BETA_I3 = 10047;
const int BETA_P1 = 10048;
const int BETA_P2 = 10049;
const int BETA_P3 = 10050;

const int RHO_S = 10051;				
const int RHO_I = 10052;
const int RHO_P = 10053;
const int RADCURV_S = 10054;		
const int RADCURV_I = 10055;
const int RADCURV_P = 10056;
const int OFFSET_S = 10057;			
const int OFFSET_I = 10058;
const int GAUSSINDEX_S = 10059;
const int GAUSSINDEX_I = 10060;
const int GAUSSINDEX_P = 10061;
const int PHI_S = 10062;
const int PHI_I = 10063;
const int PHI_P = 10064;
const int DELAY_S = 10065;
const int DELAY_I = 10066;
const int NX = 10067;					
const int NY = 10068;
const int NT = 10069;
const int NZ = 10070;
const int MAXGRID_X = 10071;			
const int MAXGRID_Y = 10072;
const int DIST = 10073;

#pragma warning(disable:4100)

//***************************************************************************
// Definition of class CInputs

class CInputs
{
public:
	
// Class constructor (initializes all the variables to zero)
	CInputs(double deff = 0.0,
		   double deltak = 0.0, 
		   double Energy_p = 0.0,
		   double Energy_s = 0.0,
		   double Energy_i = 0.0,
		   double Beamdiam_s = 0.0,
		   double Beamdiam_i = 0.0,
		   double Beamdiam_p = 0.0,
		   double Duration_s = 0.0,
		   double Duration_i = 0.0,
		   double Duration_p = 0.0,
		   double Lambda_s = 0.0,
		   double Lambda_i = 0.0,
		   double Lambda_p = 0.0,
		   double Index_s = 0.0,
		   double Index_i = 0.0,
		   double Index_p = 0.0,
		   double gvi_s = 0.0,
		   double gvi_i = 0.0,
		   double gvi_p = 0.0,
		   double gvd_s = 0.0,
		   double gvd_i = 0.0,
		   double gvd_p = 0.0,
		   double alpha_s = 0.0,
		   double alpha_i = 0.0,
		   double alpha_p = 0.0,
		   double Rc_In_s = 0.0,
		   double Rc_In_i = 0.0,
		   double Rc_In_p = 0.0,
		   double Rc_Out_s = 0.0,
		   double Rc_Out_i = 0.0,
		   double Rc_Out_p = 0.0,
		   double Crystlength = 0.0,
		   double n2_s1 = 0.0,
		   double n2_s2 = 0.0,
		   double n2_s3 = 0.0,
		   double n2_i1 = 0.0,
		   double n2_i2 = 0.0,
		   double n2_i3 = 0.0,
		   double n2_p1 = 0.0,
		   double n2_p2 = 0.0,
		   double n2_p3 = 0.0,
		   double beta_s1 = 0.0,
		   double beta_s2 = 0.0,
		   double beta_s3 = 0.0,
		   double beta_i1 = 0.0,
		   double beta_i2 = 0.0,
		   double beta_i3 = 0.0,
		   double beta_p1 = 0.0,
		   double beta_p2 = 0.0,
		   double beta_p3 = 0.0,
		   double Rho_s = 0.0,
		   double Rho_i = 0.0,
		   double Rho_p = 0.0,
		   double RadCurv_s = 0.0,
		   double RadCurv_i = 0.0,
		   double RadCurv_p = 0.0,
		   double Offset_s = 0.0,
		   double Offset_i = 0.0,
		   double GaussIndex_s = 0.0,
		   double GaussIndex_i = 0.0,
		   double GaussIndex_p = 0.0,
		   double Phi_s = 0.0,
		   double Phi_i = 0.0,
		   double Phi_p = 0.0,
		   double Delay_s = 0.0,
		   double Delay_i = 0.0,
		   double nx = 0.0,
		   double ny = 0.0,
		   double nt = 0.0,
		   double nz = 0.0,
		   double Maxgrid_x = 0.0,
		   double Maxgrid_y = 0.0,
		   double Dist = 0.0){};

// Class destructor
	~CInputs(){};

// Member functions
	double GetVar(int array);
	void SetVar(int array, double num);

	
// Class variables
private:
	double m_deff;                   // deff [pm/V]
	double m_deltak;					// delta k [1/mm] kp-ks-ki
	double m_Energy_p, m_Energy_s, m_Energy_i;	// energy [J] 
	double m_Beamdiam_s, m_Beamdiam_i, m_Beamdiam_p;	// beam diameters [mm]
	double m_Duration_s, m_Duration_i, m_Duration_p;	// pulse duration [ps]
	double m_Lambda_s, m_Lambda_i, m_Lambda_p;  // wavelengths [nm]
	double m_Index_s, m_Index_i, m_Index_p;  // index of refraction
	double m_gvi_s, m_gvi_i, m_gvi_p;		   // group velocity index
	double m_gvd_s, m_gvd_i, m_gvd_p;		   // group velocity dispersion
	double m_alpha_s, m_alpha_i, m_alpha_p;     // crystal absorption [1/mm]
	double m_Rc_In_s, m_Rc_In_i, m_Rc_In_p;  // crystal input reflectivity
	double m_Rc_Out_s, m_Rc_Out_i, m_Rc_Out_p;  // crystal output reflectivity
	double m_Crystlength;					   // crystal length [mm]
	double m_N2_s1,m_N2_s2,m_N2_s3;			// n-squared signal [sq cm/W]
	double m_N2_i1,m_N2_i2,m_N2_i3;			// n-squared idler [sq cm/W]
	double m_N2_p1,m_N2_p2,m_N2_p3;			// n-squared pump [sq cm/W]
	double m_Beta_s1,m_Beta_s2,m_Beta_s3;	// beta signal [cm/W]
	double m_Beta_i1,m_Beta_i2,m_Beta_i3;	// beta idler [cm/W]
	double m_Beta_p1,m_Beta_p2,m_Beta_p3;	// beta pump [cm/W]
	double m_Rho_s, m_Rho_i, m_Rho_p;					// walkoff angles [mrad] 
	double m_RadCurv_s, m_RadCurv_i, m_RadCurv_p;	// radius of curvature [mm in air]
	double m_Offset_s, m_Offset_i;		// offset in wo direction [mm]
	double m_nx, m_ny, m_nt, m_nz;						// number of grid points/z-steps
	double m_Maxgrid_x, m_Maxgrid_y, m_Maxgrid_t;	// maximum grid extent [mm]
	double m_GaussIndex_s, m_GaussIndex_i, m_GaussIndex_p;
	double m_Phi_s, m_Phi_i, m_Phi_p;				// phases [rad]
	double m_Delay_s, m_Delay_i;					// time delay of s,i rel. to pump [ps]
	double m_Dist;							   // distance to detector [mm]

};

#define INPUTS_H
#endif