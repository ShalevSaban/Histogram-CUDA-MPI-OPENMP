#ifndef OPENMPFUNCS_H_
#define OPENMPFUNCS_H_


int*    ompHistogram0(int* arr,int start,int end); 
void    reduceHistograms(int* histogram,int* histograms,int counterTid); 
int*    ompHistogram1(int* arr,int start,int end); 




#endif