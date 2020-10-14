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

    count--; //Subtract one, first line is movie2er

    //printf("/n/n%d/n/n", count);

    printf("\nProcessed file %s and parsed data for %d movies.\n\n", filePath, count);

    return head;
}

/*
* Print data for the given movie
*/
void printmovie(struct movie *amovie)
{
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
int menu(struct movie *listOfMovies)
{

    int flag = 0;

    //Need to print menu, get user input, display data per option chosen by user, then print menu again

    while (flag == 0)
    {
        printf("--------------------MENU--------------------\n");
        printf("1. Show movies released in the specified year.\n");
        printf("2. Show highest rated movie for each year.\n");
        printf("3. Show the title and year of release of all movies in a specific language.\n");
        printf("4. Exit from the program.\n\n");

        int optionChosen = userInput();

        if (optionChosen == 4)
        { //If user inputs 4, exit while loop and return -1 to main (exit program);
            flag = 1;
        }
        else if (optionChosen == 3)
        {
            char lang[20];

            printf("\nEnter the language for which you want to see movies: ");

            scanf("%s", &lang);

            displayMovies(optionChosen, listOfMovies, lang);
        }
        else
        {
            char *nada;
            //printf("\nMoving on to Display Movies function!!\n\n");
            displayMovies(optionChosen, listOfMovies, nada);
        }
    }
}

/*
* Gets user input, and displays data to user depending on their choice at menu
*/
int userInput()
{ //Get user input, send input choice back to menu

    char choice[10];
    int choiceNum;
    int flag = 0;

    do
    {
        printf("Enter a choice from 1 to 4: ");
        scanf("%s", &choice);     //Get user input
        choiceNum = atoi(choice); //Convert string from user to int

        //Use flag to break out of do-while loop if valid option chosen. Continue to loop if not.
        if (choiceNum == 1)
        {
            flag = 1;
            break;
        }
        else if (choiceNum == 2)
        {
            flag == 1;
            break;
        }
        else if (choiceNum == 3)
        {
            flag == 1;
            break;
        }
        else if (choiceNum == 4)
        {
            flag == 1;
            break;
        }
    } while (flag != 1);

    while (1)
    { //Infinite while loop, break when menu choice confirmed and data displayed
        if (choiceNum == 1)
        {
            //printf("\nHere I will print out movies released in the year of %d!\n\n", year);
            return choiceNum;
        }
        else if (choiceNum == 2)
        {
            //printf("\nHere I will print out the highest rated movie for each year!\n\n");
            return choiceNum;
            ;
        }
        else if (choiceNum == 3)
        {

            //NOT NEEDED, but good place for error handling/Confirm this language is included in list of movies

            //printf("\nHere are all the movies (RELEASE YEAR AND TITLE) in %s\n\n", lang);
            return choiceNum;
            ;
        }
        else if (choiceNum == 4)
        {
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
void displayMovies(int selection, struct movie *list, char *lang)
{
    //printf("Getting Movie data here, and printing!\n\n");
    //printf("%s\n\n", list->title);
    struct movie *movie2 = list;

    switch (selection)
    {
    case 1:
    {
        int *year;                          //year of release
        int i = 0;                          //counter
        char str[10];                       //Hold user input from keyboard
        char *releaseYearMovies[] = {NULL}; //Array of movies released in specific year
        //int *ptrYear;
        int userYear;
        int movieYear;

        printf("\nEnter the release year for which you want to see movies: ");
        scanf("%d", &year);

        while (list->next != NULL)
        {                      //While still movies in linked list to check
            list = list->next; //We have another movie to check, move to that movie

            //printf("User Request: %d\n\n", (int *)year);
            userYear = (int *)year;
            //printf("Movie Date: %d\n\n", (int *)list->year);
            movieYear = (int *)list->year;
            //list = list->next; //Now we point list to FIRST movie
            //printf("%s\n\n", list->title);

            if (userYear == movieYear)
            { //Compare values pointed to by these 2 pointers (cast pointers to ints)
                //printf("\nCompare successful!\n\n");
                printf("%s released in %d\n\n", list->title, userYear);
                //releaseYearMovies[i] = calloc(strlen(list->title) + 1, sizeof(char));
                //releaseYearMovies[i] = list->title;
                //printf("%d\n\n", releaseYearMovies[i]);
                i++;
            }
        }

        printf("\n");
        if (i == 0)
        {
            printf("No data about movies released in %d\n\n", year);
            break;
        }
        /*else {
                for(int i=0; i<sizeof(releaseYearMovies)-1; i++) {
                    printf("%d", releaseYearMovies[i]);
                }
            } */
        break;
    }
    case 2: //Highest rated movie for each year
    {
        printf("\n\n");
        int *years;                              //Hold release years
        //int i = 0;
        int m = 0; //Counter for BEST rated films array
        int k = 0; //Counter for 'movie1'
        int j = 0;
        int numOfMovies = counter(list); /* Get # of movies in linked list */
        struct movie *highestRated[24] = {NULL}; //Array to hold highest rated movies per year
        int len = sizeof(highestRated) / sizeof(highestRated[0]);
        struct movie *movie1 = list; //Pointer to hold movie being comapred to array
        struct movie *movie2 = list; //Pointer to hold current index in array being compared to movie1
        struct movie *movietwo = list;
        struct movie *rateCompare(struct movie * one, struct movie * arr);
        struct movie *betterMovie = (struct movie *)malloc(sizeof(struct movie *));
        //movie1 = movie2; //Set movie1 to current movie in linked list
        

        

        for (int j = 0; j < numOfMovies; j++)
        { //Malloc space for array of structs
            highestRated[j] = (struct movie *)malloc(sizeof(list));
        }

        /* FLOW OF BELOW CODE
        * movie1 --> Current film being compared to all movies in highestRated array (constant)
        * j --> Film at highestRated[i] being compared to movie1 (temporary) */

        /*1.) WHILE:  */
        int l = 0;
        while (movie1->next != NULL) //Iterate thru each film to compare to entire array
        {
            l = 0;
            movie1 = movie1->next; //Move to next film(node) as 'movie1'
            //movietwo = movie2->head;
            printf("movie1: %s\n", movie1->title);
            //printf("%s\n", movie2->title);

            //movie2 = newList;          //Start from beginning of list again

            while(l < numOfMovies) //Iterate thru list for movies to COMAPARE to movie1
            {
                betterMovie = NULL;
                movie2 = movie2->next;
                movietwo = movie2;
                printf("movie2: %s\n", movietwo->title);

                int strcmp1 = strcmp(movie1->title, movietwo->title); //Return 0 if films are the same
                int yearSame = yearCompare(movie1, movietwo);

                //START: Check if 'movie1' is being compared to itself

                if (strcmp1 == 0) //Both films are the same
                {
                    //printf("Same MOVIE\n\n");
                    continue; //skip current iteration (To next node)
                }

                /*b.) ELSE: Films are NOT duplicates*/
                else if (yearSame != 1) //Films have same release year
                {
                    char *end1;
                    char *end2;
                    double movieRating1;
                    double movieRating2;

                    movieRating1 = strtod(movie1->rating, &end1);
                    movieRating2 = strtod(movietwo->rating, &end2);

                    if(movieRating1 > movieRating2) {
                        betterMovie = movie1;
                    }
                    else { //Movie2 rating is better, or same so default to movie2
                        betterMovie = movietwo;
                    }
                }
            highestRated[k] = betterMovie;
            k++;
            l++;
            }
        }
        for(int i=0; i<len; i++) {
            printf("%s\n", highestRated[i]->title);
        }
    }

    case 3:
    {

        while(list->next != NULL) {
            list = list->next;
            printf("%s\n", list->languages);
            
            if((char *)list->languages == selection) {
                printf("%s\n\n", list->title);
            }
        }
    }
    }
}

int counter(struct movie *list) //Count and return # of films in list
{
    int i = 0;
    while (list->next != NULL)
    {
        i++; //Count # of movies
        list = list->next;
    }
    return i;
}

struct movie *rateCompare(struct movie *one, struct movie *two)
{ //Compare release years of both movies, return lower rated movie
    if ((double *)one->rating < (double *)two->rating)
    {
        return two; //Return two if has lower rating
    }
    return one; //If One has better rating, or both films have the same rating, then return one
}

void addMovie(struct movie *movieToAdd, struct movie *arr[], int index)
{
    //int len = sizeof(arr)/sizeof(arr[0]);
    arr[index] = movieToAdd;
}

int yearCompare(struct movie *one, struct movie *two)
{ //return 0 if films have same release year
    int yearOne = (int *)one->year;
    int yearTwo = (int *)two->year;

    if (yearOne == yearTwo)
    {
        return 0;
    }
    return 1;
}

//return array of years of all movies
/*int *arrayMovieYear(struct movie *list, int *arr)
{
    int i = 0;
    while (list->next != NULL)
    {
        arr[i] = list->year;
        list = list->next;
        i++;
    }
    return i;
} */

/* i.) IF: 'movie1' has same release year as highestRated[j] */
/*if ((int *)movie1->year == (int *)highestRated[j]->year) //Correctly compares release years
                {
                    printf("Same YEAR\n\n");
                    printf("%d and %d\n\n", movie1->year, highestRated[j]->year);
                    j++;
                } */

/*
                //NEED TO STILL COMPARE RATINGS, MOVE HIGHER RATED FILM TO NEW ARRAY
                //ALSO NEED TO WORK ON ITERATING THRU EACH MOVIE TO COMPARE */

/*if ((int *)movie1->rating > (int *)highestRated[j]->rating)
                    {
                        printf("ADDING %s\n\n", movie1[j].title);
                        memcpy(&bestRated[k], movie1, sizeof(highestRated[j]));
                        j++;
                        k++;
                        //Create function to remove index from array
                    }
                    /*else if ((int *)movie1->rating < (int *)highestRated[j]->rating)
                    {
                        printf("ADDING %s\n\n", highestRated[j]->title);
                        memcpy(&bestRated[k], highestRated[j], sizeof(highestRated[j]));
                        j++;
                        k++;
                    }
                    else
                    {
                        memcpy(&bestRated[k], highestRated[j], sizeof(highestRated[j]));
                        j++;
                        k++;
                    }
                }
            }
            j++;
        } */
/*4. START NEW LOOP
                a.) Display each movie in array (FORMAT: Year - Rating - Title)
            */
/* int len = sizeof(bestRated) / sizeof(bestRated[0]);
        //printf("%d", len);
        for (int i = 0; i < len; i++)
        {
            printf("%d %d %s \n", bestRated[i].year, bestRated[i].rating, bestRated[i].title);
        }
        } */

/*
                
                    ii.) ELSE IF: 'movie1' has a LOWER rating than 'filmTwo'
                        -Do nothing (continue?). *Leave filmTwo, as it will eventually become 'movie1' and be compared
                        again to the current 'movie1'. At that point, the current movie1 will be removed.*

                        -OR find 'movie1' in highestRated[] array, and remove it?

                    iii.) ELSE: If both films have the SAME rating
                        -Remove 'filmTwo' from highestRated[] by default
            3. END LOOP */

//int sizeYears = counter(list);

//years = malloc(sizeYears * sizeof(int));

/*1.) Loop thru Struct Movie List
            /*2.) Loop thru Years array 
            /* Compare movie release year at position j in Struct Movie List with movie reease years already added to Years array 
            for(int j=0; j<i; j++)  {
                for(int k=0; k<) 
            }*/
//4printf("%s", list->title);

/*while (list->next != NULL)
{ //loop thru linked list

    list = list->next;      //get next movie
    highestRated[i] = list; //Get movie and put at ith index
    i++;

    //memcpy(highestRated, &list, sizeof list);
    //printf("%s\n", highestRated[i]->title);
    //printf("%d\n\n", highestRated[i]->year);

    /* if(((int *) highestRated[i]->year == 2008)) {
                    printf("YOU DID IT!!");
                } */

//printd("%s\n\n", list->year);

//highestRated[i] = list;
//printf("%s\n", list->title);

//printf("%d\n\n", releaseYear);

//------DONE------1.) GET ARRAY OF ALL MOVIES (structs)
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
//}
/* Now we have array of all the movies.  */

/*while(0) { //keep checking until 
                        printf("Inside while loop");
                        break;

                        /*for(int m=0; m<23; m++) //itre
                        {
                            strcpy(highTitle1, highestRated[k]->title);
                            strcpy(highTitle2, higherRated->title);
                            printf("%s\n", highTitle1);
                            printf("%s\n", highTitle2);
                            break;
                            /*if(strcmp(highTitle1,highTitle2)) 
                            {
                                printf("Already in list");
                                break;
                            } 
                        } 
                    } */

/*&if(flag == 0) {
                        addMovie(higherRated, highestRated, k); //Add film w higher rating
                    }

                    //printf("Adding %s\n\n", higherRated->title);
                }
                else
                {
                    //printf("Films do NOT have the same release year\n\n");
                    continue;
                } */
//printf("%s\n", highestRated[4]->title);

//int len= sizeof(highestRated)/sizeof(highestRated[0]);

/*for(int i=0; i<len-1; i++) {
            char * str;
            double rate;
            rate = strtod(highestRated[i]->rating, str);
            //printf("%.1f %d %s\n", rate, highestRated[i]->year, highestRated[i]->title);
        } */