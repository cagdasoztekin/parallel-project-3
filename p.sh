gcc -Wall -pg  lbp_omp.c util.h util.c -o lbp_omp -fopenmp
OMP_NUM_THREADS=16 ./lbp_omp
gprof lbp_omp gmon.out > analysis-parallel.txt

