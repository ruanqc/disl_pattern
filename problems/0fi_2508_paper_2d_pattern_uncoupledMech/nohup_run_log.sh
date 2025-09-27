#!/bin/bash
read -e -p "Enter cpus     n : " nproc
read -e -p "Enter input   .i : " input
read -e -p "Enter track .log : " log
nohup mpiexec -n "$nproc" ../../disl_pattern-opt -i "$input" > "$log".log 2>&1 &


## This is used to run the simulation in parallel using mpiexec.
## and redirect output to a log file using nohup.
## @ruanqc-202509