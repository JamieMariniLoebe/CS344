#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //For ssize_t
#include "movies.h"

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);
    //printf("\n%s\n\n", currMovie->title); //TOKEN HOLDS MOVIE TITLE HERE, YEAR NEXT, ETC. AND IS MOVED TO MOVIE STRUCT

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr); //Token now contains the year
    int tokenNum = atoi(token);
    //printf("%d\n\n", tokenNum);
    currMovie->year = (int *)calloc(1, sizeof(int)); //calloc memory for one int (year)
    currMovie->year = tokenNum;
    //printf("%d\n\n", currMovie->year);
    //strcpy(currMovie->year, token);

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The last token is the rating000
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->rating, token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;
    int count = 0;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;
    

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        count++; //start counting lines/movies

        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);
        //printf("%d\n\n", newNode->title);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);

    count--; //Subtract one, first line is header

    //printf("/n/n%d/n/n", count);

    printf("\nProcessed file %s and parsed data for %d movies.\n\n", filePath, count);

    return head;
}

/*
* Print data for the given movie
*/
void printmovie(struct movie* amovie){
  printf("%s, %s %s, %s\n", amovie->title,
               amovie->languages,
               amovie->year,
               amovie->rating);
}
/*
* Print the linked list of movies
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printmovie(list);
        list = list->next;
    }
}

/*
* Prints menu to user
*/
int menu(struct movie *listOfMovies) { 

    int flag = 0;

    //Need to print menu, get user input, display data per option chosen by user, then print menu again
    
    while (flag == 0) {
        printf("--------------------MENU--------------------\n");
        printf("1. Show movies released in the specified year.\n");
        printf("2. Show highest rated movie for each year.\n");
        printf("3. Show the title and year of release of all movies in a specific language.\n");
        printf("4. Exit from the program.\n\n");

        int optionChosen = userInput(); 

        if(optionChosen == 4) { //If user inputs 4, exit while loop and return -1 to main (exit program);
            return -1;
        }
        else {
            //printf("\nMoving on to Display Movies function!!\n\n");
            displayMovies(optionChosen, listOfMovies);
        }
    }
}

/*
* Gets user input, and displays data to user depending on their choice at menu
*/
int userInput() { //Get user input, send input choice back to menu
    
    char choice[10];
    int choiceNum;
    int flag = 0;

    do {
        printf("Enter a choice from 1 to 4: ");
        scanf("%s", &choice); //Get user input 
        choiceNum = atoi(choice); //Convert string from user to int

        //Use flag to break out of do-while loop if valid option chosen. Continue to loop if not.
        if(choiceNum == 1) {
            flag = 1;
            break;
        }
        else if (choiceNum == 2) {
            flag == 1;
            break;
        }
        else if (choiceNum == 3) {
            flag == 1;
            break;
        }
        else if (choiceNum == 4) {
            flag == 1;
            break;
        }
    }
    while (flag != 1);

    while(1) { //Infinite while loop, break when menu choice confirmed and data displayed
        if(choiceNum == 1)
        {
            //printf("\nHere I will print out movies released in the year of %d!\n\n", year);
            return choiceNum;
        }
        else if (choiceNum == 2) {
            //printf("\nHere I will print out the highest rated movie for each year!\n\n");
            return choiceNum;;
        }
        else if (choiceNum == 3) {
            char lang[20];

            printf("\nEnter the language for which you want to see movies: ");

            scanf("%s", &lang); 

            //NOT NEEDED, but good place for error handling/Confirm this language is included in list of movies

            //printf("\nHere are all the movies (RELEASE YEAR AND TITLE) in %s\n\n", lang);
            return choiceNum;;
        }
        else if (choiceNum == 4) {
            printf("\nExiting program....\n\n");
            exit(0);
        }

        return choiceNum;
        /* else {
            printf("\nError! Not a valid menu option. Please choose between options 1-4: ");
            scanf("%s", choiceNum);
            continue;
        } */
    }
}

/*
* Displays movie data
*/
void displayMovies(int selection, struct movie *list) {
    //printf("Getting Movie data here, and printing!\n\n");
    //printf("%s\n\n", list->title);

    switch(selection) {
    case 1:
        {
            int *year; //year of release
            int i = 0; //counter
            char str[10]; //Hold user input from keyboard
            char *releaseYearMovies[] = { NULL }; //Array of movies released in specific year
            //int *ptrYear;
            int userYear;
            int movieYear;
    
            printf("\nEnter the release year for which you want to see movies: ");
            scanf("%d", &year);

            //printf("\nHere are the movies released in the year %d\n\n", year);

            //printf("Curr Movie: %s\n\n", list->title);
            //printf("%d\n\n", list->year);

            while (list->next != NULL) { //While still movies in linked list to check
                //ptrYear = &(list->year)
                //printf("%d\n\n", *ptrYear);
                //list is at NULL right now (AKA head)
                list = list->next; //We have anothre movie to check, move to that movie node
                
                //printf("User Request: %d\n\n", (int *)year);
                userYear = (int *) year;
                //printf("Movie Date: %d\n\n", (int *)list->year);
                movieYear = (int *)list->year;
                //list = list->next; //Now we point list to FIRST movie
                //printf("%s\n\n", list->title);

                if(userYear == movieYear) { //Compare values pointed to by these 2 pointers (cast pointers to ints)
                    //printf("Compare successful!\n\n");
                    printf("%s\n", list->title);
                    //releaseYearMovies[i] = calloc(strlen(list->title) + 1, sizeof(char));
                    //releaseYearMovies[i] = list->title; 
                    //printf("%d\n\n", releaseYearMovies[i]);
                    i++;
                }
            }
            printf("\n");
            if(i == 0) {
                printf("No data about movies released in %d\n\n", year);
            }
            /*else {
                for(int i=0; i<sizeof(releaseYearMovies)-1; i++) {
                    printf("%d", releaseYearMovies[i]);
                }
            } */
        }
    case 2: //Highest rated movie for each year
        {
            int *years; //Hold years accounted for/avoid duplicates
            int *trueYears;
            struct movie *highestRated; //Array to hold highest rated movies per year
            int i = 0;
            int releaseYear;
            int len = sizeof(trueYears)/sizeof(trueYears[0]);

            int sizeYears = counter(list);

            years = malloc(sizeYears * sizeof(int));

            /*1.) Loop thru Struct Movie List
            /*2.) Loop thru Years array 
            /* Compare movie release year at position j in Struct Movie List with movie reease years already added to Years array 
            for(int j=0; j<i; j++)  {
                for(int k=0; k<) 
            }*/
            //4printf("%s", list->title);

            trueYears = printMovieYear(list, years);

            while (list->next != NULL) {
                //printf("%s\n", list->title);
                releaseYear = (int *)list->year;
                //printf("%d\n\n", releaseYear);

                //1.) GET ARRAY OF ALL MOVIES (structs)
                //2.) TAKE FIRST FILM IN ARRAY, COMPARE TO EVERY OTHER FILM, IF YEARS MATCH,
                //    COMPARE AND REMOVE LOWER RATED MOVIE FROM ARRAY
                //3.) CONTINUE WITH REST OF MOVIES, COMPARING MOVIES WITH SAME RELEASE YEAR AND REMOVING
                //    LOWER RATED MOVIES
                //4.) AT THE END, SHOULD HAVE ONLY 1 MOVIE PER YEAR, WHICH WILL BE HIGHEST RATED MOVIE
                
                /* for(int j=0; j<len; j++) {
                    if(releaseYear == years[j]) {
                        list = list->next;
                        continue; //Duplicate, skip this year, check next
                    }
                    else {
                        trueYears[j] = releaseYear;
                        list = list->next;
                        //printf("%d\n", years[j]);
                    }
                } */
            }
            for(int k=0; k<len; k++) {
                printf("Year: %d\n", trueYears[k]);
            }

        }
    }
}

int counter(struct movie *list) {
    int i = 0;
    while(list->next != NULL) {
        i++; //Count # of movies
        list = list->next;
    }
    return i;
}

//return array of years of all movies
int *printMovieYear(struct movie *list, int *arr)
{
    int i = 0;
    while (list != NULL)
    {
        arr[i] = list->year;
        list = list->next;
        i++;
    }
}