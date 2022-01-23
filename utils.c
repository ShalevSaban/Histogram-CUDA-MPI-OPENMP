#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>



// (optional) get inputs from user 
int* getUserInput(int* inputSize)
{
  int number,i=0;
  puts("Enter amount of numbers: ");
  scanf("%d",&number);//Number of scanfs
  int* userInput=(int*)malloc(sizeof(int)*number);
  while(i<number)
  {
  puts("next number :  ");
  scanf("%d",&userInput[i]);
  i++;
  }
  *inputSize=number;
  return userInput;
}


//init empty histogram  
int* initHistogram()
{
int* histogram = (int*) calloc(256, sizeof(int));
return histogram;
}


//fulfil 2d array of histograms 
void fulfilHistograms(int** histograms,int size)
{
  for(int i=0;i<size;i++)
    histograms[i]=initHistogram();
}

// calculate finish place for arrays at main 
int calcFinishPlace(int size)
{
  if (size%2==0)
    return size/2;
  else
    return (size/2)+1;
}

//merge 2 histograms into the first one 
void mergeHistograms(int* histo1, int* histo2){
	for(int i = 0; i < 256; i++)
		histo1[i] += histo2[i];
}

//write results to the file
void writeToFile(int* histogram)
{		
	for (int i = 0; i < 256; i++) {
		if (histogram[i] > 0)
			fprintf(stdout, " %d\t: %d\n", i, histogram[i]);
	}
	
}

// print the histogram to the file/screen
void printHistogram(int* histogram)
{
	printf("HISTOGRAM:\n\n");
	for (int i = 0; i < 256; i++) {
		if (histogram[i] > 0)
			printf(" %d\t: %d\n", i, histogram[i]);
	}
	printf("\n");
}

//read inputs from file 
int* readFromFile(int* inputSize){
	int* arr = NULL;
	int i;

	fscanf(stdin, "%d", inputSize);	
	arr = (int*) malloc(*inputSize * sizeof(int));
	for(i = 0; i < *inputSize; i++){
		fscanf(stdin, "%d", &arr[i]);
	}

	return arr;
}