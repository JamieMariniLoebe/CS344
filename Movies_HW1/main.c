// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //For ssize_t
#include "movies.h"

   //read in csv file (Step 1)--------->DONE
   //process data in that file, create structs to hold data (Step 2)-->DONE

   //DISPLAY "PROCESSED FILE "FILE NAME" AND PARSED DATA FOR "NUMBER OF MOVIES" MOVIES

   //create a linked list of all structs (Step 3) -->DONE
   //display menu options to user (step 4) 

   //---------MENU-------------
   //1.) Show movies released in specific year
        //--Enter year for which you want to see movies--
        //--SORT DATA BY SPECIFIC YEAR, PRINT TO USER--

   //2.) Show highest rated movie for each year
        //SORT BY YEAR, ARRAY FOR EACH YEAR, FIND HIGHEST RATED FOR EACH YEAR, ADD TO NEW ARRAY, PRINT TO USER

   //3.) Show the title and year of release of all movies in specific language
        //--Enter the language for which you want to see movies--
        //SORT BY SPECIFIC LANGUAGE, PRINT ALL MOVIES (TITLE + RELEASE YEAR ONLY) IN THAT LANGUAGE TO USER

   //4.) Exit


   //print dat as requested by user (step 5)

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }

    //printf("%s", argv[1]);
    struct movie *list = processFile(argv[1]);

    menu(); //Displays menu, interacts with user. 
    
    //Display Menu and PROCESS USER REQUEST (MEAT OF PROGRAM IS HERE?)

    //printMovieList(list);
}
