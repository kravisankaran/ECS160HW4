#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nameIndex;
char tweeterNames[20000][50];
int freq[20000];
int singleColFlag;

// Takes a line / record from the csv file and field number for 
// tweeter name and returns the tweeter name
// if tweeter name is not found, returns NULL
char* getNameField(char* line, int num)
{
    //printf("entered get Name Field\n");
    char* token;
    for (token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n"))
    {
        if (!--num)
            return token;
    }
    return NULL;
}

// This function parses the first line of the given file and finds
// the number of the name field, if present
// If it is unable to find the name field, it returns a -1
// indicating an invalid input file format
int findNameField(char* line)
{
    char* token;
    int fieldNumber;
    fieldNumber = 1;

    //printf("%s\n", line);
    
    for (token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n"))
    {
        if (strcmp(token,"\"name\"") == 0)
            return fieldNumber;
        fieldNumber++;
    }
    
    
    // check for single column that is name scenario
    token = strtok(line,"\n");
    if (strcmp(token,"\"name\"") == 0)
    {
        singleColFlag = 1;
        return 1;
    }    
    return -1;
}

// Reads the input csv file and processes tweeter names 
// into an array of tweeter name strings
int readFile(char *fileName) {

    char* tmp;
    
    FILE* file = fopen(fileName, "r");    

    char line[10000];
    int nameFieldNumber = -1;
    int ch;
    
    // checks for a completely empty file
    if ( (ch=fgetc(file)) == EOF)
        return 0;
    
    // check for un-wanted special characters
    //if ((ch == '%') || (ch == '?'))
    //    return 0;
    
    // go back to beginning of file
    fseek(file, 0, SEEK_SET);
    
    // handle empty lines
    while (fgets(line, 10000, file)) 
    {
        tmp = strdup(line);
        if (strcmp(tmp,"\n") != 0)
            break;
    }
    
    // process the header line to find input file validity
    nameFieldNumber = findNameField(tmp);
    
    //printf("%d\n",nameFieldNumber);
    
    // if "name" header is not found, input file is incorrect
    if (nameFieldNumber == -1) 
        return 0;
    
    nameIndex = 0;
    
    while (fgets(line, 10000, file))
    {
        char* tmp = strdup(line);
        char* tweeterName = getNameField(tmp, nameFieldNumber);

        //printf("tweeter name = %s\n", tweeterName);
        
        if (tweeterName != NULL) 
        {
            // stripping off the starting and ending double quotes, if present
            if (tweeterName[0] == '\"')
                tweeterName++;
            if (tweeterName[strlen(tweeterName)-1] == '\"') 
                tweeterName[strlen(tweeterName)-1] = 0;
            
            if (singleColFlag == 1)
                tweeterName[strlen(tweeterName)-2] = 0;
            
            strcpy(tweeterNames[nameIndex],tweeterName);
            nameIndex++;
        }

        free(tmp);
    }


    //printf("exiting read file\n");
    
    // non-zero value indicates valid input file
    return 1;
}

// This function processes the Tweeter Names array to 
// aggregate the counts / frequencies of each tweeter 
// name into another frequencies array 
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

// This function sorts both arrays for Tweeter names and
// frequencies in descending order of frequencies
// while retaining duplicates
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

// This function weeds out the duplicates into Uniques arrays
// for both the Tweeter Names and their frequencies and
// discards duplicates
// Finally it formats and prints the output
void printOutput() 
{
    char UniqueTweeterNames[20000][50];
    int UniqueFreq[20000];
    int i, j, loopIndex, uniquesCount=1, flag=0;
    
    strcpy(UniqueTweeterNames[0],tweeterNames[0]);
    UniqueFreq[0] = freq[0];
    
    // discarding duplicates
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
    
    // formatting and printing the output
    printf("%20s : Count\n", "Tweeter Name");
    printf("       ----------------------\n");
    
    if (uniquesCount > 10)
        loopIndex = 10;
    else 
        loopIndex = uniquesCount;

        for(i=0; i<loopIndex; i++)
        {
                printf("%20s : %d \n", UniqueTweeterNames[i], UniqueFreq[i]);
        }

}

int main(int argc, char *argv[])
{
    //int i = 0;
        
    // Only one command line argument, that is, csv file path must be received
    // if argument count is more than 2, exit
    if (argc > 2) 
    {
        printf("Enter one argument (.csv file name) only\n");
        exit(0);
    }
    
    // When no file name is provided
    if (argc == 1) 
    {
        printf("Provide a .csv file name\n");
        exit(0);
    }
   
    // read csv file and process it  
    // if file is not valid, exit
    if (readFile(argv[1]) == 0)
    {
        printf("Invalid Input Format\n");
        exit(0);
    }
        
    // find frequency / count of each tweeter name
    findCounts();
    
    // sort tweeter names in descending order of frequency / count
    sortArrays();
    
    // print top 10 tweeter names with count of tweets
    printOutput();
}