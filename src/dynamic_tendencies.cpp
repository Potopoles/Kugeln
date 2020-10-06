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
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>


Dynamic_Tendencies::Dynamic_Tendencies(Grid &gr)
{
    TXPOS.reserve(gr.nparticles);
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        TXPOS.push_back(0.);
    }
}

void Dynamic_Tendencies::compute_tendencies(
                    const Grid &gr, State& state)
{
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        TXPOS[pid] = state.XSPEED[pid];
    }

}
