//////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Setup model grid.
*/
//////////////////////////////////////////////////////////////////////////
#include "constants.h"
#include "grid.h"
#include "namelist.h"
#include "utilities.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


Grid::Grid(Namelist nl, Timer &timer)
{
    //////////////////////////////////////////////////////////////////////
    //// READ NAMELIST INPUT
    //////////////////////////////////////////////////////////////////////
    nparticles       = nl.nparticles;
    dom_x0           = nl.dom_x0;
    dom_x1           = nl.dom_x1;
    dom_z0           = nl.dom_z0;
    dom_z1           = nl.dom_z1;
    dt               = nl.dt;
    nt               = nl.tot_time / nl.dt + 1;
    time_discr       = nl.time_discr;
    nth_tstep_out    = (int) nl.nth_sec_out / nl.dt;

    tstep            = 0;
    if (nl.dom_x1 - nl.dom_x0 != nl.dom_z1 - nl.dom_z0)
    {
        cout << "only implemented for dx = dz" << endl;
        exit(1);
    }
    dom_vol = (nl.dom_x1 - nl.dom_x0) * (nl.dom_z1 - nl.dom_z0);
    ncells = nparticles * nl.ncells_part;
    cell_dx = sqrt(dom_vol / ncells);
    cell_vol = dom_vol / ncells;
    ncells_x = (int) (nl.dom_x1 - nl.dom_x0) / cell_dx;
    ncells_z = (int) (nl.dom_z1 - nl.dom_z0) / cell_dx;

    //////////////////////////////////////////////////////////////////////
    //// SET UP GRID 
    //////////////////////////////////////////////////////////////////////
    // recompute grid (do not load it from binary file)
    // save grid information
    timer.start("Grid.save");
    save_grid_to_binary();
    timer.stop("Grid.save");

}



void Grid::save_grid_to_binary()
{
    string grid_name = to_string(nparticles);
    //_save_cart_coords_to_binary("grid", grid_name,
    //                            "hexagons_cc", hexagons);

    /*
    string tvi_0_path       = "grid/" + grid_name + "_tvi_0.bin";
    ofstream tvi_0_file;
    tvi_0_file.open(tvi_0_path, ios::binary|ios::out);
    for ( int tci = 0; tci < ntriangles; tci++ )
    {
        Cell& triangle = triangles[tci];
        //cout << triangle.vert_inds[0] << endl;
        dtype ind0 = (dtype)(triangle.vert_inds[0]);
        dtype ind1 = (dtype)(triangle.vert_inds[1]);
        dtype ind2 = (dtype)(triangle.vert_inds[2]);
        tvi_0_file.write(reinterpret_cast<char *>(&ind0),
                        sizeof(ind0));
    }
    tvi_0_file.close();
    */
}



void Grid::_save_cart_coords_to_binary(string file_dir,
                                    string grid_name,
                                    string file_name_base)
{
    /*
    string file_path_x = file_dir + "/" + grid_name + "_" +
                            file_name_base + "_x.bin";
    string file_path_y = file_dir + "/" + grid_name + "_" +
                            file_name_base + "_y.bin";
    string file_path_z = file_dir + "/" + grid_name + "_" +
                            file_name_base + "_z.bin";
    ofstream file_x;
    ofstream file_y;
    ofstream file_z;
    file_x.open(file_path_x, ios::binary|ios::out);
    file_y.open(file_path_y, ios::binary|ios::out);
    file_z.open(file_path_z, ios::binary|ios::out);
    for ( int i = 0; i < cells.size(); i++ )
    {
        //cout << cells[i].center_cc.x << endl;
        file_x.write(reinterpret_cast<char *>(&(cells[i].center_cc.x)),
                        sizeof(cells[i].center_cc.x));
        file_y.write(reinterpret_cast<char *>(&(cells[i].center_cc.y)),
                        sizeof(cells[i].center_cc.y));
        file_z.write(reinterpret_cast<char *>(&(cells[i].center_cc.z)),
                        sizeof(cells[i].center_cc.z));
    }
    file_x.close();
    file_y.close();
    file_z.close();
    */
}

