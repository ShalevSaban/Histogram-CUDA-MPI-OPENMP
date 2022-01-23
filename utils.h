#ifndef UTILS_H_
#define UTILS_H_



int*    getUserInput(int* inputSize); 
int*    initHistogram();                
void    fulfilHistograms(int** histograms,int size); 
int     calcFinishPlace(int size);
int     computeOnGPU(int *data,int* histogram, int numElements);
void    mergeHistograms(int* histo1, int* histo2);
void    printHistogram(int* histogram);
void    writeToFile(int* histogram);
int*    readFromFile(int* inputSize);

#endif
