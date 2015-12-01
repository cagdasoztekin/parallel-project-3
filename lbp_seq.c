#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "util.h"

#define num_people 18
#define num_files 15
#define rows 180
#define cols 200
void sm(int* asd);
int main(){

	int i, j;

	// 4d array to have 2 layers for height and width and 2 layers for files and people
	int**** pre_training_set = (int****)malloc(num_people * sizeof(int***));
	for(i = 0; i < num_people; i++){
		pre_training_set[i] = (int***)malloc(num_files * sizeof(int**));
	}
	char filename[80];
	printf("Hello\n");
	for(i = 0; i < num_people; i++){
		for(j = 0; j < num_files; j++){
			sprintf(filename, "/home/cagdas/Desktop/parallel-project-3/images/%d.%d.txt", i+1, j+1);
			// printf("%s\n", filename);
			pre_training_set[i][j] = read_pgm_file(filename, rows, cols);
		}
	}

	printf("Creating the training set\n");
	// the actual training set where the histogram arrays will be held for each file for each person (3 layers!)
	int*** training_set = (int***)malloc(num_people * sizeof(int**));
	for(i = 0; i < num_people; i++){
		training_set[i] = (int**)malloc(num_files * sizeof(int*));
	}

	for(i = 0; i < num_people; i++){
		for(j = 0; j < num_files; j++){
			// printf("Onto it with i %d j %d\n", i, j);
			training_set[i][j] = (int*)malloc(sizeof(int) * (rows-2)*(cols-2));
			create_histogram(training_set[i][j], pre_training_set[i][j], rows, cols);

		}
	}

	// test files

	int** t1_11 = read_pgm_file("/home/cagdas/Desktop/parallel-project-3/images/1.11.txt", rows, cols);
	int** t1_12 = read_pgm_file("/home/cagdas/Desktop/parallel-project-3/images/1.12.txt", rows, cols);
	int** t1_13 = read_pgm_file("/home/cagdas/Desktop/parallel-project-3/images/1.13.txt", rows, cols);
	int** t2_11 = read_pgm_file("/home/cagdas/Desktop/parallel-project-3/images/2.11.txt", rows, cols);
	int** t2_12 = read_pgm_file("/home/cagdas/Desktop/parallel-project-3/images/2.12.txt", rows, cols);
	int* h1 = (int*)malloc(sizeof(int) * (rows-2)*(cols-2));;
	int* h2 = (int*)malloc(sizeof(int) * (rows-2)*(cols-2));;
	int* h3 = (int*)malloc(sizeof(int) * (rows-2)*(cols-2));;
	int* h4 = (int*)malloc(sizeof(int) * (rows-2)*(cols-2));;
	int* h5 = (int*)malloc(sizeof(int) * (rows-2)*(cols-2));;
	// printf("Here?");
	create_histogram(h1, t1_11, rows, cols);
	create_histogram(h2, t1_12, rows, cols);
	create_histogram(h3, t1_13, rows, cols);
	create_histogram(h4, t2_11, rows, cols);
	// printf("Not here\n");
	create_histogram(h5, t2_12, rows, cols);
	// for(i = 0; i < (rows-2)*(cols-2); i++){
		// printf("%d ", h1[i]);
	// }
	// printf("Here!");
	printf("%d\n", find_closest(training_set, num_people, num_files, (rows-2)*(cols-2), h1));
	printf("%d\n", find_closest(training_set, num_people, num_files, (rows-2)*(cols-2), h2));
	printf("%d\n", find_closest(training_set, num_people, num_files, (rows-2)*(cols-2), h3));
	printf("%d\n", find_closest(training_set, num_people, num_files, (rows-2)*(cols-2), h4));
	printf("%d\n", find_closest(training_set, num_people, num_files, (rows-2)*(cols-2), h5));

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
	// printf("Hola histo\n");
	int i, j;
	// hist = (int*)malloc(sizeof(int) * (num_rows - 2) * (num_cols - 2));
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
			// printf("Current binary is %d\n", toDecimal(currentBinary));
			hist[(i-1)*(num_cols-2) + (j-1)] = toDecimal(currentBinary);
		}
	}
}

double distance(int* a, int* b, int size){
	int i;
	double distance = 0;
	for(i = 0; i < size; i++){
		// printf("a[i] is %f\n", (double)(a[i] - b[i])*(a[i] - b[i])/(double)(2*(a[i] + b[i])));
		if(a[i] != 0 || b[i] != 0){
			distance += (double)(a[i] - b[i])*(a[i] - b[i])/(double)(2*(a[i] + b[i]));
		}
	}
	// printf("This distance is %f\n", distance);
	return distance;
}

int find_closest(int*** training_set, int num_persons, int num_training, int size, int* test_image){
	// printf("Hey");
	int i, j;
	double closest = 100000000;
	// printf("Double max is %f", closest);
	double current = 0;
	int personid = -1;
	for(i = 0; i < num_persons; i++){
		for(j = 0; j < num_files; j++){
			current = distance(training_set[i][j], test_image, size);
			// printf("Current distance is %f with i %d and j %d\n", current, i, j);
			if(current < closest){
				// printf("IFIFIFCurrent distance is %f with i %d and j %d\n", current, i, j);
				closest = current;
				personid = i+1;
			}
		}
	}

	return personid;
}