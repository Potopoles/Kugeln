//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Define right hand side of position change equation 
     to compute tendency of XPOS and YPOS.
*/
//////////////////////////////////////////////////////////////////////////////
#include "spatial_tendencies.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>


Spatial_Tendencies::Spatial_Tendencies(Grid &gr)
{
    TXPOS.reserve(gr.nparticles);
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        TXPOS.push_back(0.);
    }
}

void Spatial_Tendencies::compute_pos_tendencies(
                    const Grid &gr, State& state)
{
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        TXPOS[pid] = 0.1;
    }

}
