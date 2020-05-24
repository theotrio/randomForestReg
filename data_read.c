#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#include <dirent.h> 

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
    return( strcmp(string, ".csv") );

  return( -1 );
}

int main(void) {

    char path[100]="../../Downloads/hartree_data/";// this path should be the folder of data
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
        // printf("%s\n",de->d_name );
        if (EndsWithCsv(de->d_name)==0)
        {
            strcpy(csvs[csv], de->d_name);
            csv=csv+1;
        }
        
    }
        // FILE *fp = fopen(de->d_name, "r");
    closedir(dr);

    FILE *fp = fopen(temp_path, "r"); //dummy opening just to instatiate fp
    printf("%s\n",temp_path );
    strcpy(temp_path,path);
    char buf[3072]; // this is a buffer that contains the line from fgets
    int k=0;
    int col_num=0;
    int row_num=0;
    int TOTAL_NUM_ROW=0;



    //##############the first opening of all the csvs is to get the sizes of columns and rows############################

    for (int i = 0; i < 52; ++i)//52 are the csvs couldn't make it more generic
    {
        /* code */
        strcat(temp_path,csvs[i]); //copies the file to path, in order to read csv later
        freopen(temp_path, "r",fp);
        strcpy(temp_path,path);
        while (fgets(buf, 3072, fp))
        {
            if (row_num==0)
            {
                /* code */
                char* tmp = strdup(buf);
                col_num=num_col(tmp);

            }
            row_num++;
        }


        TOTAL_NUM_ROW=TOTAL_NUM_ROW+row_num;
        row_num=0;
    }
    TOTAL_NUM_ROW=TOTAL_NUM_ROW-1;
    printf("Total number of rows is:%d\n",TOTAL_NUM_ROW );
    printf("Total number of columns is:%d\n",col_num );


    //################### main data procedure#################
    char line[col_num][50];       //initialize line, it should have same length as feature names [col_name][50], 50 is just a string size number
    char col_names[col_num][50];  //initialize feature_name list
    float **data = malloc(TOTAL_NUM_ROW * sizeof(float *)); //initialize data 2d array
    for (int i=0; i <= TOTAL_NUM_ROW; i++)
        data[i] = malloc((col_num-1)*sizeof(float));



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
//###########################PRINTS###############################################################
    for (int i = 0; i < col_num; ++i)
    {
        /* code */
        printf("%s\n",col_names[i] );
    }
    for (int i = 0; i < TOTAL_NUM_ROW; ++i)
    {
        /* code */
        printf("%f\n",data[i][0] );
    }
    return 0;
}