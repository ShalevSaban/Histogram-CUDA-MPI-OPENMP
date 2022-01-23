# Histogram-CUDA-MPI-OPENMP

Parallel-Programming project.
Calculate a histogram array from a file using 2 processes.

The program allowing each process to work on half the array at the same time. In each process the array is split to two again.

  -  The first half is calculated using OMP pragmas.
  -  The second half is transferred to the GPU where it's calculated in parallel using CUDA functions.

