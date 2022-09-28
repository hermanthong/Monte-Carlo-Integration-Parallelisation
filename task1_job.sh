#!/bin/bash
#SBATCH --chdir /home/thong/Monte-Carlo-Integration-Parallelisation
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 28
#SBATCH --mem 1G

echo STARTING AT `date`

./pi 4 500000

echo FINISHED at `date`