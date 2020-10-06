//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Run io. Write model fields as output to binary files
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "namelist.h"
#include "grid.h"
#include "state.h"
#include <vector>
#include <map>
#include <string>
#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
///// FIELD IO FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

void field_to_binary(Namelist nl, const Grid &gr,
                    vector<dtype> &field, string field_name);
void field_to_text(const Grid &gr, vector<dtype> &field,
                    string out_dir, string field_name);


//////////////////////////////////////////////////////////////////////////////
///// GRID IO FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
// outer separator (between different attributes) in grid save file
const string GRID_IO_SEP_OUTER = " ";
// inner separator (between different elements of one attribute) 
// in grid save file
const string GRID_IO_SEP_INNER = ",";
vector<string> grid_io_str_split(string str, char sep);
//template <typename T, typename A>
//void grid_io_intv2str(vector<T,A> const& vect, 
//                    ostringstream& oss, int nentries);
void grid_io_intv2str(vector<int> const& vect, 
                    ostringstream& oss, int nentries);
void grid_io_dtypev2str(vector<dtype> const& vect, 
                    ostringstream& oss, int nentries);
void grid_io_cc2str(cart_coords const& cc, ostringstream& oss);
void grid_io_1Dintarr2str(int arr[], ostringstream& oss, int size);
void grid_io_1Ddtypearr2str(dtype arr[], ostringstream& oss, int size);
//template <size_t rows, size_t cols>
//void grid_io_2Ddtypearr2str(dtype (&arr)[rows][cols],
//                            ostringstream& oss);

vector<int> grid_io_str2intv(string str, char sep, int nentries);
vector<dtype> grid_io_str2dtypev(string str, char sep, int nentries);
cart_coords grid_io_str2cc(string str, char sep);
void grid_io_str21Dintarr(int arr[], string str, char sep, int size);
void grid_io_str21Ddtypearr(dtype arr[], string str, char sep, int size);