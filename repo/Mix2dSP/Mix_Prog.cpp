//*********************************************************************
//
// Two dimensional short-pulse mixing program
//
// Based on the SNLO function 2D_Mix_SP by:	Arlee V. Smith 
//
// C++ code and GUI development by: Chris K. Schultz	
//
// Version 1.0 (7/00)
//
// 
// This is the main code that calculates the irradiance, spectra,
// fluence and farfield for a 2d short-pulse mixing model using the 
// input parameters specified in the input form inputs.in.  The code for 
// the Windows interface may be found in the files GraphDlg.cpp, 
// MixView.cpp, and 2dWnd.cpp.  Calls to the functions contained in 
// this file may be found within the GraphDlg::OnRunBtn function.
//
// This also contains the code for the beam analysis function
// GraphDlg::OnAnalyze.
//
//*********************************************************************

// Pre-compiler instructions
#include "StdAfx.h"
#include "Mix_Prog.h"
#include "Version.h"

// FFT package headers
#include "..\fftw2dll\fftw\Fftw-int.h"
#include "..\fftw2dll\fftw\Fftw.h"
#include "..\fftw2dll\fftw\Config.h"

// Global variables for Mix_Prog functions
int	 m_nScanIndex;
bool m_bDeltakOn;	//global var for state of DeltakDlg
bool m_bErgOn;		//global var for state of ErgDlg
bool m_bLensOn;
bool m_bLensScanOn;
bool m_bStop;		// global var for SetStop and GetStop
int  m_Radio;		// global var for SetRadio and GetRadio
double m_XRotation, m_ZRotation;	// global var for graph rotation
long   m_Tmin, m_Tmax;
double m_InFluence_s,m_InFluence_i,m_InFluence_p;

#pragma warning(disable:4244)
#pragma warning(disable:4701)
#pragma warning(disable:4189)



//************* Functions SetStop and GetStop *************************
//
// SetStop and GetStop are called in order to stop OnRunBtn
//
//*********************************************************************

void SetStop(bool Stop)
{
	m_bStop = Stop;
}

bool GetStop()
{
	return m_bStop;
}


//******************* Functions SetRadio and GetRadio *****************
//
// These are called in order to update which radio button on the
// GraphDlg dialog box is currently selected (signal,idler,pump)
//
//*********************************************************************

void SetRadio(int num)
{
	m_Radio = num;
}

int GetRadio()
{
	return m_Radio;
}


//************** Functions SetRotation and GetRotation ****************
//
// These are called in order to update the rotation of the chart
//
//*********************************************************************

void SetRotation(double numX,double numZ)
{
	m_XRotation = numX;
	m_ZRotation = numZ;
}

double GetRotX()
{
	return m_XRotation;
}

double GetRotZ()
{
	return m_ZRotation;
}


//********************* Function CheckWinMessages *********************
//
// CheckWinMessages calls PeekMessage in order to temporarily release
// control to windows during long calculations.
// 
//*********************************************************************

void CheckWinMessages(int numCycles)
{
	MSG message;
	for(int mm=0;mm<numCycles;++mm)
	{
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{			
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		else
		{
			break;
		}
	} 
}


//********************* Function cexp *********************************
//
// Function cexp will return the value for a complex exponential
// where rval and ival are the real and imaginary parts of the 
// amplitude, expval is the argument of the exponential (for use
// with sin and cos), and Isign is the sign of the exponential 
// (i.e. i or -i) 
//
// Called from:	Integrate	
//
//*********************************************************************

complex<double> cexp(double rval, double ival, double expval, int Isign)
{
	complex<double> result;

	if(Isign<0) 
	{
			result.real((rval*cos(expval)) + (ival*sin(expval)));
			result.imag((ival*cos(expval)) - (rval*sin(expval)));
			return result;
	}
	else
	{
		if(Isign>0)  
		{
			result.real((rval*cos(expval)) - (ival*sin(expval)));
			result.imag((ival*cos(expval)) + (rval*sin(expval)));
			return result;
		}
		else
		{
			//Default return value
			result.real(0);
			result.imag(0);
			return result;
		}
	}
}	// End cexp


//********************* Function cmult ********************************
//
// Function cmult will multiply two complex numbers 
// (a + ib) * (c + id)
//
// Note: c_conj should be set to 1 if the complex conjugate is used
//
// Called from:	Integrate	
//
//*********************************************************************
	
complex<double> cmult(double r_a, double i_b, double r_c, double i_d,int c_conj)
{
	complex<double> result;


	if(c_conj)
	{
		result.real((r_a*r_c)+(i_b*i_d));
		result.imag((r_a*i_d)-(i_b*r_c));
		return result;
	}
	else
	{
		result.real((r_a*r_c)-(i_b*i_d));
		result.imag((r_a*i_d)+(i_b*r_c));
		return result;
	}

	//// Default return value
	//result.real(0);
	//result.imag(0);
	//return result;
}


//******************* Function OuterProduct ***************************
//
// This function will calculate the outer product for a 
// three dimensional matrix of complex numbers
//
// Note: this function is not used in this program; it's only being 
//       kept for possible future use
//       
//*********************************************************************

void OuterProduct(float* F_x, float* F_y, float* F_t, float* XYZ, long nx, long ny, long nt)
{
	int i,j,k,l,i2,j2,k2,l2;

	for (i=1;i<=nx;i++)
	{
		for (j=1;j<=ny;j++)
		{
			for (k=1;k<=nt;k++)
			{
				l=k+(j-1)*nt+(i-1)*nt*ny;
				i2 = 2*i;
				j2 = 2*j;
				k2 = 2*k;
				l2 = 2*l;
				// real part
				XYZ[l2-1]=F_t[k2-1]*(F_x[i2-1]*F_y[j2-1] - F_x[i2]*F_y[i2])
						- F_t[k2]*(F_x[i2-1]*F_y[j2] + F_x[i2]*F_y[j2-1]);
				// imag part
				XYZ[l2]=F_t[k2]*(F_x[i2-1]*F_y[j2-1] - F_x[i2]*F_y[j2])
						- F_t[k2-1]*(F_x[i2-1]*F_y[j2] + F_x[i2]*F_y[j2-1]);
			}
		}
	}

}

//***************** Function GaussianAfterLens ************************
//
// Function GaussianAfterLens calculates the parameters of a Gaussian
// beam after a lens.
//
// Called from:	
//
//*********************************************************************
void GaussianAfterLens(double index, double wavelength, double dis,
					   double inBeamsize, double focus,
					   double* outBeamsize, double* radcurv)
{
	const double Pi = 3.14159265359;

	complex<double> Qd;
	complex<double> invQd(1,0);
	
	complex<double> q0(0,Pi*pow(inBeamsize,2)/wavelength);		//parallel's beam q-parameter

	Qd		 = q0*focus/(focus-q0)+dis;				//q-parameter after lens at distance dis
	invQd	/= Qd;
    *outBeamsize = sqrt(-wavelength/Pi/invQd.imag());			// beam size
    *radcurv  = -1/invQd.real();						// radius of curvature in air
      
	*outBeamsize *= sqrt(2*(double)log((double)2));					// convert beam size from FHWM to 1/e^2
}

//************************ Function Reader ****************************
//
// Function Reader opens the input file 'Inputs.in', reads the input
// variables, and stores them in the specified CInputs object
//
// Called from:	GraphDlg::OnRunBtn	
//
//*********************************************************************

void Reader(CInputs* pBt)
{

	CString filename = "Inputs.in";
	ifstream fin(filename);

	if(!fin)
	{
		AfxMessageBox("Error in reading 'Inputs.in'.");
		return;
	}
	   
	int i;
	char ch;
	float num;

	ch = 0;

	for(i = DEFF; i<(DIST+1); i++)
	{
	
		fin >> num;
		pBt->SetVar(i, num);

		// Check for additional variables on the same line
		do
		{
			fin >> ch;
			if (ch != ',')
			;
			else
			{
				fin >> num;
				i++;
				pBt->SetVar(i,num);
			}
		}
		while (ch != '!');
			
		// Check for end of line or end of file 
		do
			fin.get(ch);
		while ((ch != '\n') && (ch != EOF));
		
	}

	fin.close();

}	// End Reader


//************************ Function SetInputForm ***********************
// 
// This function will rewrite the input file 'Inputs.in' if the user
// chose the maximum grid extents to be zero (autoscaling of the grid).
//
// Called from: SetGrids
//
//**********************************************************************

void SetInputForm(CInputs* pBt)
{

//***************************************************************************
// Print the current information to the Inputs.in file

	CString filename = "Inputs.in";
	FILE * stream;
	stream = fopen(filename, "w");

	fprintf(stream,"%5.2f %s", pBt->GetVar(DEFF)*1.e12, 
		"                   !deff (pm/V)\n");
	fprintf(stream,"%5.2f %s", pBt->GetVar(DELTAK)*1.e-3,
		"                   !deltak (1/mm) kp-ks-ki\n");
	fprintf(stream,"%5.2e %s", pBt->GetVar(ENERGY_P),
		"                   !pump energy (J)\n");
	fprintf(stream,"%5.2e %s %5.2e %s", pBt->GetVar(ENERGY_S),",",pBt->GetVar(ENERGY_I),
		"              !signal, idler energy (J)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(BEAMDIAM_S)*1.e3,",",  
		pBt->GetVar(BEAMDIAM_I)*1.e3,",", pBt->GetVar(BEAMDIAM_P)*1.e3,
		"          !beam diameters (mm) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(DURATION_S)*1.e12,",", 
		pBt->GetVar(DURATION_I)*1.e12,",", pBt->GetVar(DURATION_P)*1.e12,
		"          !pulse duration (ps) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(LAMBDA_S)*1.e9,",", 
		pBt->GetVar(LAMBDA_I)*1.e9,",", pBt->GetVar(LAMBDA_P)*1.e9,
		"          !wavelengths (nm) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(INDEX_S),",",		
		pBt->GetVar(INDEX_I),",", pBt->GetVar(INDEX_P),
		"          !index of refraction (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(GVI_S),",", 
		pBt->GetVar(GVI_I),",", pBt->GetVar(GVI_P),
		"          !group velocity index (s,i,p)\n");
	fprintf(stream,"%5.3e %s %5.3e %s %5.3e %s", pBt->GetVar(GVD_S),",", 
		pBt->GetVar(GVD_I),",", pBt->GetVar(GVD_P),
		"          !group velocity dispersion (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(ALPHA_S)*1.e-3,",", 
		pBt->GetVar(ALPHA_I)*1.e-3,",", pBt->GetVar(ALPHA_P)*1.e-3,
		"          !crystal absorption (1/mm) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(RC_IN_S),",", 
		pBt->GetVar(RC_IN_I),",", pBt->GetVar(RC_IN_P),
		"          !crystal input reflectivity (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(RC_OUT_S),",",
		pBt->GetVar(RC_OUT_I),",", pBt->GetVar(RC_OUT_P),
		"          !crystal output reflectivity (s,i,p)\n");
	fprintf(stream,"%5.3f %s", pBt->GetVar(CRYSTLENGTH)*1.e3,
		"                    !crystal length (mm)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(N2_S1)*1.e4,",",
		pBt->GetVar(N2_S2)*1.e4,",", pBt->GetVar(N2_S3)*1.e4,
		"          !n-squared signal (sq cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(N2_I1)*1.e4,",",
		pBt->GetVar(N2_I2)*1.e4,",", pBt->GetVar(N2_I3)*1.e4,
		"          !n-squared idler (sq cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(N2_P1)*1.e4,",",
		pBt->GetVar(N2_P2)*1.e4,",", pBt->GetVar(N2_P3)*1.e4,
		"          !n-squared pump (sq cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(BETA_S1)*2*1.e2,",",
		pBt->GetVar(BETA_S2)*2*1.e2,",", pBt->GetVar(BETA_S3)*2*1.e2,
		"          !beta signal (cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(BETA_I1)*2*1.e2,",",
		pBt->GetVar(BETA_I2)*2*1.e2,",", pBt->GetVar(BETA_I3)*2*1.e2,
		"          !beta idler (cm/W)\n");
	fprintf(stream,"%5.2e %s %5.2e %s %5.2e %s", pBt->GetVar(BETA_P1)*2*1.e2,",",
		pBt->GetVar(BETA_P2)*2*1.e2,",", pBt->GetVar(BETA_P3)*2*1.e2,
		"          !beta pump (cm/W)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(RHO_S)*1.e3,",", 
		pBt->GetVar(RHO_I)*1.e3,",", pBt->GetVar(RHO_P)*1.e3,
		"          !walkoff angles (mrad) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(RADCURV_S)*1.e3,",", 
		pBt->GetVar(RADCURV_I)*1.e3,",", pBt->GetVar(RADCURV_P)*1.e3,
		"          !radius of curvature (mm in air) (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s", pBt->GetVar(OFFSET_S)*1.e3,",", 
		pBt->GetVar(OFFSET_I)*1.e3,"          !offset in wo direction (mm) (s,i,p)\n");
	fprintf(stream,"%d %s %d %s %d %s", long(pBt->GetVar(GAUSSINDEX_S)),",", 
		long(pBt->GetVar(GAUSSINDEX_I)),",", long(pBt->GetVar(GAUSSINDEX_P)),
		"          !Gaussian index order (s,i,p)\n");
	fprintf(stream,"%5.3f %s %5.3f %s %5.3f %s", pBt->GetVar(PHI_S),",", 
		pBt->GetVar(PHI_I),",", pBt->GetVar(PHI_P),
		"          !phase of s,i,p (rad)\n");
	fprintf(stream,"%5.3f %s %5.3f %s", pBt->GetVar(DELAY_S)*1.e12,",", pBt->GetVar(DELAY_I)*1.e12,
		"          !time delay (s,i) rel. to pump (ps)\n");
	fprintf(stream,"%d %s %d %s %d %s %d %s", long(pBt->GetVar(NX)),",", 
		long(pBt->GetVar(NY)),",", long(pBt->GetVar(NT)),",", long(pBt->GetVar(NZ)),
		"          !number of grid points (x,y,t,z)\n");
	fprintf(stream,"%5.2e %s %5.2e %s", pBt->GetVar(MAXGRID_X)*1.e3,",", pBt->GetVar(MAXGRID_Y)*1.e3,
		"          !maximum grid extent (mm) (x,y)\n");
	fprintf(stream,"%6.4f %s", pBt->GetVar(DIST)*1.e3,
		"                   ! distance to detector (mm)\n");
	
	fclose(stream);

}	// End SetInputForm

//************************ Function WriteOut **************************
//
// Function WriteOut creates the file '.dat' and writes the 
// input parameters to the output file
//
// Called from: GraphDlg::OnRunBtn		
//
//*********************************************************************

void WriteOut(CInputs* pBt)
{
	FILE* stream;
	CString fileout = "Mix_out.dat";    
	stream = fopen(fileout, "w");
	
	fprintf(stream,"%s %7.3f %s %7.3f %s %7.3f %s %d %s %7.3f %s",
           " d effective (pm/V)                    = ", pBt->GetVar(DEFF),
		 "\n deltak (1/mm) kp-ks-ki                = ", pBt->GetVar(DELTAK),
		 "\n Crystal Length (mm)/Number of z steps = ", pBt->GetVar(CRYSTLENGTH),
		 "  /  ",long(pBt->GetVar(NZ)),
		 "\n Distance to the detector (mm)         = ", pBt->GetVar(DIST),
		 "\n\n");

	fprintf(stream,"%s",
	 "                                           Signal      Idler      Pump\n");

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
     " Wavelength (nm)                       = ",pBt->GetVar(LAMBDA_S),
	 pBt->GetVar(LAMBDA_I), pBt->GetVar(LAMBDA_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
     "\n Index of refraction                   = ",pBt->GetVar(INDEX_S),
	 pBt->GetVar(INDEX_I), pBt->GetVar(INDEX_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
     "\n Group velocity index (gvi)            = ",pBt->GetVar(GVI_S), 
	 pBt->GetVar(GVI_I), pBt->GetVar(GVI_P));

    fprintf(stream,"%s %7.2e %10.2e %9.2e",
     "\n Group velocity dispersion (gvd)       = ",pBt->GetVar(GVD_S), 
	 pBt->GetVar(GVD_I), pBt->GetVar(GVD_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
	 "\n Phase (radians)                       = ",pBt->GetVar(PHASE_S),
	 pBt->GetVar(PHASE_I),pBt->GetVar(PHASE_P));

    fprintf(stream,"%s %7.3f %10.3f %9.3f",
     "\n Crystal input reflectivity            = ",pBt->GetVar(RC_IN_S),
	 pBt->GetVar(RC_IN_I), pBt->GetVar(RC_IN_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
	 "\n Crystal output reflectivity           = ",pBt->GetVar(RC_OUT_S),
	 pBt->GetVar(RC_OUT_I), pBt->GetVar(RC_OUT_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
     "\n Crystal absorption (1/mm)             = ",pBt->GetVar(ALPHA_S),
	 pBt->GetVar(ALPHA_I), pBt->GetVar(ALPHA_P));

    fprintf(stream,"%s %7.2e %10.2e %9.2e",
     "\n Pulse energy (J)                      = ",pBt->GetVar(ENERGY_S),
	 pBt->GetVar(ENERGY_I), pBt->GetVar(ENERGY_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
     "\n Pulse duration (ps)                   = ",pBt->GetVar(DURATION_S),
	 pBt->GetVar(DURATION_I), pBt->GetVar(DURATION_P));

	fprintf(stream,"%s %7.3f %10.3f",
	 "\n Pulse delay relative to pump (ps)     = ",pBt->GetVar(DELAY_S),
	 pBt->GetVar(DELAY_I));

	fprintf(stream,"%s %7.2e %10.2e %9.2e",
     "\n Beam Diameter (mm)                    = ",pBt->GetVar(BEAMDIAM_S),
	 pBt->GetVar(BEAMDIAM_I), pBt->GetVar(BEAMDIAM_P));

    fprintf(stream,"%s %7.3f %10.3f %9.3f",
	 "\n Walkoff angle (mrad)                  = ",pBt->GetVar(RHO_S),
	 pBt->GetVar(RHO_I), pBt->GetVar(RHO_P));

	fprintf(stream,"%s %7.3f %10.3f",
	 "\n Beam offset relative to pump (mm)     = ",pBt->GetVar(OFFSET_S),
	 pBt->GetVar(OFFSET_I));

	fprintf(stream,"%s %7.2e %10.2e %9.2e",
	 "\n Radius of curvature (mm in air)       = ",pBt->GetVar(RADCURV_S),
	 pBt->GetVar(RADCURV_I), pBt->GetVar(RADCURV_P));

	fprintf(stream,"%s %7.3f %10.3f %9.3f",
	 "\n Gaussian index order                  = ",pBt->GetVar(GAUSSINDEX_S),
	 pBt->GetVar(GAUSSINDEX_I), pBt->GetVar(GAUSSINDEX_P));

	fprintf(stream,"%s %7.3f %10.3f",
	 "\n Maximum grid extent (mm in x,y)       = ",pBt->GetVar(MAXGRID_X),
	pBt->GetVar(MAXGRID_Y));

	fprintf(stream,"%s %3d %10d %10d",
	 "\n Number of grid points (x,y,t)         =   ",long(pBt->GetVar(NX)),
	 long(pBt->GetVar(NY)),long(pBt->GetVar(NT)));

	fclose(stream);

}	// End WriteOut


//*********************** Function ConvertToSI ************************
//
// ConvertToSI will take the initial parameters from the input file 
// and convert them into SI (MKS) units 
//
// Called from: GraphDlg::OnRunBtn		
//
//********************************************************************* 

void ConvertToSI(CInputs* pBt)
{
	// Beam diameters
	pBt->SetVar(BEAMDIAM_S,pBt->GetVar(BEAMDIAM_S)*1.e-3); // from mm to m
	pBt->SetVar(BEAMDIAM_I,pBt->GetVar(BEAMDIAM_I)*1.e-3);	
	pBt->SetVar(BEAMDIAM_P,pBt->GetVar(BEAMDIAM_P)*1.e-3); 

	// Pulse duration
	pBt->SetVar(DURATION_S,pBt->GetVar(DURATION_S)*1.e-12); // from ps to sec
	pBt->SetVar(DURATION_I,pBt->GetVar(DURATION_I)*1.e-12);		
	pBt->SetVar(DURATION_P,pBt->GetVar(DURATION_P)*1.e-12);

	// Wavelengths
	pBt->SetVar(LAMBDA_S,pBt->GetVar(LAMBDA_S)*1.e-9);  // from nm to m
	pBt->SetVar(LAMBDA_I,pBt->GetVar(LAMBDA_I)*1.e-9);		
	pBt->SetVar(LAMBDA_P,pBt->GetVar(LAMBDA_P)*1.e-9);	

	// Crystal absorption
	pBt->SetVar(ALPHA_S,pBt->GetVar(ALPHA_S)*1.e+3);	// from 1/mm to 1/m
	pBt->SetVar(ALPHA_I,pBt->GetVar(ALPHA_I)*1.e+3);
	pBt->SetVar(ALPHA_P,pBt->GetVar(ALPHA_P)*1.e+3);

	// Deff and Deltak
	pBt->SetVar(DEFF,pBt->GetVar(DEFF)*1.e-12);		// from pm/V to m/V
	pBt->SetVar(DELTAK,pBt->GetVar(DELTAK)*1.e+3);	// from 1/mm to 1/m

	// Crystal length 
	pBt->SetVar(CRYSTLENGTH,pBt->GetVar(CRYSTLENGTH)*1.e-3);	// from mm to m

	// Group velocity dispersion
//	pBt->SetVar(GVD_S,pBt->GetVar(GVD_S)*1.e-4);	// from cm^2/s to m^2/s 
//	pBt->SetVar(GVD_I,pBt->GetVar(GVD_I)*1.e-4);
//	pBt->SetVar(GVD_P,pBt->GetVar(GVD_P)*1.e-4);

	// n-squared signal
	pBt->SetVar(N2_S1,pBt->GetVar(N2_S1)*1.e-4);	// from cm^2/W to m^2/W
	pBt->SetVar(N2_S2,pBt->GetVar(N2_S2)*1.e-4);
	pBt->SetVar(N2_S3,pBt->GetVar(N2_S3)*1.e-4);

	// n-squared idler
	pBt->SetVar(N2_I1,pBt->GetVar(N2_I1)*1.e-4);	// from cm^2/W to m^2/W
	pBt->SetVar(N2_I2,pBt->GetVar(N2_I2)*1.e-4);
	pBt->SetVar(N2_I3,pBt->GetVar(N2_I3)*1.e-4);

	// n-squared pump
	pBt->SetVar(N2_P1,pBt->GetVar(N2_P1)*1.e-4);	// from cm^2/W to m^2/W
	pBt->SetVar(N2_P2,pBt->GetVar(N2_P2)*1.e-4);
	pBt->SetVar(N2_P3,pBt->GetVar(N2_P3)*1.e-4);

	// Beta signal
	pBt->SetVar(BETA_S1,pBt->GetVar(BETA_S1)*1.e-2*0.5);	// from cm/W to m/W
	pBt->SetVar(BETA_S2,pBt->GetVar(BETA_S2)*1.e-2*0.5);
	pBt->SetVar(BETA_S3,pBt->GetVar(BETA_S3)*1.e-2*0.5);

	// Beta idler
	pBt->SetVar(BETA_I1,pBt->GetVar(BETA_I1)*1.e-2*0.5);	// from cm/W to m/W
	pBt->SetVar(BETA_I2,pBt->GetVar(BETA_I2)*1.e-2*0.5);
	pBt->SetVar(BETA_I3,pBt->GetVar(BETA_I3)*1.e-2*0.5);

	// Beta pump
	pBt->SetVar(BETA_P1,pBt->GetVar(BETA_P1)*1.e-2*0.5);	// from cm/W to m/W
	pBt->SetVar(BETA_P2,pBt->GetVar(BETA_P2)*1.e-2*0.5);
	pBt->SetVar(BETA_P3,pBt->GetVar(BETA_P3)*1.e-2*0.5);

	// Walkoff angles
	pBt->SetVar(RHO_S,pBt->GetVar(RHO_S)*1.e-3);	// from mrad to radians
	pBt->SetVar(RHO_I,pBt->GetVar(RHO_I)*1.e-3);
	pBt->SetVar(RHO_P,pBt->GetVar(RHO_P)*1.e-3);

	// Radius of curvature
	pBt->SetVar(RADCURV_S,pBt->GetVar(RADCURV_S)*1.e-3);	// from mm to m
	pBt->SetVar(RADCURV_I,pBt->GetVar(RADCURV_I)*1.e-3);
	pBt->SetVar(RADCURV_P,pBt->GetVar(RADCURV_P)*1.e-3);

	// Offset in walkoff direction
	pBt->SetVar(OFFSET_S,pBt->GetVar(OFFSET_S)*1.e-3);	// from mm to m
	pBt->SetVar(OFFSET_I,pBt->GetVar(OFFSET_I)*1.e-3);

	// Maximum grid extent
	pBt->SetVar(MAXGRID_X,pBt->GetVar(MAXGRID_X)*1.e-3);	// from mm to m
	pBt->SetVar(MAXGRID_Y,pBt->GetVar(MAXGRID_Y)*1.e-3);

	// Time delay rel. to pump
	pBt->SetVar(DELAY_S,pBt->GetVar(DELAY_S)*1.e-12);		// from ps to sec
	pBt->SetVar(DELAY_I,pBt->GetVar(DELAY_I)*1.e-12);

	// Distance to detector
	pBt->SetVar(DIST,pBt->GetVar(DIST)*1.e-3);				// from mm to m

}	// End ConvertToSI


//********************** Function SetGrids ****************************
//
// Function SetGrids sets the values for the x,y, and t grids
// and calculates the peak power, irradiance, and fluence for
// the signal, idler, and pump
//
// Called from: GraphDlg::OnRunBtn
//
//*********************************************************************

void SetGrids(CInputs* pBt,CGlobalVar* pGV,CFields* pField)
{
	const double c = 2.99792458e+8;	// Speed of light in vacuum [m/s]
	const double Pi = 3.14159265359;


//*********************************************************************
// Global variables from CInputs and CGlobalVar

	double Energy_s = pBt->GetVar(ENERGY_S);		// Energy [J]
	double Energy_i = pBt->GetVar(ENERGY_I);
	double Energy_p = pBt->GetVar(ENERGY_P);

	double BeamDiam_s = pBt->GetVar(BEAMDIAM_S);	// beam diameters [m]
	double BeamDiam_i = pBt->GetVar(BEAMDIAM_I);
	double BeamDiam_p = pBt->GetVar(BEAMDIAM_P);

	double Offset_s = pBt->GetVar(OFFSET_S);		// offset in wo direction [m]
	double Offset_i = pBt->GetVar(OFFSET_I);

	double Rho_s = pBt->GetVar(RHO_S);				// walkoff angles [rad]
	double Rho_i = pBt->GetVar(RHO_I);
	double Rho_p = pBt->GetVar(RHO_P);

	double Lambda_s = pBt->GetVar(LAMBDA_S);		// wavelengths [m]
	double Lambda_i = pBt->GetVar(LAMBDA_I);
	double Lambda_p = pBt->GetVar(LAMBDA_P);

	double Crystlength = pBt->GetVar(CRYSTLENGTH);	// crystal length [m]

	double gvi_s = pBt->GetVar(GVI_S);				// group velocity index
	double gvi_i = pBt->GetVar(GVI_I);
	double gvi_p = pBt->GetVar(GVI_P);

	double gvd_s = pBt->GetVar(GVD_S);				// group velocity dispersion
	double gvd_i = pBt->GetVar(GVD_I);
	double gvd_p = pBt->GetVar(GVD_P);

	long superg_s = long(pBt->GetVar(GAUSSINDEX_S));
	long superg_i = long(pBt->GetVar(GAUSSINDEX_I));
	long superg_p = long(pBt->GetVar(GAUSSINDEX_P));

	double Duration_s = pBt->GetVar(DURATION_S);	// pulse duration [sec]
	double Duration_i = pBt->GetVar(DURATION_I);
	double Duration_p = pBt->GetVar(DURATION_P);

	double delay_s = pBt->GetVar(DELAY_S);		
	double delay_i = pBt->GetVar(DELAY_I);
	double delay_p = 0.0;

	double nx = pBt->GetVar(NX);					// grid points in x & y
	double ny = pBt->GetVar(NY);
	double nt = pBt->GetVar(NT);
	double nz = pBt->GetVar(NZ);					// number of z-steps

	double Xmax = pBt->GetVar(MAXGRID_X);			// max. grid extent [m]
	double Ymax = pBt->GetVar(MAXGRID_Y);

	// Local variables
	int i,j,k;
	bool ChangeInputs;
	double dx, dy, dt, dz;			// step sizes in x,y,t,z [m]
	double lpos, rpos;			// left and right grid positions
	double Omega_s,Omega_i,Omega_p;		// frequencies [rad/s]
	double peakIr_s,peakIr_i,peakIr_p;	// input peak irradiances [W/m^2]
	double peakFl_s,peakFl_i,peakFl_p;	// input peak fluences [J/m^2]
	double peakPow_s,peakPow_i,peakPow_p;	// input peak power (W)
	
	// Variables for graphing
	long ngridx, ngridy, padval;
	double dthetax, dthetay;
	double Originx_fl, Originy_fl;

	ChangeInputs = false;


//*********************************************************************
// Calculate x,y,t grids (including x,y setup if the grid extent is 0)

	// x grid
	double l_s,l_i,l_p;
	double r_s,r_i,r_p;

	if(Xmax == 0)
	{
		l_s = MIN2((Crystlength*Rho_s)+Offset_s-2*BeamDiam_s,(Offset_s-2.5*BeamDiam_s));
		l_i = MIN2((Crystlength*Rho_i)+Offset_i-2*BeamDiam_i,(Offset_i-2.5*BeamDiam_i));
		l_p = MIN2((Crystlength*Rho_p)-2*BeamDiam_p,(-2.5*BeamDiam_p));
	
		lpos = MIN(l_s,l_i,l_p);

		r_s = MAX2((Crystlength*Rho_s)+Offset_s+2*BeamDiam_s,(Offset_s+2.5*BeamDiam_s));
		r_i = MAX2((Crystlength*Rho_i)+Offset_i+2*BeamDiam_i,(Offset_i+2.5*BeamDiam_i));
		r_p = MAX2((Crystlength*Rho_p)+2*BeamDiam_p,(+2.5*BeamDiam_p));

		rpos = MAX(r_s,r_i,r_p);

		Xmax = rpos - lpos;
		dx = Xmax/(nx - 1);

		for(i = 0; i<nx; ++i)
		{
			pField->fSetField(XGRID,i+1,lpos + i*dx);
		}

		// Set the maxgrid variable to the new value
		pBt->SetVar(MAXGRID_X,Xmax);
		ChangeInputs = true;
	
	}
	else
	{
		l_s = MIN2((Crystlength*Rho_s)+Offset_s-2*BeamDiam_s,(Offset_s-2.5*BeamDiam_s));
		l_i = MIN2((Crystlength*Rho_i)+Offset_i-2*BeamDiam_i,(Offset_i-2.5*BeamDiam_i));
		l_p = MIN2((Crystlength*Rho_p)-2*BeamDiam_p,(-2.5*BeamDiam_p));
	
		lpos = MIN(l_s,l_i,l_p);

		r_s = MAX2((Crystlength*Rho_s)+Offset_s+2*BeamDiam_s,(Offset_s+2.5*BeamDiam_s));
		r_i = MAX2((Crystlength*Rho_i)+Offset_i+2*BeamDiam_i,(Offset_i+2.5*BeamDiam_i));
		r_p = MAX2((Crystlength*Rho_p)+2*BeamDiam_p,(+2.5*BeamDiam_p));

		rpos = MAX(r_s,r_i,r_p);

		lpos = lpos - 0.5*(Xmax - (rpos - lpos));
		dx = Xmax/(nx - 1);

		for(i = 0; i<nx; ++i)
		{
			pField->fSetField(XGRID,i+1,lpos + i*dx);
		}

	}

	// y grid
	if(Ymax == 0)
	{
		Ymax = 4*(MAX(BeamDiam_s,BeamDiam_i,BeamDiam_p));
		dy = Ymax/(ny - 1);
		
		for(j = 0; j<ny; ++j)
		{
			pField->fSetField(YGRID,j+1,(-0.5*Ymax) + j*dy);
		}

		// Set the maxgrid variable to the new value
		pBt->SetVar(MAXGRID_Y,Ymax);
		ChangeInputs = true;
	}
	else
	{
		dy = Ymax/(ny - 1);
		
		for(j = 0; j<ny; ++j)
		{
			pField->fSetField(YGRID,j+1,(-0.5*Ymax) + j*dy);
		}
	}

	// Set the new origins for graphing the fluence
	Originx_fl = -0.5*(nx - 1)*dx;
	Originy_fl = -0.5*(ny - 1)*dy;

	// Compare dx and dy to find ratios for graphing
	if(Xmax > Ymax)
	{
		ngridx = nx;
		ngridy = long(ny*(Xmax/Ymax));

		padval = long((ngridy - ny)/2);

		Originy_fl = Originy_fl - padval*dy;

	}
	else if(Xmax <= Ymax)
	{
		ngridx = long(nx*(Ymax/Xmax));
		ngridy = ny;

		padval = long((ngridx - nx)/2);

		Originx_fl = Originx_fl - padval*dx;
	}

	// Set dthetax and dthetay for the farfield
	dthetax = double((nx-1)/(nx*Xmax));
	dthetay = double((ny-1)/(ny*Ymax));
	
	// time grid
	double tgvd,tgvd_s,tgvd_i,tgvd_p;

	lpos = MIN(((Crystlength*(gvi_s - gvi_p))/c)+(delay_s-2*Duration_s),
			  ((Crystlength*(gvi_i - gvi_p))/c)+(delay_i-2*Duration_i),
			  ((Crystlength*(gvi_p - gvi_p))/c)+(delay_p-2*Duration_p));

	rpos = MAX(((Crystlength*(gvi_s - gvi_p))/c)+(delay_s+2*Duration_s),
			  ((Crystlength*(gvi_i - gvi_p))/c)+(delay_i+2*Duration_i),
			  ((Crystlength*(gvi_p - gvi_p))/c)+(delay_p+2*Duration_p));

	tgvd_s = (gvd_s*Crystlength*pow(gvi_s,2))/((6.e+29)*Duration_s);
	if(tgvd_s < 0)
		tgvd_s = -tgvd_s;

	tgvd_i = (gvd_i*Crystlength*pow(gvi_i,2))/((6.e+29)*Duration_i);
	if(tgvd_i < 0)
		tgvd_i = -tgvd_i;

	tgvd_p = (gvd_p*Crystlength*pow(gvi_p,2))/((6.e+29)*Duration_p);
	if(tgvd_p < 0)
		tgvd_p = -tgvd_p;

	tgvd = MAX(tgvd_s,tgvd_i,tgvd_p);

	lpos = lpos - tgvd;
	rpos = rpos + tgvd;
	dt = (rpos - lpos)/(nt - 1);

	for(k=0; k<nt; ++k)
	{
		pField->fSetField(TGRID,k+1,lpos + k*dt);
	}
	
		
//*********************************************************************
// Compute input peak fluence, irradiance, and power

	double I_term = -4*log(0.5)/Pi;

	// peak power [W]
	peakPow_s = (Energy_s*sqrt(I_term))/Duration_s;
	peakPow_i = (Energy_i*sqrt(I_term))/Duration_i;
	peakPow_p = (Energy_p*sqrt(I_term))/Duration_p;

	// peak irradiance [J/m^2]
	peakIr_s = (peakPow_s*2*pow(log(2.0),(1/superg_s)))/(Pi*pField->fGetField(SGCOEFF,superg_s)*pow(BeamDiam_s,2));
	peakIr_i = (peakPow_i*2*pow(log(2.0),(1/superg_i)))/(Pi*pField->fGetField(SGCOEFF,superg_i)*pow(BeamDiam_i,2));
	peakIr_p = (peakPow_p*2*pow(log(2.0),(1/superg_p)))/(Pi*pField->fGetField(SGCOEFF,superg_p)*pow(BeamDiam_p,2));

	// peak fluence [W/m^2]
	peakFl_s = peakIr_s*Duration_s*sqrt(1/I_term);
	peakFl_i = peakIr_i*Duration_i*sqrt(1/I_term);
	peakFl_p = peakIr_p*Duration_p*sqrt(1/I_term);

	
//*********************************************************************
// Write the input fluence ,irradiance, and power to the output file
	
	FILE* stream;

	if(m_bDeltakOn == TRUE)
	{
		stream=fopen("Mix_outDK.dat","a+");
		
		fprintf(stream,"%s","\n\n");

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Irradiance (W/m^2)         = ",peakIr_s,
			peakIr_i, peakIr_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Fluence (J/m^2)            = ",peakFl_s,
			peakFl_i, peakFl_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Power (W)                  = ",peakPow_s,
			peakPow_i, peakPow_p);
		
		fclose(stream);
	}

	if(m_bErgOn == TRUE)
	{
		stream=fopen("Mix_outERG.dat","a+");
		
		fprintf(stream,"%s","\n\n");

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Irradiance (W/m^2)         = ",peakIr_s,
			peakIr_i, peakIr_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Fluence (J/m^2)            = ",peakFl_s,
			peakFl_i, peakFl_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Power (W)                  = ",peakPow_s,
			peakPow_i, peakPow_p);
		
		fclose(stream);
	}

	if(m_bLensOn == TRUE)
	{
		stream=fopen("Mix_outLENS.dat","a+");
		
		fprintf(stream,"%s","\n\n");

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Irradiance (W/m^2)         = ",peakIr_s,
			peakIr_i, peakIr_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Fluence (J/m^2)            = ",peakFl_s,
			peakFl_i, peakFl_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Peak Power (W)                  = ",peakPow_s,
			peakPow_i, peakPow_p);
		
		fclose(stream);
	}

	stream=fopen("Mix_out.dat","a+");

	fprintf(stream,"%s","\n\n");

	fprintf(stream,"%s %7.3e %10.3e %9.3e",
	 "\n Input Peak Irradiance (W/m^2)         = ",peakIr_s,
	 peakIr_i, peakIr_p);

	fprintf(stream,"%s %7.3e %10.3e %9.3e",
	 "\n Input Peak Fluence (J/m^2)            = ",peakFl_s,
	 peakFl_i, peakFl_p);

	fprintf(stream,"%s %7.3e %10.3e %9.3e",
	 "\n Input Peak Power (W)                  = ",peakPow_s,
	 peakPow_i, peakPow_p);

	fclose(stream);

	
//*********************************************************************
// Calculate omega
	
	Omega_s = (2*Pi*c)/Lambda_s;
	Omega_i = (2*Pi*c)/Lambda_i;
	Omega_p = (2*Pi*c)/Lambda_p;

	dz = Crystlength/nz;	// z step in crystal [m]


//*********************************************************************
// Change values of global variables to their new values

	if(ChangeInputs)
	{
		SetInputForm(pBt);
	}
		
	// CGlobalVar variables
	pGV->dSetGlobal(OMEGA_S,Omega_s);
	pGV->dSetGlobal(OMEGA_I,Omega_i);
	pGV->dSetGlobal(OMEGA_P,Omega_p);
	
	pGV->dSetGlobal(DX,dx);
	pGV->dSetGlobal(DY,dy);
	pGV->dSetGlobal(DT,dt);
	pGV->dSetGlobal(DZ,dz);

	pGV->dSetGlobal(I_TERM,I_term);

	pGV->dSetGlobal(PEAKPOW_S, peakPow_s);
	pGV->dSetGlobal(PEAKPOW_I, peakPow_i);
	pGV->dSetGlobal(PEAKPOW_P, peakPow_p);

	pGV->dSetGlobal(PEAKIR_S, peakIr_s);
	pGV->dSetGlobal(PEAKIR_I, peakIr_i);
	pGV->dSetGlobal(PEAKIR_P, peakIr_p);

	pGV->dSetGlobal(PEAKFL_S, peakFl_s);
	pGV->dSetGlobal(PEAKFL_I, peakFl_i);
	pGV->dSetGlobal(PEAKFL_P, peakFl_p);

	pGV->lSetGlobal(NGRIDX, ngridx);
	pGV->lSetGlobal(NGRIDY, ngridy);
	pGV->lSetGlobal(PADVAL, padval);

	pGV->dSetGlobal(DTHETAX, dthetax);
	pGV->dSetGlobal(DTHETAY, dthetay);

	pGV->dSetGlobal(ORIGINX_FL, Originx_fl);
	pGV->dSetGlobal(ORIGINY_FL, Originy_fl);

	pGV->ulSetGlobal(NN,1,pBt->GetVar(NX));
	pGV->ulSetGlobal(NN,2,pBt->GetVar(NY));
	pGV->ulSetGlobal(NN,3,pBt->GetVar(NT));
	
}	// End SetGrids


//********************* Function InitializeFields *********************
// Sets the initial electric fields. 
//
// Called from: GraphDlg::OnRun() 
//*********************************************************************

void InitializeFields(CInputs* pBt, CGlobalVar* pGV, CFields* pField)
{
	const double Pi = 3.14159265359;
	const double epsilon_0 = 8.854187817e-12;	// Permittivity of vacuum [F/m]
	const double c = 2.99792458e+8;				// Speed of light in vacuum [m/s]


//*********************************************************************
// Set local variables to their corresponding global variable values

	// CInputs variables
	double Energy_s = pBt->GetVar(ENERGY_S);		// Energy [J]
	double Energy_i = pBt->GetVar(ENERGY_I);
	double Energy_p = pBt->GetVar(ENERGY_P);

	double Duration_s = pBt->GetVar(DURATION_S);	// Pulse duration [sec]
	double Duration_i = pBt->GetVar(DURATION_I);
	double Duration_p = pBt->GetVar(DURATION_P);

	double Lambda_s = pBt->GetVar(LAMBDA_S);		// Wavelengths [m]
	double Lambda_i = pBt->GetVar(LAMBDA_I);
	double Lambda_p = pBt->GetVar(LAMBDA_P);

	double BeamDiam_s = pBt->GetVar(BEAMDIAM_S);	// Beam diameters [m]
	double BeamDiam_i = pBt->GetVar(BEAMDIAM_I);
	double BeamDiam_p = pBt->GetVar(BEAMDIAM_P);

	double RadCurv_s = pBt->GetVar(RADCURV_S);	// Radius of curvature [m]
	double RadCurv_i = pBt->GetVar(RADCURV_I);
	double RadCurv_p = pBt->GetVar(RADCURV_P);

	double Offset_s = pBt->GetVar(OFFSET_S);	// Offset in wo direction [m]
	double Offset_i = pBt->GetVar(OFFSET_I);

	long superG_s = long(pBt->GetVar(GAUSSINDEX_S));
	long superG_i = long(pBt->GetVar(GAUSSINDEX_I));
	long superG_p = long(pBt->GetVar(GAUSSINDEX_P));

	double Phi_s = pBt->GetVar(PHI_S);			// Phases [rad]
	double Phi_i = pBt->GetVar(PHI_I);
	double Phi_p = pBt->GetVar(PHI_P);

	double Delay_s = pBt->GetVar(DELAY_S);
	double Delay_i = pBt->GetVar(DELAY_I);

	double Crystlength = pBt->GetVar(CRYSTLENGTH);

	double peakIr_s = pGV->dGetGlobal(PEAKIR_S);
	double peakIr_i = pGV->dGetGlobal(PEAKIR_I);
	double peakIr_p = pGV->dGetGlobal(PEAKIR_P);

	long nx = pGV->ulGetGlobal(NN,1);
	long ny = pGV->ulGetGlobal(NN,2);
	long nt = pGV->ulGetGlobal(NN,3);


//*********************************************************************
// Create the fields for x,y,t

	int ii,i2,jj,j2,kk,k2,ll,l2;	// iteration variables

	double Term = -4*log(0.5);	

	double Exponent;
	complex<double> cExponent;

	float *Field_t_s, *Field_t_i, *Field_t_p;
	float *Field_x_s, *Field_x_i, *Field_x_p;
	float *Field_y_s, *Field_y_i, *Field_y_p;

	float *Fieldn_s, *Fieldn_i, *Fieldn_p;

	float *curv_s, *curv_i, *curv_p;
	float *xygrid;

	float *Fxy_s, *Fxy_i, *Fxy_p;
	float *Phase_s, *Phase_i, *Phase_p;
	

//*********************************************************************
// Setup time fields: Field_t_s, Field_t_i, Field_t_p

	Field_t_s = vector(1,nt);
	Field_t_i = vector(1,nt);
	Field_t_p = vector(1,nt);

	for(ii=0; ii<nt; ++ii)
	{
		
		// Signal
		Exponent = sqrt(exp(
			-(pow(pField->fGetField(TGRID,ii+1)-Delay_s,2)*Term)/
			pow(Duration_s,2)));
		
		Field_t_s[ii+1] = Exponent;

		// Idler
		Exponent = sqrt(exp(
			-(pow(pField->fGetField(TGRID,ii+1)-Delay_i,2)*Term)/
			pow(Duration_i,2)));
		
		Field_t_i[ii+1] = Exponent;
		
		// Pump
		Exponent = sqrt(exp(
			-(pow(pField->fGetField(TGRID,ii+1),2)*Term)/
			pow(Duration_p,2)));
		
		Field_t_p[ii+1] = Exponent;
		
	}

	
//*********************************************************************
// Set up x fields: Field_x_s, Field_x_i, Field_x_p

	Field_x_s = vector(1,nx);
	Field_x_i = vector(1,nx);
	Field_x_p = vector(1,nx);

	for(ii=0; ii<nx; ++ii)
	{
		
		// Signal
		Field_x_s[ii+1] = pow(pField->pXgrid[ii+1]-Offset_s,2);
		
		// Idler
		Field_x_i[ii+1] = pow(pField->pXgrid[ii+1]-Offset_i,2);
		
		// Pump
		Field_x_p[ii+1] = pow(pField->pXgrid[ii+1],2);
	}

		
//*********************************************************************
// Set up y fields: Field_y_s, Field_y_i, Field_y_p

	Field_y_s = vector(1,ny);
	Field_y_i = vector(1,ny);
	Field_y_p = vector(1,ny);

	for(ii=0; ii<ny; ++ii)
	{
		
		// Signal
		Field_y_s[ii+1] = pow(pField->pYgrid[ii+1],2);
		
		// Idler
		Field_y_i[ii+1] = pow(pField->pYgrid[ii+1],2);
		
		// Pump
		Field_y_p[ii+1] = pow(pField->pYgrid[ii+1],2);
	}


//*********************************************************************
// Set up the combination x and y fields: Fieldn_s, Fieldn_i, Fieldn_p

	Fieldn_s = vector(1,nx*ny);
	Fieldn_i = vector(1,nx*ny);
	Fieldn_p = vector(1,nx*ny);

	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<ny;++jj)
		{
			j2 = (jj+1) + ii*ny;
			
			// Signal
			Exponent = exp(0.5*(-log(2.0))*pow((1/(0.25*pow(BeamDiam_s,2)))*
				(Field_x_s[ii+1] + Field_y_s[jj+1]),superG_s));

			Fieldn_s[j2] = Exponent;
			
			// Idler
			Exponent = exp(0.5*(-log(2.0))*pow((1/(0.25*pow(BeamDiam_i,2)))*
				(Field_x_i[ii+1] + Field_y_i[jj+1]),superG_i));

			Fieldn_i[j2] = Exponent;

			// Pump
			Exponent = exp(0.5*(-log(2.0))*pow((1/(0.25*pow(BeamDiam_p,2)))*
				(Field_x_p[ii+1] + Field_y_p[jj+1]),superG_p));

			Fieldn_p[j2] = Exponent;
		}
	}

	// Free up memory
	free_vector(Field_x_s,1,nx);
	free_vector(Field_x_i,1,nx);
	free_vector(Field_x_p,1,nx);

	free_vector(Field_y_s,1,ny);
	free_vector(Field_y_i,1,ny);
	free_vector(Field_y_p,1,ny);


//*********************************************************************
// Set up curvature arrays
		
	curv_s = vector(1,2*nx*ny);
	curv_i = vector(1,2*nx*ny);
	curv_p = vector(1,2*nx*ny);

	xygrid = vector(1,nx*ny);

	// curv_s
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			j2 = (jj+1) + ii*ny;
			xygrid[j2] = (-Pi/(Lambda_s*RadCurv_s))*	
			(pow(pField->fGetField(XGRID,ii+1)-Offset_s,2)+	// inner product of
			pow(pField->fGetField(YGRID,jj+1),2));			// xgrid^2 + ygrid^2	

			k2 = 2*j2;

			curv_s[k2-1] = cos(xygrid[j2]);
			curv_s[k2] = sin(xygrid[j2]);
		}
	}

	// curv_i
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			j2 = (jj+1) + ii*ny;
			xygrid[j2] = (-Pi/(Lambda_i*RadCurv_i))*	
			(pow(pField->fGetField(XGRID,ii+1)-Offset_i,2)+	// inner product of
			pow(pField->fGetField(YGRID,jj+1),2));			// xgrid^2 + ygrid^2	

			k2 = 2*j2;

			curv_i[k2-1] = cos(xygrid[j2]);
			curv_i[k2] = sin(xygrid[j2]);
		}
	}


	// curv_p
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			j2 = (jj+1) + ii*ny;
			xygrid[j2] = (-Pi/(Lambda_p*RadCurv_p))*	
			(pow(pField->fGetField(XGRID,ii+1),2)+	// inner product of
			pow(pField->fGetField(YGRID,jj+1),2));	// xgrid^2 + ygrid^2	

			k2 = 2*j2;

			curv_p[k2-1] = cos(xygrid[j2]);
			curv_p[k2] = sin(xygrid[j2]);
		}
	}

	// Free up memory
	free_vector(xygrid,1,nx*ny);


//*********************************************************************
// Set up x,y,t profiles for electric fields

	//	Prepare phases
	complex<double> cnum;
	int cconj = 0;	// no complex conjugate for cmult

	Fxy_s = vector(1,2*nx*ny);
	Fxy_i = vector(1,2*nx*ny);
	Fxy_p = vector(1,2*nx*ny);

	Phase_s = vector(1,2*nx*ny*nt);
	Phase_i = vector(1,2*nx*ny*nt);
	Phase_p = vector(1,2*nx*ny*nt);

	// Signal phase
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			ll = (jj+1) + ii*ny;
			l2 = 2*ll;
			Fxy_s[l2-1] = cos(Phi_s)*Fieldn_s[ll];
			Fxy_s[l2] = sin(Phi_s)*Fieldn_s[ll];

			cnum = cmult(curv_s[l2-1],curv_s[l2],Fxy_s[l2-1],Fxy_s[l2],cconj);

			Fxy_s[l2-1] = cnum.real();
			Fxy_s[l2] = cnum.imag();

			for(kk=0; kk<nt; ++kk)
			{
				j2 = (kk+1) + jj*nt + ii*nt*ny;
				k2 = 2*j2;

				Phase_s[k2-1] = Fxy_s[l2-1]*Field_t_s[kk+1];
				Phase_s[k2] = Fxy_s[l2]*Field_t_s[kk+1];
				
			}

		}	
	}

	// Check for Windows messages, and return from the loop if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Free up memory
	free_vector(Fxy_s,1,2*nx*ny);
	free_vector(curv_s,1,2*nx*ny);
	free_vector(Fieldn_s,1,nx*ny);
	free_vector(Field_t_s,1,nt);

	// Idler phase
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			ll = (jj+1) + ii*ny;
			l2 = 2*ll;
			Fxy_i[l2-1] = cos(Phi_i)*Fieldn_i[ll];
			Fxy_i[l2] = sin(Phi_i)*Fieldn_i[ll];

			cnum = cmult(curv_i[l2-1],curv_i[l2],Fxy_i[l2-1],Fxy_i[l2],cconj);

			Fxy_i[l2-1] = cnum.real();
			Fxy_i[l2] = cnum.imag();

			for(kk=0; kk<nt; ++kk)
			{
				j2 = (kk+1) + jj*nt + ii*nt*ny;
				k2 = 2*j2;

				Phase_i[k2-1] = Fxy_i[l2-1]*Field_t_i[kk+1];
				Phase_i[k2] = Fxy_i[l2]*Field_t_i[kk+1];
				
			}

		}	
	}

	// Check for Windows messages, and return from the loop if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Free up memory
	free_vector(Fxy_i,1,2*nx*ny);
	free_vector(curv_i,1,2*nx*ny);
	free_vector(Fieldn_i,1,nx*ny);
	free_vector(Field_t_i,1,nt);
	
	// Pump phase
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			ll = (jj+1) + ii*ny;
			l2 = 2*ll;
			Fxy_p[l2-1] = cos(Phi_p)*Fieldn_p[ll];
			Fxy_p[l2] = sin(Phi_p)*Fieldn_p[ll];

			cnum = cmult(curv_p[l2-1],curv_p[l2],Fxy_p[l2-1],Fxy_p[l2],cconj);

			Fxy_p[l2-1] = cnum.real();
			Fxy_p[l2] = cnum.imag();

			for(kk=0; kk<nt; ++kk)
			{
				j2 = (kk+1) + jj*nt + ii*nt*ny;
				k2 = 2*j2;

				Phase_p[k2-1] = Fxy_p[l2-1]*Field_t_p[kk+1];
				Phase_p[k2] = Fxy_p[l2]*Field_t_p[kk+1];
				
			}

		}	
	}

	// Check for Windows messages, and return from the loop if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Free up memory
	free_vector(Fxy_p,1,2*nx*ny);
	free_vector(curv_p,1,2*nx*ny);
	free_vector(Fieldn_p,1,nx*ny);
	free_vector(Field_t_p,1,nt);
	

//*********************************************************************
// Set up electric fields

	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + kk*nt;
				l2 = 2*(ll+1);

				pField->pEField_s[l2-1] = sqrt(peakIr_s*2/(epsilon_0*c))*Phase_s[i2-1];
				pField->pEField_s[l2] = sqrt(peakIr_s*2/(epsilon_0*c))*Phase_s[i2];

				pField->pEField_i[l2-1] = sqrt(peakIr_i*2/(epsilon_0*c))*Phase_i[i2-1];
				pField->pEField_i[l2] = sqrt(peakIr_i*2/(epsilon_0*c))*Phase_i[i2];

				pField->pEField_p[l2-1] = sqrt(peakIr_p*2/(epsilon_0*c))*Phase_p[i2-1];
				pField->pEField_p[l2] = sqrt(peakIr_p*2/(epsilon_0*c))*Phase_p[i2];
			}
		}
	}

	// Check for Windows messages, and return from the loop if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	
//*********************************************************************
// Free up memory

	free_vector(Phase_s,1,2*nx*ny*nt);
	free_vector(Phase_i,1,2*nx*ny*nt);
	free_vector(Phase_p,1,2*nx*ny*nt);

}	// End InitializeFields


//******************** Function InitializePhases **********************
//
// Sets the initial phases. 
//
// Called from:	GraphDlg::OnRunBtn	
//
//*********************************************************************

void InitializePhases(CInputs* pBt, CGlobalVar* pGV, CFields* pField)
{
	const double Pi = 3.14159265359;
	const double epsilon_0 = 8.854187817e-12;	// Permittivity of vacuum [F/m]
	const double c = 2.99792458e+8;				// Speed of light in vacuum [m/s]
 

//*********************************************************************
// Set local variables to their corresponding global variable values

	// CInputs variables
	double Index_s = pBt->GetVar(INDEX_S);	// index of refraction
	double Index_i = pBt->GetVar(INDEX_I);
	double Index_p = pBt->GetVar(INDEX_P);

	double gvi_s = pBt->GetVar(GVI_S);		// group velocity index
	double gvi_i = pBt->GetVar(GVI_I);
	double gvi_p = pBt->GetVar(GVI_P);

	double gvd_s = pBt->GetVar(GVD_S);		// group velocity dispersion
	double gvd_i = pBt->GetVar(GVD_I);
	double gvd_p = pBt->GetVar(GVD_P);

	double Lambda_s = pBt->GetVar(LAMBDA_S);	// wavelengths [m]
	double Lambda_i = pBt->GetVar(LAMBDA_I);
	double Lambda_p = pBt->GetVar(LAMBDA_P);

	double Rho_s = pBt->GetVar(RHO_S);		// walkoff angles [rad]
	double Rho_i = pBt->GetVar(RHO_I);
	double Rho_p = pBt->GetVar(RHO_P);

	// CGlobalVar variables
	double dx = pGV->dGetGlobal(DX);		// step sizes in x,y,t,z [m]
	double dy = pGV->dGetGlobal(DY);
	double dt = pGV->dGetGlobal(DT);
	double dz = pGV->dGetGlobal(DZ);

	long nx = pGV->ulGetGlobal(NN,1);
	long ny = pGV->ulGetGlobal(NN,2);
	long nt = pGV->ulGetGlobal(NN,3);

	int ii,jj,kk,k2;


//*********************************************************************
// Set up the group velocity phase

	double gvdfactor_s, gvdfactor_i, gvdfactor_p;
	double gvifactor_s, gvifactor_i, gvifactor_p;

	float *phase_g, *phase_x, *phase_xwo, *phase_y;
	float *phasegv_s, *phasegv_i, *phasegv_p;
	float *phasex_s, *phasex_i, *phasex_p;
	float *phasey_s, *phasey_i, *phasey_p;

	phase_g = vector(1,nt);
	
	phasegv_s = vector(1,nt);
	phasegv_i = vector(1,nt);
	phasegv_p = vector(1,nt);
	
	gvdfactor_s = -(0.0001*gvd_s*Pi*pow(gvi_s,2))/(c*pow(c*nt*dt,2));
	gvdfactor_i = -(0.0001*gvd_i*Pi*pow(gvi_i,2))/(c*pow(c*nt*dt,2));
	gvdfactor_p = -(0.0001*gvd_p*Pi*pow(gvi_p,2))/(c*pow(c*nt*dt,2));

	for(ii=0; ii<nt; ++ii)
	{
		if( ii<=(0.5*nt) )
		{
			phase_g[ii+1] = ii;
		}
		else
		{
			phase_g[ii+1] = ii - nt;
		}
	}

	// All walkoff relative to gvi(pump)
	gvi_s = gvi_s - gvi_p;
	gvi_i = gvi_i - gvi_p;
	gvi_p = gvi_p - gvi_p;

	gvifactor_s = (2*Pi*gvi_s)/(c*nt*dt);
	gvifactor_i = (2*Pi*gvi_i)/(c*nt*dt);
	gvifactor_p = (2*Pi*gvi_p)/(c*nt*dt);

	// Fill the phasegv arrays
	for(ii=0; ii<nt; ++ii)
	{
		phasegv_s[ii+1] = gvdfactor_s*pow(phase_g[ii+1],2) + 
						  gvifactor_s*phase_g[ii+1];

		phasegv_i[ii+1] = gvdfactor_i*pow(phase_g[ii+1],2) +
						  gvifactor_i*phase_g[ii+1];

		phasegv_p[ii+1] = gvdfactor_p*pow(phase_g[ii+1],2) +
						  gvifactor_p*phase_g[ii+1];
	}

	// Free up memory
	free_vector(phase_g,1,nt);


//*********************************************************************
// Set up the y diffractive phase shifts

	phase_y = vector(1,ny);

	phasey_s = vector(1,ny);
	phasey_i = vector(1,ny);
	phasey_p = vector(1,ny);

	
	for(ii=0; ii<ny; ++ii)
	{
		if( ii<=(0.5*ny) )
		{
			phase_y[ii+1] = (float)pow((float)ii,(float)2);
		}
		else
		{
			phase_y[ii+1] = (float)pow((float)(ii - ny),(float)2);
		}
	}

	// Fill the phasey arrays
	for(ii=0; ii<ny; ++ii)
	{
		phasey_s[ii+1] = (Pi*(float)pow((float)(1/(ny*dy)),(float)2)*(Lambda_s/Index_s)) *
						 phase_y[ii+1];

		phasey_i[ii+1] = (Pi*(float)pow((float)(1/(ny*dy)),(float)2)*(Lambda_i/Index_i)) *
						 phase_y[ii+1];

		phasey_p[ii+1] = (Pi*(float)pow((float)(1/(ny*dy)),(float)2)*(Lambda_p/Index_p)) *
						 phase_y[ii+1];

	}

	// Free up memory
	free_vector(phase_y,1,ny);


//*********************************************************************
// Set up x diffractive/walkoff phase shifts

	phase_x = vector(1,nx);
	phase_xwo = vector(1,nx);

	phasex_s = vector(1,nx);
	phasex_i = vector(1,nx);
	phasex_p = vector(1,nx);


	// Set up phase_x array
	for(ii=0; ii<nx; ++ii)
	{
		if( ii<=(0.5*nx) )
		{
			phase_x[ii+1] = (float)pow((float)ii,(float)2);
		}
		else
		{
			phase_x[ii+1] = (float)pow((float)(ii - nx),(float)2);
		}
	}


	// Set up phase_xwo array
	for(ii=0; ii<nx; ++ii)
	{
		if( ii<=(0.5*nx) )
		{
			phase_xwo[ii+1] = ii;
		}
		else
		{
			phase_xwo[ii+1] = ii - nx;
		}
	}


	// Fill the phasex arrays (diffractive phase shift)
	for(ii=0; ii<nx; ++ii)
	{
		phasex_s[ii+1] = (Pi*(float)pow((float)(1/(nx*dx)),(float)2)*(Lambda_s/Index_s)) *
						 phase_x[ii+1];

		phasex_i[ii+1] = (Pi*(float)pow((float)(1/(nx*dx)),(float)2)*(Lambda_i/Index_i)) *
						 phase_x[ii+1];

		phasex_p[ii+1] = (Pi*(float)pow((float)(1/(nx*dx)),(float)2)*(Lambda_p/Index_p)) *
						 phase_x[ii+1];

	}

	// Free up memory
	free_vector(phase_x,1,nx);

	// Add walkoff phase shift terms
	for(ii=0; ii<nx; ++ii)
	{
		phasex_s[ii+1] = phasex_s[ii+1] + (2*Pi*(1/(nx*dx))*Rho_s) *
										  phase_xwo[ii+1];
		
		phasex_i[ii+1] = phasex_i[ii+1] + (2*Pi*(1/(nx*dx))*Rho_i) *
										  phase_xwo[ii+1];

		phasex_p[ii+1] = phasex_p[ii+1] + (2*Pi*(1/(nx*dx))*Rho_p) *
										  phase_xwo[ii+1];

	}

	// Free up memory
	free_vector(phase_xwo,1,nx);


//*********************************************************************
// Set up phase terms in s,i,p

	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0; jj<ny; ++jj)
		{
			for(kk=0; kk<nt; ++kk)
			{
				k2 = (kk+1) + jj*nt + ii*nt*ny;

				pField->pPhaseTerm_s[k2] = -dz*(phasegv_s[kk+1]+
								    phasey_s[jj+1] + phasex_s[ii+1]);
				
				pField->pPhaseTerm_i[k2] = -dz*(phasegv_i[kk+1]+
								    phasey_i[jj+1] + phasex_i[ii+1]);

				pField->pPhaseTerm_p[k2] = -dz*(phasegv_p[kk+1]+
									phasey_p[jj+1] + phasex_p[ii+1]);

			}
		}
	}

	// Check for Windows messages, and return from the loop if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

		
//*********************************************************************
// Free up memory

	free_vector(phasegv_s,1,nt);
	free_vector(phasegv_i,1,nt);
	free_vector(phasegv_p,1,nt);

	free_vector(phasey_s,1,ny);
	free_vector(phasey_i,1,ny);
	free_vector(phasey_p,1,ny);

	free_vector(phasex_s,1,nx);
	free_vector(phasex_i,1,nx);
	free_vector(phasex_p,1,nx);

}	// End InitializePhases


//*********************** Function Integrate **************************
//
// This function propagates the electric field through the 
// crystal, using order 2 Runge-Kutta for dE/dz.
//
// Called from: GraphDlg::OnRunBtn
//
//*********************************************************************

void Integrate(CInputs* pBt, CGlobalVar* pGV, CFields* pField, 
			   CProgressCtrl* pProgS, CProgressCtrl* pProgT)
{
	const double Pi = 3.14159265359;
	const double epsilon_0 = 8.854187817e-12;	// Permittivity of vacuum [F/m]
	const double c = 2.99792458e+8;				// Speed of light in vacuum [m/s]


//*********************************************************************
// Set local variables to their corresponding global variable values

	// CInputs variables
	double Rc_In_s = pBt->GetVar(RC_IN_S);		// Input crystal reflectivity
	double Rc_In_i = pBt->GetVar(RC_IN_I);
	double Rc_In_p = pBt->GetVar(RC_IN_P);

	double Rc_Out_s = pBt->GetVar(RC_OUT_S);	// Output crystal reflectivity
	double Rc_Out_i = pBt->GetVar(RC_OUT_I);
	double Rc_Out_p = pBt->GetVar(RC_OUT_P);

	double Index_s = pBt->GetVar(INDEX_S);		// Index of refraction
	double Index_i = pBt->GetVar(INDEX_I);	
	double Index_p = pBt->GetVar(INDEX_P);

	double deff = pBt->GetVar(DEFF);			// Effective nonlinear coefficient [m/V]
	double deltak = pBt->GetVar(DELTAK);		// Delta k [1/m]

	double alpha_s = pBt->GetVar(ALPHA_S);		// Crystal absorption [1/m]
	double alpha_i = pBt->GetVar(ALPHA_I);
	double alpha_p = pBt->GetVar(ALPHA_P);

	double n2_s1 = pBt->GetVar(N2_S1);			// n-squared [sq m/W]
	double n2_s2 = pBt->GetVar(N2_S2);
	double n2_s3 = pBt->GetVar(N2_S3);
	
	double n2_i1 = pBt->GetVar(N2_I1);
	double n2_i2 = pBt->GetVar(N2_I2);
	double n2_i3 = pBt->GetVar(N2_I3);

	double n2_p1 = pBt->GetVar(N2_P1);
	double n2_p2 = pBt->GetVar(N2_P2);
	double n2_p3 = pBt->GetVar(N2_P3);

	double Beta_s1 = pBt->GetVar(BETA_S1);		// beta [m/W]
	double Beta_s2 = pBt->GetVar(BETA_S2);
	double Beta_s3 = pBt->GetVar(BETA_S3);
	
	double Beta_i1 = pBt->GetVar(BETA_I1);
	double Beta_i2 = pBt->GetVar(BETA_I2);
	double Beta_i3 = pBt->GetVar(BETA_I3);

	double Beta_p1 = pBt->GetVar(BETA_P1);
	double Beta_p2 = pBt->GetVar(BETA_P2);
	double Beta_p3 = pBt->GetVar(BETA_P3);

	long nz = long(pBt->GetVar(NZ));			// no. of grid points in z-direction

	// CGlobalVar variables
	long nx = pGV->ulGetGlobal(NN,1);
	long ny = pGV->ulGetGlobal(NN,2);
	long nt = pGV->ulGetGlobal(NN,3);

	double dx = pGV->dGetGlobal(DX);
	double dy = pGV->dGetGlobal(DY);
	double dz = pGV->dGetGlobal(DZ);			// step size in z-direction
	double dt = pGV->dGetGlobal(DT);			// step size in t-direction

	double Omega_s = pGV->dGetGlobal(OMEGA_S);	// Frequency [rad/s]
	double Omega_i = pGV->dGetGlobal(OMEGA_I);
	double Omega_p = pGV->dGetGlobal(OMEGA_P);

	// Local variables
	double T_In_s,T_In_i,T_In_p;
	double T_Out_s, T_Out_i, T_Out_p;

	int i,ii,i2,jj,kk,k2,ll,l2,nn,n2;

	int max = nx*(ny/2)*nt;

	// Variables for graphing
	long ngridx = pGV->lGetGlobal(NGRIDX);
	long ngridy = pGV->lGetGlobal(NGRIDY);
	long padval = pGV->lGetGlobal(PADVAL);

	// Variables for the loop
	float *Oldfield_s, *Oldfield_i, *Oldfield_p;
	float *Newfield_s, *Newfield_i, *Newfield_p;
	float *Temp_s, *Temp_i, *Temp_p;
	double z;
	int Cconj;	// for the cmult function
	complex<double> Exponent;
	complex<double> Ep_Ei, Ep_Es, Es_Ei;

	float *EField_s, *EField_i, *EField_p;
	EField_s = vector(1,2*nx*(ny/2)*nt);
	EField_i = vector(1,2*nx*(ny/2)*nt);
	EField_p = vector(1,2*nx*(ny/2)*nt);
	
	float *TempFl, *TField;				// temp fields for fluence
	float *TempE_s, *TempE_i, *TempE_p;	
	float *TempF_s, *TempF_i, *TempF_p;

	CString chstring;	// char string for graph header
	int Radioval;		// int value for radio buttons


//*********************************************************************
// Set up EField 

	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				EField_s[i2-1] = pField->pEField_s[i2-1];
				EField_s[i2] = pField->pEField_s[i2];

				EField_i[i2-1] = pField->pEField_i[i2-1];
				EField_i[i2] = pField->pEField_i[i2];

				EField_p[i2-1] = pField->pEField_p[i2-1];
				EField_p[i2] = pField->pEField_p[i2];
				
			}
		}
	}

			
//*********************************************************************
// Enter the crystal

	// Multiply fields by transmission coeff. at input face
	T_In_s = sqrt((1 - Rc_In_s)/Index_s);
	T_In_i = sqrt((1 - Rc_In_i)/Index_i);
	T_In_p = sqrt((1 - Rc_In_p)/Index_p);

	for(ii=0; ii<2*nx*(ny/2)*nt; ++ii)
	{
		EField_s[ii+1] = T_In_s*EField_s[ii+1];
		EField_i[ii+1] = T_In_i*EField_i[ii+1];
		EField_p[ii+1] = T_In_p*EField_p[ii+1];
	}


//*********************************************************************
// Propagate one-half z-step

	// Set up phases for 1/2 dz
	complex<double> Exponent_s, Exponent_i, Exponent_p;
	
	for(ii=0; ii<nx*ny*nt; ++ii)
	{
		i2 = 2*(ii+1);

		Exponent_s = cexp(1,0,(0.5*pField->pPhaseTerm_s[ii+1]),1);
		Exponent_i = cexp(1,0,(0.5*pField->pPhaseTerm_i[ii+1]),1);
		Exponent_p = cexp(1,0,(0.5*pField->pPhaseTerm_p[ii+1]),1);

		pField->pPhase_s[i2-1] = Exponent_s.real();
		pField->pPhase_s[i2] = Exponent_s.imag();

		pField->pPhase_i[i2-1] = Exponent_i.real();
		pField->pPhase_i[i2] = Exponent_i.imag();

		pField->pPhase_p[i2-1] = Exponent_p.real();
		pField->pPhase_p[i2] = Exponent_p.imag();

	}

	// Check for Windows messages, and return if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Update the progress bar
	pProgS->SetPos(20);

	// Propagate
	PropagateField(pField,EField_s,pField->pPhase_s,nx,ny,nt);

	pProgS->SetPos(47);

	PropagateField(pField,EField_i,pField->pPhase_i,nx,ny,nt);

	pProgS->SetPos(75);

	PropagateField(pField,EField_p,pField->pPhase_p,nx,ny,nt);

	pProgS->SetPos(100);
	pProgT->SetPos(2);


//*********************************************************************
// Graph the fluence for current z position

	// Check for Windows messages, and return from the loop if 
	// the stop button has been pressed, and update the radio buttons
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Reset the EField array to include all y values
	TempE_s = vector(1,2*nx*ny*nt);
	TempE_i = vector(1,2*nx*ny*nt);
	TempE_p = vector(1,2*nx*ny*nt);

	TempF_s = vector(1,2*nx*(ny/2)*nt);
	TempF_i = vector(1,2*nx*(ny/2)*nt);
	TempF_p = vector(1,2*nx*(ny/2)*nt);

	// Mirror the EField array for Temp2
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + ((ny/2)-(kk+1))*nt;
				l2 = 2*(ll+1);

				TempF_s[i2-1] = EField_s[l2-1];
				TempF_s[i2] = EField_s[l2];
				TempF_i[i2-1] = EField_i[l2-1];
				TempF_i[i2] = EField_i[l2];
				TempF_p[i2-1] = EField_p[l2-1];
				TempF_p[i2] = EField_p[l2];
			}
		}
	}

	// Fill TempE with the EField values
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{

			for(kk=0;kk<ny;++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + (kk-(ny/2))*nt;
				l2 = 2*(ll+1);

				nn = ii + jj*nt*(ny/2) + kk*nt;
				n2 = 2*(nn+1);

				if(kk<(ny/2))
				{
					TempE_s[i2-1] = EField_s[n2-1];
					TempE_s[i2] = EField_s[n2];
					TempE_i[i2-1] = EField_i[n2-1];
					TempE_i[i2] = EField_i[n2];
					TempE_p[i2-1] = EField_p[n2-1];
					TempE_p[i2] = EField_p[n2];
				}
				else
				{
					TempE_s[i2-1] = TempF_s[l2-1];
					TempE_s[i2] = TempF_s[l2];
					TempE_i[i2-1] = TempF_i[l2-1];
					TempE_i[i2] = TempF_i[l2];
					TempE_p[i2-1] = TempF_p[l2-1];
					TempE_p[i2] = TempF_p[l2];
				}
				
			}
		}
	}
				
	// Free up memory
	free_vector(TempF_s,1,2*nx*(ny/2)*nt);
	free_vector(TempF_i,1,2*nx*(ny/2)*nt);
	free_vector(TempF_p,1,2*nx*(ny/2)*nt);

	// Batch the chart updates
	//pChart->SetIsBatched(true);
	//pData->SetIsBatched(true);

	// Reset the TempFl and TField arrays
	TempFl = vector(1,nx*ny);
	TField = vector(1,2*nx*ny*nt);

	// Check the Radio Buttons to see which one is selected, then set
	// to graph either signal, idler, or pump
	Radioval = GetRadio();

	if(Radioval == 1)	// signal
	{
		for(ii=0;ii<2*nx*ny*nt;++ii)
		{
			TField[ii+1] = TempE_s[ii+1];
		}
	}
	else if(Radioval == 2)	// idler
	{
		for(ii=0;ii<2*nx*ny*nt;++ii)
		{
			TField[ii+1] = TempE_i[ii+1];
		}
	}
	else if(Radioval == 3)	// pump
	{
		for(ii=0;ii<2*nx*ny*nt;++ii)
		{
			TField[ii+1] = TempE_p[ii+1];
		}
	}

	// Free up memory
	free_vector(TempE_s,1,2*nx*ny*nt);
	free_vector(TempE_i,1,2*nx*ny*nt);
	free_vector(TempE_p,1,2*nx*ny*nt);

	// Initialize TempFl to zero
	for(ii=0;ii<nx*ny;++ii)
	{
		TempFl[ii+1] = 0.0;
	}

	// Calculate the fluence
	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<ny;++jj)
		{
			for(kk=0;kk<nt;++kk)
			{
				k2=(kk+1) + jj*nt + ii*nt*ny;
				i2=2*k2;
				l2=(jj+1) + ii*ny;

				TempFl[l2] = TempFl[l2] +
					(pow(TField[i2-1],2) + pow(TField[i2],2));
				}
		}
	}

	for(ii=0;ii<nx*ny;++ii)
	{
		TempFl[ii+1] = dt*((epsilon_0*c)/2)*TempFl[ii+1];
			
//		TempFl[ii+1] = (dt/754)*TempFl[ii+1];
	}

	// Check for Windows messages, and return if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Free up memory
	free_vector(TempFl,1,nx*ny);
	free_vector(TField,1,2*nx*ny*nt);


//*********************************************************************
// Reset the phase for full step dz

	for(ii=0; ii<nx*ny*nt; ++ii)
	{
		i2 = 2*(ii+1);

		Exponent_s = cexp(1,0,pField->pPhaseTerm_s[ii+1],1);
		Exponent_i = cexp(1,0,pField->pPhaseTerm_i[ii+1],1);
		Exponent_p = cexp(1,0,pField->pPhaseTerm_p[ii+1],1);

		pField->pPhase_s[i2-1] = Exponent_s.real();
		pField->pPhase_s[i2] = Exponent_s.imag();

		pField->pPhase_i[i2-1] = Exponent_i.real();
		pField->pPhase_i[i2] = Exponent_i.imag();

		pField->pPhase_p[i2-1] = Exponent_p.real();
		pField->pPhase_p[i2] = Exponent_p.imag();
	}

	// Update the progress bar
	pProgS->SetPos(0);
	
//*********************************************************************
// Begin loop to propagate through crystal

	for(i=1; i<=nz; ++i)
	{

		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			break;
		}

		// Initialize Oldfields
		Oldfield_s = vector(1,2*nx*(ny/2)*nt);
		Oldfield_i = vector(1,2*nx*(ny/2)*nt);
		Oldfield_p = vector(1,2*nx*(ny/2)*nt);

		// Set current fields to Oldfield
		for(ii=0; ii<2*nx*(ny/2)*nt; ++ii)
		{
			Oldfield_s[ii+1] = EField_s[ii+1];
			Oldfield_i[ii+1] = EField_i[ii+1];
			Oldfield_p[ii+1] = EField_p[ii+1];
		}

		// Set z to the current value in z-direction
		z = dz*i;


		//*******************************************
		// Nonlinear mixing using order 2 Runge-Kutta

		if(deff != 0)
		{
		
		for(ii=0; ii<nx*(ny/2)*nt; ++ii)
		{

			// Check for Windows messages, and return from the loop if 
			// the stop button has been pressed
			CheckWinMessages(10000);
			if( GetStop() )
			{
				return;
			}

			i2 = 2*(ii+1);
			
			Cconj = 1;	// complex conjugate for electric terms
			Ep_Ei = cmult(Oldfield_i[i2-1],Oldfield_i[i2],
						  Oldfield_p[i2-1],Oldfield_p[i2],Cconj);

			Ep_Es = cmult(Oldfield_s[i2-1],Oldfield_s[i2],
						  Oldfield_p[i2-1],Oldfield_p[i2],Cconj);

			Cconj = 0;	// no complex conjugate for this term
			Es_Ei = cmult(Oldfield_s[i2-1],Oldfield_s[i2],
						  Oldfield_i[i2-1],Oldfield_i[i2],Cconj);


			//*****************
			// Signal (dE_s/dz)
			Exponent = cexp(Ep_Ei.real(),Ep_Ei.imag(),deltak*z,1);

			// real part of signal field
			EField_s[i2-1] = 
				(-(Omega_s*deff/(c*Index_s))*Exponent.imag());

			// imaginary part of signal field
			EField_s[i2] = 
				(+(Omega_s*deff/(c*Index_s))*Exponent.real());


			//****************
			// Idler (dE_i/dz)
			Exponent = cexp(Ep_Es.real(),Ep_Es.imag(),deltak*z,1);

			// real part 
			EField_i[i2-1] = 
				(-(Omega_i*deff/(c*Index_i))*Exponent.imag());

			// imaginary part 
			EField_i[i2] = 
				(+(Omega_i*deff/(c*Index_i))*Exponent.real());


			//***************
			// Pump (dE_p/dz)
			Exponent = cexp(Es_Ei.real(),Es_Ei.imag(),deltak*z,-1);

			// real part 
			EField_p[i2-1] = 
				(-(Omega_p*deff/(c*Index_p))*Exponent.imag());

			// imaginary part 
			EField_p[i2] = 
				(+(Omega_p*deff/(c*Index_p))*Exponent.real());
			
		}

		}
		else
		{
			
		for(ii=0;ii<nx*(ny/2)*nt;++ii)
		{
			i2=2*(ii+1);
			
			EField_s[i2-1] = 0.0;
			EField_s[i2] = 0.0;
			EField_i[i2-1] = 0.0;
			EField_i[i2] = 0.0;
			EField_p[i2-1] = 0.0;
			EField_p[i2] = 0.0;
		}
		
		}// End if(deff != 0)


		//**************************
		// Linear crystal absorption

		if((alpha_s != 0) || (alpha_i != 0) || (alpha_p != 0))
		{
			for(ii=0;ii<nx*(ny/2)*nt;++ii)
			{
				i2=2*(ii+1);

				EField_s[i2-1] = EField_s[i2-1] - alpha_s*Oldfield_s[i2-1];
				EField_s[i2] = EField_s[i2] - alpha_s*Oldfield_s[i2];

				EField_i[i2-1] = EField_i[i2-1] - alpha_i*Oldfield_i[i2-1];
				EField_i[i2] = EField_i[i2] - alpha_i*Oldfield_i[i2];

				EField_p[i2-1] = EField_p[i2-1] - alpha_p*Oldfield_p[i2-1];
				EField_p[i2] = EField_p[i2] - alpha_p*Oldfield_p[i2];

			}
		}

		// Update the progress bar
		pProgS->SetPos(5);


		//*********************
		// Nonlinear absorption

		if((Beta_s1 != 0) || (Beta_s2 != 0) || (Beta_s3 != 0) ||
	       (Beta_i1 != 0) || (Beta_i2 != 0) || (Beta_i3 != 0) ||
		   (Beta_p1 != 0) || (Beta_p2 != 0) || (Beta_p3 != 0))
		{

		Temp_s = vector(1,nx*(ny/2)*nt);
		Temp_i = vector(1,nx*(ny/2)*nt);
		Temp_p = vector(1,nx*(ny/2)*nt);

		// Calculate mod squared for Oldfield
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					Temp_s[k2+1] = pow(Oldfield_s[i2-1],2) + pow(Oldfield_s[i2],2);
					Temp_i[k2+1] = pow(Oldfield_i[i2-1],2) + pow(Oldfield_i[i2],2);
					Temp_p[k2+1] = pow(Oldfield_p[i2-1],2) + pow(Oldfield_p[i2],2);

				}
			}
		}

		// Set EField to the new values
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					EField_s[i2-1] = EField_s[i2-1] - Oldfield_s[i2-1]*
					((Beta_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_s[i2] = EField_s[i2] - Oldfield_s[i2]*
					((Beta_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2-1] = EField_i[i2-1] - Oldfield_i[i2-1]*
					((Beta_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2] = EField_i[i2] - Oldfield_i[i2]*
					((Beta_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2-1] = EField_p[i2-1] - Oldfield_p[i2-1]*
					((Beta_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2] = EField_p[i2] - Oldfield_p[i2]*
					((Beta_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

				}
			}
		}

		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Free up memory
		free_vector(Temp_s,1,nx*(ny/2)*nt);
		free_vector(Temp_i,1,nx*(ny/2)*nt);
		free_vector(Temp_p,1,nx*(ny/2)*nt);

		}// End if(Beta != 0)

		// Update the progress bar
		pProgS->SetPos(10);


		//***************************
		// Nonlinear refractive index

		if((n2_s1 != 0) || (n2_s2 != 0) || (n2_s3 != 0) ||
	       (n2_i1 != 0) || (n2_i2 != 0) || (n2_i3 != 0) ||
		   (n2_p1 != 0) || (n2_p2 != 0) || (n2_p3 != 0))
		{

		Temp_s = vector(1,nx*(ny/2)*nt);
		Temp_i = vector(1,nx*(ny/2)*nt);
		Temp_p = vector(1,nx*(ny/2)*nt);

		// Calculate mod squared for Oldfield
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					Temp_s[k2+1] = pow(Oldfield_s[i2-1],2) + pow(Oldfield_s[i2],2);
					Temp_i[k2+1] = pow(Oldfield_i[i2-1],2) + pow(Oldfield_i[i2],2);
					Temp_p[k2+1] = pow(Oldfield_p[i2-1],2) + pow(Oldfield_p[i2],2);

				}
			}
		}

		// Set EField to the new values
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					EField_s[i2-1] = EField_s[i2-1] - (Oldfield_s[i2]*Omega_s/c)*
					((n2_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) + 
					 (n2_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_s[i2] = EField_s[i2] + (Oldfield_s[i2-1]*Omega_s/c)*
					((n2_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (n2_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2-1] = EField_i[i2-1] - (Oldfield_i[i2]*Omega_i/c)*
					((n2_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) + 
					 (n2_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2] = EField_i[i2] + (Oldfield_i[i2-1]*Omega_i/c)*
					((n2_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (n2_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2-1] = EField_p[i2-1] - (Oldfield_p[i2]*Omega_p/c)*
					((n2_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) + 
					 (n2_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2] = EField_p[i2] + (Oldfield_p[i2-1]*Omega_p/c)*
					((n2_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (n2_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

				}
			}
		}

		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Free up memory
		free_vector(Temp_s,1,nx*(ny/2)*nt);
		free_vector(Temp_i,1,nx*(ny/2)*nt);
		free_vector(Temp_p,1,nx*(ny/2)*nt);

		}// End if(n2 != 0)

		// Update the progress bar
		pProgS->SetPos(15);

		// Initialize Newfields
		Newfield_s = vector(1,2*nx*(ny/2)*nt);
		Newfield_i = vector(1,2*nx*(ny/2)*nt);
		Newfield_p = vector(1,2*nx*(ny/2)*nt);
		
		// Set up Newfield
		for(ii=0; ii<2*nx*(ny/2)*nt; ++ii)
		{
			Newfield_s[ii+1] = Oldfield_s[ii+1] + 
							   0.5*dz*EField_s[ii+1];

			Newfield_i[ii+1] = Oldfield_i[ii+1] +
							   0.5*dz*EField_i[ii+1];

			Newfield_p[ii+1] = Oldfield_p[ii+1] +
							   0.5*dz*EField_p[ii+1];

		}


		//********************************
		// Nonlinear mixing using Newfield

		if(deff != 0)
		{

		for(ii=0; ii<nx*(ny/2)*nt; ++ii)
		{
			// Check for Windows messages, and return if 
			// the stop button has been pressed
			CheckWinMessages(10000);
			if( GetStop() )
			{
				return;
			}

			i2 = 2*(ii+1);
			
			Cconj = 1;	// complex conjugate for electric terms
			Ep_Ei = cmult(Newfield_i[i2-1],Newfield_i[i2],
						  Newfield_p[i2-1],Newfield_p[i2],Cconj);

			Ep_Es = cmult(Newfield_s[i2-1],Newfield_s[i2],
						  Newfield_p[i2-1],Newfield_p[i2],Cconj);

			Cconj = 0;	// no complex conjugate for this term
			Es_Ei = cmult(Newfield_s[i2-1],Newfield_s[i2],
						  Newfield_i[i2-1],Newfield_i[i2],Cconj);


			//*****************
			// Signal (dE_s/dz)
			Exponent = cexp(Ep_Ei.real(),Ep_Ei.imag(),deltak*z,1);

			// real part of signal field
			EField_s[i2-1] = 
				(-(Omega_s*deff/(c*Index_s))*Exponent.imag());

			// imaginary part of signal field
			EField_s[i2] = 
				(+(Omega_s*deff/(c*Index_s))*Exponent.real());


			//****************
			// Idler (dE_i/dz)
			Exponent = cexp(Ep_Es.real(),Ep_Es.imag(),deltak*z,1);

			// real part 
			EField_i[i2-1] = 
				(-(Omega_i*deff/(c*Index_i))*Exponent.imag());

			// imaginary part 
			EField_i[i2] = 
				(+(Omega_i*deff/(c*Index_i))*Exponent.real());


			//***************
			// Pump (dE_p/dz)
			Exponent = cexp(Es_Ei.real(),Es_Ei.imag(),deltak*z,-1);

			// real part 
			EField_p[i2-1] = 
				(-(Omega_p*deff/(c*Index_p))*Exponent.imag());

			// imaginary part 
			EField_p[i2] = 
				(+(Omega_p*deff/(c*Index_p))*Exponent.real());
		}

		}
		else
		{
			
		for(ii=0;ii<nx*(ny/2)*nt;++ii)
		{
			i2=2*(ii+1);
			
			EField_s[i2-1] = 0.0;
			EField_s[i2] = 0.0;
			EField_i[i2-1] = 0.0;
			EField_i[i2] = 0.0;
			EField_p[i2-1] = 0.0;
			EField_p[i2] = 0.0;
		}
		
		}// End if(deff != 0)


		//**************************
		// Linear crystal absorption

		if((alpha_s != 0) || (alpha_i != 0) || (alpha_p != 0))
		{
			for(ii=0;ii<nx*(ny/2)*nt;++ii)
			{
				i2=2*(ii+1);

				EField_s[i2-1] = EField_s[i2-1] - alpha_s*Newfield_s[i2-1];
				EField_s[i2] = EField_s[i2] - alpha_s*Newfield_s[i2];

				EField_i[i2-1] = EField_i[i2-1] - alpha_i*Newfield_i[i2-1];
				EField_i[i2] = EField_i[i2] - alpha_i*Newfield_i[i2];

				EField_p[i2-1] = EField_p[i2-1] - alpha_p*Newfield_p[i2-1];
				EField_p[i2] = EField_p[i2] - alpha_p*Newfield_p[i2];

			}
		}

		// Update the progress bar
		pProgS->SetPos(25);


		//************************************
		// Nonlinear absorption using Newfield

		if((Beta_s1 != 0) || (Beta_s2 != 0) || (Beta_s3 != 0) ||
	       (Beta_i1 != 0) || (Beta_i2 != 0) || (Beta_i3 != 0) ||
		   (Beta_p1 != 0) || (Beta_p2 != 0) || (Beta_p3 != 0))
		{

		Temp_s = vector(1,nx*(ny/2)*nt);
		Temp_i = vector(1,nx*(ny/2)*nt);
		Temp_p = vector(1,nx*(ny/2)*nt);

		// Calculate mod squared for Newfield
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					Temp_s[k2+1] = pow(Newfield_s[i2-1],2) + pow(Newfield_s[i2],2);
					Temp_i[k2+1] = pow(Newfield_i[i2-1],2) + pow(Newfield_i[i2],2);
					Temp_p[k2+1] = pow(Newfield_p[i2-1],2) + pow(Newfield_p[i2],2);

				}
			}
		}

		// Set EField to the new values
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					EField_s[i2-1] = EField_s[i2-1] - Newfield_s[i2-1]*
					((Beta_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_s[i2] = EField_s[i2] - Newfield_s[i2]*
					((Beta_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2-1] = EField_i[i2-1] - Newfield_i[i2-1]*
					((Beta_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2] = EField_i[i2] - Newfield_i[i2]*
					((Beta_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2-1] = EField_p[i2-1] - Newfield_p[i2-1]*
					((Beta_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2] = EField_p[i2] - Newfield_p[i2]*
					((Beta_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (Beta_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (Beta_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

				}
			}
		}

		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Free up memory
		free_vector(Temp_s,1,nx*(ny/2)*nt);
		free_vector(Temp_i,1,nx*(ny/2)*nt);
		free_vector(Temp_p,1,nx*(ny/2)*nt);

		}// End if(Beta != 0)

		// Update the progress bar
		pProgS->SetPos(30);


		//***************************
		// Nonlinear refractive index

		if((n2_s1 != 0) || (n2_s2 != 0) || (n2_s3 != 0) ||
	       (n2_i1 != 0) || (n2_i2 != 0) || (n2_i3 != 0) ||
		   (n2_p1 != 0) || (n2_p2 != 0) || (n2_p3 != 0))
		{

		Temp_s = vector(1,nx*(ny/2)*nt);
		Temp_i = vector(1,nx*(ny/2)*nt);
		Temp_p = vector(1,nx*(ny/2)*nt);

		// Calculate mod squared for Newfield
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					Temp_s[k2+1] = pow(Newfield_s[i2-1],2) + pow(Newfield_s[i2],2);
					Temp_i[k2+1] = pow(Newfield_i[i2-1],2) + pow(Newfield_i[i2],2);
					Temp_p[k2+1] = pow(Newfield_p[i2-1],2) + pow(Newfield_p[i2],2);

				}
			}
		}

		// Set EField to the new values
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<(ny/2);++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=kk+jj*nt+ii*(ny/2)*nt;
					i2=2*(k2+1);

					EField_s[i2-1] = EField_s[i2-1] - (Newfield_s[i2]*Omega_s/3.e8)*
					((n2_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) + 
					 (n2_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_s[i2] = EField_s[i2] + (Newfield_s[i2-1]*Omega_s/3.e8)*
					((n2_s1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_s2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (n2_s3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2-1] = EField_i[i2-1] - (Newfield_i[i2]*Omega_i/3.e8)*
					((n2_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) + 
					 (n2_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_i[i2] = EField_i[i2] + (Newfield_i[i2-1]*Omega_i/3.e8)*
					((n2_i1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_i2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (n2_i3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2-1] = EField_p[i2-1] - (Newfield_p[i2]*Omega_p/3.e8)*
					((n2_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) + 
					 (n2_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

					EField_p[i2] = EField_p[i2] + (Newfield_p[i2-1]*Omega_p/3.e8)*
					((n2_p1*Index_s*((epsilon_0*c)/2)*Temp_s[k2+1]) +
					 (n2_p2*Index_i*((epsilon_0*c)/2)*Temp_i[k2+1]) +
					 (n2_p3*Index_p*((epsilon_0*c)/2)*Temp_p[k2+1]));

				}
			}
		}

		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Free up memory
		free_vector(Temp_s,1,nx*(ny/2)*nt);
		free_vector(Temp_i,1,nx*(ny/2)*nt);
		free_vector(Temp_p,1,nx*(ny/2)*nt);

		}// End if(n2 != 0)

		// Update the progress bar
		pProgS->SetPos(35);

		// Reset the electric fields
		for(ii=0; ii<2*nx*(ny/2)*nt; ++ii)
		{
			EField_s[ii+1] = Oldfield_s[ii+1] + 
							   dz*EField_s[ii+1];

			EField_i[ii+1] = Oldfield_i[ii+1] +
							   dz*EField_i[ii+1];

			EField_p[ii+1] = Oldfield_p[ii+1] +
							   dz*EField_p[ii+1];

		}

		// Free up memory
		free_vector(Oldfield_s,1,2*nx*(ny/2)*nt);
		free_vector(Oldfield_i,1,2*nx*(ny/2)*nt);
		free_vector(Oldfield_p,1,2*nx*(ny/2)*nt);

		free_vector(Newfield_s,1,2*nx*(ny/2)*nt);
		free_vector(Newfield_i,1,2*nx*(ny/2)*nt);
		free_vector(Newfield_p,1,2*nx*(ny/2)*nt);
			
		// Reset the phases for a 1/2 dz step if last step in crystal
		if(i==nz)
		{
			for(ii=0; ii<nx*ny*nt; ++ii)
			{
				i2 = 2*(ii+1);

				Exponent_s = cexp(1,0,0.5*pField->pPhaseTerm_s[ii+1],1);
				Exponent_i = cexp(1,0,0.5*pField->pPhaseTerm_i[ii+1],1);
				Exponent_p = cexp(1,0,0.5*pField->pPhaseTerm_p[ii+1],1);

				pField->pPhase_s[i2-1] = Exponent_s.real();
				pField->pPhase_s[i2] = Exponent_s.imag();

				pField->pPhase_i[i2-1] = Exponent_i.real();
				pField->pPhase_i[i2] = Exponent_i.imag();
	
				pField->pPhase_p[i2-1] = Exponent_p.real();
				pField->pPhase_p[i2] = Exponent_p.imag();
			}
		}

		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Propagate
		PropagateField(pField,EField_s,pField->pPhase_s,nx,ny,nt);

		pProgS->SetPos(55);

		PropagateField(pField,EField_i,pField->pPhase_i,nx,ny,nt);

		pProgS->SetPos(75);

		PropagateField(pField,EField_p,pField->pPhase_p,nx,ny,nt);

		pProgS->SetPos(99);

		
		//*****************************************
		// Graph the fluence for current z position

		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed, and update the radio buttons
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Reset the EField array to include all y values
		TempE_s = vector(1,2*nx*ny*nt);
		TempE_i = vector(1,2*nx*ny*nt);
		TempE_p = vector(1,2*nx*ny*nt);

		TempF_s = vector(1,2*nx*(ny/2)*nt);
		TempF_i = vector(1,2*nx*(ny/2)*nt);
		TempF_p = vector(1,2*nx*(ny/2)*nt);

		// Mirror the EField array for Temp2
		for(ii=0;ii<nt;++ii)
		{
			for(jj=0;jj<nx;++jj)
			{
				for(kk=0;kk<(ny/2);++kk)
				{
					k2 = ii + jj*nt*(ny/2) + kk*nt;
					i2 = 2*(k2+1);

					ll = ii + jj*nt*(ny/2) + ((ny/2)-(kk+1))*nt;
					l2 = 2*(ll+1);

					TempF_s[i2-1] = EField_s[l2-1];
					TempF_s[i2] = EField_s[l2];
					TempF_i[i2-1] = EField_i[l2-1];
					TempF_i[i2] = EField_i[l2];
					TempF_p[i2-1] = EField_p[l2-1];
					TempF_p[i2] = EField_p[l2];
				}
			}
		}

		// Refill TempE with the EField values
		for(ii=0;ii<nt;++ii)
		{
			for(jj=0;jj<nx;++jj)
			{

				for(kk=0;kk<ny;++kk)
				{
					k2 = ii + jj*nt*ny + kk*nt;
					i2 = 2*(k2+1);
	
					ll = ii + jj*nt*(ny/2) + (kk-(ny/2))*nt;
					l2 = 2*(ll+1);

					nn = ii + jj*nt*(ny/2) + kk*nt;
					n2 = 2*(nn+1);

					if(kk<(ny/2))
					{
						TempE_s[i2-1] = EField_s[n2-1];
						TempE_s[i2] = EField_s[n2];
						TempE_i[i2-1] = EField_i[n2-1];
						TempE_i[i2] = EField_i[n2];
						TempE_p[i2-1] = EField_p[n2-1];
						TempE_p[i2] = EField_p[n2];
					}
					else
					{
						TempE_s[i2-1] = TempF_s[l2-1];
						TempE_s[i2] = TempF_s[l2];
						TempE_i[i2-1] = TempF_i[l2-1];
						TempE_i[i2] = TempF_i[l2];
						TempE_p[i2-1] = TempF_p[l2-1];
						TempE_p[i2] = TempF_p[l2];
					}
				
				}
			}
		}
				
		// Free up memory
		free_vector(TempF_s,1,2*nx*(ny/2)*nt);
		free_vector(TempF_i,1,2*nx*(ny/2)*nt);
		free_vector(TempF_p,1,2*nx*(ny/2)*nt);

		// Batch the chart updates
		//pChart->SetIsBatched(true);
		//pData->SetIsBatched(true);

		// Reset the TempFl and TField arrays
		TempFl = vector(1,nx*ny);
		TField = vector(1,2*nx*ny*nt);

		// Check the Radio Buttons to see which one is selected, then set
		// to graph either signal, idler, or pump
		Radioval = GetRadio();

		if(Radioval == 1)	// signal
		{
			for(ii=0;ii<2*nx*ny*nt;++ii)
			{
				TField[ii+1] = TempE_s[ii+1];
			}
		}
		else if(Radioval == 2)	// idler
		{
			for(ii=0;ii<2*nx*ny*nt;++ii)
			{
				TField[ii+1] = TempE_i[ii+1];
			}
		}
		else if(Radioval == 3)	// pump
		{
			for(ii=0;ii<2*nx*ny*nt;++ii)
			{
				TField[ii+1] = TempE_p[ii+1];
			}
		}

		// Free up memory
		free_vector(TempE_s,1,2*nx*ny*nt);
		free_vector(TempE_i,1,2*nx*ny*nt);
		free_vector(TempE_p,1,2*nx*ny*nt);

		// Initialize TempFl to zero
		for(ii=0;ii<nx*ny;++ii)
		{
			TempFl[ii+1] = 0.0;
		}

		// Calculate the fluence
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<ny;++jj)
			{
				for(kk=0;kk<nt;++kk)
				{
					k2=(kk+1) + jj*nt + ii*nt*ny;
					i2=2*k2;
					l2=(jj+1) + ii*ny;

					TempFl[l2] = TempFl[l2] +
						(pow(TField[i2-1],2) + pow(TField[i2],2));
					}
			}
		}

		for(ii=0;ii<nx*ny;++ii)
		{
			TempFl[ii+1] = dt*((epsilon_0*c)/2)*TempFl[ii+1];
		}

/*		// Set the header text for the graph
		if(i != nz)
		{
			chstring.Format("%5.3f",z*1.e3);
			pChart->GetHeader().GetText().SetText("Fluence at z = " + chstring + " mm");
		}
		else
		{
			pChart->GetHeader().GetText().SetText("Fluence at crystal exit face");
		}

		// Update the chart with the fluence values
//		if(Maxgridx > Maxgridy)
//		{
			for(ii=0;ii<nx;++ii)
			{	
				for(jj=0;jj<ny;++jj)
				{
//					if((jj<padval) || (jj>=ny+padval))
//					{
//						pData->SetValue(ii+1, jj+1, 0);
//					}
//					else
//					{
						k2=(jj+1) + ii*ny;
						pData->SetValue(ii+1, jj+1, TempFl[k2]);
//					}
//	
				}
			}

//		}
//		else if(Maxgridx <= Maxgridy)
//		{
//			for(ii=0;ii<ny;++ii)
//			{
//				for(jj=0;jj<nx;++jj)
//				{
//					if((jj<padval) || (jj>=nx+padval))
//					{
//						pData->SetValue(jj+1, ii+1, 0);
//					}
//					else
//					{
//						k2=(ii+1) + (jj-padval)*ny;
//						pData->SetValue(jj+1, ii+1, TempFl[k2]);
//					}
//				}
//			}
//		}
*/		
		// Free up memory
		free_vector(TempFl,1,nx*ny);
		free_vector(TField,1,2*nx*ny*nt);

/*		// Set the mesh color to the appropriate type
		if(Radioval == 1)	// signal
		{
			pGroup->GetElevation().SetMeshTopColor(RGB(255,0,0));
		}
		else if(Radioval == 2)	// idler
		{
			pGroup->GetElevation().SetMeshTopColor(RGB(0,0,255));
		}
		else if(Radioval == 3)	// pump
		{
			pGroup->GetElevation().SetMeshTopColor(RGB(0,150,0));
		}

		// Set the graph rotation
		XRot = GetRotX();
		ZRot = GetRotZ();

		pArea->GetView3D().SetXRotation(XRot);
		pArea->GetView3D().SetYRotation(0);
		pArea->GetView3D().SetZRotation(ZRot);
	
		// Update the chart
		pChart->SetIsBatched(false);
		pData->SetIsBatched(false);
*/
		// Update the progress bar
		pProgS->SetPos(0);
		pProgT->SetPos(int((100*i)/nz));

	}	// End propagation loop

	
//*********************************************************************
// Exit the crystal

	// Multiply fields by transmission coeff. at output face
	T_Out_s = sqrt((1 - Rc_Out_s)*Index_s);
	T_Out_i = sqrt((1 - Rc_Out_i)*Index_i);
	T_Out_p = sqrt((1 - Rc_Out_p)*Index_p);

	for(ii=0; ii<2*nx*(ny/2)*nt; ++ii)
	{
		EField_s[ii+1] = T_Out_s*EField_s[ii+1];
		EField_i[ii+1] = T_Out_i*EField_i[ii+1];
		EField_p[ii+1] = T_Out_p*EField_p[ii+1];
	}

	// Update the progress bar
	pProgS->SetPos(5);


//*********************************************************************
// Reset the pField->pEField array 
	
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				pField->pEField_s[i2-1] = EField_s[i2-1];
				pField->pEField_s[i2] = EField_s[i2];

				pField->pEField_i[i2-1] = EField_i[i2-1];
				pField->pEField_i[i2] = EField_i[i2];

				pField->pEField_p[i2-1] = EField_p[i2-1];
				pField->pEField_p[i2] = EField_p[i2];
				
			}
		}
	}


//*********************************************************************
// Free up memory

	free_vector(EField_s,1,2*nx*(ny/2)*nt);
	free_vector(EField_i,1,2*nx*(ny/2)*nt);
	free_vector(EField_p,1,2*nx*(ny/2)*nt);
	
}	// End Integrate


//********************** Function PropagateField **********************
//
// This function propagates the electric field through the crystal
// using FFTs.  
//
// Called from: Integrate
//
//*********************************************************************

void PropagateField(CFields* pField,float* Efield,float* Phase,long nx, long ny, long nt)
{
	int ii,i2,jj,kk,k2,ll,l2,nn,n2,Cconj;

	Cconj = 0;	// no complex conjugate for cmult

	complex<double> Value;

	fftwnd_plan p;

	float *TempE;
	TempE = vector(1,2*nx*ny*nt);

	float *TempF;
	TempF = vector(1,2*nx*(ny/2)*nt);

	int max = nx*(ny/2)*nt;


//*********************************************************************
// Reset the EField array to include all y values
	
	// Mirror the EField array for Temp2
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + ((ny/2)-(kk+1))*nt;
				l2 = 2*(ll+1);

				TempF[i2-1] = Efield[l2-1];
				TempF[i2] = Efield[l2];
			}
		}
	}

	// Refill pField->pEField with the EField values
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{

			for(kk=0;kk<ny;++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + (kk-(ny/2))*nt;
				l2 = 2*(ll+1);

				nn = ii + jj*nt*(ny/2) + kk*nt;
				n2 = 2*(nn+1);

				if(kk<(ny/2))
				{
					TempE[i2-1] = Efield[n2-1];
					TempE[i2] = Efield[n2];
				}
				else
				{
					TempE[i2-1] = TempF[l2-1];
					TempE[i2] = TempF[l2];
				}
				
			}
		}
	}
				
	// Free up memory
	free_vector(TempF,1,2*nx*(ny/2)*nt);
	
	// Set the FFT variable to the Efield values
	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<ny;++jj)
		{
			for(kk=0;kk<nt;++kk)
			{
				k2 = kk + jj*nt + ii*nt*ny;
				i2 = 2*(k2+1);

				pField->pFFTW[k2].re = TempE[i2-1];
				pField->pFFTW[k2].im = TempE[i2];
			}
		}
	}

	// Free up memory
	free_vector(TempE,1,2*nx*ny*nt);

	// Fast Fourier Transform from time to frequency
	p = fftw3d_create_plan(nx,ny,nt,FFTW_FORWARD,
							FFTW_ESTIMATE | FFTW_IN_PLACE);

	fftwnd_one(p, &pField->pFFTW[0], NULL);

	fftwnd_destroy_plan(p);

	// Check for Windows messages, and return if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Multiply by phase matrix
	for(ii=0; ii<nx; ++ii)
	{
		for(jj=0;jj<ny;++jj)
		{
			for(kk=0;kk<nt;++kk)
			{
				k2 = kk + jj*nt + ii*nt*ny;
				i2 = 2*(k2+1);

				Value = cmult(Phase[i2-1],Phase[i2],
					pField->pFFTW[k2].re,pField->pFFTW[k2].im,Cconj);

				pField->pFFTW[k2].re = Value.real();
				pField->pFFTW[k2].im = Value.imag();
			}
		}
	}

	// Fast Fourier Transform from frequency to time
	p = fftw3d_create_plan(nx,ny,nt,FFTW_BACKWARD,
							FFTW_ESTIMATE | FFTW_IN_PLACE);

	fftwnd_one(p, &pField->pFFTW[0], NULL);

	fftwnd_destroy_plan(p);

	// Check for Windows messages, and return if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Reset Efield
	for(ii=0;ii<nt;++ii)
	{
		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				
				ll = ii + jj*nt*(ny/2) + kk*nt;
				l2 = 2*(ll+1);

				Efield[l2-1] = pField->pFFTW[k2].re/(nx*ny*nt);
				Efield[l2] = pField->pFFTW[k2].im/(nx*ny*nt);
			}
		}
	}
	
}	// End PropagateField


//********************* Function CalculateOutput **********************
//
// This function calcuates the output fluence, spectra, and 
// far field for the signal, idler, and pump.
//
// Called from: GraphDlg::OnRunBtn
//
//*********************************************************************

void CalculateOutput(CInputs* pBt,CGlobalVar* pGV,CFields* pField,
					 double* pMinff,double* pMaxff,
					 double* pSmax,double* pSmin,
					 double* pMaxS_s,double* pMaxS_i,double* pMaxS_p,
					 double* pMaxP_s,double* pMaxP_i,double* pMaxP_p,
					 CProgressCtrl* pProgS)
{
	const double Pi = 3.14159265359;
	const double epsilon_0 = 8.854187817e-12;	// Permittivity of vacuum [F/m]
	const double c = 2.99792458e+8;				// Speed of light in vacuum [m/s]


	//*********************************************************************
	// Set local variables to their corresponding global variable values

	// CInputs variables
	double Lambda_s = pBt->GetVar(LAMBDA_S);	// Wavelengths [m]
	double Lambda_i = pBt->GetVar(LAMBDA_I);
	double Lambda_p = pBt->GetVar(LAMBDA_P);

	double Dist = pBt->GetVar(DIST);			// Distance to the detector [m]

	long nz = long(pBt->GetVar(NZ));			// no. of grid pts. z-direction

	// CGlobalVar variables
	long nx = pGV->ulGetGlobal(NN,1);			// size of grid x,y,t
	long ny = pGV->ulGetGlobal(NN,2);
	long nt = pGV->ulGetGlobal(NN,3);

	double dx = pGV->dGetGlobal(DX);			// step sizes in x,y,t
	double dy = pGV->dGetGlobal(DY);
	double dt = pGV->dGetGlobal(DT);

	// Local variables
	int ii,i2,jj,j2,kk,k2,ll,l2,nn,n2;

	FILE* stream;
		

	//*********************************************************************
	// Calculate the power vs time

	float *Power_s, *Power_i, *Power_p;
	float *Tempsig_1, *Tempid_1, *Temppump_1;
	double MaxP_s, MaxP_i, MaxP_p;	
	
	Power_s = vector(1,nt);
	Power_i = vector(1,nt);
	Power_p = vector(1,nt);

	Tempsig_1 = vector(1,(ny/2)*nt);
	Tempid_1 = vector(1,(ny/2)*nt);
	Temppump_1 = vector(1,(ny/2)*nt);

	// Initialize Power_s, Power_i, Power_p to zero
	for(ii=0;ii<nt;++ii)
	{
		Power_s[ii+1] = 0.0;
		Power_i[ii+1] = 0.0;
		Power_p[ii+1] = 0.0;
	}

	// Initialize Tempsig_1,Tempid_1,Temppump_1 to zero
	for(ii=0;ii<(ny/2)*nt;++ii)
	{
		Tempsig_1[ii+1] = 0.0;
		Tempid_1[ii+1] = 0.0;
		Temppump_1[ii+1] = 0.0;
	}

	// Sum EField^2 for real/imag and x-direction
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<nx;++kk)
			{
				j2=(ii+1) + jj*nt + kk*(ny/2)*nt;
				i2=2*j2;
				l2=(jj+1) + ii*(ny/2);

				Tempsig_1[l2]=Tempsig_1[l2] + 
					(pow(pField->pEField_s[i2-1],2) + pow(pField->pEField_s[i2],2));

				Tempid_1[l2]=Tempid_1[l2] + 
					(pow(pField->pEField_i[i2-1],2) + pow(pField->pEField_i[i2],2));

				Temppump_1[l2]=Temppump_1[l2] + 
					(pow(pField->pEField_p[i2-1],2) + pow(pField->pEField_p[i2],2));
			}
		}
	}

	// Sum EField^2 in y-direction
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			j2=(jj+1) + ii*(ny/2);
			Power_s[ii+1] = Power_s[ii+1] + Tempsig_1[j2];
			Power_i[ii+1] = Power_i[ii+1] + Tempid_1[j2];
			Power_p[ii+1] = Power_p[ii+1] + Temppump_1[j2];
		}
	}

	// Free up memory
	free_vector(Tempsig_1,1,(ny/2)*nt);
	free_vector(Tempid_1,1,(ny/2)*nt);
	free_vector(Temppump_1,1,(ny/2)*nt);

	for(ii=0; ii<nt; ++ii)	// factor of 2 is for total ny
	{
		Power_s[ii+1] = 2*dx*dy*((epsilon_0*c)/2)*Power_s[ii+1];
		Power_i[ii+1] = 2*dx*dy*((epsilon_0*c)/2)*Power_i[ii+1];
		Power_p[ii+1] = 2*dx*dy*((epsilon_0*c)/2)*Power_p[ii+1];

		if(*pMaxP_s < Power_s[ii+1]) *pMaxP_s = Power_s[ii+1];
		if(*pMaxP_i < Power_i[ii+1]) *pMaxP_i = Power_i[ii+1];
		if(*pMaxP_p < Power_p[ii+1]) *pMaxP_p = Power_p[ii+1];
	}

	//Record maximum powers
	MaxP_s = *pMaxP_s;
	MaxP_i = *pMaxP_i;
	MaxP_p = *pMaxP_p;

	stream=fopen("Power.dat","w");

	for(ii=0; ii<nt; ++ii)
	{
		fprintf(stream,"%E %E %E %E %s",
			pField->pTgrid[ii+1],Power_s[ii+1],Power_i[ii+1],Power_p[ii+1],"\n");
	}
	fclose(stream);

	// Check for Windows messages, and return if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	// Free up memory
	free_vector(Power_s,1,nt);
	free_vector(Power_i,1,nt);
	free_vector(Power_p,1,nt);

	// Update the progress bar if it exists
	if(!pProgS) pProgS->SetPos(10);
	

	//*********************************************************************
	// Calculate spectra

	// Expansion factor for smoothing the spectra by zero buffering
	long XFactor = pField->XFactor;

	float *Spectrum_s, *Spectrum_i, *Spectrum_p;
	double Max_s, Max_i, Max_p;

	Spectrum_s = vector(1,XFactor*nt);
	Spectrum_i = vector(1,XFactor*nt);
	Spectrum_p = vector(1,XFactor*nt);

	Max_s = *pMaxS_s;
	Max_i = *pMaxS_i;
	Max_p = *pMaxS_p;

	// Call the Spectra function
	Spectra(pField,pField->pEField_s,Spectrum_s,nx,ny,nt,XFactor,&Max_s);

	if(!pProgS) pProgS->SetPos(20);

	Spectra(pField,pField->pEField_i,Spectrum_i,nx,ny,nt,XFactor,&Max_i);

	if(!pProgS) pProgS->SetPos(30);

	Spectra(pField,pField->pEField_p,Spectrum_p,nx,ny,nt,XFactor,&Max_p);

	if(!pProgS) pProgS->SetPos(40);
	
	*pMaxS_s = Max_s/2;
	*pMaxS_i = Max_i/2;
	*pMaxS_p = Max_p/2;

	stream=fopen("Spectra.dat","w");

	double Gridpt;

	int Extra = (XFactor*nt)/2 - nt/2;

	for(ii=0;ii<XFactor*nt;++ii)
	{
		Gridpt = (1/(1.e12*nt*XFactor*dt))*(ii-(0.5*XFactor*nt));

		if(ii == Extra+1)		*pSmin = Gridpt;
		else if(ii == Extra+nt) *pSmax = Gridpt;

		fprintf(stream,"%E %E %E %E %s",Gridpt,
			(Spectrum_s[ii+1]/2),(Spectrum_i[ii+1]/2),(Spectrum_p[ii+1]/2),"\n");
	}
	fclose(stream);

	// Free up memory
	free_vector(Spectrum_s,1,XFactor*nt);
	free_vector(Spectrum_i,1,XFactor*nt);
	free_vector(Spectrum_p,1,XFactor*nt);

	// Update the progress bar
	if(!pProgS) pProgS->SetPos(50);

	
	//*********************************************************************
	// Propagate to the detector

	if(Dist != 0)
	{
		float *phase_x, *phase_y;
		float *phasegv_s, *phasegv_i, *phasegv_p;
		float *phasex_s, *phasex_i, *phasex_p;
		float *phasey_s, *phasey_i, *phasey_p;

		phasegv_s = vector(1,nt);
		phasegv_i = vector(1,nt);
		phasegv_p = vector(1,nt);
	
		//********************************
		// Set up the group velocity phase 
		for(ii=0; ii<nt; ++ii)
		{
			phasegv_s[ii+1] = 0.0;
			phasegv_i[ii+1] = 0.0;
			phasegv_p[ii+1] = 0.0;
		}

		//*******************************
		// Set up the y diffractive phase 

		phase_y = vector(1,ny);

		phasey_s = vector(1,ny);
		phasey_i = vector(1,ny);
		phasey_p = vector(1,ny);

	
		for(ii=0; ii<ny; ++ii)
		{
			if( ii<=(0.5*ny) )	phase_y[ii+1] = (float)pow((float)ii,(float)2);
			else				phase_y[ii+1] = (float)pow((float)(ii - ny),(float)2);
		}

		// Fill the phasey arrays
		for(ii=0; ii<ny; ++ii)
		{
			phasey_s[ii+1] = (Pi*(float)pow((float)(1/(ny*dy)),(float)2)*Lambda_s)*phase_y[ii+1];
			phasey_i[ii+1] = (Pi*(float)pow((float)(1/(ny*dy)),(float)2)*Lambda_i)*phase_y[ii+1];
			phasey_p[ii+1] = (Pi*(float)pow((float)(1/(ny*dy)),(float)2)*Lambda_p)*phase_y[ii+1];
		}

		// Free up memory
		free_vector(phase_y,1,ny);
	
		//***********************************
		// Set up x diffractive/walkoff phase 

		phase_x = vector(1,nx);
		
		phasex_s = vector(1,nx);
		phasex_i = vector(1,nx);
		phasex_p = vector(1,nx);


		// Set up phase_x array
		for(ii=0; ii<nx; ++ii)
		{
			if( ii<=(0.5*nx) )	phase_x[ii+1] = (float)pow((float)ii,(float)2);
			else				phase_x[ii+1] = (float)pow((float)(ii - nx),(float)2);
		}

		// Fill the phasex arrays (diffractive phase shift)
		for(ii=0; ii<nx; ++ii)
		{
			phasex_s[ii+1] = (Pi*(float)pow((float)(1/(nx*dx)),(float)2)*Lambda_s)*phase_x[ii+1];
			phasex_i[ii+1] = (Pi*(float)pow((float)(1/(nx*dx)),(float)2)*Lambda_i)*phase_x[ii+1];
			phasex_p[ii+1] = (Pi*(float)pow((float)(1/(nx*dx)),(float)2)*Lambda_p)*phase_x[ii+1];
		}

		// Free up memory
		free_vector(phase_x,1,nx);
		
		//****************************
		// Set up phase terms in s,i,p

		for(ii=0; ii<nx; ++ii)
		{
			for(jj=0; jj<ny; ++jj)
			{
				for(kk=0; kk<nt; ++kk)
				{
					k2 = (kk+1) + jj*nt + ii*nt*ny;

					pField->pPhaseTerm_s[k2] = -Dist*(phasegv_s[kk+1]+
									    phasey_s[jj+1] + phasex_s[ii+1]);
				
					pField->pPhaseTerm_i[k2] = -Dist*(phasegv_i[kk+1]+
									    phasey_i[jj+1] + phasex_i[ii+1]);

					pField->pPhaseTerm_p[k2] = -Dist*(phasegv_p[kk+1]+
										phasey_p[jj+1] + phasex_p[ii+1]);
				}
			}
		}

		//***************
		// Free up memory
		free_vector(phasegv_s,1,nt);
		free_vector(phasegv_i,1,nt);
		free_vector(phasegv_p,1,nt);

		free_vector(phasey_s,1,ny);
		free_vector(phasey_i,1,ny);
		free_vector(phasey_p,1,ny);

		free_vector(phasex_s,1,nx);
		free_vector(phasex_i,1,nx);
		free_vector(phasex_p,1,nx);


		//********************
		// Set up phase arrays
		complex<double> Exponent_s, Exponent_i, Exponent_p;
	
		for(ii=0; ii<nx*ny*nt; ++ii)
		{
			i2 = 2*(ii+1);

			Exponent_s = cexp(1,0,(pField->pPhaseTerm_s[ii+1]),1);
			Exponent_i = cexp(1,0,(pField->pPhaseTerm_i[ii+1]),1);
			Exponent_p = cexp(1,0,(pField->pPhaseTerm_p[ii+1]),1);

			pField->pPhase_s[i2-1] = Exponent_s.real();
			pField->pPhase_s[i2] = Exponent_s.imag();
	
			pField->pPhase_i[i2-1] = Exponent_i.real();
			pField->pPhase_i[i2] = Exponent_i.imag();

			pField->pPhase_p[i2-1] = Exponent_p.real();
			pField->pPhase_p[i2] = Exponent_p.imag();
		}

		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Propagate
		PropagateField(pField,pField->pEField_s,pField->pPhase_s,nx,ny,nt);

		if(!pProgS) pProgS->SetPos(57);

		PropagateField(pField,pField->pEField_i,pField->pPhase_i,nx,ny,nt);

		if(!pProgS) pProgS->SetPos(64);

		PropagateField(pField,pField->pEField_p,pField->pPhase_p,nx,ny,nt);

		if(!pProgS) pProgS->SetPos(70);
		
	} // end if(Dist != 0)


//*********************************************************************
// Calculate fluences

	float *TField_s, *TField_i, *TField_p;
	float *TempF_s, *TempF_i, *TempF_p;

	TField_s = vector(1,2*nx*ny*nt);
	TField_i = vector(1,2*nx*ny*nt);
	TField_p = vector(1,2*nx*ny*nt);

	TempF_s = vector(1,2*nx*(ny/2)*nt);
	TempF_i = vector(1,2*nx*(ny/2)*nt);
	TempF_p = vector(1,2*nx*(ny/2)*nt);
	
	// Mirror the EField array for Temp2
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + ((ny/2)-(kk+1))*nt;
				l2 = 2*(ll+1);

				TempF_s[i2-1] = pField->pEField_s[l2-1];
				TempF_s[i2] = pField->pEField_s[l2];
				TempF_i[i2-1] = pField->pEField_i[l2-1];
				TempF_i[i2] = pField->pEField_i[l2];
				TempF_p[i2-1] = pField->pEField_p[l2-1];
				TempF_p[i2] = pField->pEField_p[l2];
			}
		}
	}

	// Fill TField with the EField values
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<ny;++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);
	
				ll = ii + jj*nt*(ny/2) + (kk-(ny/2))*nt;
				l2 = 2*(ll+1);

				nn = ii + jj*nt*(ny/2) + kk*nt;
				n2 = 2*(nn+1);

				if(kk<(ny/2))
				{
					TField_s[i2-1] = pField->pEField_s[n2-1];
					TField_s[i2] = pField->pEField_s[n2];
					TField_i[i2-1] = pField->pEField_i[n2-1];
					TField_i[i2] = pField->pEField_i[n2];
					TField_p[i2-1] = pField->pEField_p[n2-1];
					TField_p[i2] = pField->pEField_p[n2];
				}
				else
				{
					TField_s[i2-1] = TempF_s[l2-1];
					TField_s[i2] = TempF_s[l2];
					TField_i[i2-1] = TempF_i[l2-1];
					TField_i[i2] = TempF_i[l2];
					TField_p[i2-1] = TempF_p[l2-1];
					TField_p[i2] = TempF_p[l2];
				}
				
			}
		}
	}
				
	// Free up memory
	free_vector(TempF_s,1,2*nx*(ny/2)*nt);
	free_vector(TempF_i,1,2*nx*(ny/2)*nt);
	free_vector(TempF_p,1,2*nx*(ny/2)*nt);

	FILE* stream3;
	stream3=fopen("Fluence.dat","w");

	float *Fluence_s, *Fluence_i, *Fluence_p;

	Fluence_s=vector(1,nx*ny);
	Fluence_i=vector(1,nx*ny);
	Fluence_p=vector(1,nx*ny);

	for(ii=0;ii<nx*ny;++ii)
	{
		Fluence_s[ii+1] = 0.0;
		Fluence_i[ii+1] = 0.0;
		Fluence_p[ii+1] = 0.0;
	}

	// Calculate the fluences
	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<ny;++jj)
		{
			for(kk=0;kk<nt;++kk)
			{
				k2=(kk+1) + jj*nt + ii*nt*ny;
				i2=2*k2;
				l2=(jj+1) + ii*ny;

				Fluence_s[l2] = Fluence_s[l2] +
					(pow(TField_s[i2-1],2) + pow(TField_s[i2],2));

				Fluence_i[l2] = Fluence_i[l2] +
					(pow(TField_i[i2-1],2) + pow(TField_i[i2],2));

				Fluence_p[l2] = Fluence_p[l2] +
					(pow(TField_p[i2-1],2) + pow(TField_p[i2],2));
			}
		}
	}

	// Free up memory
	free_vector(TField_s,1,2*nx*ny*nt);
	free_vector(TField_i,1,2*nx*ny*nt);
	free_vector(TField_p,1,2*nx*ny*nt);

	for(ii=0;ii<nx*ny;++ii)
	{
		Fluence_s[ii+1] = dt*((epsilon_0*c)/2)*Fluence_s[ii+1];
		Fluence_i[ii+1] = dt*((epsilon_0*c)/2)*Fluence_i[ii+1];
		Fluence_p[ii+1] = dt*((epsilon_0*c)/2)*Fluence_p[ii+1];
	}

	for(ii=0;ii<ny;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			k2=(ii+1) + jj*ny;
			fprintf(stream3,"%e %e %e %e %e %s",
				pField->pXgrid[jj+1],pField->pYgrid[ii+1],
				Fluence_s[k2],Fluence_i[k2],Fluence_p[k2],"\n");
		}
	}
	fclose(stream3);

	// Check for Windows messages, and return if 
	// the stop button has been pressed
	CheckWinMessages(10000);
	if( GetStop() )
	{
		return;
	}

	//******************************
	// Calculate output pulse energy
	double Epulse_s, Epulse_i, Epulse_p;
	double Tempxs, Tempxi, Tempxp;
	double Tempys, Tempyi, Tempyp;

	Epulse_s = 0.0;
	Epulse_i = 0.0;
	Epulse_p = 0.0;

	Tempxs = 0.0;
	Tempxi = 0.0;
	Tempxp = 0.0;

	Tempys = 0.0;
	Tempyi = 0.0;
	Tempyp = 0.0;
	
	for(ii=0;ii<ny;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			k2=(ii+1) + jj*ny;

			Tempxs += Fluence_s[k2];
			Tempxi += Fluence_i[k2];
			Tempxp += Fluence_p[k2];
		}

		Tempys += Tempxs;
		Tempyi += Tempxi;
		Tempyp += Tempxp;

		// reset temp
		Tempxs = 0.0;
		Tempxi = 0.0;
		Tempxp = 0.0;
	}

	Epulse_s = dx*dy*Tempys;
	Epulse_i = dx*dy*Tempyi;
	Epulse_p = dx*dy*Tempyp;

	// Save output energy values to CGlobalVar variables
	pGV->dSetGlobal(EPULSE_S, Epulse_s);
	pGV->dSetGlobal(EPULSE_I, Epulse_i);
	pGV->dSetGlobal(EPULSE_P, Epulse_p);

	// Free up memory
	free_vector(Fluence_s,1,nx*ny);
	free_vector(Fluence_i,1,nx*ny);
	free_vector(Fluence_p,1,nx*ny);
	
	// Update the progress bar
	pProgS->SetPos(80);
	

//*********************************************************************
// Calculate maximum irradiance

	double MaxIr_s, MaxIr_i, MaxIr_p, Temp_s, Temp_i, Temp_p;

	MaxIr_s = 0.0;
	MaxIr_i = 0.0;
	MaxIr_p = 0.0;

	for(ii=0;ii<nx*(ny/2)*nt;++ii)
	{
		i2=2*(ii+1);

		Temp_s = ((epsilon_0*c)/2)*(pow(pField->pEField_s[i2-1],2) + pow(pField->pEField_s[i2],2));
		Temp_i = ((epsilon_0*c)/2)*(pow(pField->pEField_i[i2-1],2) + pow(pField->pEField_i[i2],2));
		Temp_p = ((epsilon_0*c)/2)*(pow(pField->pEField_p[i2-1],2) + pow(pField->pEField_p[i2],2));

		if(Temp_s > MaxIr_s)
		{
			MaxIr_s = Temp_s;
		}

		if(Temp_i > MaxIr_i)
		{
			MaxIr_i = Temp_i;
		}

		if(Temp_p > MaxIr_p)
		{
			MaxIr_p = Temp_p;
		}

	}

	// Save output irradiance values to CGlobalVar variables
	pGV->dSetGlobal(IROUT_S, MaxIr_s);
	pGV->dSetGlobal(IROUT_I, MaxIr_i);
	pGV->dSetGlobal(IROUT_P, MaxIr_p);


//*********************************************************************
// Write output pulse energy and maximum irradiance to output file
	
	if(m_bDeltakOn == TRUE)
	{
		stream=fopen("Mix_outDK.dat","a+");
		
		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Output Peak Irradiance (W/m^2)        = ",MaxIr_s,
			MaxIr_i, MaxIr_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Output Pulse Energy (J)               = ",Epulse_s,
			Epulse_i, Epulse_p);

		fprintf(stream,"%s %5.3f",
			"\n Wavevector mismatch deltak (1/mm)     = ", pBt->GetVar(DELTAK));
		
		fclose(stream);
	}

	if(m_bErgOn == TRUE)
	{
		stream=fopen("Mix_outERG.dat","a+");
		
		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Output Peak Irradiance (W/m^2)        = ",MaxIr_s,
			MaxIr_i, MaxIr_p);

		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Output Pulse Energy (J)               = ",Epulse_s,
			Epulse_i, Epulse_p);
		
		fprintf(stream,"%s %7.3e %10.3e %9.3e",
			"\n Input Pulse Energies (s, i, p) (J)    = ", pBt->GetVar(ENERGY_S), 
			pBt->GetVar(ENERGY_I), pBt->GetVar(ENERGY_P));

		fclose(stream);
	}

	if(m_bLensOn == TRUE)
	{
		stream=fopen("Mix_outLENS.dat","a+");
		
		fprintf(stream,"%s %7.3e %7.3e %9.3e",
			"\n Output Pulse Energy (J)               = ",
			Epulse_s, Epulse_i, Epulse_p);
		
		fprintf(stream,"%s %5.3e %5.3e %5.3e",
			"\n Input Beam Diameters (s, i, p) (mm)   = ", 
			pBt->GetVar(BEAMDIAM_S), pBt->GetVar(BEAMDIAM_I), pBt->GetVar(BEAMDIAM_P));

		fprintf(stream,"%s %5.3e %5.3e %5.3e",
			"\n Input Radii of Curvature (s, i, p)(mm)= ", 
			pBt->GetVar(RADCURV_S), pBt->GetVar(RADCURV_I), pBt->GetVar(RADCURV_P));

		fclose(stream);
	}

	stream=fopen("Mix_out.dat","a+");
	fprintf(stream,"%s %7.3e %10.3e %9.3e",
		"\n Output Peak Irradiance (W/m^2)        = ",MaxIr_s,
		MaxIr_i, MaxIr_p);

	fprintf(stream,"%s %7.3e %10.3e %9.3e",
		"\n Output Pulse Energy (J)               = ",Epulse_s,
		Epulse_i, Epulse_p);

	fclose(stream);


//*********************************************************************
// Propagate to far field and calculate fluence

	float *FField_s, *FField_i, *FField_p;
	float *agrid, *bgrid;
	double dthetax, dthetay;

	dthetax = pGV->dGetGlobal(DTHETAX);
	dthetay = pGV->dGetGlobal(DTHETAY);

	agrid = vector(1,nx);
	bgrid = vector(1,ny);

	FField_s = vector(1,nx*ny);
	FField_i = vector(1,nx*ny);
	FField_p = vector(1,nx*ny);

	// Initialize farfields to zero
	for(ii=0;ii<nx*ny;++ii)
	{
		FField_s[ii+1] = 0.0;
		FField_i[ii+1] = 0.0;
		FField_p[ii+1] = 0.0;
	}

	// Call the FarField function
	FarField(pField,pField->pEField_s,FField_s,nx,ny,nt);

	pProgS->SetPos(85);

	FarField(pField,pField->pEField_i,FField_i,nx,ny,nt);

	pProgS->SetPos(87);

	FarField(pField,pField->pEField_p,FField_p,nx,ny,nt);

	pProgS->SetPos(90);

	// Setup agrid and bgrid
	for(ii=0;ii<nx;++ii)
	{
		agrid[ii+1] = ((-0.5*nx) + ii)*dthetax;
	}

	for(ii=0;ii<ny;++ii)
	{
		bgrid[ii+1] = ((-0.5*ny) + ii)*dthetay;
	}
	

	//****************************************
	// Save farfield results to 'Farfield.dat'

	stream=fopen("Farfield.dat","w");
	for(ii=0;ii<ny;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			k2=(ii+1) + jj*ny;
			fprintf(stream,"%e %e %e %e %e %s",
				agrid[jj+1],bgrid[ii+1],
				FField_s[k2],FField_i[k2],FField_p[k2],"\n");
		}
	}
	fclose(stream);

	// Free up memory
	free_vector(agrid,1,nx);
	free_vector(bgrid,1,ny);
	free_vector(FField_s,1,nx*ny);
	free_vector(FField_i,1,nx*ny);
	free_vector(FField_p,1,nx*ny);

	// Update the progress bar
	pProgS->SetPos(93);


//*********************************************************************
// Pad farfield fluence to match the k resolution of the finer
// grid mesh direction if x-dimension does not equal the y-dimension


/*
	if(nx<ny)
	{
		
		agrid = vector(1,ny);

		float *NewField_s,*NewField_i,*NewField_p;

		NewField_s = vector(1,ny*ny);
		NewField_i = vector(1,ny*ny);
		NewField_p = vector(1,ny*ny);

		// Initialize NewField to zero
		for(ii=0;ii<ny*ny;++ii)
		{
			NewField_s[ii+1] = 0.0;
			NewField_i[ii+1] = 0.0;
			NewField_p[ii+1] = 0.0;
		}

		// Make FField symmetric in y-direction
		for(ii=0;ii<ny;++ii)
		{
			for(jj=0;jj<nx;++jj)
			{
				k2 = ii+1 + jj*ny;

				if(ii==0)
				{
					FField_s[k2] = 0.0;
					FField_i[k2] = 0.0;
					FField_p[k2] = 0.0;
				}
				else if(ii<(ny/2))
				{
					i2 = (ny - (ii-1)) + jj*ny;

					FField_s[k2] = FField_s[i2];
					FField_i[k2] = FField_i[i2];
					FField_p[k2] = FField_p[i2];
				}
				else
				{
					i2 = ii+1 + jj*ny;

					FField_s[k2] = FField_s[i2];
					FField_i[k2] = FField_i[i2];
					FField_p[k2] = FField_p[i2];
				}
				
			}
		}
		
		long diff = (ny-nx)/2;
	
		// Add the elements of FField to NewField
		for(ii=0;ii<ny;++ii)
		{
			for(jj=0;jj<ny;++jj)
			{
				k2 = (ii+1) + jj*ny;

				if( (jj>=diff) && (jj<(ny - diff)) )
				{
					NewField_s[k2] = FField_s[k2-diff*ny];
					NewField_i[k2] = FField_i[k2-diff*ny];
					NewField_p[k2] = FField_p[k2-diff*ny];
				}
			}
		}

		// Setup agrid
		for(kk=0;kk<ny;++kk)
		{
			agrid[kk+1] = ((-0.5*ny) + kk);
		}

		// Set the global variables for max and min farfield
		*pMinff = agrid[1];
		*pMaxff = agrid[ny];


		//***************************
		// Print out farfield results

		stream=fopen("Farfield.dat","w");

		for(ii=0;ii<ny;++ii)
		{
			for(jj=0;jj<ny;++jj)
			{
				k2=(ii+1) + jj*ny;
				fprintf(stream,"%e %e %e %e %e %s",
					agrid[jj+1]*dthetax,agrid[ii+1]*dthetay,
					NewField_s[k2],NewField_i[k2],NewField_p[k2],"\n");
			}
		}
		fclose(stream);

		// Free up memory
		free_vector(agrid,1,ny);
		free_vector(NewField_s,1,ny*ny);
		free_vector(NewField_i,1,ny*ny);
		free_vector(NewField_p,1,ny*ny);

	}
	else if(nx>ny)
	{

		agrid = vector(1,nx);

		float *NewField_s,*NewField_i,*NewField_p;

		NewField_s = vector(1,nx*nx);
		NewField_i = vector(1,nx*nx);
		NewField_p = vector(1,nx*nx);

		// Initialize NewField to zero
		for(ii=0;ii<nx*nx;++ii)
		{
			NewField_s[ii+1] = 0.0; 
			NewField_i[ii+1] = 0.0;
			NewField_p[ii+1] = 0.0;
		}

		long diff = (nx-ny)/2;
		
		long count = 0;

		// Add the elements of FField to NewField
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<nx;++jj)
			{
				k2 = (jj+1) + ii*nx;

				if( (jj>=diff) && (jj<(nx - diff)) )
				{
					count += 1;

					NewField_s[k2] = FField_s[count];
					NewField_i[k2] = FField_i[count];
					NewField_p[k2] = FField_p[count];
				}
			}
		}

		// Setup agrid
		for(kk=0;kk<nx;++kk)
		{
			agrid[kk+1] = ((-0.5*nx) + kk);
		}

		// Set the global variables for max and min farfield
		*pMinff = agrid[1];
		*pMaxff = agrid[nx];


		//***************************
		// Print out farfield results

		stream=fopen("Farfield.dat","w");
	
		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<nx;++jj)
			{
				k2=(ii+1) + jj*nx;
				fprintf(stream,"%e %e %e %e %e %s",
					agrid[jj+1]*dthetax,agrid[ii+1]*dthetay,
					NewField_s[k2],NewField_i[k2],NewField_p[k2],"\n");
			}
		}
		fclose(stream);


		// Free up memory
		free_vector(agrid,1,nx);
		free_vector(NewField_s,1,nx*nx);
		free_vector(NewField_i,1,nx*nx);
		free_vector(NewField_p,1,nx*nx);
		
	}
	else
	{

		agrid = vector(1,nx);

		// Setup agrid
		for(kk=0; kk<nx; ++kk)
		{
			agrid[kk+1] = ((-0.5*nx) + kk);
		}

		// Set the global variables for max and min farfield
		*pMinff = agrid[1];
		*pMaxff = agrid[nx];


		//***************************
		// Print out farfield results

		stream=fopen("Farfield.dat","w");

		for(ii=0;ii<nx;++ii)
		{
			for(jj=0;jj<ny;++jj)
			{
				k2=(ii+1) + jj*ny;
				fprintf(stream,"%e %e %e %e %e %s",
					agrid[jj+1]*dthetax,agrid[ii+1]*dthetay,
					FField_s[k2],FField_i[k2],FField_p[k2],"\n");
			}
		}
		fclose(stream);

		// Free up memory
		free_vector(agrid,1,nx);

	}
*/
	

//*********************************************************************
// Save the electric fields to output files

	FILE *streams, *streami, *streamp;
	
	streams=fopen("EField_s.dat","w");
	streami=fopen("EField_i.dat","w");
	streamp=fopen("EField_p.dat","w");

	for(ii=0;ii<nx*(ny/2)*nt;++ii)
	{
		i2=2*(ii+1);

		fprintf(streams,"%e %e %s",
			pField->pEField_s[i2-1],pField->pEField_s[i2],"\n");
		fprintf(streami,"%e %e %s",
			pField->pEField_i[i2-1],pField->pEField_i[i2],"\n");
		fprintf(streamp,"%e %e %s",
			pField->pEField_p[i2-1],pField->pEField_p[i2],"\n");
	}

	fclose(streams);
	fclose(streami);
	fclose(streamp);

}	// End CalculateOutput


//************************ Function Farfield **************************
//
// This function calculates the farfield fluence.
//
// Called from: CalculateOutput
//
//*********************************************************************

void FarField(CFields* pField, float* EField, float* FField, int nx, int ny, int nt)
{

	fftw_plan p_x, p_y;
	
	p_x = fftw_create_plan(nx, FFTW_FORWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
	p_y = fftw_create_plan(ny, FFTW_FORWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);

	int ii,i2,jj,kk,k2,ll,l2,nn,n2;

	int Half;

	float *TempField, *Temp1, *Temp2;
	float *TField, *TempF;

	TempField = vector(1,2*nx*ny*nt);
	Temp1 = vector(1,nx*ny);
	Temp2 = vector(1,nx*ny);

	TField = vector(1,2*nx*ny*nt);
	TempF = vector(1,2*nx*(ny/2)*nt);
	
	// Mirror the EField array for Temp2
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + ((ny/2)-(kk+1))*nt;
				l2 = 2*(ll+1);

				TempF[i2-1] = EField[l2-1];
				TempF[i2] = EField[l2];
			}
		}
	}

	// Fill TempField with the EField values
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<ny;++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);
	
				ll = ii + jj*nt*(ny/2) + (kk-(ny/2))*nt;
				l2 = 2*(ll+1);

				nn = ii + jj*nt*(ny/2) + kk*nt;
				n2 = 2*(nn+1);

				if(kk<(ny/2))
				{
					TField[i2-1] = EField[n2-1];
					TField[i2] = EField[n2];
				}
				else
				{
					TField[i2-1] = TempF[l2-1];
					TField[i2] = TempF[l2];
				}
				
			}
		}
	}
				
	// Free up memory
	free_vector(TempF,1,2*nx*(ny/2)*nt);
		
	// Fourier Transform of EField in x-direction
	for(ii=0;ii<nt;++ii)
	{
		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<ny;++jj)
		{

			for(kk=0;kk<nx;++kk)
			{
				k2 = ii + jj*nt + kk*nt*ny;
				i2 = 2*(k2+1);

				// Fill FFT variable with TField
				pField->pFarFFT_x[kk].re = TField[i2-1];
				pField->pFarFFT_x[kk].im = TField[i2];
			}

			// FFT
			fftw_one(p_x, &pField->pFarFFT_x[0], NULL);

			// Set TempField to the new values
			for(nn=0;nn<nx;++nn)
			{
				n2 = ii + jj*nt + nn*nt*ny;
				i2 = 2*(n2+1);

				TempField[i2-1] = pField->pFarFFT_x[nn].re;
				TempField[i2] = pField->pFarFFT_x[nn].im;
			}

		}
	}

	// Free up memory
	free_vector(TField,1,2*nx*ny*nt);

	// Fourier Transform of EField in y-direction
	for(ii=0;ii<nt;++ii)
	{

		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<nx;++jj)
		{

			for(kk=0;kk<ny;++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				// Fill FFT variable with EField
				pField->pFarFFT_y[kk].re = TempField[i2-1];
				pField->pFarFFT_y[kk].im = TempField[i2];
			}

			// FFT
			fftw_one(p_y, &pField->pFarFFT_y[0], NULL);

			// Set FFT variable to the new values
			for(nn=0;nn<ny;++nn)
			{
				n2 = ii + jj*nt*ny + nn*nt;
				i2 = 2*(n2+1);

				TempField[i2-1] = pField->pFarFFT_y[nn].re;
				TempField[i2] = pField->pFarFFT_y[nn].im;
			}

		}
	}

	// Calculate the farfield fluence 
	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<ny;++jj)
		{
			for(kk=0;kk<nt;++kk)
			{
				k2=kk + jj*nt + ii*nt*ny;
				i2=2*(k2+1);
				l2=(jj+1) + ii*ny;

				FField[l2] = FField[l2] +
					(pow(TempField[i2-1],2) + pow(TempField[i2],2));
			}
		}
	}

	// Free up memory
	free_vector(TempField,1,2*nx*ny*nt);
	
	for(ii=0;ii<nx*ny;++ii)
	{
		Temp1[ii+1] = FField[ii+1];
	}

	// Reshape in the x-direction
	Half = ((nx*ny)/2);

	for(ii=0;ii<Half;++ii)
	{
		Temp2[ii+1] = Temp1[Half+ii+1];
		Temp2[Half+ii+1] = Temp1[ii+1];
	}

	// Reshape in the y-direction
	Half = (ny/2);

	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<Half;++jj)
		{
			l2 = jj + ii*ny;

			FField[l2+1] = Temp2[(l2+1) + Half];
			FField[(l2+1) + Half] = Temp2[l2+1];
		}
	}

	// Free up memory
	fftw_destroy_plan(p_x);
	fftw_destroy_plan(p_y);

	free_vector(Temp1,1,nx*ny);
	free_vector(Temp2,1,nx*ny);

}	// End FarField


//*************************** Function Spectra ************************
//
// This function calculates the spectra.
//
// Called from: CalculateOutput
//
//*********************************************************************

void Spectra(CFields* pField, float* EField, float* Spectra, 
			 int nx, int ny, int nt, long XFactor, double* maxnum)
{

	fftw_plan p_t;

	// FFT from time to frequency
	p_t = fftw_create_plan(XFactor*nt, FFTW_FORWARD,
							FFTW_ESTIMATE | FFTW_IN_PLACE);


	int ii,i2,jj,j2,kk,k2,l2,nn,n2;

	int Half;


	float *FFTSpect, *TempSp, *TempSp1;

	FFTSpect = vector(1,2*nx*(ny/2)*XFactor*nt);
	TempSp = vector(1,XFactor*nt);
	TempSp1 = vector(1,XFactor*(ny/2)*nt);
	
	// Initialize TempSp to zero
	for(ii=0;ii<XFactor*nt;++ii)
	{
		TempSp[ii+1] = 0.0;
	}

	// Initialize TempSp1 to zero
	for(ii=0;ii<XFactor*(ny/2)*nt;++ii)
	{
		TempSp1[ii+1] = 0.0;
	}

	// Fourier Transform of EField in t-direction
	for(ii=0;ii<nx;++ii)
	{
		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<XFactor*nt;++kk)
			{
				k2 = kk + jj*nt + ii*nt*(ny/2);
				i2 = 2*(k2+1);

				if(kk<nt)	// fill with EField
				{
					pField->pSpecFFT[kk].re = EField[i2-1];
					pField->pSpecFFT[kk].im = EField[i2];
				}
				else	// pad with zeros
				{
					pField->pSpecFFT[kk].re = 0.0;
					pField->pSpecFFT[kk].im = 0.0;
				}

			}

			// FFT
			fftw_one(p_t, &pField->pSpecFFT[0], NULL);

			// Set FFTSpect_s to the new values
			for(nn=0;nn<XFactor*nt;++nn)
			{
				n2 = nn + jj*XFactor*nt + ii*XFactor*nt*(ny/2);
				i2 = 2*(n2+1);

				FFTSpect[i2-1] = pField->pSpecFFT[nn].re;
				FFTSpect[i2] = pField->pSpecFFT[nn].im;
			}

		}
	}

	// Sum EField^2 for real/imag and x-direction
	for(ii=0;ii<XFactor*nt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<nx;++kk)
			{
				j2=(ii+1) + jj*XFactor*nt + kk*XFactor*nt*(ny/2);
				i2=2*j2;
				l2=(jj+1) + ii*(ny/2);

				TempSp1[l2]=TempSp1[l2] + 
					(pow(FFTSpect[i2-1],2) + pow(FFTSpect[i2],2));
			}
		}
	}

	// Free up memory
	free_vector(FFTSpect,1,2*nx*(ny/2)*XFactor*nt);

	// Sum EField^2 in y-direction
	for(ii=0;ii<XFactor*nt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			j2=(jj+1) + ii*(ny/2);
			TempSp[ii+1] = TempSp[ii+1] + TempSp1[j2];
		}
	}

	// Free up memory
	free_vector(TempSp1,1,XFactor*(ny/2)*nt);
	
	Half = (XFactor*nt)/2;
	
	// Prepare the Spectrum arrays
	for(ii=0;ii<Half;++ii)
	{
		i2=2*(ii+1);
		
		Spectra[ii+1] = TempSp[Half+(ii+1)];
		Spectra[Half+(ii+1)] = TempSp[ii+1];
	}

	// Reverse the order of the array
	for(ii=0;ii<XFactor*nt;++ii)
	{
		if(ii != 0)
		{
			TempSp[ii+1] = Spectra[(XFactor*nt) - (ii - 1)];
		}
		else
		{
			TempSp[ii+1] = Spectra[ii+1];
		}
	}

	// Refill the spectrum array (factor of 2 is for total ny)
	for(ii=0;ii<XFactor*nt;++ii)
	{
		Spectra[ii+1] = 2*TempSp[ii+1];

		if(*maxnum < Spectra[ii+1])
		{
			*maxnum = Spectra[ii+1];
		}
	}

	// Free up memory
	fftw_destroy_plan(p_t);
	free_vector(TempSp,1,XFactor*nt);
	
}	// End Spectra


//********************* Function AnalyzeBeams_t ***********************
//
// This function performs the beam analysis in the time domain.  It 
// returns a CBeams object containing the pertinant information.
//
// Called from: GraphDlg::OnAnalyze
//
//*********************************************************************

void AnalyzeBeams_t(float* Power,float* EField,CBeams* pBeams,
					double Lambda,double MaxP,long nx,long ny,long nt,
					double dx,double dy,double dt,CProgressCtrl* pProgS,
					CProgressCtrl* pProgT,int T,double* Mavg)
{
	const double Pi = 3.14159265359;
	const double epsilon_0 = 8.854187817e-12;	// Permittivity of vacuum [F/m]
	const double c = 2.99792458e+8;				// Speed of light in vacuum [m/s]


	// Initialize local variables
	double pavg, sigsqavg, aavg, xybaravg, tiltavg;
	double sigsqsavg, sigsqoavg;

	int ii,i2,jj,j2,kk,l2;
	long right, left;

	bool done;
	done = false;

	MaxP = 0.0;


//************************** Time Processing **************************

	// Calculate the power
	float *Temp_1;

	Temp_1 = vector(1,(ny/2)*nt);

	// Initialize Temp_1 to zero
	for(ii=0;ii<(ny/2)*nt;++ii)
	{
		Temp_1[ii+1] = 0.0;
	}

	// Initialize Power to zero
	for(ii=0;ii<nt;++ii)
	{
		Power[ii+1] = 0.0;
	}

	// Sum EField^2 for real/imag and x-direction
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<nx;++kk)
			{
				j2=(ii+1) + jj*nt + kk*(ny/2)*nt;
				i2=2*j2;
				l2=(jj+1) + ii*(ny/2);

				Temp_1[l2]=Temp_1[l2] + 
					(pow(EField[i2-1],2) + pow(EField[i2],2));
			}
		}
	}

	// Sum EField^2 in y-direction
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			j2=(jj+1) + ii*(ny/2);
			
			Power[ii+1] = Power[ii+1] + Temp_1[j2];
		}
	}

	// Free up memory
	free_vector(Temp_1,1,(ny/2)*nt);
	
	for(ii=0; ii<nt; ++ii)	// factor of 2 is for total ny
	{
		Power[ii+1] = 2*dx*dy*((epsilon_0*c)/2)*Power[ii+1];
		
		if(MaxP < Power[ii+1])
		{
			MaxP = Power[ii+1];
		}
	}
	

	//***************************************************************
	// Calculate MSquared
	
	Msquared(Power,EField,pBeams,Lambda,MaxP,nx,ny,nt,dx,dy,pProgS);

	// Update the progress bars
	pProgT->SetPos(T+7);
	

	//*****************************************************************
	// Keep only the field slices with significant power for the MSQUARE 
	// calculations to save time and make nicer plots without the ragged
	// tilts, etc. that arise in the far wings of the pulse

	// Find the leftmost array value greater than 0.001 norm power
	for(ii=0;ii<nt;++ii)
	{
		if(!done)			
		{
			if(Power[ii+1]/MaxP > 0.001)
			{
				left = ii;
				done = true;
			}
		}
	}

	// Reset done
	done = false;
	
	// Find rightmost array value greater than 0.001 norm power
	for(ii=left+1;ii<nt;++ii)
	{
		if(!done)	
		{
			if(Power[nt - ii]/MaxP > 0.001)
			{
				right = nt - ii;
				done = true;
			}
		}
	}

	
	// Put in continuation values at low field pts (norm power < .001)
	for(ii=0;ii<left;++ii)
	{
		pBeams->SetBeamx(ii+1,
						pBeams->GetTiltx(left+1),
						pBeams->GetMsqrx(left+1),
						pBeams->GetZOx(left+1),
						pBeams->GetWavofCurvx(left+1),
						pBeams->GetSigsqx(left+1),
						pBeams->GetSigsqs_x(left+1),
						pBeams->GetAsubx(left+1),
						pBeams->GetXbar(left+1));

		pBeams->SetBeamy(ii+1,
						pBeams->GetTilty(left+1),
						pBeams->GetMsqry(left+1),
						pBeams->GetZOy(left+1),
						pBeams->GetWavofCurvy(left+1),
						pBeams->GetSigsqy(left+1),
						pBeams->GetSigsqs_y(left+1),
						pBeams->GetAsuby(left+1),
						pBeams->GetYbar(left+1));
	}

	for(ii=right;ii<nt;++ii)
	{
		pBeams->SetBeamx(ii+1,
						pBeams->GetTiltx(right),
						pBeams->GetMsqrx(right),
						pBeams->GetZOx(right),
						pBeams->GetWavofCurvx(right),
						pBeams->GetSigsqx(right),
						pBeams->GetSigsqs_x(right),
						pBeams->GetAsubx(right),
						pBeams->GetXbar(right));

		pBeams->SetBeamy(ii+1,
						pBeams->GetTilty(right),
						pBeams->GetMsqry(right),
						pBeams->GetZOy(right),
						pBeams->GetWavofCurvy(right),
						pBeams->GetSigsqy(right),
						pBeams->GetSigsqs_y(right),
						pBeams->GetAsuby(right),
						pBeams->GetYbar(right));
	}


	//*****************************************************************
	// Calculate average tilt, curvature, M^2, and power

	// First for x
	pavg = 0.0;
	sigsqavg = 0.0;
	aavg = 0.0;
	xybaravg = 0.0;
	tiltavg = 0.0;
	sigsqsavg = 0.0;
	sigsqoavg = 0.0;
	
	for(ii=0;ii<nt;++ii)
	{
		pavg += Power[ii+1];
	}

	pavg = double(1./pavg);		// average power

	for(ii=0;ii<nt;++ii)
	{
		sigsqavg += pBeams->GetSigsqx(ii+1)*Power[ii+1];
		aavg += pBeams->GetAsubx(ii+1)*Power[ii+1];
		xybaravg += pBeams->GetXbar(ii+1)*Power[ii+1];
		tiltavg += pBeams->GetTiltx(ii+1)*Power[ii+1];
		sigsqsavg += pBeams->GetSigsqs_x(ii+1)*Power[ii+1];
	}

	sigsqavg *= pavg;		// average sigsqx

	aavg *=pavg;			// average asubx
	xybaravg *= pavg;		// average xbar
	tiltavg *=pavg;			// average tilt x
	sigsqsavg *= pavg;		// average sigsqs x
	sigsqoavg = sigsqavg - pow((aavg + 2.e-6*tiltavg*xybaravg),2)/
							  (4.e6*sigsqsavg*pow(Lambda,2));
	
	Mavg[1] = (4*Pi)*sqrt(sigsqoavg*sigsqsavg);	// average msquared x

	// Now for y
	sigsqavg = 0.0;
	aavg = 0.0;
	xybaravg = 0.0;
	tiltavg = 0.0;
	sigsqsavg = 0.0;
	sigsqoavg = 0.0;
	
	for(ii=0;ii<nt;++ii)
	{
		sigsqavg += pBeams->GetSigsqy(ii+1)*Power[ii+1];
		aavg += pBeams->GetAsuby(ii+1)*Power[ii+1];
		xybaravg += pBeams->GetYbar(ii+1)*Power[ii+1];
		tiltavg += pBeams->GetTilty(ii+1)*Power[ii+1];
		sigsqsavg += pBeams->GetSigsqs_y(ii+1)*Power[ii+1];
	}

	sigsqavg *= pavg;		// average sigsqx
	aavg *=pavg;			// average asuby
	xybaravg *= pavg;		// average ybar
	tiltavg *=pavg;			// average tilt y
	sigsqsavg *= pavg;		// average sigsqs y
	sigsqoavg = sigsqavg - pow((aavg + 2.e-6*tiltavg*xybaravg),2)/
							  (4.e6*sigsqsavg*pow(Lambda,2));

	CString s;
	s.Format("%s %e","myavg should be",
		(4*Pi)*sqrt(sigsqoavg*sigsqsavg));
//	AfxMessageBox(s);
	
	Mavg[2] = (4*Pi)*sqrt(sigsqoavg*sigsqsavg);	// average msquared y



	CString s1;
	s1.Format("%s %e",
		"M^2 perp (from Beam_t) = ",Mavg[2]);
//	AfxMessageBox(s1);
	
}	// End AnalyzeBeams_t



//********************** Function AnalyzeBeams_w **********************
//
// This function performs the beam analysis in the frequency domain.
// It returns a CBeams object containing the pertinant information.
//
// Called from: GraphDlg::OnAnalyze
//
//*********************************************************************

void AnalyzeBeams_w(float* Spectrum,float* EField,CBeams* pSBeam,
					double Lambda,double MaxSp,long nx,long ny,long nt,
					double dx,double dy,double dt,CProgressCtrl* pProgS,
					CProgressCtrl* pProgT,int T,double* Mavg)
{
	
	const double Pi = 3.14159265359;

	// Initialize local variables
	double pavg;
	double sigsqavg, aavg, xybaravg, tiltavg;
	double sigsqsavg, sigsqoavg;
	
	int ii,i2,jj,kk,k2,l2,nn,n2;

	bool done;
	long left, right;


//************************** Frequency Processing *********************

	long Newt, XFactor;
	XFactor = 4;	// This is a factor used for spectral smoothing
	Newt = XFactor*nt;

	long j2, Half;

	float *FFTSpect, *TempSp, *TempSp1;

	FFTSpect = vector(1,2*nx*(ny/2)*Newt);
	
	fftw_plan p_t;
	
	p_t = fftw_create_plan(Newt, FFTW_FORWARD,
							FFTW_ESTIMATE | FFTW_IN_PLACE);

	fftw_complex* FFT_t;

	FFT_t = (fftw_complex *) malloc(Newt*sizeof(fftw_complex));


	//*****************************************************************
	// Fourier Transform of EField in t-direction
	for(ii=0;ii<nx;++ii)
	{
		// Check for Windows messages, and return if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<Newt;++kk)
			{
				k2 = kk + jj*nt + ii*nt*(ny/2);
				i2 = 2*(k2+1);

				if(kk<nt)	// fill with EField
				{
					FFT_t[kk].re = EField[i2-1];
					FFT_t[kk].im = EField[i2];
				}
				else	// pad with zeros
				{
					FFT_t[kk].re = 0.0;
					FFT_t[kk].im = 0.0;
				}

			}

			// FFT
			fftw_one(p_t, &FFT_t[0], NULL);

			// Set FFTSpect_s and EField to the new values
			for(nn=0;nn<Newt;++nn)
			{
				n2 = nn + jj*Newt + ii*Newt*(ny/2);
				i2 = 2*(n2+1);

				FFTSpect[i2-1] = FFT_t[nn].re;
				FFTSpect[i2] = FFT_t[nn].im;
			}

		}
	}


	//*****************************************************************
	// Reset FFTSpect so zero frequency is at the center

	TempSp = vector(1,2*Newt);
	TempSp1 = vector(1,2*Newt);
		
	Half = (Newt)/2;

	for(ii=0;ii<nx;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<Newt;++kk)
			{
				j2 = 2*(kk+1);
				k2 = kk + jj*Newt + ii*Newt*(ny/2);
				i2 = 2*(k2+1);

				// Fill TempSp2 with the FFTSpect array
				TempSp[j2-1] = FFTSpect[i2-1];
				TempSp[j2] = FFTSpect[i2];
			}
			
			for(kk=0;kk<Half;++kk)
			{
				k2=2*(kk+1);
				
				// Reshape for FFTSpect
				TempSp1[k2-1] = TempSp[2*Half+k2-1];
				TempSp1[k2] = TempSp[2*Half+k2];
				
				TempSp1[2*Half+k2-1] = TempSp[k2-1];
				TempSp1[2*Half+k2] = TempSp[k2];
			}

			// Reverse the order of the array
			for(kk=0;kk<Newt;++kk)
			{
				k2=2*(kk+1);

				if(kk != 0)
				{
					TempSp[k2-1] = TempSp1[(2*Newt) - (k2-3)];
					TempSp[k2] = TempSp1[(2*Newt) - (k2-4)];
				}
				else
				{
					TempSp[k2-1] = TempSp1[k2-1];
					TempSp[k2] = TempSp1[k2];
				}
			}

			// Refill the FFTSpect array 
			for(kk=0;kk<Newt;++kk)
			{
				j2 = 2*(kk+1);
				k2 = kk + jj*Newt + ii*Newt*(ny/2);
				i2 = 2*(k2+1);

				FFTSpect[i2-1] = TempSp[j2-1];
				FFTSpect[i2] = TempSp[j2];
			}
		}
	}

	// Free up memory
	free_vector(TempSp,1,2*Newt);
	free_vector(TempSp1,1,2*Newt);


	//*****************************************************************
	// Calculate the spectra

	TempSp = vector(1,Newt);
	TempSp1 = vector(1,(ny/2)*Newt);
	
	// Initialize TempSp to zero
	for(ii=0;ii<Newt;++ii)
	{
		TempSp[ii+1] = 0.0;
	}

	// Initialize TempSp1 to zero
	for(ii=0;ii<(ny/2)*Newt;++ii)
	{
		TempSp1[ii+1] = 0.0;
	}
	
	// Sum EField^2 for real/imag and x-direction
	for(ii=0;ii<Newt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			for(kk=0;kk<nx;++kk)
			{
				j2=(ii+1) + jj*Newt + kk*Newt*(ny/2);
				i2=2*j2;
				l2=(jj+1) + ii*(ny/2);

				TempSp1[l2]=TempSp1[l2] + 
					(pow(FFTSpect[i2-1],2) + pow(FFTSpect[i2],2));
			}
		}
	}

	// Sum EField^2 in y-direction
	for(ii=0;ii<Newt;++ii)
	{
		for(jj=0;jj<(ny/2);++jj)
		{
			j2=(jj+1) + ii*(ny/2);
			TempSp[ii+1] = TempSp[ii+1] + TempSp1[j2];
		}
	}

	// Free up memory
	free_vector(TempSp1,1,(ny/2)*Newt);
		
	// Fill the spectrum array 
	for(ii=0;ii<Newt;++ii)
	{
		Spectrum[ii+1] = TempSp[ii+1];

		if(MaxSp < Spectrum[ii+1])
		{
			MaxSp = Spectrum[ii+1];
		}
	}

	free_vector(TempSp,1,Newt);


	//***************************************************************
	// Calculate MSquared

	// Calculate beamx and beamy values
	Msquared(Spectrum,FFTSpect,pSBeam,Lambda,MaxSp,nx,ny,Newt,dx,dy,
			 pProgS);

	// Update the progress bars
	pProgT->SetPos(T+7);
	
	
	//**************************************************************
	// Keep only the field slices with significant power for the MSQUARE 
	// calculations to save time and make nicer plots without the ragged
	// tilts, etc. that arise in the far wings of the pulse

	done = false;
	
	// Find the leftmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<Newt;++ii)
	{
		if(!done)			
		{
			if(Spectrum[ii+1]/MaxSp > 0.001)
			{
				left = ii;
				done = true;
			}
		}
	}
	
	// Reset done
	done = false;
	
	// Find rightmost array value greater than 0.001 norm spectrum
	for(ii=0;ii<Newt;++ii)
	{
		if(!done)	// Signal
		{
			if(Spectrum[Newt - ii]/MaxSp > 0.001)
			{
				right = Newt - ii;
				done = true;
			}
		}
	}


	//*****************************************************************
	// Put in continuation values at low field pts (norm power < .001)

	for(ii=0;ii<left;++ii)
	{
		pSBeam->SetBeamx(ii+1,
						pSBeam->GetTiltx(left+1),
						pSBeam->GetMsqrx(left+1),
						pSBeam->GetZOx(left+1),
						pSBeam->GetWavofCurvx(left+1),
						pSBeam->GetSigsqx(left+1),
						pSBeam->GetSigsqs_x(left+1),
						pSBeam->GetAsubx(left+1),
						pSBeam->GetXbar(left+1));

		pSBeam->SetBeamy(ii+1,
						pSBeam->GetTilty(left+1),
						pSBeam->GetMsqry(left+1),
						pSBeam->GetZOy(left+1),
						pSBeam->GetWavofCurvy(left+1),
						pSBeam->GetSigsqy(left+1),
						pSBeam->GetSigsqs_y(left+1),
						pSBeam->GetAsuby(left+1),
						pSBeam->GetYbar(left+1));
	}

	for(ii=right;ii<Newt;++ii)
	{
		pSBeam->SetBeamx(ii+1,
						pSBeam->GetTiltx(right),
						pSBeam->GetMsqrx(right),
						pSBeam->GetZOx(right),
						pSBeam->GetWavofCurvx(right),
						pSBeam->GetSigsqx(right),
						pSBeam->GetSigsqs_x(right),
						pSBeam->GetAsubx(right),
						pSBeam->GetXbar(right));

		pSBeam->SetBeamy(ii+1,
						pSBeam->GetTilty(right),
						pSBeam->GetMsqry(right),
						pSBeam->GetZOy(right),
						pSBeam->GetWavofCurvy(right),
						pSBeam->GetSigsqy(right),
						pSBeam->GetSigsqs_y(right),
						pSBeam->GetAsuby(right),
						pSBeam->GetYbar(right));
	}

	//*****************************************************************
	// Calculate average tilt, curvature, M^2, and spectrum

	// First for x
	pavg = 0.0;
	sigsqavg = 0.0;
	aavg = 0.0;
	xybaravg = 0.0;
	tiltavg = 0.0;
	sigsqsavg = 0.0;
	sigsqoavg = 0.0;
	
	for(ii=0;ii<Newt;++ii)
	{
		pavg += Spectrum[ii+1];
	}

	pavg = double(1./pavg);		// average spectrum

	for(ii=0;ii<Newt;++ii)
	{
		sigsqavg += pSBeam->GetSigsqx(ii+1)*Spectrum[ii+1];
		aavg += pSBeam->GetAsubx(ii+1)*Spectrum[ii+1];
		xybaravg += pSBeam->GetXbar(ii+1)*Spectrum[ii+1];
		tiltavg += pSBeam->GetTiltx(ii+1)*Spectrum[ii+1];
		sigsqsavg += pSBeam->GetSigsqs_x(ii+1)*Spectrum[ii+1];
	}

	sigsqavg *= pavg;		// average sigsqx
	aavg *=pavg;			// average asubx
	xybaravg *= pavg;		// average xbar
	tiltavg *=pavg;			// average tilt x
	sigsqsavg *= pavg;		// average sigsqs x
	sigsqoavg = sigsqavg - pow((aavg + 2.e-6*tiltavg*xybaravg),2)/
							  (4.e6*sigsqsavg*pow(Lambda,2));
	
	Mavg[1] = (4*Pi)*sqrt(sigsqoavg*sigsqsavg);	// average msquared x

	// Now for y
	sigsqavg = 0.0;
	aavg = 0.0;
	xybaravg = 0.0;
	tiltavg = 0.0;
	sigsqsavg = 0.0;
	sigsqoavg = 0.0;
	
	for(ii=0;ii<Newt;++ii)
	{
		sigsqavg += pSBeam->GetSigsqy(ii+1)*Spectrum[ii+1];
		aavg += pSBeam->GetAsuby(ii+1)*Spectrum[ii+1];
		xybaravg += pSBeam->GetYbar(ii+1)*Spectrum[ii+1];
		tiltavg += pSBeam->GetTilty(ii+1)*Spectrum[ii+1];
		sigsqsavg += pSBeam->GetSigsqs_y(ii+1)*Spectrum[ii+1];
	}

	sigsqavg *= pavg;		// average sigsqx
	aavg *=pavg;			// average asuby
	xybaravg *= pavg;		// average ybar
	tiltavg *=pavg;			// average tilt y
	sigsqsavg *= pavg;		// average sigsqs y
	sigsqoavg = sigsqavg - pow((aavg + 2.e-6*tiltavg*xybaravg),2)/
							  (4.e6*sigsqsavg*pow(Lambda,2));

	CString s;
	s.Format("%s %e","myavgsp should be ",(4*Pi)*sqrt(sigsqoavg*sigsqsavg));
//	AfxMessageBox(s);

	Mavg[2] = (4*Pi)*sqrt(sigsqoavg*sigsqsavg);	// average msquared y

	CString s1;
	s1.Format("%s %e",
		"M^2 perp (from Beam_t) = ",Mavg[1]);
//	AfxMessageBox(s1);

	// Free up memory
	fftw_destroy_plan(p_t);
	free(FFT_t);

	free_vector(FFTSpect,1,2*nx*(ny/2)*Newt);

}



//*************************** Msquared Function ***********************
//
// This function performs the actual beam calculations for
// AnalyzeBeams_t and AnalyzeBeams_w.  See Beams.cpp for a description
// of the calculated parameters.
//
// Called from: AnalyzeBeams_t, AnalyzeBeams_w
//
//*********************************************************************

void Msquared(float* Power,float* Field,CBeams* pBeams,double Lambda,
			  double MaxP,long nx,long ny,long nt,double dx,double dy,
			  CProgressCtrl* pProgS)
{
	const double Pi = 3.14159265359;

	float *TempField;
	float *TField, *TempF;
	
	// Local variables for x and s
	float *x, *ex, *ix;

	// Local variables for y
	float *y, *ey, *iy;

	// Local variables for s
	float *s, *es, *tempes, *is, *de, *ede;

	// Loop variables for x and s
	double normx, xbar, asubx, sigsqx, sigsqox;
	
	// Loop variables for y
	double normy, ybar, asuby, sigsqy, sigsqoy;

	// Loop variables for s
	double norms, sbar, sigsqs;

	// Variables for beamx/beamy
	double msquared,zo,zrayleigh,radofcurv,wavofcurv;

	int ii,i2,jj,j2,kk,k2,ll,l2,nn,n2,mm;

	int Half;
	
	double esval;

	
//*********************************************************************
// Do x calculations first

	// Change dx into mm
	dx = 1000.*dx;

	fftw_plan p_x;
	
	p_x = fftw_create_plan(nx, FFTW_FORWARD,
							FFTW_ESTIMATE | FFTW_IN_PLACE);

	fftw_complex* FFT_x;

	FFT_x = (fftw_complex *) malloc(nx*sizeof(fftw_complex));

	// Setup x and s grids
	x = vector(1,nx);
	s = vector(1,nx);

	for(ii=0;ii<nx;++ii)
	{
		x[ii+1] = ((-0.5*(nx + 1)) + ii+1)*dx;
		s[ii+1] = ((-0.5*(2 + nx)) + ii+1)/(nx*dx);
	}
	
	// Fourier Transform of EField in x-direction
	TempField = vector(1,2*nx*(ny/2)*nt);

	for(ii=0;ii<nt;++ii)
	{

		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<(ny/2);++jj)
		{

			for(kk=0;kk<nx;++kk)
			{
				k2 = ii + jj*nt + kk*nt*(ny/2);
				i2 = 2*(k2+1);

				// Fill FFT variable with TField
				FFT_x[kk].re = Field[i2-1];
				FFT_x[kk].im = Field[i2];
			}

			// FFT
			fftw_one(p_x, &FFT_x[0], NULL);

			// Set TempField to the new values
			for(nn=0;nn<nx;++nn)
			{
				n2 = ii + jj*nt + nn*nt*(ny/2);
				i2 = 2*(n2+1);

				TempField[i2-1] = FFT_x[nn].re;
				TempField[i2] = FFT_x[nn].im;
			}

		}
	}


	//*********************************
	// Main loop for beamx calculations

	for(ii=0;ii<nt;++ii)	// t-loop
	{
		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Reset variables to zero
		normx = 0.0;
		norms = 0.0;
		xbar = 0.0;
		sbar = 0.0;
		asubx = 0.0;
		sigsqx = 0.0;
		sigsqs = 0.0;

		for(jj=0;jj<(ny/2);++jj)	// y-loop 1
		{
	
			ex = vector(1,2*nx);
			es = vector(1,2*nx);
			tempes = vector(1,2*nx);

			// Setup tempes and ex
			for(kk=0;kk<nx;++kk)
			{
				j2 = 2*(kk+1);

				k2 = ii + jj*nt + kk*nt*(ny/2);
				i2 = 2*(k2+1);

				// Set ex to Field values
				ex[j2-1] = Field[i2-1];
				ex[j2] = Field[i2];

				// Fill temporary es with TempField values
				tempes[j2-1] = TempField[i2-1];
				tempes[j2] = TempField[i2];
			}

			// Setup the es field
			Half = long(nx/2);
			esval = double(2./nx); 
			for(nn=0;nn<Half;++nn)
			{
				n2 = 2*(nn+1);

				es[n2-1] = esval*tempes[2*Half+n2-1];
				es[n2] = esval*tempes[2*Half+n2];

				es[2*Half+n2-1] = esval*tempes[n2-1];
				es[2*Half+n2] = esval*tempes[n2];
			}

			// Initialize ix and is to zero
			ix = vector(1,nx);
			is = vector(1,nx);

			for(nn=0;nn<nx;++nn)
			{
				ix[nn+1] = 0.0;
				is[nn+1] = 0.0;
			}

			// Fill ix and is with mod(ex^2) and mod(es^2)
			for(nn=0;nn<nx;++nn)
			{
				n2=2*(nn+1);

				ix[nn+1] = (pow(ex[n2-1],2) + pow(ex[n2],2));
				is[nn+1] = (pow(es[n2-1],2) + pow(es[n2],2));
					
			}

			// Free up memory
			free_vector(es,1,2*nx);
			free_vector(tempes,1,2*nx);

			// Initialize de and ede
			de = vector(1,2*nx);
			ede = vector(1,nx);

			// Set variables
			for(nn=0;nn<nx;++nn)
			{
				n2=2*(nn+1);
	
				// Calculate normx/s and x/sbar
				normx += ix[nn+1];
				norms += is[nn+1];

				xbar = xbar + (x[nn+1]*ix[nn+1]);
				sbar = sbar + (s[nn+1]*is[nn+1]);
					 
			}

			for(nn=0;nn<nx;++nn)
			{
				n2=2*(nn+1);
				// Fill de
				if(nn<(nx-1))
				{
					de[n2-1] = ex[n2+1] - ex[n2-1];
					de[n2] = ex[n2+2] - ex[n2];
				}
				else
				{
					de[n2-1] = ex[1] - ex[n2-1];
					de[n2] = ex[2] - ex[n2];
				}
				
				// Fill ede
				ede[nn+1] = (ex[n2-1]*de[n2]) - (ex[n2]*de[n2-1]);
	
				// Set asubx
				if(nn>0 && nn<(nx-1))
				{
					asubx += (x[nn+1] + 0.5*dx)*(1./dx)*ede[nn+1];
				}

			}

			// Free up memory
			free_vector(ex,1,2*nx);
			free_vector(ix,1,nx);
			free_vector(is,1,nx);
			free_vector(de,1,2*nx);
			free_vector(ede,1,nx);

		}	// End y-loop 1

		// Reset normalized xbar and sbar
		xbar = xbar/normx;
		sbar = sbar/norms;

		for(mm=0;mm<(ny/2);++mm)	// y-loop 2
		{
			ex = vector(1,2*nx);
			es = vector(1,2*nx);
			tempes = vector(1,2*nx);

			// Setup tempes and ex
			for(kk=0;kk<nx;++kk)
			{
				j2 = 2*(kk+1);

				k2 = ii + mm*nt + kk*nt*(ny/2);
				i2 = 2*(k2+1);
	
				// Set ex to Field values
				ex[j2-1] = Field[i2-1];
				ex[j2] = Field[i2];

				tempes[j2-1] = TempField[i2-1];
				tempes[j2] = TempField[i2];
	
			}

			// Setup the es field
			Half = long(nx/2);
			esval = double(2./nx);
			for(nn=0;nn<Half;++nn)
			{
				n2 = 2*(nn+1);

				i2 = (nx-1) - nn;
				j2 = 2*(i2+1);

				es[n2-1] = esval*tempes[2*Half + n2-1];
				es[n2] = esval*tempes[2*Half + n2];
	
				es[2*Half+n2-1] = esval*tempes[n2-1];
				es[2*Half+n2] = esval*tempes[n2];
			}
		
			// Initialize ix and is to zero
			ix = vector(1,nx);
			is = vector(1,nx);

			for(nn=0;nn<nx;++nn)
			{
				ix[nn+1] = 0.0;
				is[nn+1] = 0.0;
			}

			// Fill ix and is with mod(ex^2) and mod(es^2)
			for(nn=0;nn<nx;++nn)
			{
				n2=2*(nn+1);

				ix[nn+1] = (pow(ex[n2-1],2) + pow(ex[n2],2));
				is[nn+1] = (pow(es[n2-1],2) + pow(es[n2],2));
			}

			// Free up memory
			free_vector(es,1,2*nx);
			free_vector(ex,1,2*nx);
			free_vector(tempes,1,2*nx);

			// Set sigsqx and sigsqs
			for(nn=0;nn<nx;++nn)
			{
				sigsqx += pow((x[nn+1] - xbar),2)*ix[nn+1];
				sigsqs += pow((s[nn+1] - sbar),2)*is[nn+1];
			}

			// Free up memory
			free_vector(ix,1,nx);
			free_vector(is,1,nx);

		}	// End y-loop 2
		
		// Calculate beamx values
		asubx = -(asubx*Lambda*1.e3/(normx*Pi));
		sigsqx = sigsqx/normx;
		sigsqs = sigsqs/norms;
		sigsqox = sigsqx - pow((asubx+(2*Lambda*1.e3*sbar*xbar)),2)/
						   (4.e6*sigsqs*pow(Lambda,2));

		msquared = (4*Pi)*sqrt(sigsqox*sigsqs);

		// Distance to waist in m
		zo = (asubx+2*Lambda*1.e3*sbar*xbar)/(2.e6*sigsqs*pow(Lambda,2));
	
		// Rayleigh range in m
		zrayleigh = sqrt(sigsqox/(1.e6*sigsqs*pow(Lambda,2)));

		// Radius of curvature in m
		radofcurv = zo+(pow(zrayleigh,2)/zo);
	
		// Waves of curvature
		wavofcurv = sigsqx/(Lambda*1.e3*radofcurv);

		// Set the values for the beamx array
		pBeams->SetBeamx(ii+1,(sbar*Lambda*1.e9),msquared,zo,wavofcurv,
						 sigsqx,sigsqs,asubx,xbar);

		// Update the progress bar
		pProgS->SetPos(int((100*ii)/nt));
		
	}	// End t-loop 
	
	// Free up memory
	fftw_destroy_plan(p_x);
	free(FFT_x);

	free_vector(x,1,nx);
	free_vector(s,1,nx);

	free_vector(TempField,1,2*nx*(ny/2)*nt);
			
			
//*********************************************************************
// Now do beamy calculations 
	
	// Change dy to mm
	dy = 1000.*dy;

	fftw_plan p_y;

	p_y = fftw_create_plan(ny, FFTW_FORWARD,
					FFTW_ESTIMATE | FFTW_IN_PLACE);

	fftw_complex* FFT_y;

	FFT_y = (fftw_complex *) malloc(ny*sizeof(fftw_complex));
	
	// Setup x and s grids
	y = vector(1,ny);
	s = vector(1,ny);

	for(ii=0;ii<ny;++ii)
	{
		y[ii+1] = ((-0.5*(ny + 1)) + ii+1)*dy;
		s[ii+1] = ((-0.5*(2 + ny)) + ii+1)/(ny*dy);
	}

	// Setup Field for full y direction
	TField = vector(1,2*nx*ny*nt);
	TempF = vector(1,2*nx*(ny/2)*nt);

	// Mirror the Field array for TempF
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<(ny/2);++kk)
			{
				j2 = 2*(kk+1);
				k2 = ii + jj*nt*(ny/2) + kk*nt;
				i2 = 2*(k2+1);

				ll = ii + jj*nt*(ny/2) + ((ny/2)-(kk+1))*nt;
				l2 = 2*(ll+1);

				TempF[i2-1] = Field[l2-1];
				TempF[i2] = Field[l2];
			}
		}
	}
			
	// Fill TField with the EField values
	for(ii=0;ii<nt;++ii)
	{
		for(jj=0;jj<nx;++jj)
		{
			for(kk=0;kk<ny;++kk)
			{
				j2 = 2*(kk+1);
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);
		
				ll = ii + jj*nt*(ny/2) + (kk-(ny/2))*nt;
				l2 = 2*(ll+1);

				nn = ii + jj*nt*(ny/2) + kk*nt;
				n2 = 2*(nn+1);

				if(kk<(ny/2))
				{
					TField[i2-1] = Field[n2-1];
					TField[i2] = Field[n2];
				}
				else
				{
					TField[i2-1] = TempF[l2-1];
					TField[i2] = TempF[l2];
				}
				
			}
		}
	}
						
	// Free up memory
	free_vector(TempF,1,2*nx*(ny/2)*nt);

	// Fourier Transform of EField in y-direction
	TempField = vector(1,2*nx*ny*nt);

	for(ii=0;ii<nt;++ii)
	{

		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		for(jj=0;jj<nx;++jj)
		{

			for(kk=0;kk<ny;++kk)
			{
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				// Fill FFT variable with EField
				FFT_y[kk].re = TField[i2-1];
				FFT_y[kk].im = TField[i2];
			}

			// FFT
			fftw_one(p_y, &FFT_y[0], NULL);

			// Set FFT variable to the new values
			for(nn=0;nn<ny;++nn)
			{
				n2 = ii + jj*nt*ny + nn*nt;
				i2 = 2*(n2+1);

				TempField[i2-1] = FFT_y[nn].re;
				TempField[i2] = FFT_y[nn].im;
			}

		}
	}


	//*********************************
	// Main loop for beamy calculations 
	
	for(ii=0;ii<nt;++ii)	// t-loop
	{
		// Check for Windows messages, and return from the loop if 
		// the stop button has been pressed
		CheckWinMessages(10000);
		if( GetStop() )
		{
			return;
		}

		// Reset variables to zero
		normy = 0.0;
		norms = 0.0;
		ybar = 0.0;
		sbar = 0.0;
		asuby = 0.0;
		sigsqy = 0.0;
		sigsqs = 0.0;

		for(jj=0;jj<nx;++jj)	// x-loop 1
		{

			ey = vector(1,2*ny);
			es = vector(1,2*ny);
			tempes = vector(1,2*ny);

			// Setup ey and tempes
			for(kk=0;kk<ny;++kk)
			{
				j2 = 2*(kk+1);
				k2 = ii + jj*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				// Set ey to TField values
				ey[j2-1] = TField[i2-1];
				ey[j2] = TField[i2];

				// Set tempes to TempField values
				tempes[j2-1] = TempField[i2-1];
				tempes[j2] = TempField[i2];
			}

			// Setup the es field
			Half = long(ny/2);
			esval = double(2./ny);
			for(nn=0;nn<Half;++nn)
			{
				n2 = 2*(nn+1);

				es[n2-1] = esval*tempes[2*Half+n2-1];
				es[n2] = esval*tempes[2*Half+n2];

				es[2*Half+n2-1] = esval*tempes[n2-1];
				es[2*Half+n2] = esval*tempes[n2];
			}

			// Initialize iy and is to zero
			iy = vector(1,ny);
			is = vector(1,ny);

			for(nn=0;nn<ny;++nn)
			{
				iy[nn+1] = 0.0;
				is[nn+1] = 0.0;
			}

			// Fill iy and is with mod(ey^2) and mod(es^2)
			for(nn=0;nn<ny;++nn)
			{
				n2=2*(nn+1);

				iy[nn+1] = (pow(ey[n2-1],2) + pow(ey[n2],2));
				is[nn+1] = (pow(es[n2-1],2) + pow(es[n2],2));
			}

			// Free up memory
			free_vector(es,1,2*ny);
			free_vector(tempes,1,2*ny);

			// Initialize de and ede
			de = vector(1,2*ny);
			ede = vector(1,ny);

			// Set variables
			for(nn=0;nn<ny;++nn)
			{
				n2=2*(nn+1);

				// Calculate normy/s and y/sbar
				normy += iy[nn+1];
				norms += is[nn+1];
	
				ybar = ybar + (y[nn+1]*iy[nn+1]);
				sbar = sbar + (s[nn+1]*is[nn+1]);

			}

			for(nn=0;nn<ny;++nn)
			{
				n2=2*(nn+1);
				// Fill de
				if(nn<(ny-1))
				{
					de[n2-1] = ey[n2+1] - ey[n2-1];
					de[n2] = ey[n2+2] - ey[n2];
				}
				else
				{
					de[n2-1] = ey[1] - ey[n2-1];
					de[n2] = ey[2] - ey[n2];
				}

				// Fill ede
				ede[nn+1] = (ey[n2-1]*de[n2]) - (ey[n2]*de[n2-1]);

				// Set asuby
				if(nn>0 && nn<(ny-1))
				{
					asuby += (y[nn+1] + 0.5*dy)*(1./dy)*ede[nn+1];
				}

			}

			// Free up memory
			free_vector(ey,1,2*ny);
			free_vector(iy,1,ny);
			free_vector(is,1,ny);
			free_vector(de,1,2*ny);
			free_vector(ede,1,ny);
			
		}	// End x-loop 1

		// Reset normalized ybar and sbar
		ybar = ybar/normy;
		sbar = sbar/norms;

		for(mm=0;mm<nx;++mm)	// x-loop 2
		{
			ey = vector(1,2*ny);
			es = vector(1,2*ny);
			tempes = vector(1,2*ny);
	
			// Setup tempes and ey
			for(kk=0;kk<ny;++kk)
			{
				j2 = 2*(kk+1);
				k2 = ii + mm*nt*ny + kk*nt;
				i2 = 2*(k2+1);

				// Set ey to TField values
				ey[j2-1] = TField[i2-1];
				ey[j2] = TField[i2];

				// Set tempes to TempField values
				tempes[j2-1] = TempField[i2-1];
				tempes[j2] = TempField[i2];
			}

			// Setup the es field
			Half = long(ny/2);
			esval = double(2./ny);

			for(nn=0;nn<Half;++nn)
			{
				n2 = 2*(nn+1);

				es[n2-1] = esval*tempes[2*Half+n2-1];
				es[n2] = esval*tempes[2*Half+n2];

				es[2*Half+n2-1] = esval*tempes[n2-1];
				es[2*Half+n2] = esval*tempes[n2];
			}

			// Initialize iy and is to zero
			iy = vector(1,ny);
			is = vector(1,ny);

			for(nn=0;nn<ny;++nn)
			{
				iy[nn+1] = 0.0;
				is[nn+1] = 0.0;
			}

			// Fill iy and is with mod(ey^2) and mod(es^2)
			for(nn=0;nn<ny;++nn)
			{
				n2=2*(nn+1);

				iy[nn+1] = (pow(ey[n2-1],2) + pow(ey[n2],2));
				is[nn+1] = (pow(es[n2-1],2) + pow(es[n2],2));
			}

			// Free up memory
			free_vector(es,1,2*ny);
			free_vector(ey,1,2*ny);
			free_vector(tempes,1,2*ny);

			// Set sigsqy and sigsqs
			for(nn=0;nn<ny;++nn)
			{
				sigsqy += pow((y[nn+1] - ybar),2)*iy[nn+1];
				sigsqs += pow((s[nn+1] - sbar),2)*is[nn+1];
			}

			// Free up memory
			free_vector(iy,1,ny);
			free_vector(is,1,ny);

		}	// End x-loop 2
			
		// Calculate values
		asuby = -(asuby*Lambda*1.e3/(normy*Pi));
		sigsqy = sigsqy/normy;
		sigsqs = sigsqs/norms;
		sigsqoy = sigsqy - pow((asuby+(2*Lambda*1.e3*sbar*ybar)),2)/
						   (4.e6*sigsqs*pow(Lambda,2));

		msquared = (4*Pi)*sqrt(sigsqoy*sigsqs);

		// Distance to waist in m
		zo = (asuby+2*Lambda*1.e3*sbar*ybar)/(2.e6*sigsqs*pow(Lambda,2));

		// Rayleigh range in m
		zrayleigh = sqrt(sigsqoy/(sigsqs*1.e6*pow(Lambda,2)));

		// Radius of curvature in m
		radofcurv = zo+(pow(zrayleigh,2)/zo);

		// Waves of curvature
		wavofcurv = sigsqy/(Lambda*1.e3*radofcurv);

		// Set the values for the beamy array
		pBeams->SetBeamy(ii+1,(sbar*Lambda*1.e9),msquared,zo,wavofcurv,
						 sigsqy,sigsqs,asuby,ybar);

		// Update the progress bars
		pProgS->SetPos(int((100*ii)/nt));

	}	// End t-loop
	
	// Free up memory
	fftw_destroy_plan(p_y);
	free(FFT_y);

	free_vector(TField,1,2*nx*ny*nt);
	free_vector(TempField,1,2*nx*ny*nt);

	free_vector(y,1,ny);
	free_vector(s,1,ny);

}	// End Msquared


//*********************** NUMERICAL RECIPIES **************************

void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

unsigned long *lvector(long nl, long nh)
/* allocate an unsigned long vector with subscript range v[nl..nh] */
{
	unsigned long *v;

	v=(unsigned long *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(long)));
	if (!v) nrerror("allocation failure in lvector()");
	return v-nl+NR_END;
}

void free_lvector(unsigned long *v, long nl, long nh)
/* free an unsigned long vector allocated with lvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

double *dvector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_dvector(double *v, long nl, long nh)
/* free a double vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}
