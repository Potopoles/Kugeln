//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Integrate a state of the model forward in time.
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "grid.h"
#include "state.h"
#include "dynamic_tendencies.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

class Integrator
/*
Organize integration of model. Using Grid and State, compute
the physical tendencies using spatial discretisation and integrate
forward in time.
*/
{
    public:
        Integrator(Grid &gr, Timer* timer_in);

        Timer* timer;

        // advance one time step
        void integrate(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_tmp);

        // exchange values between model states to complete time step
        void exchange_time_levels(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_tmp);

        // options for time integration
        static string EULER_FORWARD;
        static string RK4          ;
        static string RK3          ;
        static string RK2          ;
        static string RK1          ;
        static string MATSUNO      ;
        static string LEAP_FROG    ;


    private:
        // pointers to objects of tendency classes
        Dynamic_Tendencies* dynamic_tendencies;

        // organise computation of time tendencies
        void _compute_tendencies(const Grid &gr, State &state);

        // compute state_new based on state_now (make time step)
        // by adding tendencies * time_step to state_now.
        void _advance_time(const Grid &gr, dtype time_step,
                                State &state_now, State &state_new);

        //////////////////////////////////////////////////////////////////////
        //// TIME DISCRETISATIONS
        //////////////////////////////////////////////////////////////////////

        // euler forward
        void _euler_forward(const Grid &gr,                             
                            State &state_now,
                            State &state_new);

        // runge-kutta (2nd - 4th order)
        void _runge_kutta(const Grid &gr, string rk_order,
                            State &state_now,
                            State &state_new,
                            State &state_tmp);
        void _rk_substep(const Grid &gr, dtype time_coef, dtype weight,
                            State &state_now,
                            State &state_new,
                            State &state_tmp);

        // leap frog
        void _leap_frog(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_old);

        // matsuno
        void _matsuno(const Grid &gr,
                            State &state_now,
                            State &state_new,
                            State &state_tmp);
};
