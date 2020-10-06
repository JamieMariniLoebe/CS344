#ifndef movies.h
#define movies.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie
{
    char *title;
    char *year;
    char *languages;
    char *rating;
    struct movie *next;
};

struct movie *createmovie(char *currLine);
struct movie *processFile(char *filePath);
void printmovie(struct movie* amovie);
void printmovieList(struct movie *list);

#endif 
