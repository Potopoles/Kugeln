//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Store model fields of a certain state.
*/
//////////////////////////////////////////////////////////////////////////////
#include "state.h"
#include <vector>
#include <map>
using namespace std;

State::State(int nparticles, int nclosest_part)
{
    //////////////////////////////////////////////////////////////////////////
    //// particle quantities
    //////////////////////////////////////////////////////////////////////////
    XPOS.reserve(nparticles);
    ZPOS.reserve(nparticles);
    XSPEED.reserve(nparticles);
    ZSPEED.reserve(nparticles);
    VOLUME.reserve(nparticles);
    TEMP.reserve(nparticles);

    NEIGHID.resize(nparticles, vector<int>(nclosest_part, 0));

    for ( int pid = 0; pid < nparticles; pid++ )
    {
        XPOS.push_back(0.);
        ZPOS.push_back(0.);
        XSPEED.push_back(0.);
        ZSPEED.push_back(0.);
        VOLUME.push_back(0.);
        TEMP.push_back(0.);
    }

}
