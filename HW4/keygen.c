#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/* 
This program creates a key file of specified length. The characters in the file generated will be any of the 27 allowed characters, 
generated using the standard Unix randomization methods. Do not create spaces every five characters, as has been historically done.
Note that you specifically do not have to do any fancy random number generation: we’re not looking for cryptographically secure random
number generation. rand() is just fine. The last character keygen outputs should be a newline. Any error text must be output to stderr.

The syntax for keygen is as follows:

$ keygen keylength

where keylength is the length of the key file in characters. keygen outputs to stdout.

Here is an example run, which creates a key of 256 characters and redirects stdout a file called 
mykey (note that mykey is 257 characters long because of the newline):

$ keygen 256 > mykey

*/

int main(int argc, char *argv[]) {

    char myKey[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    srand(time(NULL)); //Seed random number

    //FILE* fp;
    int count = 0;
    int rNum;
    char *ch;
    long num;

    /*Checking for correct num of args*/ 
    if (argc != 2) {
        fprintf(stderr, "Insufficient args....\n\n");
        exit(1);
    }

    /*Parsing nums and chars, place chars in 'ch'*/
    num = strtol(argv[1], &ch, 10);

    /*If no nums input as args*/
    if(num == '\0') {
        fprintf(stderr, "Invalid input: %s\n", argv[1]);
        exit(1);
    }

    //fp = fopen("mykey", "w+");

    /*Write "num" random chars (from myKey of acceptable values) to stdout  increment count*/
    while(count < num) {
        rNum = rand() % 27; //get random val 
        fprintf(stdout, "%c", myKey[rNum]);
        count++;
    } 

    fprintf(stdout, "\n"); //Last thing keygen outputs is a newline
    //fclose(fp);
    
    return 0;
}