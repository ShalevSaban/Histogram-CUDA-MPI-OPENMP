#include <cuda_runtime.h>
#include <helper_cuda.h>


#define HISTOGRAM_SIZE 256


__global__  void cudaCalcHisto(int *arr,int numElements,int* histogram) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

     __shared__ int sharedHisto[HISTOGRAM_SIZE];

    //init shared histogram
	if(threadIdx.x==0){
		int j;
		for(j=0; j<HISTOGRAM_SIZE; j++){
			sharedHisto[j]=0;
		}
	}
	__syncthreads();
	
    //check match between threads to blocks 
    if (i < numElements)
    	atomicAdd(&sharedHisto[arr[i]],1);
    __syncthreads();
   if(threadIdx.x < HISTOGRAM_SIZE)
    	atomicAdd(&histogram[i], sharedHisto[i]);
}

int* cudaAllocate(int numElements,int* data)
{
   cudaError_t err = cudaSuccess;

    size_t size = numElements * sizeof(float);
  

    // Allocate memory on GPU to copy the data from the host
    int *d_A;
    err = cudaMalloc((void **)&d_A, size);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate device memory - %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copy data from host to the GPU memory
    err = cudaMemcpy(d_A, data, size, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy data from host to device - %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);   
    }

    return d_A;
}


int computeOnGPU(int *data,int* histogram, int numElements) {
    // Error code to check return values for CUDA calls
    cudaError_t err = cudaSuccess;

    int* cudaHisto=cudaAllocate(HISTOGRAM_SIZE,&histogram[0]);
    int* cudaArray=cudaAllocate(numElements,&data[0]);


    // Launch the Kernel
    int threadsPerBlock = 256;
    int blocksPerGrid =(numElements + threadsPerBlock - 1) / threadsPerBlock;
    cudaCalcHisto<<<blocksPerGrid, threadsPerBlock>>>(cudaArray, numElements,cudaHisto);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to launch vectorAdd kernel -  %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copy the  result from GPU to the host memory.
    err = cudaMemcpy(histogram,cudaHisto,HISTOGRAM_SIZE, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy result array from device to host -%s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Free allocated memory on GPU
    if (cudaFree(cudaArray) != cudaSuccess) {
        fprintf(stderr, "Failed to free device data - %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

     if (cudaFree(cudaHisto) != cudaSuccess) {
        fprintf(stderr, "Failed to free device data - %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    return 0;
}

