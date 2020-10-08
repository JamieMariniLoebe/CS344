#ifndef MOVIES_H
#define MOVIES_H

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

struct movie *createMovie(char *currLine);
struct movie *processFile(char *filePath);
void printMovie(struct movie* amovie);
void printMovieList(struct movie *list);
void menu();
void userInput();

#endif