#include <stdio.h>
#include <math.h>
#define FEATURE_COL 6
#define FEATURE_ROW 4


void get_best_descriptor(int *size_matrix, double parent[][size_matrix[1]],double target[]);

int main() {

	double test_matrix[4][6] = {
	    {10.0, 11.6, 12.4, 13.9, 10.4, 84.1},
	    {4.01, 0.6, 2.4, 3.9, 0.4, 4.1},
	    {1.0, 1.6, 1.4, 1.9, 1.4, 8.1},
	    {14.9, 15.8, 16.6, 1.7, 29.02, 32.9}
	};

	double target[4]={11.2,31.3,2.0,22.5};
	int indices_matrix[4]={4,6,4,3};



	get_best_descriptor(indices_matrix,test_matrix,target);
	printf("%d\n", indices_matrix[2]);

    return 0;
}





void get_best_descriptor(int *size_matrix, double parent[][size_matrix[1]],double target[])
{
	int parent_i = size_matrix[0];
	int parent_j = size_matrix[1];

	int k=0; //check for all feature colms
	double min_mse=100000; //a very big mse to start with
	double average_descripor=0;
	double se=0;
	int best_descriptor;

	for (int j=0;j<parent_j;j++)
	{
		average_descripor=0;
		for (int i = 0; i < parent_i; ++i)
		{
			/* sum all features */
			average_descripor=average_descripor+parent[i][j];
		}
		//get avrg
		average_descripor=average_descripor/parent_i;
		printf("%f\n", average_descripor);

		for (int i = 0; i < parent_i; ++i)
		{
			/* sum  */
			se=se+pow((average_descripor-target[i]),2);
		}

		if (se<min_mse)
		{
			min_mse=se;
			best_descriptor=j;
			se=0;		/* code */
		}
		else
		{
			se=0;
		}

	}
	size_matrix[2]=best_descriptor;
}

// int get_best_threshold(int *size_matrix, double parent[][size_matrix[1]],double target[])
