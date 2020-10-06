// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //For ssize_t
#include "movies.h"

   //read in csv file (Step 1)
   //process data in that file, create structs to hold data (Step 2)
   //create a linked list of all structs (Step 3)
   //display menu options to user (step 4)
   //print datq as requested by user (step 5)

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }

    printf("Here we go");
    struct movie *list = processFile(argv[1]);
    printMovieList(list);    
}
