//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Define right hand side of position change equation 
     to compute tendency of XPOS and YPOS.
*/
//////////////////////////////////////////////////////////////////////////////
#include "dynamic_tendencies.h"
#include "grid_funcs.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include "math.h"
using namespace std;


Dynamic_Tendencies::Dynamic_Tendencies(Grid &gr)
{
    TXPOS.reserve(gr.nparticles);
    TXPOS.reserve(gr.nparticles);
    TZSPEED.reserve(gr.nparticles);
    TZSPEED.reserve(gr.nparticles);
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        TXPOS.push_back(0.);
        TZPOS.push_back(0.);
        TXSPEED.push_back(0.);
        TZSPEED.push_back(0.);
    }
}

void Dynamic_Tendencies::compute_tendencies(
                    const Grid &gr, State& state)
{
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        // POSITION TENDENCY
        //////////////////////////////////////////////////////
        TXPOS[pid] = state.XSPEED[pid];
        TZPOS[pid] = state.ZSPEED[pid];

        // SPEED TENDENCY
        //////////////////////////////////////////////////////

        // compute pressure gradient force
        // from neighbor particles
        dtype mass = gr.dom_vol / gr.nparticles;
        dtype temp = 290.;
        // pressure
        dtype press = CON_RD * temp / 
                            (state.VOLUME[pid] / mass);
        dtype dpdx = 0.;
        dtype dpdz = 0.;
        dtype diffx = 0.;
        dtype diffz = 0.;
        for ( int i = 0; i < gr.nclosest_part; i++ )
        {
            int npid = state.NEIGHID[pid][i];
            cart_coords norm_vec = {
                state.XPOS[pid] - state.XPOS[npid],
                state.ZPOS[pid] - state.ZPOS[npid],
            };
            dtype dist = sqrt(
                pow(state.XPOS[pid] - state.XPOS[npid], 2.) +
                pow(state.ZPOS[pid] - state.ZPOS[npid], 2.) );
            norm_vec = normalize_cart(norm_vec);
            //cout << norm_vec.x << " " << norm_vec.z << endl;
            //cout << dist << endl;
            // pressure of neighbor particle
            dtype npress = CON_RD * temp /
                                (state.VOLUME[npid] / mass);
            //cout << "press  " << press << "  " << npress << endl;
            dpdx += - (npress - press) / dist * norm_vec.x;
            dpdz += - (npress - press) / dist * norm_vec.z;

            //cout << "dpdx/z  " << dpdx << "  " << dpdz << endl;
            // diffusion
            diffx += (state.XSPEED[npid] - state.XSPEED[pid])/dist;
            diffz += (state.ZSPEED[npid] - state.ZSPEED[pid])/dist;
        }
        // from domain walls
        dpdx += - press / max(state.XPOS[pid] - gr.dom_x0, gr.cell_dx);
        dpdx += + press / max(gr.dom_x1 - state.XPOS[pid], gr.cell_dx);
        dpdz += - press / max(state.ZPOS[pid] - gr.dom_z0, gr.cell_dx);
        dpdz += + press / max(gr.dom_z1 - state.ZPOS[pid], gr.cell_dx);

        dpdx /= (gr.nclosest_part + 2);
        dpdz /= (gr.nclosest_part + 2);

        // compute density
        dtype rho = mass / state.VOLUME[pid];
        //cout << "rho " << rho << endl;

        // compute tendency
        TXSPEED[pid] = - 1./rho * dpdx;
        TZSPEED[pid] = - 1./rho * dpdz;
        TZSPEED[pid] -= CON_G;

        dtype diff_coef = 50.;
        TXSPEED[pid] += diff_coef * diffx;
        TZSPEED[pid] += diff_coef * diffz;


        //cout << "tend  " << TXSPEED[pid] << "  " << TZSPEED[pid] << endl;
        //exit(1);
    }

}
