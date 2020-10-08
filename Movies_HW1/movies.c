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
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The last token is the rating
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
void menu() { //Display menu
    printf("\nMenu\n\n");
    printf("1. Show movies released in the specified year.\n");
    printf("2. Show highest rated movie for each year.\n");
    printf("3. Show the title and year of release of all movies in a specific language.\n");
    printf("4. Exit from the program.\n\n");
    
    userInput(); //Get user input and display data
}


/*
* Gets user input, and displays data to user depending on their choice at menu
*/
void userInput() { //Void for now, WILL RETURN STRUCT MOVIE
    
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
            int year;
            char str[10];

            printf("\nEnter the release year for which you want to see movies: ");
            scanf("%s", &str);
            year = atoi(str);

            printf("\nHere I will print out movies released in the year of %d!\n\n", year);
            break;
        }
        else if (choiceNum == 2) {
            printf("\nHere I will print out the highest rated movie for each year!\n\n");
            break;
        }
        else if (choiceNum == 3) {
            char lang[20];

            printf("\nEnter the language for which you want to see movies: ");

            scanf("%s", &lang); 

            //NOT NEEDED, but good place for error handling/Confirm this language is included in list of movies

            printf("\nHere are all the movies (RELEASE YEAR AND TITLE) in %s\n\n", lang);
            break;
        }
        else if (choiceNum == 4) {
            printf("\nExiting program....\n\n");
            exit(0);
        }
        /* else {
            printf("\nError! Not a valid menu option. Please choose between options 1-4: ");
            scanf("%s", choiceNum);
            continue;
        } */
    }
}
