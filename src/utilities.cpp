//////////////////////////////////////////////////////////////////////////////
/*
Project: Meteomatics weather model
Created: 2020-03-17 by Christoph Heim <mailto:cheim@meteomatics.ch>
Description:
     Utility functions
*/
//////////////////////////////////////////////////////////////////////////////
#include "utilities.h"
#include "constants.h"
#include "math.h"
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
////// GENERIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

bool cmpf(dtype a, dtype b, dtype epsilon /*=0.000001f*/)
/*
    Compare to dtype numbers and return true if a == b with precision epsilon.
*/
{
    return(fabs(a - b) < epsilon);
}



void set_dist_position(int pid, dtype dist,
                    vector<dtype>& dists,
                    vector<int>& neighids)
{
    for (int i = 0; i < dists.size(); i++)
    {
        // if dist is smaller than current position
        if (dist < dists[i])
        {
            // move all subsequent values one position forward
            for (int j = dists.size()-1; j > i; j--)
            {
                dists[j] = dists[j-1];
                neighids[j] = neighids[j-1];
            }
            // insert new value at current position
            dists[i] = dist;
            neighids[i] = pid;
            break;
        }
    }
}