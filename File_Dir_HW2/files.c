#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "files.h"

#define PREFIX "movie_"

/*
* Prints menu to user
*/
void menu()
{
    //Need to print menu, get user input, do work, then print menu again
    int flag = 0;

    while (flag == 0)
    {
        printf("--------------------MENU---------------------\n");
        printf("1. Select file to process\n");
        printf("2. Exit the program\n\n");

        userInput();

        int num = menuTwo();

        if (num == 0)
        {
            flag = 0;
        }
        else
        {
            flag = 1;
        }
    }
}

int userInput()
{ //Get user input, send input choice back to menu

    char choice[10];
    int choiceNum;
    int flag = 0;

    do
    {
        printf("Enter a choice from 1 to 2: ");

        fgets(choice, 10, stdin);     //Get user input
        choiceNum = atoi(choice); //Convert string from user to int
        printf("\n");
        printf("--------------------------------\n");
        printf("\n");

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
        else
        {
            printf("Error! Invalid entry, try again\n\n");
            flag = 0;
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
            printf("\nExiting program....\n\n");
            exit(0);
        }

        return choiceNum;
    }
}

int menuTwo()
{
    char userSelection[10];
    int optionNum;
    int flag = 1;

    while (1) { //Infinite loop, break when user selects valid option
        printf("Which file do you want to process?\n\n");
        printf("--Enter 1 to pick the largest file\n");
        printf("--Enter 2 to pick the smallest file\n");
        printf("--Enter 3 to specify the name of a file\n\n");
        printf("Enter a choice from 1 to 3: ");

        fgets(userSelection, 10, stdin);

        optionNum = atoi(userSelection);

        if(optionNum == 1 || optionNum == 2 || optionNum == 3) {
            break;
        }
        printf("\nError, invalid input. Please try again\n\n");
    }

    if (optionNum == 1)
    {
        fileOption(optionNum);
        return 0;
    }
    else if (optionNum == 2)
    {
        fileOption(optionNum);
        return 0;
    }
    else if (optionNum == 3)
    {
        fileOption(optionNum);
        return 0;
    }
    else {

    }
}

void fileOption(int option)
{

    switch(option) {
        case 1: ;
            char *largeFile = sizeFile(1); //Find largest file with extension CSV in CURRENT DIRECTORY whose name STARTS with "movies_"
            //printf("OUR LARGEST FILE %s will be processed\n", largeFile);

            //Process that file
            printf("Now processing the chosen file named %s\n\n", largeFile);
            processFile(largeFile);

            break;


        case 2: ;
            char* smallFile = sizeFile(2);   //Find smallest file with extension CSV in CURRENT DIRECTORY whose name STARTS with "movies_"

            //printf("OUR SMALLEST FILE %s will be processed\n", smallFile);
            printf("Now processing the chosen file named %s\n\n", smallFile);
            processFile(smallFile);
    
            break;
        
        case 3: ;
            //printf("Enter the chosen file to process :\n\n");
            char * userFile1 = sizeFile(3);
            if(userFile1 == NULL) {
                printf("File does not exist");
                return userFile1;
            }
            //printf("File successful: %s\n\n", userFile1);
            printf("Now processing the chosen file named %s\n\n", userFile1);
            processFile(userFile1);

            break;
    }
    
    printf("------------------------------");
}

char *sizeFile(int option) { //Returns file requested by user
    DIR *currDir = opendir(".");
    struct dirent *myDir;
    off_t fileSize;
    struct stat dirStat;
    char *fileExt;

    if (option == 1) //Get largest file
    {
        char *largestFile = (char *) malloc(sizeof(char) * 30);
        char *largestFile2;
        long long largestFileSize;
        int i = 0;

        while((myDir = readdir(currDir)) != NULL)  //open directory
        {

            if(strncmp(PREFIX, myDir->d_name, strlen(PREFIX)) == 0) 
            { //Only check files with prefix "movie_"

                stat(myDir->d_name, &dirStat); //Get info on file, put in dirStat

                fileExt = strchr(myDir->d_name, '.');

                if(strcmp(fileExt, ".csv") != 0) { //check file extension
                    printf("Not a CSV file!");
                    break;
                }
                //printf("Good CSV file!\n");

                if(i == 0 || largestFileSize < (long long) dirStat.st_size) 
                    { //For first file
                    largestFile = myDir->d_name;
                    largestFileSize = dirStat.st_size;
                    //printf("Largest file on %d iteration : %s at size: %lld\n\n", i, largestFile, largestFileSize);
                    }
                else 
                {
                    //printf("Continuing\n\n");
                    continue;
                }
                i++;
            }
            //printf("File name at %d: %s \n\n", i, largestFile);
        }
        //printf("File size: %lld \n\n", largestFileSize);
        closedir(currDir);
        //memcpy(largestFile2, largestFile, sizeof(largestFile));
        //free(largestFile);
        return largestFile; //Returning NAME of file
    }

    else if (option == 2) { //Get smallest file
        char *smallestFile = (char *) malloc(sizeof(char) * 30);
        long long smallestFileSize;
        int i = 0;

        while((myDir = readdir(currDir)) != NULL) 
        {
            if(strncmp(PREFIX, myDir->d_name, strlen(PREFIX)) == 0) 
            { //Only check files with prefix "movie_"

                stat(myDir->d_name, &dirStat); //Get info on file, put in dirStat

                fileExt = strchr(myDir->d_name, '.'); //Get file extension

                if(strcmp(fileExt, ".csv") != 0) { //Check if CSV file or not2

                    //printf("Not a CSV file!");
                    break;
                }
                //printf("Good CSV file!\n");

                if(i == 0 || smallestFileSize > (long long) dirStat.st_size) 
                    { //For first file OR if Current 'smallestFile' is LARGER than file it is being compared to
                    smallestFile = myDir->d_name;
                    smallestFileSize = dirStat.st_size;
                    //printf("Largest file on %d iteration : %s at size: %lld\n\n", i, largestFile, largestFileSize);
                    }
                else 
                {
                    //printf("Continuing\n\n");
                    continue;
                }
                i++;
            }
            //printf("File name at %d: %s \n\n", i, largestFile);
        }
        printf("File name: %lld \n\n", smallestFileSize);
        closedir(currDir);
        return smallestFile; //Returning NAME of file

    }

    else if (option == 3) { //Get user specific file

        char userFile[18];
        char *chosenFile = (char *) malloc(sizeof(char) * 30);
        //Check if file exists in current directory.

        printf("Enter the complete file name: ");
        fgets(userFile, 19, stdin);

        while((myDir = readdir(currDir)) != NULL) 
        {
            printf("%s %s\n\n", userFile, (char *) myDir->d_name);
            if(strcmp(userFile, (char *)myDir->d_name) == 0)
            { //if file exists
                strcpy(chosenFile, userFile);
                printf("Successful: %s\n\n", chosenFile);
                break;
            }
            else { //keep looking for file
                printf("File not found");
                continue;
            }

            return chosenFile;
            //--if NOT found, print error message and print 3 menu options again
            //(i.e., Repeat menuTwo options, do NOT return to main menu)
        }
        if(chosenFile != NULL) {
            printf("File found!\n\n");
            return chosenFile;
        }
        printf("File not found\n\n");
        return chosenFile;
    }
}
    


void processFile(char *file) { // Create directory, parse given file, and add data to directory
    char *myNewDir = createDirectory();
    struct movie *list = parseFile(file);
    addToDir(myNewDir, list);
    
}

char *createDirectory() 
{

	int rando = rand() % 999999; //Random number between 0-100,000
	char rNum[sizeof(int) * 8 + 1];				 //Array of chars. String to hold random number string
    sprintf(rNum, "%d", rando);

	char dirName[256];
	strcpy(dirName, "loebej.movies."); //Copy directory name into variable 'dirName' (variable, directory name)
    strcat(dirName, rNum);

	//initialize success to mkdir
	int success = mkdir(dirName, 0755);

	//If creates directory, print success
	if (success == 0)
	{
		printf("\nCreated directory with name %s\n\n", dirName);
	}
	else
	{
		//Else, print fail message and error
		perror("Error: ");
		exit(0);
	}

	return dirName;
}

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
struct movie *parseFile(char *filePath)
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

void addToDir(char *newDir, struct movie *dataMovies) { //Add data to files in new direcotry
    DIR *myDir = opendir(newDir);
    int year;
    int i = 0;
    
    while(dataMovies->next != NULL) { //Get each movie in file
        dataMovies = dataMovies->next;
        year = dataMovies->year;
        char *strYear;
        char *message;
        printf("%d\n", year);
        sprintf(strYear, "%d", year);

        int fd;
        char* pathFile;
        //sprintf(pathFile, "%s.txt", strYear);
        //printf("%s\n\n", pathFile);
        fd = open(pathFile, O_RDWR | O_CREAT | O_TRUNC, 6640); //Open file

        if(fd == -1) {
            printf("open() failed on \"%s\"\n", pathFile);
            perror("Error");
            exit(1);
        }

        //int movieFile = write(fd, messa, strlen(dataMovies->title + 2));
        close(fd);
    }
    closedir(myDir);
}