gcc -Wall -pg  lbp_seq.c util.h util.c -o lbp_seq -fopenmp
./lbp_seq
gprof lbp_seq gmon.out > analysis-sequential.txt

