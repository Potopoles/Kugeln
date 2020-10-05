//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Setup model grid.
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "debug.h"
#include "structures.h"
#include "namelist.h"
#include <vector>
#include <string>
using namespace std;


class Grid
/*
Represent model grid.
*/
{
public:
    Grid(Namelist nl, Timer &timer);
    // NAMELIST VARIABLES
    int nparticles;

    // domain lower left and upper right corner
    dtype dom_x0;
    dtype dom_x1;
    dtype dom_z0;
    dtype dom_z1;

    // time increments of time steps
    dtype dt;
    // total number of time steps
    int nt;
    // current time step
    int tstep = 0;
    // write output every nth tstep
    int nth_tstep_out = 0;
    // simulation time in seconds
    dtype sim_time = 0.;
    string time_discr;
    string spatial_discr;

    void save_grid_to_binary();

private:
    void _save_cart_coords_to_binary(string file_dir,
                                    string grid_name,
                                    string file_name_base);
};
