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
#include "spatial_tendencies.h"
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

    spatial_tendencies = new Spatial_Tendencies(gr);
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
            state_tmp.XPOS[pid] = state_now.XPOS[pid];
        }
    }
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state_now.XPOS[pid] = state_new.XPOS[pid];
    }
}

void Integrator::_compute_tendencies(const Grid &gr, State &state)
{
    timer->start("integrator.tendencies");
    // compute tendencies
    spatial_tendencies->compute_pos_tendencies(gr, state);
    timer->stop("integrator.tendencies");
}

void Integrator::_advance_time(const Grid &gr, dtype time_step,
                                State &state_now, State &state_new)
{
    // advance positions
    for ( int pid = 0; pid < gr.nparticles; pid++ )
    {
        state_new.XPOS[pid] = state_now.XPOS[pid] + 
                        spatial_tendencies->TXPOS[pid] * time_step;
    }

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
        spatial_tendencies->TXPOS[pid] = state_tmp.XPOS[pid];
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
