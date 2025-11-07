#read -e -p "Enter cpus     n : " nproc
#read -e -p "Enter input   .i : " input
#mpiexec -n "$nproc" ../../disl_pattern-opt -i "$input"


read -e -p "Enter input( .i ): " input
mpiexec -n 3  ../../disl_pattern-opt -i "$input"


#!/bin/bash
## This is used to run the simulation in parallel using mpiexec.
## @ruanqc-202509
