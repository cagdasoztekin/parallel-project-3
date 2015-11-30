#include <stdlib.h>
#include <stdio.h>
#include "util.h"

int main(){

	// int i, j;
	// int**** pre_training_set = (int****)malloc(18 * sizeof(int***));
	// for(i = 0; i < 18; i++){
	// 	pre_training_set[i] = (int***)malloc(20 * sizeof(int**));
	// }
	// char filename[80];
	// printf("Hello\n");
	// for(i = 0; i < 18; i++){
	// 	for(j = 0; j < 20; j++){
	// 		sprintf(filename, "/home/cagdas/Desktop/parallel-project-3/images/%d.%d.txt", i+1, j+1);
	// 		printf("%s\n", filename);
	// 		pre_training_set[i][j] = read_pgm_file(filename, 180, 200);
	// 	}
	// }

	// int*** training_set = (int***)malloc(18 * sizeof(int**));
	// for(i = 0; i < 18; i++){
	// 	training_set[i] = (int**)malloc(20 * sizeof(int*));
	// }



	return 0;
}

int toDecimal(int binary){
	int result = 0;
	int current = 1;
	int a;
	while(binary > 0){
		a = binary % 10;
		result = result + a * current;
		binary /= 10;
		current *= 2;
	}

	return result;
}

void create_histogram(int* hist, int** img, int num_rows, int num_cols){
	int i, j;
	hist = (int*)malloc(sizeof(int) * (num_rows - 2) * (num_cols - 2));
	for(i = 1; i < num_rows - 1; i++){
		for(j = 1; j < num_cols - 1; j++){
			int currentBinary = 0;
			int base = 1;
			int k = img[i][j];

			currentBinary += (k < img[i][j-1]) * base;
			base *= 10;
			currentBinary += (k < img[i+1][j-1]) * base;
			base *= 10;
			currentBinary += (k < img[i+1][j]) * base;
			base *= 10;
			currentBinary += (k < img[i+1][j+1]) * base;
			base *= 10;
			currentBinary += (k < img[i][j+1]) * base;
			base *= 10;
			currentBinary += (k < img[i-1][j+1]) * base;
			base *= 10;
			currentBinary += (k < img[i-1][j]) * base;
			base *= 10;
			currentBinary += (k < img[i-1][j-1]) * base;
		}
	}
}

double distance(int* a, int* b, int size){
	return 0.0;
}

int find_closest(int*** training_set, int num_persons, int num_training, int size, int* test_image){
	return 0;
}

