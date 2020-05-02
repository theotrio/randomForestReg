#include <stdio.h>
#include <math.h>
#define FEATURE_COL 6
#define FEATURE_ROW 4

void split(int *size_matrix,double child1[][size_matrix[3]], double child2[][size_matrix[5]], double parent[][size_matrix[1]]);

int get_best_descriptor(int *size_matrix, double parent[][size_matrix[1]],double target[]);

int main() {

	double test_matrix[4][6] = {
	    {10.0, 11.6, 12.4, 13.9, 10.4, 84.1},
	    {4.01, 0.6, 2.4, 3.9, 0.4, 4.1},
	    {1.0, 1.6, 1.4, 1.9, 1.4, 8.1},
	    {14.9, 15.8, 16.6, 1.7, 29.02, 32.9}
	};

	double target[4]={11.2,31.3,2.0,22.5};
	int indices_matrix[4]={4,6,4,3};

	double child1[indices_matrix[0]][indices_matrix[3]];
	double child2[indices_matrix[0]][indices_matrix[1]-indices_matrix[3]];



	split(indices_matrix,child1,child2,test_matrix);

	for (int i = 0; i < indices_matrix[0]; ++i)
	{
		/* code */
		for (int j = 0; j < indices_matrix[3]; ++j)
		{
			/* code */
			printf("%f ",child2[i][j]);

		}
		printf("\n");
	}


    return 0;
}



void split(int *size_matrix,double child1[][size_matrix[3]], double child2[][size_matrix[1]-size_matrix[3]], double parent[][size_matrix[1]])
{
	int parent_i = size_matrix[0];
	int parent_j = size_matrix[1];

	int child1_i = size_matrix[0];
	int child1_j = size_matrix[3];

	int child2_i = size_matrix[0];
	int child2_j = size_matrix[1]-size_matrix[3];

	int c1i=0;
	int c1j=0;
	int c2i=0;
	int c2j=0;

	for (int i=0;i<parent_i;i++)
	{
		c1j=0;
		c2j=0;
		for (int j=0;j<parent_j;j++)
		{
			if (j<child1_j)
			{
				child1[c1i][c1j]=parent[i][j];
				c1j++;
			}
			else
			{
				child2[c2i][c2j]=parent[i][j];
				c2j++;
			}
		}
		c1i++;
		c2i++;
	}

}



