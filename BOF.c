#include <time.h>
#include <stdlib.h>
#include <stdio.h>
 
/**
 * This is a function to select randomly features for each tree of the Random Forest.
 * This is a functions that takes as an input the number of features (cols)
 * and returns an array with randomly selected numbers from 0 to cols. The 
 * returned array has size Nfeatures.
**/
int * createBOF(int cols, int Nfeatures)
{
	srand(time(NULL));   // Initialization, should only be called once.
	static int bof[210];
	int r = 0;

	int array1[cols];

	for(int i=0; i<cols; i++)
	{
		array1[i] = i; 
	}

	for(int i=0; i<Nfeatures; i++)
	{
		r = rand() % (cols-i);
		bof[i] = array1[r];

		array1[r] = array1[cols-1-i];
	}

	return bof;

}

void printAr1(int myArr[], int length)
{
	for(int i=0; i<length; i++)
	{
		printf("%d\n", myArr[i]);
	}
}




int main() {

	//printf("!!!Hello World!!! %d" , r);
	int *arr;
	arr = createBOF(200,50);
	printAr1(arr,50);
	return 0;
}

