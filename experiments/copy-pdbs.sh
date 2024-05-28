#! /bin/bash -l
#SBATCH --job-name=copy-pdbs
#SBATCH --partition=infai_1
#SBATCH --qos=normal
#SBATCH --time=0
#SBATCH --mem-per-cpu=3872M
#SBATCH --cpus-per-task=8
### Number of tasks in array job.
#SBATCH --array=1-8
#SBATCH --nice=5000

for i in $(seq -w 24)
do
  sbatch --nodelist=ase$i copy_files_to_scratch.sh
done
