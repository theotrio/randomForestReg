#include <stdio.h>
#include <math.h>
#define FEATURE_COL 6
#define FEATURE_ROW 4


void get_best_threshold(int *size_matrix, double parent[][size_matrix[1]],double target[]);

int main() {

	double test_matrix[4][6] = {
	    {10.0, 11.6, 12.4, 13.9, 10.4, 84.1},
	    {4.01, 0.6, 2.4, 3.9, 0.4, 4.1},
	    {1.0, 1.6, 1.4, 1.9, 1.4, 8.1},
	    {14.9, 15.8, 16.6, 1.7, 29.02, 32.9}
	};

	double target[4]={1.2,1.3,2.0,2.5};
	int indices_matrix[4]={4,6,5,3};

	get_best_threshold(indices_matrix,test_matrix,target);

	printf("%d\n", indices_matrix[3]);

    return 0;
}

void get_best_threshold(int *size_matrix, double parent[][size_matrix[1]],double target[])
{
	int parent_i = size_matrix[0];
	int parent_j = size_matrix[1];
	int best_descriptor= size_matrix[2];
	int k=0; //slides across feature col
	int count=0;//slides until k to compute avrg
	double avg=0;
	double se=0;
	double min_mse=10000;


	for (k=0;k<parent_i;k++)
	{
		avg=0;
		for (count=0;count<k+1;count++)
		{
			avg=avg+parent[count][best_descriptor];
		}
		avg=avg/(k+1);

		for (int i = 0; i < parent_i; ++i)
		{
			/* code */
			se=se+pow((avg-target[i]),2);
		}

		if (se<min_mse)
		{
			min_mse=se;
			size_matrix[3]=k;
			se=0;
		}
		else
		{
			se=0;
		}

	}

}
