Experimental Lagrangian Fluid Dynamics Model

Can be compiled with:  
make clean
make


After compilation, run with e.g.:  
./main 100
args:  
    - 1: number of particles  

things to consider:  
- make sure that output directory exists in same folder
  as main.exe (currently "binaries")
- Output can be plotted with python scripts in directory analysis.
- more model settings in namelist.h
