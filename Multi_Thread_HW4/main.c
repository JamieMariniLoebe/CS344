#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h> // must link with -lm

// Size of the buffers, hold no more than 50 lines of 1000 chars
#define MAX_LINES 50
#define CHARS 1000 //Account for null terminator
#define CHUNKSIZE 80

/* Buffer 1, shared resource between input thread and line separator thread. */
char buffer_1[MAX_LINES*CHARS]; 

char* input_thread_ptr = buffer_1; 
char* newLineProc = buffer_1;
char* plusSignProc = buffer_1;



int prod_idx = 0;
int con_idx = 0;
int stopCond = 0;
int plusSignCount = 0;
int fullBuf = 0;

//Initialize the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Initialize condiiton variables
pthread_cond_t inputReceived = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER; 
pthread_cond_t stopProc = PTHREAD_COND_INITIALIZER;
pthread_cond_t outputNow = PTHREAD_COND_INITIALIZER;


/*Reads in lines of characters from the standard input*/
void* getInput(void* args){
    /*-->Read in lines of chars from user input )
    -->Lines of input: A sequence of the allowed characters ASCII 32-126 that does NOT include a line separator.
    -->Followed by a line separator (\n). 
    -->Process lines until encounter input line with only 'STOP' chars followed immediately by a line spearator */

    char* stop = "STOP\n"; //When to stop reading input
    size_t len = sizeof(buffer_1)/sizeof(buffer_1[0]); 
    ssize_t lineSize = 0;
    int flag = 0;
    int lineNum = 0;

    /*Read entire line from stdin, store in buffer_1*/

    while(stopCond == 0) 
    {
        //printf("%d\n", input_thread_ptr);
        lineSize = getline(&input_thread_ptr, &len, stdin);

        pthread_mutex_lock(&mutex);
        //printf("%d, %d, %d: %s\n", len, stopCond, lineSize, input_thread_ptr);
        if(lineSize >= 80) {
            fullBuf = 1;
        }

        if(strcmp(input_thread_ptr, stop) == 0) {
            //printf("Stop: %d\n", stopCond);
            stopCond = 1;
            //printf("Stop: %d\n", stopCond);
        }
        else 
        {
            input_thread_ptr += lineSize; //Move to next idx
            len -= lineSize; //Decreasing buffer size
        }
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&inputReceived);
    }
    pthread_cond_signal(&stopProc); //Signal to stop processing reached and begin outputting data
    return NULL;
}

/*Replaces every line separator in the input by a space*/
void* replaceNewLines(void* args){
    int stop = 0;

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&inputReceived, &mutex); //wait for input 

    while(!stop) {
        
        // Process up to 80 chars and only while newLineProc < input_thread_ptr
        for(int i=0; i<CHUNKSIZE && newLineProc < input_thread_ptr; i++) {
            if(*newLineProc == '\n') {
                *newLineProc = ' ';
            }
            newLineProc++;
        }
        if(stopCond == 1 && newLineProc == input_thread_ptr) {
            stop = 1;
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

/*Replaces every pair of plus signs with '^' i.e., "++" replace with a "^"*/
void* replacePlusSigns(void* args){
    int stop = 0;

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&inputReceived, &mutex); //wait for input 


    while(!stop) {

        //printf("%d == %d, %d\n", plusSignProc, input_thread_ptr, stopCond);

        // Process up to 80 chars and only while plusSignProc < input_thread_ptr
        for(int i=0; i<CHUNKSIZE && plusSignProc < input_thread_ptr; i++) {
            if(strncmp(plusSignProc, "++", 2) == 0) { //Look for 2 plus signs
                *plusSignProc = '^';

                plusSignCount++;

                char* ptr = plusSignProc + 1;

                while(ptr < input_thread_ptr) {
                    *ptr = *(ptr+1);
                    ptr++;
                }
                input_thread_ptr--; //Shifting input_thread_ptr by 1 to accomoadate for removal of 2nd +
            }
            plusSignProc++;
        }
        if(stopCond == 1 && plusSignProc >= input_thread_ptr) {
            stop = 1;
        }
        pthread_cond_signal(&outputNow);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

/*Write this processed data to standard output as lines of exactly 80 characters*/
void* outputText(void* args){
    char* outputPtr = buffer_1;
    char finalBuf[82] = { 0 };
    int i = 0;
    int size = 80;

    pthread_mutex_lock(&mutex);
    //printf("\n=======Break=======\n");
    //printf("FULLBUF: %d", fullBuf);

    /*while(fullBuf) {
        for(int i=0; i<81; i++) {

        }
    }*/

    printf("input: %d, output: %d\n", input_thread_ptr, outputPtr);

    pthread_cond_wait(&outputNow, &mutex);

    while(outputPtr < input_thread_ptr) { //
        printf("\n========BREAK========\n");
        memset(finalBuf, '\0', sizeof(finalBuf));
        
        //printf("SIZE: %d\n", size);

        if((int)(input_thread_ptr - outputPtr) < 80) { //Checking buffer size
            size = (int)(input_thread_ptr - outputPtr);
            printf("SIZE: %d\n", size);
        } 


        memcpy(finalBuf, outputPtr, size);
        outputPtr += size;
        printf("SIZE: %d\n", size);
        printf("%s", finalBuf);
        pthread_mutex_unlock(&mutex);
    }
    printf("Ending program\n\n");
    return NULL;
}

int main(void) {

    pthread_t input, newLines, plusSigns, output;

    pthread_create(&input, NULL, getInput, NULL);

    pthread_create(&newLines, NULL, replaceNewLines, NULL);

    pthread_create(&plusSigns, NULL, replacePlusSigns, NULL);

    pthread_create(&output, NULL, outputText, NULL);

    pthread_join(input, NULL);
    pthread_join(newLines, NULL);
    pthread_join(plusSigns, NULL);
    pthread_join(output, NULL); 

    /*getInput();

    replaceNewLines();

    replacePlusSigns();

    outputText();*/
    
    return 0;
}