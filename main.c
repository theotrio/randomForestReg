
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#include <dirent.h>
#include <time.h>
#include <math.h>


//################# FUNCTIONS OF THE ALGORITHM ##############################

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
        float mean = mean/rows;

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
    printf("%f\n", min_mse );
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
int * createBOF(int cols, int Nfeatures,int target, int seed)
{
    srand(time(NULL));   // Initialization, should only be called once.
    static int bof[210];
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
        r = ((rand()*seed)%3 + (rand()*seed)%13 + (rand()*seed)%19 + (rand()*seed)%71 + (rand()*seed)%203 )% (cols-i);
        //printf("%d  ", r);
        if (r!=target)
        {
            bof[i] = array1[r];
            array1[r] = array1[cols-1-i];
            i++;
        }

    }
    //printf("\n-------------------------------------------------------------------------------------------\n");
    return bof;

}


/**
 * This functions fills a submatrix that is in essence the root of a tree of the random forest. 
 * As input it takes the whole data set which has "bigCols" features and then it selects randomly 
 * "Nfeatures" features to fill the smallMatr.
**/
void fillBof(int rows, int bigCols, int Nfeatures,float **bigMatr, float **smallMatr,int target,int seed)
{
    int *bof = createBOF(bigCols,Nfeatures,target,seed);

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<Nfeatures; j++)
        {
            smallMatr[i][j] = bigMatr[i][bof[j]];
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

    char path[100]="C:\\Users\\theot\\eclipse-workspaceC\\VSC\\Hartree\\Hartree_Data\\";// this path should be the folder of data
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

    FILE *fp; // initialise fp
    char buf[3072]; // this is a buffer that contains the line from fgets
    int k=0;
    int col_num=0;
    int row_num=0;
    int TOTAL_NUM_ROW=0;


    //##############the first opening of all the csvs is to get the sizes of columns and rows############################

    for (int i = 0; i < 1; ++i)//52 are the csvs couldn't make it more generic
    {
        strcat(temp_path,csvs[i]); //copies the file to path, in order to read csv later
        fp = fopen(temp_path, "r");
        //printf("%s\n",temp_path );
        /**if(fp==NULL)
        {
            perror("Did not read");
        }**/
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
        fclose(fp);
    }
    TOTAL_NUM_ROW=TOTAL_NUM_ROW-1;
    printf("Total number of rows is:%d\n",TOTAL_NUM_ROW );
    printf("Total number of columns is:%d\n",col_num );


    //################### main data procedure#################

    char line[col_num][50];       //initialize line, it should have same length as feature names [col_name][50], 50 is just a string size number
    char col_names[col_num][50];  //initialize feature_name list
    float **data = malloc(TOTAL_NUM_ROW * sizeof(float *)); //initialize data 2d array

    for (int i=0; i < TOTAL_NUM_ROW; i++)
        data[i] = malloc((col_num)*sizeof(float));

    float *target_y=malloc(TOTAL_NUM_ROW*sizeof(float));


    // 52 is the number of csvs in the dir
    for (int i = 0; i < 52; ++i)
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
    float ***forest = malloc(num_of_trees * sizeof(float **));
    for (int j = 0; j < num_of_trees; ++j)
    {
        forest[j]=malloc(TOTAL_NUM_ROW * sizeof(float *));
        for (int i=0; i <= TOTAL_NUM_ROW; i++)
            forest[j][i] = malloc((tree_size)*sizeof(float));
    }

    //Normalise the data table
    normalise(data,TOTAL_NUM_ROW,col_num);

    //Copy the target column from data
    get_target( TOTAL_NUM_ROW,target_feature,data,target_y);

    for (int i = 0; i < num_of_trees; ++i)
    {
        fillBof(TOTAL_NUM_ROW,col_num,tree_size,data,forest[i],target_feature,i+3);
    }

    printMatrF(TOTAL_NUM_ROW,tree_size,forest[0]);
    printMatrF(TOTAL_NUM_ROW,tree_size,forest[2]);

    /**
     * For the best_descriptor function a size_matrix[4] should be defined to record the best 
     * descriptor and the threhold. We have such a vector for each tree.
     * --Theo says that we may need to reconsider that...
    **/
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

    //Calculate the best descriptor for each root.
    for(int i=0; i<num_of_trees; i++)
    {
        get_best_descriptor(utilities_matrix[i], forest[i],target_y);
    }

    printMatr(num_of_trees,4,utilities_matrix);

    

}
