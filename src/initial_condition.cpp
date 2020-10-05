//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
    Set up initial condition of prognostic variables
*/
//////////////////////////////////////////////////////////////////////////////
#include "initial_condition.h"
using namespace std;



void init_position(Grid &gr,
            State &state_now, State &state_new,
            State &state_tmp)
{
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state_now.XPOS[pid] = rand() / dtype(RAND_MAX) * 
                            (gr.dom_x1 - gr.dom_x0) + gr.dom_x0;
        state_now.ZPOS[pid] = rand() / dtype(RAND_MAX) * 
                            (gr.dom_z1 - gr.dom_x0) + gr.dom_z0;

        state_new.XPOS[pid] = state_now.XPOS[pid];
        state_new.ZPOS[pid] = state_now.ZPOS[pid];

        state_tmp.XPOS[pid] = state_now.XPOS[pid];
        state_tmp.ZPOS[pid] = state_now.ZPOS[pid];
    }

}