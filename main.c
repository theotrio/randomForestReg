Skip to content
Search or jump to…

Pull requests
Issues
Marketplace
Explore
 
@theotrio 
Learn Git and GitHub without any code!
Using the Hello World guide, you’ll start a branch, write comments, and open a pull request.


kostas1515
/
randomForestReg
1
00
 Code
 Issues 0
 Pull requests 0 Actions
 Projects 0
 Wiki
 Security 0
 Insights
randomForestReg/main.c
@kostas1515 kostas1515 split rewritten, binary tree, count non-zero,fix termination condition
eb4fecd 14 hours ago
@kostas1515@theotrio
665 lines (554 sloc)  18.9 KB
  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#include <dirent.h>
#include <time.h>
#include <math.h>
#define FILE_NUMBER 2 //default 52



//################# FUNCTIONS OF THE ALGORITHM ##############################



void split(int *size_matrix,float **child1, float **child2, float **parent)
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

void get_best_threshold(int *size_matrix, float **parent,float *target)
{
    /*
    updates utilities_matrix last position, which is the row index of the best threshold where the split must happen.
    the two submatrices are [0..,k] , [k+1,..,row_number] 
    */

    int parent_i = size_matrix[0];
    int parent_j = size_matrix[1];
    int best_descriptor= size_matrix[2];
    int k=0; //slides across feature col
    int count=0;//slides until k to compute avrg
    double avg=0;
    double se=0;
    double min_mse=10000;


    for (k=0;k<parent_i-1;k++)
    {
        avg=0;
        //calculate mean squere error
        for (count=0;count<k+1;count++)
        {
            avg=avg+parent[count][best_descriptor];
        }
        avg=avg/(k+1);

        for (int i = 0; i < parent_i; ++i)
        {
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

/**
 * This function normalises the elements of a matirx column-wise 
**/
void normalise(float **matrix, int rows, int cols)
{
    for(int j=0; j<cols; ++j)
    {
        float mean =0;
        for(int i=0; i<rows; ++i)
        {
            mean += matrix[i][j];
        }
        mean = mean/rows;

        float sdev=0;
        for(int i=0; i<rows; i++)
        {
            sdev += pow((matrix[i][j]-mean),2);
        }
        sdev = sqrt(sdev/(rows-1));

        if(sdev==0)
        {
            //printf("%f",mean);
            sdev=1; 
        }

        for(int i=0; i<rows; i++)
        {
            matrix[i][j]=(matrix[i][j]-mean)/sdev;
        }
    }
}


/**
 * This function calculates which feature has the best mse, to decide the next split as a
 * decision tree is created and a also decides a proper threshold.
 * This functions works along with a vector of integers called size_matrix which is defined as
 * size_matrix[4] = [num_row_of_the parent,num_col_of_the_parent,best_feature,best_threshold]
 * This functions stores in matrix[2] the best feature (number of column) and matrix[3] the
 * best threshold.
**/
void get_best_descriptor(int *size_matrix, float **parent,float *target)
{

    int parent_i = size_matrix[0];
    int parent_j = size_matrix[1];

    int k=0; //check for all feature colms
    double min_mse=100000000.0; //a very big mse to start with
    double average_descripor=0;
    double se=0;
    int best_descriptor;

    for (int j =0;j<parent_j;j++)
    {
        //calculate mean squere error
        average_descripor=0;
        for (int i = 0; i < parent_i; ++i)
        {
            /* sum all features */
            average_descripor=average_descripor+parent[i][j];
        }
        //get avrg
        average_descripor=average_descripor/parent_i;
        for (int i = 0; i < parent_i; ++i)
        {
            /* sum  */
            se=se+pow((average_descripor-target[i]),2);
        }
        se=se/parent_i;
        
        if (se<min_mse)
        {
            min_mse=se;
            best_descriptor=j;
            //printf("%d\n",j );
            se=0;       /* code */
        }
        else
        {
            se=0;
        }

    }
    // printf("%f\n", min_mse );
    size_matrix[2]=best_descriptor;
}


/**
 * Create Bag Of Features
 * This is a function to select randomly features for each tree of the Random Forest.
 * This is a functions that takes as an input the number of features in total, 
 * the number of features that the algorithm selects randomly to create the root of each tree
 * the number of column that is the target (the feture that we are trying to estimate in the regression)
 * this is an input of this function to ensure that the target is not selected in the random selection.
 * and an integer number called seed that creates some noise and helps in the random selection.
 * Returns an array with randomly selected numbers from 0 to cols. The 
 * returned array has size Nfeatures.
**/
void createBOF(int cols, int Nfeatures,int target, int seed, int *bof)
{
    srand(time(NULL));   // Initialization, should only be called once.
    int r = 0;


    int array1[cols];

    for(int i=0; i<cols; i++)
    {
        array1[i] = i; 
    }

    int i=0;
    if(seed==0)
    {
        seed = rand()%5;
    }

    while(i<Nfeatures)
    {
        r = ((rand()%11)*seed + (rand()%13)*seed + (rand()%109)*seed + (rand()%71)*seed + (rand()%203)*seed ) % (cols-i);
        if (r!=target)
        {
            bof[i] = array1[r];
            array1[r] = array1[cols-1-i];
            i++;
        }

    }
    //printf("\n-------------------------------------------------------------------------------------------\n");

}


/**
 * This functions fills a submatrix that is in essence the root of a tree of the random forest. 
 * As input it takes the whole data set which has "bigCols" features and then it selects randomly 
 * "Nfeatures" features to fill the smallMatr.
**/
void fillBof(int rows, int bigCols, int Nfeatures,float **bigMatr, float **smallMatr,int target,int seed)
{
    int bofAr[Nfeatures];
    createBOF(bigCols,Nfeatures,target,seed,bofAr);

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<Nfeatures; j++)
        {
            smallMatr[i][j] = bigMatr[i][bofAr[j]];
        }
    } 
}

/**
 * This function copies the target feature from the column col_name of matrix data 
 * to an array y that has length length (the first argument)
**/
void get_target(int length,int col_name,float **data,float *y)
{
    for(int i=0; i<length; i++)
    {
        y[i]=data[i][col_name];
    }
}

// A function to print a matrix with integers
void printMatr(int rows, int cols, int **Matr)
{
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            printf("%d  ", Matr[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------------------------------------------\n");
}

// A function to print a matrix with floats
void printMatrF(int rows, int cols, float **Matr)
{
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            printf("%f  ", Matr[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------------------------------------------------\n");
}

// A function to print an array with floats
void printAr1(float *myArr, int length)
{
    for(int i=0; i<length; i++)
    {
        printf("%f\n", myArr[i]);
    }
}


void getrow(char* line,char feature_names[][50])
{
    //breaks the line into tokens and returns the feature names
    const char s[2] = ",";
    char *token;

    /* get the first token */
    token = strtok(line, s);
    int i=0;

    /* walk through other tokens */
    while( token != NULL ) {
      strcpy(feature_names[i], token);
      i=i+1;
      token = strtok(NULL, s);
    }
}

void fix_id(char *array)
{
//removes the E from area_id and stores it to data as float
    for (int i = 0; i < NELEMS(array); ++i)
    {
        /* code */
        array[i]=array[i+1];
    }
    for (int i = NELEMS(array)-1; i >0; --i)
    {
        /* code */
        array[i]=array[i-1];
    }
}

int num_col(char* line)
{
    //counts the number of columns aka feature names
    const char s[2] = ",";
    char *token;

    /* get the first token */
    token = strtok(line, s);
    int i=0;

    /* walk through other tokens */
    while( token != NULL ) {
      i=i+1;
      token = strtok(NULL, s);
    }
    return i;
}

int EndsWithCsv( char *string )
{
    //returns 0 if file ends with csv
  string = strrchr(string, '.');

  if( string != NULL )
  {
    return( strcmp(string, ".csv") );
  }

  return( -1 );
}


int main(void) 
{
    //############################### READING THE DATA FROM CSV FILES ###########################################

    char path[100]="/home/konsa/Downloads/hartree_data/sample/";// this path should be the folder of data
    char temp_path[100];// this is just a helper path
    strcpy(temp_path,path);

    struct dirent *de;  // Pointer for directory entry 
    char csvs[52][50]; //this is a list of the csvs
    int csv=0; //counter for csv
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(path); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    while ((de = readdir(dr)) != NULL)
    {
        //printf("%s\n",de->d_name );
        if (EndsWithCsv(de->d_name)==0)
        {
            strcpy(csvs[csv], de->d_name);
            csv=csv+1;
        }
        
    }
    // FILE *fp = fopen(de->d_name, "r");
    closedir(dr);

    FILE *fp = fopen(temp_path, "r"); // FOR LINUX
    // FILE *fp; //for WINDOWS 
    char buf[3072]; // this is a buffer that contains the line from fgets
    int k=0;
    int col_num=0;
    int row_num=0;
    int TOTAL_NUM_ROW=0;


    //##############the first opening of all the csvs is to get the sizes of columns and rows############################


    for (int i = 0; i < FILE_NUMBER; ++i)//52 are the csvs couldn't make it more generic
    {
        strcat(temp_path,csvs[i]); //copies the file to path, in order to read csv later
        fp = freopen(temp_path, "r",fp);
        // fp=fopen(temp_path,"r");
        //printf("%s\n",temp_path );
        strcpy(temp_path,path);
        while (fgets(buf, 3072, fp))
        {
            if (row_num==0)
            {
                char* tmp = strdup(buf);
                col_num=num_col(tmp);
            }
            row_num++;
        }


        TOTAL_NUM_ROW=TOTAL_NUM_ROW+row_num;
        row_num=0;
        //fclose(fp);
        
    }
    TOTAL_NUM_ROW=TOTAL_NUM_ROW-1;
    printf("Total number of rows is:%d\n",TOTAL_NUM_ROW );
    printf("Total number of columns is:%d\n",col_num );


    //################### main data procedure#################

    char line[col_num][50];       //initialize line, it should have same length as feature names [col_name][50], 50 is just a string size number
    char col_names[col_num][50];  
    //initialize feature_name list
    float **data = malloc(TOTAL_NUM_ROW * sizeof(float *)); //initialize data 2d array
    for (int i=0; i < TOTAL_NUM_ROW; i++)
        data[i] = malloc((col_num)*sizeof(float));

    float *target_y=malloc(TOTAL_NUM_ROW*sizeof(float));


    // 52 is the number of csvs in the dir
    for (int i = 0; i < FILE_NUMBER; ++i)
    {
        /* itearate through csvs */
        strcat(temp_path,csvs[i]); //add file to path
        freopen(temp_path, "r",fp);
        strcpy(temp_path,path); //restore absolute path

        while (fgets(buf, 3072, fp))
        {
            char* tmp = strdup(buf);
            getrow(tmp,line);//puts temp to line
            if (k==0) //first row is col_names
            {
                for (int c = 0; c < NELEMS(line); ++c)
                {
                    strcpy( col_names[c], line[c]); //just copies the first line to col_names
                }
            }
            else
            {
                for (int c = 0; c < NELEMS(line); ++c)
                {
                    if (c==0)
                    {
                        fix_id(line[c]); //fixes the first column, aka area_id, makes it numerical string without the E
                        
                    }
                    data[k-1][c]=atof(line[c]);// starts from k-1, because for k==0, we have the first row which is the feature names
                }
            }
            k=k+1;
        }  
    }
    fclose(fp);
 //printMatr(TOTAL_NUM_ROW,col_num,data);

 //############################### CREATING THE RANDOM FOREST ###########################################

    //########################     Some Initialisation    ###############################################
    int target_feature = 193; //Target feautr is the population in 193th col
    int tree_size=round(sqrt(col_num)); // The number of feature that the root of each tree gets
    int num_of_trees=10;  //The number of trees

    // Memory allocation for the forest. Each tree is a 2D matrix so the forest is a 3D matrix.
    /*
    float ***forest = malloc(num_of_trees * sizeof(float **));
    for (int j = 0; j < num_of_trees; ++j)
    {
        forest[j]=malloc(TOTAL_NUM_ROW * sizeof(float *));
        for (int i=0; i <= TOTAL_NUM_ROW; i++)
            forest[j][i] = malloc((tree_size)*sizeof(float));
    }
    */
    int depth = 3;
    int no_Of_nodes = pow(2,depth)-1;
    printf("The number of nodes is %d\n",no_Of_nodes);

    /**
     * Memory allocation for the forest.
     * Each node is a 2D matrix thus each tree is a 3D matrix and the forest is a 4D matrix.
     **/
    float ****forest = malloc(num_of_trees * sizeof(float ***));
    for (int f = 0; f < num_of_trees; ++f)
    {
        forest[f]=malloc(no_Of_nodes * sizeof(float **));
        for (int n=0; n < no_Of_nodes; n++)
        {
            forest[f][n] = malloc(TOTAL_NUM_ROW * sizeof(float *));
            for(int i=0; i<TOTAL_NUM_ROW; i++)
            {
                forest[f][n][i] = malloc((tree_size)*sizeof(float));
            }
        }
    }

    //Normalise the data table
    normalise(data,TOTAL_NUM_ROW,col_num);

    //Copy the target column from data
    get_target( TOTAL_NUM_ROW,target_feature,data,target_y);

    //fill the ROOOOOTTT of the tree, it will be the middle index of the node array
    int root=(no_Of_nodes/2);
    for (int i = 0; i < num_of_trees; ++i)
    {   
        fillBof(TOTAL_NUM_ROW,col_num,tree_size,data,forest[i][root],target_feature,i+3);
    }



    int **utilities_matrix = malloc(num_of_trees * sizeof(int *));
    for (int i=0; i < num_of_trees; i++)
    {
            utilities_matrix[i] = malloc(4 * sizeof(int));
    }

    //Initialise the above matrix.
    for(int i=0; i<num_of_trees; i++)
    {
        utilities_matrix[i][0]=TOTAL_NUM_ROW;
        utilities_matrix[i][1]=tree_size;
        utilities_matrix[i][2]=0;
        utilities_matrix[i][3]=0;
    }
    // feat_thres is a 3-d matrix [num_of_tress][num_of_nodes][best_feature,float threshold]
    float ***feat_thres = malloc(num_of_trees * sizeof(float **));
    for (int j = 0; j < num_of_trees; ++j)
    {
        feat_thres[j]=malloc(no_Of_nodes * sizeof(float *));
        for (int i=0; i < no_Of_nodes; i++)
            feat_thres[j][i] = malloc((2)*sizeof(float));
    }

    int current_node = 0;
    int nodes_per_level[depth];
    nodes_per_level[0]=1;
    int next_node_pos[pow(2,depth-1)];
    int next_left_child=0;
    int next_right_child=0;
    int number_non_zero=0;
    int left_flag=1;
    int right_flag=1;

    //initialise nodes_per_level
    for(int i=1; i<depth; i++)
    {
        nodes_per_level[i]=0;
    }

    for (int fi = 0; fi < num_of_trees; ++fi)
    {
        /* forest iteration */
        for (int ki = 0; ki < depth; ++ki)
        {
            current_node=root/pow(2,ki);

            for (int kj = 0; kj < nodes_per_level[ki]; ++kj)
            {
                get_best_descriptor(utilities_matrix[fi], forest[fi][current_node],target_y);
                get_best_threshold(utilities_matrix[fi], forest[fi][current_node],target_y);
                feat_thres[fi][current_node][0]=utilities_matrix[fi][2];
                feat_thres[fi][current_node][1]=utilities_matrix[fi][3];

                if(ki<depth-1)
                {
                    next_left_child=current_node-pow(2,depth-ki-2);//next level left
                    next_node_pos[nodes_per_level[ki+1]] = next_left_child;
                    nodes_per_level[ki+1]++;

                    next_right_child=next_left_child+pow(2,ki-1); //next level right
                    next_node_pos[nodes_per_level[ki+1]] = next_right_child;
                    nodes_per_level[ki+1]++;

                    printf("new\n");
                    
                    split(utilities_matrix[fi],forest[fi][next_left_child], forest[fi][next_right_child], forest[fi][current_node]);
                    // count the number of non-zero elements
                    // if the count <2 then don't do the split
                    // create two flags for left and right, if 1 do not do the split
                    for (int nel = 0; nel < NELEMS(forest[fi][next_left_child][0]); ++nel)
                    {
                        if (forest[fi][next_left_child][0][nel]!=0)
                        {
                            number_non_zero++;
                        }
                    }
                    if(number_non_zero<2)
                    {
                        left_flag=1;
                    }
                    number_non_zero=0;

                    for (int nel = 0; nel < NELEMS(forest[fi][next_right_child][0]); ++nel)
                    {
                        if (forest[fi][next_right_child][0][nel]!=0)
                        {
                            number_non_zero++;
                        }
                    }
                    if(number_non_zero<2)
                    {
                        right_flag=1;
                    }
                    number_non_zero=0;
                }
                current_node=next_node_pos[kj];

            }
            
        }
    }


    // printMatrF(TOTAL_NUM_ROW,tree_size,forest[0][root]);

    // printMatrF(TOTAL_NUM_ROW,tree_size,forest[2]);

    /**
     * For the best_descriptor function a size_matrix[4] should be defined to record the best 
     * descriptor and the threhold. We have such a vector for each tree.
     * --Theo says that we may need to reconsider that...
    **/
    // MAIN PROGRAM LOOP


    //Calculate the best descriptor for each root.


    

}

