//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Main program. 
*/
//////////////////////////////////////////////////////////////////////////////
#include "constants.h"
#include "namelist.h"
#include "debug.h"
#include "integrator.h"
#include "utilities.h"
#include "state.h"
#include "io.h"
#include "initial_condition.h"
#include "omp.h"
#include <string>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main(int argc, char** argv)
{
    (void)argc;
    //////////////////////////////////////////////////////////////////////////
    ///////////////////////// MODEL INITIALIZATION ///////////////////////////
    //////////////////////////////////////////////////////////////////////////
    Timer timer = Timer();
    timer.start("TOTAL");
    omp_set_num_threads(OMP_NUM_THREADS);
    //StopWatch timer = StopWatch();
    //timer.Restart();
    //cout << timer.ElapsedMs() << endl;
    
    //// NAMELIST
    //////////////////////////////////////////////////////////////////////////
    Namelist nl;
    //////// GENERAL SETTINGS
    // WARNING: do not use _ in sim tag!!
    nl.sim_tag = "test";
    //////// GRID
    nl.nparticles   = atoi(argv[1]);
    nl.dom_x0       = 0.;
    nl.dom_x1       = 10.;
    nl.dom_z0       = 0.;
    nl.dom_z1       = 10.;
    nl.ncells_part = 20;
    //////// INTEGRATION
    nl.tot_time = 60.0;
    nl.dt = 1.;
    cout << "time step " << nl.dt << endl;
    //// DISCRETISATION
    // TIME
    nl.time_discr = Integrator::EULER_FORWARD;
    nl.time_discr = Integrator::RK4;
    nl.time_discr = Integrator::RK3;
    nl.time_discr = Integrator::RK2;
    nl.time_discr = Integrator::RK1;
    nl.time_discr = Integrator::LEAP_FROG;
    nl.time_discr = Integrator::MATSUNO;
    //// INITIAL CONDITIONS
    //// OUTPUT
    nl.bin_dir  = "output";
    // write output every xxx hours.
    nl.nth_sec_out = 10.;

    ////// CREATE GRID
    ////////////////////////////////////////////////////////////////////////////
    timer.start("grid");
    Grid gr = Grid(nl, timer);
    timer.stop("grid");

    //// SETUP MODEL FIELDS
    //////////////////////////////////////////////////////////////////////////
    // state at current time step
    State state_now = State(gr.nparticles);
    // state at next time step
    State state_new = State(gr.nparticles);
    // temporary state used for Runge Kutta
    // last time step state for leap frog
    // estimate state for Matsuno
    State state_tmp = State(gr.nparticles);

    // setting initial conditions
    init_position(gr, state_now, state_new, state_tmp);

    //// OUTPUT INITIAL CONDITION
    //////////////////////////////////////////////////////////////////////////
    field_to_binary(nl, gr, state_now.XPOS, "XPOS");
    field_to_binary(nl, gr, state_now.ZPOS, "ZPOS");
    field_to_binary(nl, gr, state_now.XSPEED, "XSPEED");
    field_to_binary(nl, gr, state_now.ZSPEED, "ZSPEED");
    field_to_binary(nl, gr, state_now.VOLUME, "VOLUME");
    field_to_binary(nl, gr, state_now.TEMP, "TEMP");

    ////////////////////////////////////////////////////////////////////////////
    /////////////////////////////// TIME STEPPING //////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    timer.start("timeloop");
    Integrator integrator {gr, &timer};
    for (gr.tstep = 0; gr.tstep < gr.nt; gr.tstep++) {

        //// TIME STEP DIAGNOSTICS
        //////////////////////////////////////////////////////////////////////////
        if (gr.tstep % 1 == 0)
        {
            timer.start("timeloop diagnostics");
            string s(16, '\0');
            auto written = snprintf(&s[0], s.size(),
                            "%.2f", gr.sim_time/60);
            s.resize(written);
            cout << "###### step: " << gr.tstep <<
                    " sim time: " << s << " mins" << endl;
            timer.stop("timeloop diagnostics");
        }

        //// INTEGRATION
        //////////////////////////////////////////////////////////////////////////
        timer.start("integrator.integrate");
        integrator.integrate(gr, state_now, state_new, state_tmp);
        timer.stop("integrator.integrate");
        //timer.start("integrator.exchange_time_levels");
        integrator.exchange_time_levels(gr, state_now,
                                    state_new, state_tmp);
        //timer.stop("integrator.exchange_time_levels");
        // update time (sim_time represents time of state_now)
        gr.sim_time = gr.tstep * gr.dt;

        //// OUTPUT
        //////////////////////////////////////////////////////////////////////////
        //if ((gr.tstep == 0))
        if ((gr.tstep % gr.nth_tstep_out == 0))
        {
            timer.start("save_fields");
            field_to_binary(nl, gr, state_now.XPOS, "XPOS");
            field_to_binary(nl, gr, state_now.ZPOS, "ZPOS");
            field_to_binary(nl, gr, state_now.XSPEED, "XSPEED");
            field_to_binary(nl, gr, state_now.ZSPEED, "ZSPEED");
            field_to_binary(nl, gr, state_now.VOLUME, "VOLUME");
            field_to_binary(nl, gr, state_now.TEMP, "TEMP");
            //exit(1);
            timer.stop("save_fields");
        }
    }
    timer.stop("timeloop");

    string s(16, '\0');
    auto written = snprintf(&s[0], s.size(),
                    "%.2f", gr.sim_time/60.);
    cout << "FINISHED SIMULATION ###### step: " << gr.tstep <<
            " sim time: " << s << " mins" << endl;
    timer.stop("TOTAL");
    timer.print_report();
    //cout << timer.ElapsedMs() << endl;


    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// FINALIZE /////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    return(0);
}



