build:
	mpicxx -fopenmp -c main.c -o main.o
	mpicxx -fopenmp -c mpiCreate.c -o mpiCreate.o
	mpicxx -fopenmp -c utils.c -o utils.o
	mpicxx -fopenmp -c openMpFuncs.c -o openMpFuncs.o
	
	nvcc -I./inc -c cudaFunctions.cu -o cudaFunctions.o
	mpicxx -fopenmp -o mpiCudaOpemMP  main.o mpiCreate.o utils.o  cudaFunctions.o openMpFuncs.o  /usr/local/cuda/lib64/libcudart_static.a -ldl -lrt

clean:
	rm -f *.o ./mpiCudaOpemMP

run:
	mpiexec -np 2 ./mpiCudaOpemMP < input.txt > output.txt

