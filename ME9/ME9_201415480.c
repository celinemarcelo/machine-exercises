#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>


float ave_times[3] = {};
long int best_times[3] = {};
long int worst_times[3] = {};

int iterated = 0;



//Sorting functions (taken from http://rosettacode.org/wiki/Sorting_algorithms)
void selection_sort (int *data_tbs, int size, int mode) {
	int i, j, m, t;

	struct timeval t1, t2;
	long int elapsed = 0;

	puts(" using selection sort...");

	gettimeofday(&t1, NULL);

	for (i = 0; i < size; i++) {
		for (j = i, m = i; j < size; j++) {
			if (data_tbs[j] < data_tbs[m]) {
				m = j;
			}
		}
		t = data_tbs[i];
		data_tbs[i] = data_tbs[m];
		data_tbs[m] = t;
	}

	gettimeofday(&t2, NULL);

	elapsed = t2.tv_usec - t1.tv_usec;
	elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

	puts("End of selection sort.");
	printf("Elapsed time: %ld us\n", elapsed);

	ave_times[mode] += elapsed;

	if(!iterated){
		best_times[mode] = elapsed;
		worst_times[mode] = elapsed;
	}

	if (elapsed < best_times[mode]) {
		best_times[mode] = elapsed;
	} else if (elapsed > worst_times[mode]) {
		worst_times[mode] = elapsed;
	}


}


void quick_sort (int *data_tbs, int size) {
	int i, j, p, t;

	if (size < 2)
		return;
	p = data_tbs[size / 2];
	for (i = 0, j = size - 1;; i++, j--) {
		while (data_tbs[i] < p)
			i++;
		while (p < data_tbs[j])
			j--;
		if (i >= j)
			break;
		t = data_tbs[i];
		data_tbs[i] = data_tbs[j];
		data_tbs[j] = t;
	}
	quick_sort(data_tbs, i);
	quick_sort(data_tbs + i, size - i);
}


void bubble_sort(int *data_tbs, int size, int mode) {
	int i, t, s = 1;

	struct timeval t1, t2;
	long int elapsed = 0;

	puts(" using bubble sort...");

	gettimeofday(&t1, NULL);

	while (s) {
		s = 0;
		for (i = 1; i < size; i++) {
			if (data_tbs[i] < data_tbs[i - 1]) {
				t = data_tbs[i];
				data_tbs[i] = data_tbs[i - 1];
				data_tbs[i - 1] = t;
				s = 1;
			}
		}
	}

	gettimeofday(&t2, NULL);

	elapsed = t2.tv_usec - t1.tv_usec;
	elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

	puts("End of bubble sort.");
	printf("Elapsed time: %ld us\n", elapsed);

	ave_times[mode] += elapsed;

	if(!iterated){
		best_times[mode] = elapsed;
		worst_times[mode] = elapsed;
	}

	if (elapsed < best_times[mode]) {
		best_times[mode] = elapsed;
	} else if (elapsed > worst_times[mode]) {
		worst_times[mode] = elapsed;
	}
}


void shell_sort(int *data_tbs, int size, int mode) {
	int h, i, j, t;

	struct timeval t1, t2;
	long int elapsed = 0;

	puts(" using shell sort...");

	gettimeofday(&t1, NULL);

	for (h = size; h /= 2;) {
		for (i = h; i < size; i++) {
			t = data_tbs[i];
			for (j = i; j >= h && t < data_tbs[j - h]; j -= h) {
				data_tbs[j] = data_tbs[j - h];
			}
			data_tbs[j] = t;
		}
	}

	gettimeofday(&t2, NULL);

	elapsed = t2.tv_usec - t1.tv_usec;
	elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

	puts("End of shell sort.");
	printf("Elapsed time: %ld us\n", elapsed);

	ave_times[mode] += elapsed;

	if(!iterated){
		best_times[mode] = elapsed;
		worst_times[mode] = elapsed;
	}

	if (elapsed < best_times[mode]) {
		best_times[mode] = elapsed;
	} else if (elapsed > worst_times[mode]) {
		worst_times[mode] = elapsed;
	}
}


void insertion_sort(int *data_tbs, int size, int mode) {
	int i = 0, j = 0, tmp = 0;
	struct timeval t1, t2;
	long int elapsed = 0;

	puts(" using insertion sort...");

	gettimeofday(&t1, NULL);


	for (i = 1; i < size; i++) {
		tmp = data_tbs[i];
		j = i;
		while (j > 0 && tmp < data_tbs[j - 1]) {
			data_tbs[j] = data_tbs[j - 1];
			j--;
		}
		data_tbs[j] = tmp;
	}

	gettimeofday(&t2, NULL);

	elapsed = t2.tv_usec - t1.tv_usec;
	elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

	puts("End of insertion sort.");
	printf("Elapsed time: %ld us\n", elapsed);

	ave_times[mode] += elapsed;

	if(!iterated){
		best_times[mode] = elapsed;
		worst_times[mode] = elapsed;
	}

	if (elapsed < best_times[mode]) {
		best_times[mode] = elapsed;
	} else if (elapsed > worst_times[mode]) {
		worst_times[mode] = elapsed;
	}

}

//Printing of data
void show_data(int *rand_data, int *sorted_data, int *rev_data, int size) {
	int i = 0;

	puts("Randomized Data:");
	for (i = 0; i < size; i++) {
		printf("%d\n", rand_data[i]);
	}
	puts("-----------------------------");
	sleep(3);

	puts("Sorted Data:");
	for (i = 0; i < size; i++) {
		printf("%d\n", sorted_data[i]);
	}
	puts("-----------------------------");
	sleep(3);

	puts("Reverse-sorted Data:");
	for (i = 0; i < size; i++) {
		printf("%d\n", rev_data[i]);
	}

}



//Runtime generation of data

void revsort_data(int *rev_data, int size) {

	int i = 0;

	for (i = size - 1; i >= 0; i--) {

		*(rev_data + i) = i;

		//printf("NUMBER = %d\n", rev_data[i]);
	}

	//printf("i = %d\n", i);
}


void sort_data(int *sorted_data, int size) {

	int i = 0;

	for (i = 0; i < size; i++) {

		*(sorted_data + i) = i;

		//printf("NUMBER = %d\n", sorted_data[i]);
	}

	//printf("i = %d\n", i);
}


void randomize_data(int *rand_data, int size) {

	int i = 0;

	for (i = 0; i < size; i++) {

		*(rand_data + i) = (rand() % (size * 10)) + 1;

		//printf("NUMBER = %d\n", rand_data[i]);
	}

	//printf("i = %d\n", i);
}



//Main function

int main(int argc, char const *argv[]) {
	int *rand_data, *sorted_data, *rev_data, size = 0, i = 0;
	char choice[10] = {};

	while (size <= 0) {
		puts("Please input array size.");
		fgets(choice, 10, stdin);
		choice[strlen(choice) - 1] = 0;

		size = atoi(choice);

		if (size <= 0) {
			puts("Invalid array size. Please try again.\n\n");
		}
	}

	puts("");

	rand_data = malloc(size * sizeof(int));
	sorted_data = malloc(size * sizeof(int));
	rev_data = malloc(size * sizeof(int));

	memset(choice, 0, 5);

	randomize_data(rand_data, size);
	sort_data(sorted_data, size);
	revsort_data(rev_data, size);

	while (1) {
		puts("Please select data sorting algorithm.");
		puts("1 - Insertion Sort");
		puts("2 - Shell Sort");
		puts("3 - Bubble Sort");
		puts("4 - Selection Sort");
		puts("5 - Quicksort");

		fgets(choice, 10, stdin);

		if (!strcmp("1\n", choice)) {
			for (i = 0; i < 5; i++) {
				printf("Sorting randomized data");
				insertion_sort(rand_data, size, 0);
				printf("Sorting already-sorted data");
				insertion_sort(sorted_data, size, 1);
				printf("Sorting reverse-sorted data");
				insertion_sort(rev_data, size, 2);
				//show_data(rand_data, sorted_data, rev_data, size);

				randomize_data(rand_data, size);
				sort_data(sorted_data, size);
				revsort_data(rev_data, size);

				iterated++;
			}
			

			puts("--------INSERTION SORT--------");


			break;
		} else if (!strcmp("2\n", choice)) {
			for (i = 0; i < 5; i++) {
				printf("Sorting randomized data");
				shell_sort(rand_data, size, 0);
				printf("Sorting already-sorted data");
				shell_sort(sorted_data, size, 1);
				printf("Sorting reverse-sorted data");
				shell_sort(rev_data, size, 2);
				//show_data(rand_data, sorted_data, rev_data, size);

				randomize_data(rand_data, size);
				sort_data(sorted_data, size);
				revsort_data(rev_data, size);


				iterated++;
			}

			puts("--------SHELL SORT--------");


			break;
		} else if (!strcmp("3\n", choice)) {
			for (i = 0; i < 25; i++) {
				printf("Sorting randomized data");
				bubble_sort(rand_data, size, 0);
				printf("Sorting already-sorted data");
				bubble_sort(sorted_data, size, 1);
				printf("Sorting reverse-sorted data");
				bubble_sort(rev_data, size, 2);
				//show_data(rand_data, sorted_data, rev_data, size);


				randomize_data(rand_data, size);
				sort_data(sorted_data, size);
				revsort_data(rev_data, size);


				iterated++;
			}

			puts("--------BUBBLE SORT--------");

			break;
		} else if (!strcmp("4\n", choice)) {
			for (i = 0; i < 25; i++) {
				printf("Sorting randomized data");
				selection_sort(rand_data, size, 0);
				printf("Sorting already-sorted data");
				selection_sort(sorted_data, size, 1);
				printf("Sorting reverse-sorted data");
				selection_sort(rev_data, size, 2);
				//show_data(rand_data, sorted_data, rev_data, size);


				randomize_data(rand_data, size);
				sort_data(sorted_data, size);
				revsort_data(rev_data, size);


				iterated++;
			}

			puts("--------SELECTION SORT--------");

			break;
		} else if (!strcmp("5\n", choice)) {
			struct timeval t1, t2;
			long int elapsed = 0;

			for (i = 0; i < 25; i++) {

				puts("Sorting randomized data using quicksort...");

				gettimeofday(&t1, NULL);


				quick_sort(rand_data, size);


				gettimeofday(&t2, NULL);

				elapsed = t2.tv_usec - t1.tv_usec;
				elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

				puts("End of quick sort.");
				printf("Elapsed time: %ld us\n", elapsed);

				memset(&t1, 0, sizeof(t1));
				memset(&t2, 0, sizeof(t2));


				randomize_data(rand_data, size);

			}


			for (i = 0; i < 25; i++) {

				puts("Sorting already-sorted data using quicksort...");

				gettimeofday(&t1, NULL);


				quick_sort(sorted_data, size);


				gettimeofday(&t2, NULL);

				elapsed = t2.tv_usec - t1.tv_usec;
				elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

				puts("End of quick sort.");
				printf("Elapsed time: %ld us\n", elapsed);

				memset(&t1, 0, sizeof(t1));
				memset(&t2, 0, sizeof(t2));

				sort_data(sorted_data, size);

			}

			for (i = 0; i < 25; i++) {
				puts("Sorting reverse-sorted data using quicksort...");

				gettimeofday(&t1, NULL);

				quick_sort(rev_data, size);

				gettimeofday(&t2, NULL);

				elapsed = t2.tv_usec - t1.tv_usec;
				elapsed +=  (t2.tv_sec - t1.tv_sec) * 1000000;

				puts("End of quick sort.");
				printf("Elapsed time: %ld us\n", elapsed);


				memset(&t1, 0, sizeof(t1));
				memset(&t2, 0, sizeof(t2));

				revsort_data(rev_data, size);
			}



			iterated++;

			//show_data(rand_data, sorted_data, rev_data, size);

			break;
		} else {
			puts("Invalid choice. Please try again.\n\n");
			continue;
		}
	}


	puts("For randomized data:");
	printf("Best: %ld, Average: %f, Worst: %ld\n", best_times[0], ave_times[0] / 25.0, worst_times[0]);
	puts("For already-sorted data:");
	printf("Best: %ld, Average: %f, Worst: %ld\n", best_times[1], ave_times[1] / 25.0, worst_times[1]);
	puts("For reverse-sorted data:");
	printf("Best: %ld, Average: %f, Worst: %ld\n", best_times[2], ave_times[2] / 25.0, worst_times[2]);


	return 0;
}