#include <time.h>
#include <stdlib.h>
#include <stdio.h>
 
/**
 * This is a function to select randomly features for each tree of the Random Forest.
 * This is a functions that takes as an input the numver of features (cols)
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


/**
 * This functions fills a submatrix that is in essence the root of a tree of the random forest. 
 * As input it takes the whole data set which has "bigCols" features and then it selects randomly 
 * "Nfeatures" features to fill the smallMatr.
**/
void fillBof(int rows, int bigCols, int Nfeatures,double bigMatr[][bigCols], double smallMatr[][Nfeatures])
{
	int *bof = createBOF(bigCols,Nfeatures);

	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<Nfeatures; j++)
		{
			smallMatr[i][j] = bigMatr[i][bof[j]];
		}
	} 
}

void printAr1(int myArr[], int length)
{
	for(int i=0; i<length; i++)
	{
		printf("%d\n", myArr[i]);
	}
}


void printMatr(int rows, int cols, double Matr[rows][cols])
{
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			printf("%f  ", Matr[i][j]);
		}
		printf("\n");
	}
	printf("----------------------------------\n");
}



int main() {

	double test_matrix[4][6] = {
	{10.0, 11.6, 12.4, 13.9, 10.4, 84.1},
	{4.01, 0.6, 2.4, 3.9, 0.4, 4.1},
	{1.0, 1.6, 1.4, 1.9, 1.4, 8.1},
	{14.9, 15.8, 16.6, 1.7, 29.02, 32.9}
	};

	double smallmat[4][2];
	fillBof(4,6,2,test_matrix,smallmat);

	//printf("!!!Hello World!!! %d" , r);
	//int *arr;
	//arr = createBOF(200,50);

	printMatr(4,6,test_matrix);
	printMatr(4,2,smallmat);
	return 0;
}

