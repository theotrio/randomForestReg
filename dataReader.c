#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
/*** Constants ***/
#define BUFFER_SIZE 4096

/*** Structs ***/
typedef struct {
    char feats[300][254];
    float vals[1000][10];
} datastruct;

/*** Functions ***/
datastruct retrieveFileData();

int main() {
    datastruct data = retrieveFileData();
    printf("FEAT 0: %s\n", data.feats[0]);
    printf("VAL 0: %f\n", data.vals[0][0]);

    return 0;
}

datastruct retrieveFileData(){
    printf("Hello World!\n");
    DIR *dir;
    FILE *entry_file;
    struct dirent *input_file_current;
    struct _WDIR *wc_input_file_current; //wide-character struct in DIR which preserves initial directory name
    char dirname_base[256] = "D:/Work/PhD/HartreeTraining/Hartree Assignment/data/"; //note: replace data directory here
    char dirname_current[256]; 
    char temp_dirname_base[256]; //refreshes the concatenated path every iter.
    char line[BUFFER_SIZE]; //buffer to hold each file row read
    char features[300][254]; //[300] [256]
    float values[1000][5]; // [100000][300] - Initialzed with -1 || WARNING: Rows number should be swapped to 100.000 (was kept at 3k due to machine mem. limitations)
    datastruct data;
    int i, j = 0;

    /* Innit 2D matrix */
    int columnslen=sizeof(values[0])/sizeof(values[0][0]);
    int rowslen=sizeof(values)/sizeof(values[0]);
    printf("Columnslen: %d, Rowslen: %d\n", columnslen, rowslen);
    for (i = 0; i < rowslen; i++){
        for (j = 0; j < columnslen; j++){
            values[i][j] = -1;
        }
    }
    
    /* Get file path */
    dir = opendir(dirname_base);
    if(dir==NULL){
        printf("Error! Unable to read directory");
        exit(1);
    }

    while( (input_file_current=readdir(dir)) != NULL) {
        /*
        These comparisons avoid current and parent directories

        input_file_current->d_name is an expression that means:
            take address of "input_file_current", add the offset of "d_name" to it (the byte position of d_name in the allocated memory so far)
            and, in this case, compare it to ".". As d_name belongs in the
            "inpput_file_current" struct, this is essencially saying to find that struct and shit
            as many bytes as the poistion of "d_name" inside it, which will obviously give us the value in the memory adress of "d_name"
        */
        if (!strcmp (input_file_current->d_name, ".")) //d_name is a char[256] contained in a "dirent" type struct
            continue;
        if (!strcmp (input_file_current->d_name, ".."))
            continue;
        strcpy(temp_dirname_base, dirname_base); //refresh due to the cat
        strcpy(dirname_current ,strcat(temp_dirname_base, input_file_current->d_name));

        //After getting full path, open file
        entry_file = fopen(dirname_current, "r");
        //entry_file = fopen("D:/Work/PhD/HartreeTraining/Hartree Assignment/data/Apr_borough_grocery.csv", "r");
        if(entry_file == NULL) {
            perror("File opening failed");
            fclose(entry_file);
            exit(1);
        }
        printf("Currently opened file: %s\n", dirname_current);

        /* Store data */
        int field_count = 0;
        int row_count = 0;
        char *field;
        while (fgets(line, sizeof(line), entry_file)) {
            //printf("Here,  File: %s\n", dirname_current);
            field_count = 0;
            row_count++;
            
            /* walk through and store row values */
            for (field = strtok(line, ","); field && *field; field = strtok(NULL, ",")){
                //field = strtok(NULL, ",");
                field_count++;
                if(field_count == 1){
                    /*Ignore ID clumn */
                    continue;
                }
                if(row_count == 1){
                    /*Save feature names*/
                    strcpy(features[field_count-2], field);
                    //printf("File: %s,  Row: %d, Column: %d, Field: %s\n", input_file_current->d_name, row_count, field_count, field);
                }
                else{
                    /*Save features values in 2D matrix */
                    values[row_count-2][field_count-2] = (float)strtod(field, NULL);  
                    //printf("File: %s,  Row: %d, Column: %d,  Value: %f\n", input_file_current->d_name, row_count, field_count, values[row_count-2][field_count-2]);
                }
                
                
            }
     
        }

        fclose(entry_file);
    }

    closedir(dir);   

    /* Check features*/
    /*
    i=0;
    while (strcmp(features[i], "")) { //returns zero if they match (meaning it's unexistent)
        printf("%d, %s\n", i, features[i]);
        i++;
    }
    printf("retrieveFileData() fully executed!\n");
    /*

    /* Check values */
    //int rowlen=sizeof(features[0])/sizeof(features[0][0]);
    /*
    j=0;
    i=0;
    while(values[i][0] != -1){ //check populated rows
        j = 0;
        while (values[i][j] != -1){ //check pupulated fields 
            printf("Row: %d, Column: %d,  Value: %f\n", i+2, j+2, values[0][0]);
            j++;
        }
        i++;
    }
    */

    /* Copy features to struct to be returned */
    i=0;
    while (strcmp(features[i], "")) { //returns zero if they match (meaning it's unexistent)
        strcpy(data.feats[i], features[i]);
        printf("%d, %s\n", i, data.feats[i]);
        i++;
    }

    /* Copy values to struct to be returned */
    j=0;
    i=0;
    while(values[i][0] != -1  &&  i < rowslen){ //check populated rows
        j = 0;
        while (values[i][j] != -1  &&  j < columnslen){ //check pupulated fields 
            data.vals[i][j] = values[i][j];
            //printf("Row: %d, Column: %d,  Value: %f\n", i+2, j+2, data.vals[i][j]);
            j++;
        }
        i++;
    }

    return data;
}

   
