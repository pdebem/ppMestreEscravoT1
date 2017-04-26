#!/bin/bash
clear
clear
clear

mpicc mestreEscravo.c -o me
mpirun -np $1 me