
//***************************************************************************
//
// Class CInputs
//
// List of member functions:  
//
//	* GetVar(int array) 
//
//        This function will return the value of the variable specified 
//        by the 'array' argument.  The 'array' arguments are in all 
//        capitals, and correspond to the variables with the same name.
//     
//
//	   Example of use:  pInputs->GetVar(DELTAK) will return the value
//                      of the 'deltak' variable.
//
//
//  * SetVar(int array, double num) 
//
//        Similar to GetVar above, this function will set the variable
//        specified by the 'array' argument equal to the specified 
//        'num' value of type double.
//                            
//
//     Example of use:  pInputs->SetVar(DELTAK,10.3) will set deltak = 10.3       	
//
//
// (Note: for a complete list of the 'array' arguments, see 'Inputs.h')
//
//***************************************************************************

#include "StdAfx.h"
#include "DecIncl.h"
#include "Inputs.h"

//***************************************************************************
// Function CInputs::GetVar

double CInputs::GetVar(int array)
{

	switch (array)
	{
	case DEFF:
		return m_deff;
		break;
	case DELTAK:
		return m_deltak;
		break;
	case ENERGY_P:
		return m_Energy_p;
		break;
	case ENERGY_S:
		return m_Energy_s;
		break;
	case ENERGY_I:
		return m_Energy_i;
		break;
	case BEAMDIAM_S:
		return m_Beamdiam_s;
		break;
	case BEAMDIAM_I:
		return m_Beamdiam_i;
		break;
	case BEAMDIAM_P:
		return m_Beamdiam_p;
		break;
	case DURATION_S:
		return m_Duration_s;
		break;
	case DURATION_I:
		return m_Duration_i;
		break;
	case DURATION_P:
		return m_Duration_p;
		break;
	case LAMBDA_S:
		return m_Lambda_s;
		break;
	case LAMBDA_I:
		return m_Lambda_i;
		break;
	case LAMBDA_P:
		return m_Lambda_p;
		break;
	case INDEX_S:
		return m_Index_s;
		break;
	case INDEX_I:
		return m_Index_i;
		break;
	case INDEX_P:
		return m_Index_p;
		break;
	case GVI_S:
		return m_gvi_s;
		break;
	case GVI_I:
		return m_gvi_i;
		break;
	case GVI_P:
		return m_gvi_p;
		break;
	case GVD_S:
		return m_gvd_s;
		break;
	case GVD_I:
		return m_gvd_i;
		break;
	case GVD_P:
		return m_gvd_p;
		break;
	case ALPHA_S:
		return m_alpha_s;
		break;
	case ALPHA_I:
		return m_alpha_i;
		break;
	case ALPHA_P:
		return m_alpha_p;
		break;
	case RC_IN_S:
		return m_Rc_In_s;
		break;
	case RC_IN_I:
		return m_Rc_In_i;
		break;
	case RC_IN_P:
		return m_Rc_In_p;
		break;
	case RC_OUT_S:
		return m_Rc_Out_s;
		break;
	case RC_OUT_I:
		return m_Rc_Out_i;
		break;
	case RC_OUT_P:
		return m_Rc_Out_p;
		break;
	case CRYSTLENGTH:
		return m_Crystlength;
		break;
	case N2_S1:
		return m_N2_s1;
		break;
	case N2_S2:
		return m_N2_s2;
		break;
	case N2_S3:
		return m_N2_s3;
		break;
	case N2_I1:
		return m_N2_i1;
		break;
	case N2_I2:
		return m_N2_i2;
		break;
	case N2_I3:
		return m_N2_i3;
		break;
	case N2_P1:
		return m_N2_p1;
		break;
	case N2_P2:
		return m_N2_p2;
		break;
	case N2_P3:
		return m_N2_p3;
		break;
	case BETA_S1:
		return m_Beta_s1;
		break;
	case BETA_S2:
		return m_Beta_s2;
		break;
	case BETA_S3:
		return m_Beta_s3;
		break;
	case BETA_I1:
		return m_Beta_i1;
		break;
	case BETA_I2:
		return m_Beta_i2;
		break;
	case BETA_I3:
		return m_Beta_i3;
		break;
	case BETA_P1:
		return m_Beta_p1;
		break;
	case BETA_P2:
		return m_Beta_p2;
		break;
	case BETA_P3:
		return m_Beta_p3;
		break;
	case RHO_S:
		return m_Rho_s;
		break;
	case RHO_I:
		return m_Rho_i;
		break;
	case RHO_P:
		return m_Rho_p;
		break;
	case RADCURV_S:
		return m_RadCurv_s;
		break;
	case RADCURV_I:
		return m_RadCurv_i;
		break;
	case RADCURV_P:
		return m_RadCurv_p;
		break;
	case OFFSET_S:
		return m_Offset_s;
		break;
	case OFFSET_I:
		return m_Offset_i;
		break;
	case NX:
		return m_nx;
		break;
	case NY:
		return m_ny;
		break;
	case NT:
		return m_nt;
		break;
	case NZ:
		return m_nz;
		break;
	case MAXGRID_X:
		return m_Maxgrid_x;
		break;
	case MAXGRID_Y:
		return m_Maxgrid_y;
		break;
	case GAUSSINDEX_S:
		return m_GaussIndex_s;
		break;
	case GAUSSINDEX_I:
		return m_GaussIndex_i;
		break;
	case GAUSSINDEX_P:
		return m_GaussIndex_p;
		break;
	case PHI_S:
		return m_Phi_s;
		break;
	case PHI_I:
		return m_Phi_i;
		break;
	case PHI_P:
		return m_Phi_p;
		break;
	case DELAY_S:
		return m_Delay_s;
		break;
	case DELAY_I:
		return m_Delay_i;
		break;
	case DIST:
		return m_Dist;
		break;
	default:
		return 0.0;
		break;
	}	
		
}


//***************************************************************************
// Function CInputs::SetVar  

void CInputs::SetVar(int array, double num)
{

	switch (array)
	{
	case DEFF:
		m_deff = num;
		break;
	case DELTAK:
		m_deltak = num;
		break;
	case ENERGY_P:
		m_Energy_p = num;
		break;
	case ENERGY_S:
		m_Energy_s = num;
		break;
	case ENERGY_I:
		m_Energy_i = num;
		break;
	case BEAMDIAM_S:
		m_Beamdiam_s = num;
		break;
	case BEAMDIAM_I:
		m_Beamdiam_i = num;
		break;
	case BEAMDIAM_P:
		m_Beamdiam_p = num;
		break;
	case DURATION_S:
		m_Duration_s = num;
		break;
	case DURATION_I:
		m_Duration_i = num;
		break;
	case DURATION_P:
		m_Duration_p = num;
		break;
	case LAMBDA_S:
		m_Lambda_s = num;
		break;
	case LAMBDA_I:
		m_Lambda_i = num;
		break;
	case LAMBDA_P:
		m_Lambda_p = num;
		break;
	case INDEX_S:
		m_Index_s = num;
		break;
	case INDEX_I:
		m_Index_i = num;
		break;
	case INDEX_P:
		m_Index_p = num;
		break;
	case GVI_S:
		m_gvi_s = num;
		break;
	case GVI_I:
		m_gvi_i = num;
		break;
	case GVI_P:
		m_gvi_p = num;
		break;
	case GVD_S:
		m_gvd_s = num;
		break;
	case GVD_I:
		m_gvd_i = num;
		break;
	case GVD_P:
		m_gvd_p = num;
		break;
	case ALPHA_S:
		m_alpha_s = num;
		break;
	case ALPHA_I:
		m_alpha_i = num;
		break;
	case ALPHA_P:
		m_alpha_p = num;
		break;
	case RC_IN_S:
		m_Rc_In_s = num;
		break;
	case RC_IN_I:
		m_Rc_In_i = num;
		break;
	case RC_IN_P:
		m_Rc_In_p = num;
		break;
	case RC_OUT_S:
		m_Rc_Out_s = num;
		break;
	case RC_OUT_I:
		m_Rc_Out_i = num;
		break;
	case RC_OUT_P:
		m_Rc_Out_p = num;
		break;
	case CRYSTLENGTH:
		m_Crystlength = num;
		break;
	case N2_S1:
		m_N2_s1 = num;
		break;
	case N2_S2:
		m_N2_s2 = num;
		break;
	case N2_S3:
		m_N2_s3 = num;
		break;
	case N2_I1:
		m_N2_i1 = num;
		break;
	case N2_I2:
		m_N2_i2 = num;
		break;
	case N2_I3:
		m_N2_i3 = num;
		break;
	case N2_P1:
		m_N2_p1 = num;
		break;
	case N2_P2:
		m_N2_p2 = num;
		break;
	case N2_P3:
		m_N2_p3 = num;
		break;
	case BETA_S1:
		m_Beta_s1 = num;
		break;
	case BETA_S2:
		m_Beta_s2 = num;
		break;
	case BETA_S3:
		m_Beta_s3 = num;
		break;
	case BETA_I1:
		m_Beta_i1 = num;
		break;
	case BETA_I2:
		m_Beta_i2 = num;
		break;
	case BETA_I3:
		m_Beta_i3 = num;
		break;
	case BETA_P1:
		m_Beta_p1 = num;
		break;
	case BETA_P2:
		m_Beta_p2 = num;
		break;
	case BETA_P3:
		m_Beta_p3 = num;
		break;
	case RHO_S:
		m_Rho_s = num;
		break;
	case RHO_I:
		m_Rho_i = num;
		break;
	case RHO_P:
		m_Rho_p = num;
		break;
	case RADCURV_S:
		m_RadCurv_s = num;
		break;
	case RADCURV_I:
		m_RadCurv_i = num;
		break;
	case RADCURV_P:
		m_RadCurv_p = num;
		break;
	case OFFSET_S:
		m_Offset_s = num;
		break;
	case OFFSET_I:
		m_Offset_i = num;
		break;
	case NX:
		m_nx = num;
		break;
	case NY:
		m_ny = num;
		break;
	case NT:
		m_nt = num;
		break;
	case NZ:
		m_nz = num;
		break;
	case MAXGRID_X:
		m_Maxgrid_x = num;
		break;
	case MAXGRID_Y:
		m_Maxgrid_y = num;
		break;
	case GAUSSINDEX_S:
		m_GaussIndex_s = num;
		break;
	case GAUSSINDEX_I:
		m_GaussIndex_i = num;
		break;
	case GAUSSINDEX_P:
		m_GaussIndex_p = num;
		break;
	case PHI_S:
		m_Phi_s = num;
		break;
	case PHI_I:
		m_Phi_i = num;
		break;
	case PHI_P:
		m_Phi_p = num;
		break;
	case DELAY_S:
		m_Delay_s = num;
		break;
	case DELAY_I:
		m_Delay_i = num;
		break;
	case DIST:
		m_Dist = num;
		break;
	default:
		break;
	}

}
