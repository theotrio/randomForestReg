#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define FEATURE_COL 6
#define FEATURE_ROW 4
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


void split(int *size_matrix,float **child1, float **child2, float parent[][6])
{
	int parent_i = size_matrix[0];//4
	int parent_j = size_matrix[1];//6

	int child1_i = size_matrix[3]+1;//3
	int child1_j = size_matrix[1];//6

	int child2_i = size_matrix[0]-size_matrix[3]-1;//1
	int child2_j = size_matrix[1];//6

	int c1i=0;
	int c1j=0;
	int c2i=0;
	int c2j=0;
	printf("complete\n");



	for (int j=0;j<parent_j;j++)
	{
		c1i=0;
		c2i=0;
		for (int i=0;i<parent_i;i++)
		{
			if (i<child1_i)//true for {0,1,2}
			{
				child1[c1i][c1j]=parent[i][j];
				c1i++;
			}
			else
			{
				child2[c2i][c2j]=parent[i][j];
				c2i++;
			}
		}
		c1j++;
		c2j++;
	}

}


int main() {
	
	float test_matrix[4][6] = {
	    {10.0, 11.6, 12.4, 13.9, 10.4, 84.1},
	    {4.01, 0.6, 2.4, 3.9, 0.4, 4.1},
	    {1.0, 1.6, 1.4, 1.9, 1.4, 8.1},
	    {14.9, 15.8, 16.6, 1.7, 29.02, 32.9}
	};

	double target[4]={11.2,31.3,2.0,22.5};
	int indices_matrix[4]={4,6,2,2};

    float **child1 = malloc((indices_matrix[3]+1) * sizeof(float *)); //initialize data 2d array
    for (int i=0; i < indices_matrix[3]+1; i++)
        child1[i] = malloc((indices_matrix[1])*sizeof(float));


    float **child2 = malloc((indices_matrix[0]-indices_matrix[3]-1) * sizeof(float *)); //initialize data 2d array
    for (int i=0; i < indices_matrix[0]-indices_matrix[3]-1; i++)
        child2[i] = malloc((indices_matrix[1])*sizeof(float));


    printf("hello\n");
	split(indices_matrix,child1,child2,test_matrix);


		/* code */
	for (int i = 0; i < indices_matrix[3]+1; ++i)
	{
			/* code */
		for (int j = 0; j < indices_matrix[1]; ++j)
		{
			printf("%f ",child1[i][j]);

		}
		printf("\n");
	}


    return 0;
}







