//////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Auxilliary functions for geometrical computations.
*/
//////////////////////////////////////////////////////////////////////////
#include "constants.h"
#include "grid_funcs.h"
#include <iostream>
#include "math.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
////// AUXILIARY FUNCTIONS
//////////////////////////////////////////////////////////////////////////////









//////////////////////////////////////////////////////////////////////////////
////// GEOMETRIC FUNCTIONS GENERIC
//////////////////////////////////////////////////////////////////////////////
dtype scalar_product(cart_coords a, cart_coords b)
{
    //dtype scalar = a.x * b.x + a.y * b.y + a.z * b.z;
    dtype scalar = a.x * b.x + a.z * b.z;
    return(scalar);
}

dtype magnitude_cart(cart_coords vector)
{
    return(
        sqrt( pow(vector.x,2.) + 
              //pow(vector.y,2.) + 
              pow(vector.z,2.) ) );
}


cart_coords normalize_cart(cart_coords vector)
/*
Return vector with length = unit length. Radius of unit sphere.
*/
{
    dtype length = sqrt(pow(vector.x,2) +
                        //pow(vector.y,2) +
                        pow(vector.z,2));
    vector.x /= length;
    //vector.y /= length;
    vector.z /= length;
    return(vector);
}

cart_coords find_middle_point(cart_coords point1,
                              cart_coords point2)
{
    /*
    Find a middle point between two points
    */
    //cart_coords middle_point = {0., 0., 0.};
    cart_coords middle_point = {0., 0.};
    middle_point.x = 0.5 * (point1.x + point2.x);
    //middle_point.y = 0.5 * (point1.y + point2.y);
    middle_point.z = 0.5 * (point1.z + point2.z);
    return(middle_point);
}
