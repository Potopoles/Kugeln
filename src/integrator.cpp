//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Integrate a state of the model forward in time.
*/
//////////////////////////////////////////////////////////////////////////////
#include "integrator.h"
#include "utilities.h"
#include "io.h"
#include "dynamic_tendencies.h"
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

string Integrator::EULER_FORWARD    = "EULER_FORWARD";
string Integrator::RK4              = "RK4";
string Integrator::RK3              = "RK3";
string Integrator::RK2              = "RK2";
string Integrator::RK1              = "RK1";
string Integrator::MATSUNO          = "MATSUNO";
string Integrator::LEAP_FROG        = "LEAP_FROG";

Integrator::Integrator(Grid &gr, Timer* timer_in)
{
    timer = timer_in;

    dynamic_tendencies = new Dynamic_Tendencies(gr);
}

void Integrator::integrate(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_tmp)
{
    if (gr.time_discr == Integrator::EULER_FORWARD)
    {
        _euler_forward(gr, state_now, state_new);
    }
    else if (gr.time_discr == Integrator::RK4 ||
             gr.time_discr == Integrator::RK3 || 
             gr.time_discr == Integrator::RK2 || 
             gr.time_discr == Integrator::RK1)
    {
        _runge_kutta(gr, gr.time_discr, state_now, state_new, state_tmp);
    }
    else if (gr.time_discr == Integrator::LEAP_FROG)
    {
        int nth_matsuno = 10;
        // every nth_matsuno time step make matsuno integration step
        // instead of leap frog to stabilize leap frog scheme.
        if (gr.tstep % nth_matsuno == 0)
        {
            _matsuno(gr, state_now, state_new, state_tmp);
        }
        else
        {
            _leap_frog(gr, state_now, state_new, state_tmp);
        }
    }
    else if (gr.time_discr == Integrator::MATSUNO)
    {
        _matsuno(gr, state_now, state_new, state_tmp);
    }
}


void Integrator::exchange_time_levels(const Grid &gr,
                                    State &state_now,
                                    State &state_new,
                                    State &state_tmp)
{
    if (gr.time_discr == Integrator::LEAP_FROG)
    {
        for ( int pid = 0; pid < gr.nparticles; pid++ )
        {
            // particle position
            state_tmp.XPOS[pid] = state_now.XPOS[pid];
            state_tmp.ZPOS[pid] = state_now.ZPOS[pid];

            // particle speed
            state_tmp.XSPEED[pid] = state_now.XSPEED[pid];
            state_tmp.ZSPEED[pid] = state_now.ZSPEED[pid];
        }
    }
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state_now.XPOS[pid] = state_new.XPOS[pid];
        state_now.ZPOS[pid] = state_new.ZPOS[pid];

        state_now.XSPEED[pid] = state_new.XSPEED[pid];
        state_now.ZSPEED[pid] = state_new.ZSPEED[pid];
    }
}

void Integrator::_compute_tendencies(const Grid &gr, State &state)
{
    timer->start("integrator.tendencies");

    timer->start("integrator.tendencies.neighids");
    // find closest particles
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        // vector storing distances
        vector<dtype> dists (gr.nclosest_part, 1.E10);
        
        // get distance to walls
        // left wall
        dtype dist = state.XPOS[pid] - gr.dom_x0;
        set_dist_position(-1, dist, dists, state.NEIGHID[pid]);
        // lower wall
        dist = state.ZPOS[pid] - gr.dom_z0;
        set_dist_position(-2, dist, dists, state.NEIGHID[pid]);
        // right wall
        dist = gr.dom_x1 - state.XPOS[pid];
        set_dist_position(-3, dist, dists, state.NEIGHID[pid]);
        // upper wall
        dist = gr.dom_z1 - state.ZPOS[pid];
        set_dist_position(-4, dist, dists, state.NEIGHID[pid]);

        // get distance to closest neighbors
        for ( int pid2 = 0; pid2 < gr.nparticles; pid2++ )
        {
            if (pid2 == pid) continue;

            dtype dist = sqrt(
                pow(state.XPOS[pid] - state.XPOS[pid2], 2.) +
                pow(state.ZPOS[pid] - state.ZPOS[pid2], 2.) );

            set_dist_position(pid2, dist, dists, state.NEIGHID[pid]);
        }
        /*
        for (int i = 0; i < gr.nclosest_part; i++)
        {
            cout << state.NEIGHID[pid][i] << " ";
            cout << dists[i] << "   ";
        }
        cout << endl;
        */
    }
    //exit(1);
    timer->stop("integrator.tendencies.neighids");
    //exit(1);

    timer->start("integrator.tendencies.volume");
    // find particle volume
    dtype vol_sum = 0.;
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state.VOLUME[pid] = 0.;
        for ( int i = 0; i < gr.nclosest_part; i++ )
        {
            int npid = state.NEIGHID[pid][i];
            dtype dist;
            if (npid >= 0)
            {
                dist = sqrt(
                    pow(state.XPOS[pid] - state.XPOS[npid], 2.) +
                    pow(state.ZPOS[pid] - state.ZPOS[npid], 2.) );
            }
            else
            {
                switch (npid)
                {
                // left wall
                case -1:
                    dist = state.XPOS[pid] - gr.dom_x0;
                    break;
                // lower wall
                case -2:
                    dist = state.ZPOS[pid] - gr.dom_z0;
                    break;
                // right wall
                case -3:
                    dist = gr.dom_x1 - state.XPOS[pid];
                    break;
                // upper wall
                case -4:
                    dist = gr.dom_z1 - state.ZPOS[pid];
                    break;
                default:
                    cout << "unkown case" << endl;
                    exit(1);;
                }
            }
            state.VOLUME[pid] += dist;
        }
        //cout << state.VOLUME[pid] << endl;
        vol_sum += state.VOLUME[pid];
    }
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state.VOLUME[pid] = state.VOLUME[pid] / vol_sum * gr.dom_vol;
    }
    //exit(1);
    /*
    // find particle volume
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state.VOLUME[pid] = 0.;
    }
    #pragma omp parallel for
    for (int i = 0; i < gr.ncells_x; i++)
    {
        for (int k = 0; k < gr.ncells_z; k++)
        {
            // find closest particle to current cell
            dtype min_dist = 99999999.;
            int pid_min = 0;
            for ( int pid = 0; pid < gr.nparticles; pid++ )
            {
                dtype cell_x = (i + 0.5) * gr.cell_dx;
                dtype cell_z = (k + 0.5) * gr.cell_dx;
                dtype dist = sqrt(
                    pow(state.XPOS[pid] - cell_x, 2.) +
                    pow(state.ZPOS[pid] - cell_z, 2.) );
                if (dist < min_dist)
                {
                    min_dist = dist;
                    pid_min = pid;
                    //cout << min_dist << "  " << pid_min << endl;
                } 
            }
            // add volume of current cell to closest particle
            state.VOLUME[pid_min] += gr.cell_vol;
        }
    }
    // test for zero particle volumes and set those to minimum vol.
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        if (state.VOLUME[pid] == 0.)
        {
            cout << "Cell volume is zero!!" << endl;
            //exit(1);
            state.VOLUME[pid] = gr.cell_vol;
        }
    }
    */
    timer->stop("integrator.tendencies.volume");

    // compute tendencies
    dynamic_tendencies->compute_tendencies(gr, state);
    //exit(1);
    timer->stop("integrator.tendencies");
}

void Integrator::_advance_time(const Grid &gr, dtype time_step,
                                State &state_now, State &state_new)
{
    // advance particle position and speed
    #pragma omp parallel for
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        // particle position
        state_new.XPOS[pid] = state_now.XPOS[pid] + 
                        dynamic_tendencies->TXPOS[pid] * time_step;
        state_new.ZPOS[pid] = state_now.ZPOS[pid] + 
                        dynamic_tendencies->TZPOS[pid] * time_step;

        // particle speed
        state_new.XSPEED[pid] = state_now.XSPEED[pid] + 
                    dynamic_tendencies->TXSPEED[pid] * time_step;
        state_new.ZSPEED[pid] = state_now.ZSPEED[pid] + 
                    dynamic_tendencies->TZSPEED[pid] * time_step;

        // rigid boundaries x
        if (state_new.XPOS[pid] < gr.dom_x0)
        {
            state_new.XPOS[pid] = gr.dom_x0 - state_new.XPOS[pid];
            //state_new.XPOS[pid] = gr.dom_x0;
            state_new.XSPEED[pid] = 0.;
        }
        if (state_new.XPOS[pid] > gr.dom_x1)
        {
            state_new.XPOS[pid] = gr.dom_x1 - 
                        (state_new.XPOS[pid] - gr.dom_x1);
            //state_new.XPOS[pid] = gr.dom_x1;
            state_new.XSPEED[pid] = 0.;
        }
        /*
        // periodic boundaries x
        if (state_new.XPOS[pid] < gr.dom_x0)
        {
            state_new.XPOS[pid] = gr.dom_x1 - 
                        (gr.dom_x0 - state_new.XPOS[pid]);
        }
        if (state_new.XPOS[pid] > gr.dom_x1)
        {
            state_new.XPOS[pid] = gr.dom_x0 + 
                        (state_new.XPOS[pid] - gr.dom_x1);
        }
        */
        // rigid boundaries z
        if (state_new.ZPOS[pid] < gr.dom_z0)
        {
            state_new.ZPOS[pid] = gr.dom_z0 - state_new.ZPOS[pid];
            //state_new.ZPOS[pid] = gr.dom_z0;
            state_new.ZSPEED[pid] = 0.;
        }
        if (state_new.ZPOS[pid] > gr.dom_z1)
        {
            state_new.ZPOS[pid] = gr.dom_z1 - 
                        (state_new.ZPOS[pid] - gr.dom_z1);
            //state_new.ZPOS[pid] = gr.dom_z1;
            state_new.ZSPEED[pid] = 0.;
        }

        //cout << "xspeed " << state_new.XSPEED[pid] - state_now.XSPEED[pid] << endl;
    }
    //exit(1);

}


void Integrator::_euler_forward(const Grid &gr,
                            State &state_now,
                            State &state_new)
{
    _compute_tendencies(gr, state_now);
    _advance_time(gr, gr.dt, state_now, state_new);
}

void Integrator::_runge_kutta(const Grid &gr, string rk_order,
                            State &state_now,
                            State &state_new,
                            State &state_tmp)
{
    vector<dtype> time_coefs;
    vector<dtype> weights;
    int ncoefs = 0;
    // set Runge Kutta coefficients depending on order of RK scheme.
    if ( rk_order == Integrator::RK4)
    {
        time_coefs = {0.   , 1./2., 1./2., 1.   };
        weights    = {1./6., 1./3., 1./3., 1./6.};
        ncoefs     = 4;
    }
    else if (rk_order == Integrator::RK3)
    {
        time_coefs = {0.   , 1./2., 1.   };
        weights    = {1./6., 2./3., 1./6.};
        ncoefs     = 3;
    }
    else if (rk_order == Integrator::RK2)
    {
        time_coefs = {0.   , 1./2.};
        weights    = {0.   , 1.   };
        ncoefs     = 2;
    }
    else if (rk_order == Integrator::RK1)
    {
        time_coefs = {0.};
        weights    = {1.};
        ncoefs     = 1;
    }

    //// initialize rhs_sum (storing intermediate solutions)
    //// and set to zero.
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state_tmp.XPOS[pid] = 0.;
    }

    /* RK algorithm:
    // first RK substep:
    new = now + tend * time_coef[0]     = now
    tend = f(new)                       = f(now)
    tmp = tmp + weight[0] * tend        = weight[0] * tend

    // all following RK substeps:
    new = now + tend * time_coef[1]
    tend = f(new)
    tmp = tmp + weight[j] * tend

    repeat last 3 lines for all RK substeps after first..

    new = now + tmp * dt
    */

    // run RK sub steps
    for ( int step_ind = 0; step_ind < ncoefs; step_ind++ )
    {
        _rk_substep(gr, time_coefs[step_ind], weights[step_ind],
                   state_now, state_new, state_tmp);
    }

    // set tendencies to corresponding state_tmp fields to do 
    // definitive time step based on final sum of RHS of equation.
    // Actually only necessary for RK3 & RK4.
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        dynamic_tendencies->TXPOS[pid] = state_tmp.XPOS[pid];
    }

    // do definitive time step
    _advance_time(gr, gr.dt, state_now, state_new);
}



void Integrator::_rk_substep(const Grid &gr, dtype time_coef, dtype weight,
                            State &state_now,
                            State &state_new,
                            State &state_tmp)
{
    _advance_time(gr, gr.dt * time_coef, state_now, state_new);
    _compute_tendencies(gr, state_new);
    _advance_time(gr, weight, state_tmp, state_tmp);
}


void Integrator::_leap_frog(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_old)
{
    _compute_tendencies(gr, state_now);
    _advance_time(gr, 2.*gr.dt, state_old, state_new);
}


void Integrator::_matsuno(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_tmp)
{
    _compute_tendencies(gr, state_now);
    _advance_time(gr, gr.dt, state_now, state_tmp);
    _compute_tendencies(gr, state_tmp);
    _advance_time(gr, gr.dt, state_now, state_new);
}
