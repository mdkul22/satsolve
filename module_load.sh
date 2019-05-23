#srun --pty --nodes=1 --ntasks-per-node=4 -p debug -t 00:30:00 --wait 0 /bin/bash
module purge
module load gnutools
export MODULEPATH=/share/apps/compute/modulesfiles:$MODULEPATH
source ~/.bashrc
module load intel mvapich2_ib
module load intel/2016.3.210
module load gnu/4.9.2
