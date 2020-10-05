//////////////////////////////////////////////////////////////////////////////
/*
Project: Meteomatics weather model
Created: 2020-05-11 by Christoph Heim <mailto:cheim@meteomatics.ch>
Description:
    Structures defining objects of the grid.
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "constants.h"
using namespace std;

/*
Cartesian coordinates
*/
struct cart_coords
{
    dtype x;
    dtype y;
    dtype z;
};

/*
Polar coordinates
*/
struct polar_coords
{
    dtype lon;
    dtype lat;
};

///*
//Vertex indices of each triangular cell
//*/
//struct vert_inds_of_cell
//{
//    int v0;
//    int v1;
//    int v2;
//};