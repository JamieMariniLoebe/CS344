#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "loebej.buildrooms.h"

int main() {
    srand(time(NULL)); //Seed random number


    char* newDirectory = createDirectory();
    createRooms(newDirectory);

    //createRooms(newDirectory);

    return 0;

}