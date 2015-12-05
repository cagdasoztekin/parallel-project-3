#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include "util.h"

#define num_people 18
// #define num_files 10	// this is 'k' as defined in the assignment
#define total_num_files 20 
#define rows 180
#define cols 200

int num_files;

void create_histogram(int* hist, int** img, int num_rows, int num_cols);
int find_closest(int*** training_set, int num_persons, int num_training, int size, int* test_image);

clock_t begin, end;
double time_spent;

int main(int argc, char** argv){
	begin = clock();
	int i, j;
	int b; 

	num_files = atoi(argv[1]);

	// 4d array to have 2 layers for height and width and 2 layers for files and people
	int**** pre_training_set = (int****)malloc(num_people * sizeof(int***));
	for(i = 0; i < num_people; i++){
		pre_training_set[i] = (int***)malloc(num_files * sizeof(int**));
	}
	char filename[128];
	// printf("Hello\n");
	for(i = 0; i < num_people; i++){
		for(j = 0; j < num_files; j++){
			sprintf(filename, "/home/cagdas/Desktop/parallel-project-3/images/%d.%d.txt", i+1, j+1);
			// printf("%s\n", filename);
			pre_training_set[i][j] = read_pgm_file(filename, rows, cols);
		}
	}

	// printf("Creating the training set\n");
	// the actual training set where the histogram arrays will be held for each file for each person (3 layers!)
	int*** training_set = (int***)malloc(num_people * sizeof(int**));
	for(i = 0; i < num_people; i++){
		training_set[i] = (int**)malloc(num_files * sizeof(int*));
	}

	// create histograms for each person's each picture
	for(i = 0; i < num_people; i++){
		for(j = 0; j < num_files; j++){
			// printf("Onto it with i %d j %d\n", i, j);
			training_set[i][j] = (int*)malloc(sizeof(int) * 256);
			for(b = 0; b < 256; b++){
				training_set[i][j][b] = 0;
			}
			create_histogram(training_set[i][j], pre_training_set[i][j], rows, cols);

		}
	}
	// printf("created the training set\n");
	// test files
	int**** pre_test_set = (int****)malloc(num_people * sizeof(int***));
	for(i = 0; i < num_people; i++){
		pre_test_set[i] = (int***)malloc(sizeof(int**) * (total_num_files - num_files));
	}

	// printf("pre test set\n");


	for(i = 0; i < num_people; i++){
		for(j = 0; j < (total_num_files - num_files); j++){
			sprintf(filename, "/home/cagdas/Desktop/parallel-project-3/images/%d.%d.txt", i+1, j+1+num_files);
			// printf("Current name is %s\n", filename);
			pre_test_set[i][j] = read_pgm_file(filename, rows, cols);
		}
	}
	
	// printf("created pre test set\n");
	int*** test_set = (int***)malloc(num_people * sizeof(int**));
	for(i = 0; i < num_people; i++){
		test_set[i] = (int**)malloc((total_num_files - num_files) * sizeof(int*));
		for(j = 0; j < total_num_files - num_files; j++){
			test_set[i][j] = (int*)malloc(sizeof(int) * 256);
			for(b = 0; b < 256; b++){
				test_set[i][j][b] = 0;
			}
			create_histogram(test_set[i][j], pre_test_set[i][j], rows, cols);
		}
	}
	// printf("np abi\n");
	int totalCount = 0;
	int falseCount = 0;
	for(i = 0; i < num_people; i++){
		for(j = 0; j < total_num_files - num_files; j++){
			int cur = find_closest(training_set, num_people, num_files, 256, test_set[i][j]);

			// test the file with file name "%d.%d.txt\t\t%d\t\t%d\n", i+1, j+1+num_files
			totalCount++;	// a judgment is made
			if(cur != i+1){ // a false judgment is made
				printf("%d.%d.txt\t\t%d\t\t%d\n", i+1, j+1+num_files, i+1, cur);
				falseCount += 1;
			}
		}
	}
	printf("%d false out of %d\n", falseCount, totalCount);

	// TODO:: deallocate the allocated memory space

		int a;

	for(i = 0; i < num_people; i++){
		for(j = 0; j < num_files; j++){
			for(a = 0; a < rows; a++){
				free(pre_training_set[i][j][a]);
			}
			free(pre_training_set[i][j]);
		}
		free(pre_training_set[i]);
	}
	free(pre_training_set);
	// printf("free pre training set\n");

	for(i = 0; i < num_people; i++){
		for(j = 0; j < total_num_files - num_files; j++){
			for(a = 0; a < rows; a++){
				free(pre_test_set[i][j][a]);
			}
			free(pre_test_set[i][j]);
		}
		free(pre_test_set[i]);
	}
	free(pre_test_set);
	// printf("free pre test set\n");

	for(i = 0; i < num_people; i++){
		for(j = 0; j < num_files; j++){
			free(training_set[i][j]);
		}
		free(training_set[i]);
	}
	free(training_set);
	// printf("free training set\n");

	for(i = 0; i < num_people; i++){
		for(j = 0; j < total_num_files - num_files; j++){
			free(test_set[i][j]);
		}
		free(test_set[i]);
	}
	free(test_set);
	// printf("free test set\n");
	end = clock();
	time_spent = (double)(end - begin);
	printf("Total runtime: %f\n", time_spent/CLOCKS_PER_SEC);

	return 0;
}

// create a histogram of a given image and put the result for each pixel in the 1D hist array 
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
			base *= 2;
			currentBinary += (k < img[i+1][j-1]) * base;
			base *= 2;
			currentBinary += (k < img[i+1][j]) * base;
			base *= 2;
			currentBinary += (k < img[i+1][j+1]) * base;
			base *= 2;
			currentBinary += (k < img[i][j+1]) * base;
			base *= 2;
			currentBinary += (k < img[i-1][j+1]) * base;
			base *= 2;
			currentBinary += (k < img[i-1][j]) * base;
			base *= 2;
			currentBinary += (k < img[i-1][j-1]) * base;
			// printf("Current binary is %d\n", toDecimal(currentBinary));
			// hist[(i-1)*(num_cols-2) + (j-1)] = toDecimal(currentBinary);
			hist[currentBinary] += 1;
		}
	}
}

// calculate distance between each histogram
double distance(int* a, int* b, int size){
	int i;
	double distance = 0;

	for(i = 0; i < size; i++){
		// don't change the current sum if both are zero // disrupts the whole thing
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
	double closest = 256 * size * size + 1; // actual farthest possible distance between two histograms and plus one obviously
	// printf("Double max is %f", closest);
	double current = 0;
	int personid = -1;
	
	for(i = 0; i < num_persons; i++){
		// printf("Running on thread %d and i is %d\n", tid, i);
		for(j = 0; j < num_files; j++){
			current = distance(training_set[i][j], test_image, size);
			// printf("Current distance is %f with i %d and j %d\n", current, i, j);
			if(current <= closest){
				// printf("IFIFIFCurrent distance is %f with i %d and j %d\n", current, i, j);
				closest = current;
				personid = i+1;
			}
		}
	}

	return personid;
}