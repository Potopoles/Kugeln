//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Define right hand side of position change equation 
     to compute tendency of XPOS and YPOS.
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "grid.h"
#include "state.h"
#include <vector>
#include <map>


class Spatial_Tendencies
{
    public:
        Spatial_Tendencies(Grid &gr);

        // contains tendency of XPOS
        vector<dtype> TXPOS;
        // contains tendency of YPOS
        vector<dtype> TZPOS;

        // contains tendency of XSPEED
        vector<dtype> TXSPEED;
        // contains tendency of ZSPEED
        vector<dtype> TZSPEED;

        void compute_pos_tendencies(const Grid &gr, State& state);
        void compute_speed_tendencies(const Grid &gr, State& state);

    private:
        
};
