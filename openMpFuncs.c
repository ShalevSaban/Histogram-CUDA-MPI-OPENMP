#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <omp.h>
#include "utils.h"

#define HISTOGRAM_SIZE sizeof(int)*256
#define ARRAY_HISTOGRAM_SIZE 256

//create histogram for procs 0 
int* ompHistogram0(int* arr,int start,int end)
{
int tid,i,counterTid=0;
int numOfThreads=omp_get_num_threads();

// array of private histogram for each thread
int** histograms=(int**)calloc(numOfThreads,sizeof(HISTOGRAM_SIZE));
int* histogram=initHistogram();
#pragma omp parallel shared(arr) private(i,tid)
{
    tid=omp_get_thread_num();
    histograms[tid]=initHistogram();
    counterTid++;

    #pragma omp for
        for(i=start;i<end;i++)
        {
        #pragma omp atomic   
        (histograms[tid][arr[i]])++;//increase histograms 
        }
        
        
}

//Merge all histograms
 #pragma omp parallel for reduction(+:histogram[:ARRAY_HISTOGRAM_SIZE])  
    for(int i=0;i<counterTid;i++)
    for(int j=0;j<ARRAY_HISTOGRAM_SIZE;j++)
    histogram[j]+=histograms[i][j];

return histogram;

}






//reduce for histograms at main 
int* ompHistogram1(int* arr,int start,int end)
{
int tid,i,counterTid=0,k,l;
int numOfThreads=omp_get_num_threads();

// array of private histogram for each thread
int** histograms=(int**)calloc(numOfThreads,sizeof(HISTOGRAM_SIZE));
int* histogram=initHistogram();
#pragma omp parallel shared(arr) private(i,tid)
{
    tid=omp_get_thread_num();
    histograms[tid]=initHistogram();
    counterTid++;

    #pragma omp for
        for(i=start;i<end;i++)
        (histograms[tid][arr[i]])++;//increase histograms 
}
     
	#pragma omp parallel for private(k, l) shared(histogram,histograms, counterTid)
		    for(k=0;k<counterTid;k++)
             for(l=0;l<ARRAY_HISTOGRAM_SIZE;l++)
               histogram[l]+=histograms[k][l];
return histogram;

}





