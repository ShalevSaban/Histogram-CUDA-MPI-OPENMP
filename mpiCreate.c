#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <stddef.h>


#define NUM_OF_ATTRIBUTES 3

void InitMPI(int *argc, char **argv[], int *rank, int *numOfProcs)
{
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, numOfProcs);

}


