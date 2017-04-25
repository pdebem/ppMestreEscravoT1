#include "mpi.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int* getMatrixOfIntegers(int length, int numberOfVectors){
	srand(time(NULL));   // should only be called once
	
	int i;
	int matrix[numberOfVectors][length];
	for(i=0; i < numberOfVectors; ++j){
		int j;
		for (j = 0; j < length; ++j)
		{
			matrix[i][j]= rand();      // returns a pseudo-random integer between 0 and RAND_MAX
		}
	}
	return matrix;
}