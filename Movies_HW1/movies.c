#include "movies.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //For ssize_t

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createmovie(char *currLine)
{
    struct movie *currmovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, " ", &saveptr);
    currmovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currmovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, " ", &saveptr);
    currmovie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currmovie->year, token);

    // The next token is the languages
    token = strtok_r(NULL, " ", &saveptr);
    currmovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currmovie->languages, token);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    currmovie->rating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currmovie->rating, token);

    // Set the next node to NULL in the newly created movie entry
    currmovie->next = NULL;

    return currmovie;
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

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createmovie(currLine);

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
void printmovieList(struct movie *list)
{
    while (list != NULL)
    {
        printmovie(list);
        list = list->next;
    }
}
