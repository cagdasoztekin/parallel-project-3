#!/bin/bash
k_vals=(1 2 5 7 10)
num_threads=(1 2 4 6 8 16)
rm analysis-sequential.txt
rm analysis-parallel.txt
touch analysis-parallel.txt
touch analysis-sequential.txt
for i in ${k_vals[@]}; 
do
	echo "Running sequential program for k = ${i}. Output is written into prof_sequential.txt"
	gcc -Wall -pg  lbp_seq.c util.h util.c -o lbp_seq
	./lbp_seq ${i} >> analysis-sequential.txt
	echo "Results for k=${i}" >> analysis-sequential.txt
	gprof lbp_seq gmon.out >> analysis-sequential.txt
	for j in ${num_threads[@]};
	do
		echo "Running parallel program for k = ${i} with number of threads = ${j}. Output is written into prof_parallel.txt"
		gcc -Wall -pg  lbp_omp.c util.h util.c -o lbp_omp -fopenmp
		OMP_NUM_THREADS=${j} ./lbp_omp ${i} >> analysis-parallel.txt
		echo "Results for k=${i}" >> analysis-parallel.txt
		gprof lbp_omp gmon.out >> analysis-parallel.txt
	done
done


