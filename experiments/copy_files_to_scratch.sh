#! /bin/bash -l
#SBATCH --nice=5000

mkdir -p /scratch/heuser0000/pdbs
cp /infai/heuser0000/stp-pho-solver/PDBFILES/* /scratch/heuser0000/pdbs