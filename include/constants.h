//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Set model constants.
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
using namespace std;

// Run in single or double precision
#define USE_DOUBLE_PRECISION 1
#ifdef USE_DOUBLE_PRECISION
typedef double dtype;
#else
typedef float dtype;
#endif

// gravity constant [m/s] (taken from Williamson 1992 test suite)
const dtype CON_G = 9.80616;

// gas constant for dry air [J/kg/K]
const dtype CON_RD = 287.058;

// pi
const dtype CON_PI = 3.141592;

// number of parallel OMP threads
const int OMP_NUM_THREADS = 8;
