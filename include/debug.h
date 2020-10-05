//////////////////////////////////////////////////////////////////////////////
/*
Project: Kugelns
Created: 2020-10-05 by Christoph Heim
Description:
     Code to debug
*/
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "StopWatch.h"
#include <string>
#include <unordered_map>
using namespace std;

class Timer
/*
Track timings over course of simulation.
*/
{
     public:
          Timer();
          void start(string key);
          void stop(string key);

          void print_report();

     private:
          unordered_map<string, StopWatch> counters;
          unordered_map<string, bool> counters_active;
          unordered_map<string, int> timings;
};