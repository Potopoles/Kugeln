//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Code to debug
*/
//////////////////////////////////////////////////////////////////////////////
#include "debug.h"
#include <iostream>
using namespace std;

Timer::Timer()
{
}

void Timer::start(string key)
{
    // timer does not yet exist.
    if (counters.find(key) == counters.end())
    {
        // create new counter
        StopWatch counter = StopWatch();
        counter.Restart();
        pair<string, StopWatch> counters_entry (key, counter);
        counters.insert(counters_entry);

        // create new counter running flag
        pair<string, bool> counters_active_entry (key, true);
        counters_active.insert(counters_active_entry);

        // create new timing
        pair<string, bool> timing_entry (key, 0);
        timings.insert(timing_entry);
    }
    // timer already used.
    else
    {
        // timer currently active.
        if (counters_active[key])
        {
            cout << "Timer " << key << " already running." << endl;
            exit(1);
        }
        // timer currently not active.
        else
        {
            counters[key].Restart();
            counters_active[key] = true;
        }
        
    }
    
}

void Timer::stop(string key)
{
    // timer does not yet exist.
    if (counters.find(key) == counters.end())
    {
        cout << "Timer " << key << " does not exist." << endl;
        exit(1);
    }
    // timer already used.
    else
    {
        // timer currently active.
        if (counters_active[key])
        {
            timings[key] += counters[key].ElapsedMs();
            counters_active[key] = false;
        }
        // timer currently not active.
        else
        {
            cout << "Timer " << key << " not running." << endl;
            exit(1);
        }
        
    }
    
}


void Timer::print_report()
{
    cout << "Timers reports:" << endl;
    cout << "#########################" << endl;
    unordered_map<string, int>::iterator it = timings.begin();
    while(it != timings.end())
    {
        cout << it->first << ": " << timings[it->first] << " ms." << endl;
        it++;
        //if (counters_active[it->first])
        //{
        //    cout << "WARNING: Timer " << it->first << 
        //            " is still active!" << endl;
        //}
    }
    cout << "#########################" << endl;
}