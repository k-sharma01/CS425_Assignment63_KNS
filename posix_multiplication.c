/**
CS 425 Assignment 6.3
Kirin Sharma
This program creates a number of posix threads specified by a command-line argument and
distributes a flattened 2-d array of integers across processes, and multiplies by a second array,
storing to a third array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 1000 // constant for the size of the matrix

// Globally accessible variables that can be used by any thread
int thread_count; /*Global thread count*/
int matrix1[SIZE][SIZE];
int matrix2[SIZE][SIZE];
int resultMatrix[SIZE][SIZE];
int chunk_size;

void *multiplyMatrix(void* thread); // Thread function

int main(int argc, char* argv[]) {
    long thread;
	pthread_t* thread_handles;

    // Initialize 2D matrices
    srand(time(NULL));
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            matrix1[i][j] = rand() % 10;
            matrix2[i][j] = rand() % 10;
        }
    }

	/* Get num of threads from command line and allocate space for their handles*/
	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count * sizeof(pthread_t));
    chunk_size = SIZE / thread_count;

    /* Create each thread and start working. */
	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, multiplyMatrix, (void*) thread);
	}

    // Join the threads
	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}

    // Print the first 3 entries of result matrix
    printf("Multiplication Completed.\n");
    printf("First 3 elements:\n %d   %d   %d \n", resultMatrix[0][0], resultMatrix[0][1], resultMatrix[0][2]);

    // Free memory and return
	free(thread_handles);
	return 0;
} /* end main */

// Function to multiply the matrix which is executed by each thread
void *multiplyMatrix(void* thread) {
    int thread_id = (long) thread; // gets the id of the thread
    int startIndex = thread_id * chunk_size;
    int endIndex = (thread_id == thread_count - 1) ? SIZE : startIndex + chunk_size;

    for(int i = startIndex; i < endIndex; i++) {
        for(int j = 0; j < SIZE; j++) {
            for(int k = 0; k < SIZE; k++) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return NULL;
} // end multiplyMatrix
