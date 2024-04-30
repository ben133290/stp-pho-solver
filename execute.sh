#!/bin/bash
#SBATCH --chdir /infai/heuser0000/stp-pho-solver
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 36
#SBATCH --mem 10G
#SBATCH --partition infai_1
#SBATCH --nice 5000


echo STARTING AT `date`

pwd 
./cmake-build-debug/lp-solver

echo FINISHED at `date`
