#!/bin/bash
mpicc mestreEscravo.c -o me
mpirun -np $1 me