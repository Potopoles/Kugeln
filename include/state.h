//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Store model fields of a certain state.
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "constants.h"
#include <vector>
#include <map>
using namespace std;


class State
{
/*
Contains arrays storing variables at one (sub) time level.
*/
public:
    State(int nparticles, int nclosest_part);

    //////////////////////////////////////////////////////////////////////////
    //// particle quantities
    //////////////////////////////////////////////////////////////////////////
    // XPOS: particle location in x direction [m]
    vector<dtype> XPOS;
    // ZPOS: particle location in z direction [m]
    vector<dtype> ZPOS;
    // XSPEED: particle speed in x directino [m s-1]
    vector<dtype> XSPEED;
    // ZSPEED: particle speed in z direction [m s-1]
    vector<dtype> ZSPEED;
    // VOLUME: particle voluem [m3]
    vector<dtype> VOLUME;
    // TEMP: particle temperature  [K]
    vector<dtype> TEMP;

    // NEIGHID: indices (pids) of CON_N_CLOSEST_NEIGHS particles
    vector<vector<int>> NEIGHID;
};
