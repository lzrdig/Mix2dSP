
//***************************************************************************
//
// DecIncl.h - contains all the standard C++ include statements
// necessary for the main program
//
//***************************************************************************

// Pre-compiler instructions
#ifndef DECINCL_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <complex>


//typedef std::complex<double> complex;

// For use by the member functions of CFields
const int REAL = 9990;	
const int IMAG = 9991;

// Macro functions (only valid for values > 0)
#define MAX2(x,y) ( (x) > (y) ? (x) : (y) )
#define MIN2(x,y) ( (x) < (y) ? (x) : (y) )
#define MIN(x,y,z) ( ( ((x) < (y)) && ((x) < (z)) ) ? (x) : ((y) < (z) ? (y) : (z)) )
#define MAX(x,y,z) ( ( ((x) > (y)) && ((x) > (z)) ) ? (x) : ((y) > (z) ? (y) : (z)) )



#define DECINCL_H
#endif