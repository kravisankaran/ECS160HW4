#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nameIndex;
char tweeterNames[20000][50];
int freq[20000];

char* getfield(char* line, int num)
{
    char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        //printf(tok);
        if (!--num)
            return tok;
    }
    return NULL;
}

int findNameField(char* line)
{
    char* tok;
    int fieldNumber;
    fieldNumber = 1;
    
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        //printf(tok);
        if (strcmp(tok,"\"name\"") == 0)
            return fieldNumber;
        fieldNumber++;
    }
    return -1;
}

// Read input file and process tweeter names into an array of strings
int readFile(char *fileName) {

    FILE* stream = fopen(fileName, "r");    

    char line[10000];
    int nameFieldNumber = -1;
    
    if (fgets(line, 10000, stream)) 
    {
        char* tmp = strdup(line);
        nameFieldNumber = findNameField(tmp);
    }  
    
    // if "name" header is not found, input file is incorrect
    if (nameFieldNumber == -1) 
        return 0;
    
    nameIndex = 0;
    
    while (fgets(line, 10000, stream))
    {
        char* tmp = strdup(line);
        char* tweeterName = getfield(tmp, nameFieldNumber);

        tweeterName++;
        tweeterName[strlen(tweeterName)-1] = 0;
        
        strcpy(tweeterNames[nameIndex],tweeterName);
        nameIndex++;

        free(tmp);
    }

    // non-zero value indicates valid input file
    return 1;
}

void findCounts() 
{
    int count, i, j;
    
    for(i=0; i<nameIndex; i++)
    {
        count = 0;
        for(j=0; j<nameIndex; j++)
        {
            // if duplicate name is found
            if(strcmp(tweeterNames[i],tweeterNames[j]) == 0)
            {
                count++;
            }
        }

        freq[i] = count;
    }
}

void sortArrays() 
{
    int i, j, a, n;
    char tmp[50];
 
    for (i = 0; i < nameIndex; i++) 
    {
        for (j = i + 1; j < nameIndex; j++)
        {
            if (freq[i] < freq[j]) 
            {
                a =  freq[i];
                strcpy(tmp, tweeterNames[i]);
                
                freq[i] = freq[j];
                strcpy(tweeterNames[i], tweeterNames[j]);
                
                freq[j] = a;
                strcpy(tweeterNames[j], tmp);
            } 
        } 
    }
}

void printOutput() 
{
    char UniqueTweeterNames[20000][50];
    int UniqueFreq[20000];
    int i, j, uniquesCount=1, flag=0;
    
    strcpy(UniqueTweeterNames[0],tweeterNames[0]);
    UniqueFreq[0] = freq[0];
    
    for(i=1; i<nameIndex; i++)
    {
            for(j=0;j<=uniquesCount;j++)
            {
                if(strcmp(tweeterNames[i], UniqueTweeterNames[j]) == 0) 
                {
                    flag = 1;
                }
            }
            
            if (flag == 0)
            {
                strcpy(UniqueTweeterNames[uniquesCount],tweeterNames[i]);
                UniqueFreq[uniquesCount] = freq[i];
                uniquesCount++;
            }
            
            flag = 0;
    }
    
    printf("%20s: Count\n", "Tweeter Name");
    printf("       ---------------------\n");
    for(i=0; i<10; i++)
    {
            printf("%20s : %d \n", UniqueTweeterNames[i], UniqueFreq[i]);
    }
    
}

int main(int argc, char *argv[])
{
    //int i = 0;
        
    // Only one command line argument, that is, csv file path must be received
    // if argument count is more than 2, stop processing further
    if (argc > 2) 
    {
        printf("Enter one argument (.csv file name) only");
        return 0;
    }
   
    // read csv file and process it  
    if (readFile(argv[1]) == 0)
    {
        printf("Invalid Input Format: Please enter a valid .csv file\n");
        return 0;
    }
        
    // printf("There are %d names\n",nameIndex);
    //for (i=0; i <= nameIndex;i++) 
    //{
    //    printf("%s\n", tweeterNames[i]);
    //} 

    // find count of each tweeter name
    findCounts();
    
    // sort arrays
    sortArrays();
    
    //for(i=0; i<nameIndex; i++)
    //{
    //        printf("%s occurs %d times\n", tweeterNames[i], freq[i]);
    //}
    //printf("\n\n");
    
    // print only unique tweeter names
    printOutput();
    
}

