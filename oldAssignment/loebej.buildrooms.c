#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "loebej.buildrooms.h"

//**ASSIGNMENT 1: PART 1 OF 2**

/*STEP 1: The first thing your rooms program must do is create a directory called <YOUR STUDENT ONID USERNAME>.rooms.<random_number> where random_number is a random number between 
0 and 100,000. Next, it must generate 7 different room files, which will contain one room per file, in the directory just created. You may use any filenames you want for 
these 7 room files, and these names should be hardcoded into your program. */

/*The elements that make up an actual room defined inside a room file are listed below, along with some additional specifications:

A Room Name
	-A room name cannot be assigned to more than one room.
	-Each name can be at max 8 characters long, with only uppercase and lowercase letters allowed (thus, no numbers, special characters, or spaces). This restriction is not extended to the room file's filename.
	-You must hard code a list of ten different Room Names into your rooms program and have your rooms program randomly assign one of these to each room generated. Thus, for a given run of your rooms program, 7 of the 10 hard-coded room names will be used.

A Room Type
	-The possible room type entries are: START_ROOM, END_ROOM, and MID_ROOM.
	-The assignment of which room gets which type should be randomly generated each time the rooms program is run.
	-Naturally, only one room should be assigned the START_ROOM type, and only one room should be assigned the END_ROOM type. The rest of the rooms will receive the MID_ROOM type.

Outbound connections to other rooms
	-There must be at least 3 outbound connections and at most 6 outbound connections from this room to other rooms.
	-The outbound connections from one room to other rooms should be assigned randomly each time the rooms program is run.
	-Outbound connections must have matching connections coming back: if room A connects to room B, then room B must have a connection back to room A. Because of all of these specs, there will always be at least one path through.
	-A room cannot have an outbound connection that points to itself.
	-A room cannot have more than one outbound connection to the same room.

FORMAT: 
	ROOM NAME: <room name>
	CONNECTION 1: <room name>
	…
	
ROOM TYPE: <room type>

*/

// WHEN THIS FILE IS COMPILED AND RUN, (EX: GCC++ ... JL295.BUILDROOMS), IT CREATES A SERIES OF FILES THAT HOLD
//DESCRIPTIONS OF THE IN-GAME ROOMS AND HOW THE ROOMS ARE CONNECTED.  control ins

// 1.) Create directory
char *createDirectory()
{

	int rando = rand() % 100000; //Random number between 0-100,000
	char rNum[8];				 //Array of chars. String to hold random number string

	char dirName[256];
	strcpy(dirName, "/nfs/stak/users/loebej/CS344/"); //Copy directory name into variable 'dirName' (variable, directory name)

	//rando --> Holds random int
	//rNum --> Empty 'string' to hold random number after conversion

	char *dir = (char *)malloc((50) * sizeof(char)); //Malloc memory to hold directory

	int nameDir;
	nameDir = mkdir(rando, 0755); //Make directory, set to int to confirm created successfully

	if (nameDir != 0)
	{
		printf("Error with memory!"); //Memory not allocated
		exit(0);
	}

	//strcat(dir, dirName); //Append directory name to empty string. Dir being empty string

	printf("\n%s \n\n", dir); //test

	//snprintf(rNum, sizeof(rNum), "%d", rando); //Convert randomized integer to string

	strcat(dir, rNum); //Add random number string to directory name (JL295.buildrooms.<RandomNumber>)

	//printf("%s", dir); //test

	//initialize success to mkdir
	int success = mkdir(dir, 0755);

	//If creates directory, print success
	if (success == 0)
	{
		printf("Success! Rooms directory created: \n%s\n\n", dir);
	}
	else
	{
		//Else, print fail message and error
		perror("Error: ");
		exit(0);
	}

	return dir;
}

void swap(int arr[], int *i, int *j)
{ //Using pointers to address where values are stored in array

	int temp = *i; //placeholder
	*i = *j;	   //J goes to I
	*j = temp;	   ////Old I goes to J
}

void shuffle(int arr[], int n)
{

	srand(time(NULL)); //Seed each time for diff results

	for (int i = n - 1; i > 0; i--)
	{ //loop from high to low index in array (last element to first)

		int rando = rand() % (i + 1); //generate random # from 0 to n(9)

		swap(arr, &arr[i], &arr[rando]); //Swap each array index with random number
	}
}

void createRooms(char *direct)
{
	//FILE *fileptr; //File Pointer

	char *roomNames[] = {
		//Array of room names
		"/Rio.txt\0",	//0. Rio
		"/Berlin.txt\0", //1. Berlin
		"/Tokyo.txt\0",  //2. Tokyo
		"/Lisboa.txt\0", //3. Lisboa
		"/Denver.txt\0", //4. Denver
		"/Moscow.txt\0", //5. Moscow
		"/Rome.txt\0",   //6. Rome
		"/Madrid.txt\0", //7. Madrid
		"/London.txt\0", //8. London
		"/Prague.txt\0"  //9. Prague
	};

	//Initialize array of 10 elements (0-9)
	//Shuffle the array
	//Use the first 7 as Room Names.

	//int arrNums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	//int n = sizeof(arrNums) / sizeof(arrNums[0]);

	//shuffle(arrNums, n);

	//printf("%d", arr1[0]); test

	//char* randomName = roomNames[2];
	//printf("%s \n \n", randomName);
	//printf("%s \n\n", direct);

	


	for (int i = 0; i <= 0; i++)
	{ //Loop to create each ROOM_FILE

		char *libPath = (char *)malloc((50) * sizeof(char)); //Malloc memory to hold new file
		libPath = direct; //Move File Path in directory to libPath var (libPath = /nfs/stak/users/loebej/CS344/*RandomNum*)

		//Why not changing to new file path?? keeps adding to same file path, should create new one
		//char *rName = "/Tata";

		char *rName = roomNames[i];
		strcat(libPath, rName);

		//int len = strlen(libPath);
		//snprintf(libPath+ len, sizeof(libPath), "%d", i);
		//snprintf(libPath+len, sizeof(libPath), "%s", rName);

		printf("%s \n\n", libPath);

		FILE *fptr;
		fptr = fopen(libPath, "w");

		if(fptr == NULL)
    	{
        	printf("Unable to create file.\n");
        	exit(EXIT_FAILURE);
    	}


		fclose(fptr); 

		//printf("%s\n\n", libPath);
		//snprintf(libPath, sizeof(libPath), "/nfs/stak/users/loebej/CS344/%s", roomNames[i]); 
		//printf("%s\n\n", libPath);
		/*if (libPath == NULL)
		{ //Confirm malloc'ed correctly
			printf("Error with memory!");
			exit(0);
		} else
		{
			{
				printf("Malloc successful \n");
			}
		}
		
		//snprintf(libPath, 4096+1,"%d", i);

		//fptr = fopen(newFi4096+1, le, "w");
		//fclose(fptr);

		/* 

		char *randomRoomName = roomNames[arrNums[i]]; //Get random room name from array
		/* 1.) strcat(libPath, direct);			  //Add current libPath to directory (By editing file path)
		/* 2.) strcat(libPath, randomRoomName);	  //Add randomly selected room name to current libPath

		//Above strcat operations create file path as below:

		//1.) JL295.buildRooms.7093 --> Update filepath to include directory
		//2.) JL295.buildRooms.7093/RandomRoomName --> Add room name to end of file path.

		printf("%s", libPath); //Print randomly selected name (of libPath)
		
		printf("\n");
		printf("\n");
	} */
	}
}
