#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie
{
    char *title;
    int *year;
    char *languages;
    char *rating;
    struct movie *next;
};

struct movie *createMovie(char *currLine);
struct movie *parseFile(char *filePath);
void menu();
int userInput();
char *sizeFile(int option);
void fileOption(int option2);
void processFile(char *file);
char *createDirectory();
void addToDir(char *newDir, struct movie *dataMovies);

//struct movie *createMovie(char *currLine);
//struct movie *processFile(char *filePath);
//void printMovie(struct movie* amovie);
//void printMovieList(struct movie *list);
//int menu(struct movie *movieList);
//* struct movie * */ void displayMovies(int selection, struct movie *movies, char *langgg);
//what
#endif
