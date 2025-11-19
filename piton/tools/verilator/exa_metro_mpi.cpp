#include <iostream>
#include <omp.h>
#include <mpi.h>
#include "verilated.h"

int chipset_main(int argc,char **argv);
int tile_main(int argc,char **argv);

int main(int argc, char **argv, char **env) {
    Verilated::commandArgs(argc, argv);
   
   int provided ,rank;
    MPI_Init_thread(&argc,&argv, MPI_THREAD_MULTIPLE, &provided );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(rank==0){
#pragma omp parallel num_threads(1)
	    chipset_main(argc,argv);
    }
    else
     {
#pragma omp parallel num_threads(16)
	    tile_main(argc,argv);
     }
    exit(0);
}



