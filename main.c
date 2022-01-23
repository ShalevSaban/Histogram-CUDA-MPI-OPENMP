#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include "mpi.h"
#include <stdio.h>
#include <omp.h>

#include "utils.h"
#include "mpiCreate.h"
#include "openMpFuncs.h"


#define HISTOGRAM_SIZE sizeof(int)*256
#define NUM_OF_PROCS 2


void InitMPI(int *argc, char **argv[], int *rank, int *numOfProcs);

int main(int argc, char *argv[])
{

	int rank,inputSize;
	int numOfProcs=NUM_OF_PROCS;

	InitMPI(&argc, &argv, &rank, &numOfProcs);
	MPI_Status status;
	
	if(rank==0)
	{

	int* arr=readFromFile(&inputSize);		//get numbers from file
	int* finishHistogram=initHistogram(); 	//allocate empty histogram
		
	//brodcast for input size
	MPI_Bcast(&inputSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Send half 1 of array to open mp his' operations. recive merge histogram
	 int* procs_0_histo =ompHistogram0(arr,0,calcFinishPlace(inputSize));

	//send half 2 to pro' 2
	MPI_Send(&arr[calcFinishPlace(inputSize)],inputSize/2, MPI_INT,1,0, MPI_COMM_WORLD);

	//recive procs 2 histogram and merge seq'
	MPI_Recv(&finishHistogram[0],ARRAY_HISTOGRAM_SIZE,MPI_INT,1, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    
	mergeHistograms(finishHistogram,procs_0_histo);
	
	//print or write the histogram
	printHistogram(&finishHistogram[0]);

	}
	else  // rank 1 
	{

	//Get inputs size
	MPI_Bcast(&inputSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//make array for half of recived  inputs 
	int* procsInput=(int*)malloc((inputSize/2)*sizeof(int));
	MPI_Recv(procsInput,inputSize/2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
	
	// open mp histogram (half of procs input) operation with optimal for 
    int* openMpHistogram=ompHistogram1(procsInput,0,calcFinishPlace(inputSize/2));
	
	//cuda histogram operation  
	int* cudaHistogram=initHistogram();
	computeOnGPU(&procsInput[calcFinishPlace(inputSize/2)],cudaHistogram,inputSize/4);

	//Merge 2 histograms intoo open mp histogram
	mergeHistograms(openMpHistogram,cudaHistogram);

	//Send the histograms
	MPI_Send(&openMpHistogram[0],ARRAY_HISTOGRAM_SIZE,MPI_INT,0,1,MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}






