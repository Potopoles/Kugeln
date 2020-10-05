//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
    Set up initial condition of prognostic variables
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "grid.h"
#include "state.h"
using namespace std;

void init_position(Grid &gr,
            State &state_now, State &state_new,
            State &state_tmp);



