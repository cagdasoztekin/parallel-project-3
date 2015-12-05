#!/bin/bash
k_vals=(1 2 5 7 10)
rm analysis-sequential.txt
rm analysis-parallel.txt
touch analysis-parallel.txt
touch analysis-sequential.txt
for i in ${k_vals[@]}; 
	do
	echo "Running sequential program for k = ${i}"
	gcc -Wall -pg  lbp_seq.c util.h util.c -o lbp_seq
	./lbp_seq ${i}
	echo "Results for k=${i} \n" >> analysis-sequential.txt
	gprof lbp_seq gmon.out >> analysis-sequential.txt
	echo "Running parallel program for k = ${i}"
	gcc -Wall -pg  lbp_omp.c util.h util.c -o lbp_omp -fopenmp
	./lbp_omp ${i}
	echo "Results for k=${i} \n" >> analysis-parallel.txt
	gprof lbp_omp gmon.out >> analysis-parallel.txt
	done


