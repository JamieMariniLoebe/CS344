#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "files.h"

#define PREFIX "movies_"

int main() {
    srand(time(NULL)); //Seed random number
    

    menu();
    
    return 0;

}