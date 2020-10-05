//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Model namelist
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "constants.h"
#include <string>
using namespace std;

struct Namelist
{
    //////// GENERAL SETTINGS
    string sim_tag;

    //////// GRID
    // number of particles
    int nparticles;
    // domain lower left and upper right corner
    dtype dom_x0;
    dtype dom_x1;
    dtype dom_z0;
    dtype dom_z1;

    //////// INTEGRATION
    // total integration time in seconds
    dtype tot_time;
    // time step in seconds
    dtype dt;

    //////// DISCRETISATION
    // choice of time discretisation (static members of Integrator class)
    // options: EULER_FORWARD, RK2, RK3, RK4, LEAP_FROG, MATSUNO
    string time_discr;
    //// choice of spatial discretisation (static members of Flux class)
    //// options: DISCR_CENTRED, DISCR_UPWIND (does not work with pressure gradient)
    //string spatial_discr;

    //////// INITIAL CONDITION

    //////// OUTPUT
    // directory to store binary output files
    string bin_dir;
    // write output every nth minute
    dtype nth_sec_out;
};
