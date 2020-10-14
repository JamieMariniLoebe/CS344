#ifndef MOVIES_H
#define MOVIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie
{
    char *title;
    int *year;
    char *languages;
    char *rating;
    struct movie *head;
    struct movie *next;
};

struct movie *createMovie(char *currLine);
struct movie *processFile(char *filePath);
void printMovie(struct movie* amovie);
void printMovieList(struct movie *list);
int menu(struct movie *movieList);
int userInput();
/* struct movie * */ void displayMovies(int selection, struct movie *movies, char *langgg);

#endif
