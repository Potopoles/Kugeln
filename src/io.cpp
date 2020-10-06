//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Run io. Write model fields as output to binary files
*/
//////////////////////////////////////////////////////////////////////////////
#include "io.h"
#include "constants.h"
#include "namelist.h"
#include "math.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
///// FIELD IO FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

void diagnose_fields_for_io(const Grid &gr, State &state)
{
}

void field_to_binary(Namelist nl, const Grid &gr,
                    vector<dtype> &field, string field_name)
{
    // construct output path
    ostringstream oss;
    oss << internal << setfill('0') << setw(5) << (int) gr.sim_time;
    //oss << internal << setfill('0') << setw(5) << (int) gr.tstep;
    string field_out_path = nl.bin_dir + "/" + nl.sim_tag + 
                    "_np" + to_string(gr.nparticles) +
                    "_" + field_name +
                    "_t_" + oss.str() + "_sec.bin";

    // open binary file for field
    ofstream field_out_file;
    field_out_file.open(field_out_path, ios::binary|ios::out);

    for ( int i = 0; i < field.size(); i++ )
    {
        field_out_file.write(reinterpret_cast<char *>(&field[i]),
                        sizeof(field[i]));
    }

    // close binary file
    field_out_file.close();
}


void field_to_text(const Grid &gr, vector<dtype> &field,
                   string out_dir, string field_name)
{
    string file_path = out_dir + "/" + field_name + ".txt";
    ostringstream oss;
    for ( int i = 0; i < field.size(); i++ )
    {
        oss << field[i] << endl;
    }

    ofstream out_file;
    out_file.open(file_path);
    out_file << oss.str();
    out_file.close();
}





//////////////////////////////////////////////////////////////////////////////
///// GRID IO FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

vector<string> grid_io_str_split(string str, char sep)
{
    istringstream iss(str);
    vector<string> split;
    string token;
    while (getline(iss, token, sep)) 
    {
        //cout << "split " << token << "    ";
        split.push_back(token);
        //cout << token << " ";
        //cout << split[split.size()-1] << " ";
    }
    return(split);
}

void grid_io_intv2str(vector<int> const& vect, 
                    ostringstream& oss, int nentries)
{
    for (int i = 0; i < nentries; i++)
    {
        oss << vect[i];
        if (i != nentries - 1) 
        {
            oss << GRID_IO_SEP_INNER;
        }
    }
    oss << GRID_IO_SEP_OUTER;
}
void grid_io_dtypev2str(vector<dtype> const& vect, 
                    ostringstream& oss, int nentries)
{
    for (int i = 0; i < nentries; i++)
    {
        oss << vect[i];
        if (i != nentries - 1) 
        {
            oss << GRID_IO_SEP_INNER;
        }
    }
    oss << GRID_IO_SEP_OUTER;
}
void grid_io_cc2str(cart_coords const& cc, ostringstream& oss)
{
    oss << cc.x;
    oss << GRID_IO_SEP_INNER;
    //oss << cc.y;
    //oss << GRID_IO_SEP_INNER;
    oss << cc.z;
    oss << GRID_IO_SEP_OUTER;
}
void grid_io_1Dintarr2str(int arr[], ostringstream& oss, int size)
{
    for (int i = 0; i < size; i++)
    {
        oss << arr[i];
        if (i != size - 1) 
        {
            oss << GRID_IO_SEP_INNER;
        }
    }
    oss << GRID_IO_SEP_OUTER;
}
void grid_io_1Ddtypearr2str(dtype arr[], ostringstream& oss, int size)
{
    for (int i = 0; i < size; i++)
    {
        oss << arr[i];
        if (i != size - 1) 
        {
            oss << GRID_IO_SEP_INNER;
        }
    }
    oss << GRID_IO_SEP_OUTER;
}


vector<int> grid_io_str2intv(string str, char sep, int nentries)
{
    vector<string> split = grid_io_str_split(str, sep);
    vector<int> out_vect;
    out_vect.reserve(nentries);
    for (int i = 0; i < nentries; i++)
    {
        out_vect.push_back(stoi(split[i]));
        //cout << out_vect[i] << " ";
    }
    return(out_vect);
}
vector<dtype> grid_io_str2dtypev(string str, char sep, int nentries)
{
    vector<string> split = grid_io_str_split(str, sep);
    vector<dtype> out_vect;
    out_vect.reserve(nentries);
    for (int i = 0; i < nentries; i++)
    {
        out_vect.push_back(stof(split[i]));
        //cout << out_vect[i] << " ";
    }
    return(out_vect);
}
cart_coords grid_io_str2cc(string str, char sep)
{
    vector<string> split = grid_io_str_split(str, sep);
    cart_coords cc;
    cc.x = stof(split[0]);
    //cc.y = stof(split[1]);
    cc.z = stof(split[2]);
    //cout << cc.x << " " << cc.y << " " << cc.z << " ";
    return(cc);
}
void grid_io_str21Dintarr(int arr[], string str, char sep, int size)
{
    vector<string> split = grid_io_str_split(str, sep);
    for (int i = 0; i < size; i++)
    {
        arr[i] = stoi(split[i]);
        //cout << arr[i] << " ";
    }
}
void grid_io_str21Ddtypearr(dtype arr[], string str, char sep, int size)
{
    vector<string> split = grid_io_str_split(str, sep);
    for (int i = 0; i < size; i++)
    {
        arr[i] = stof(split[i]);
        //cout << arr[i] << " ";
    }
}