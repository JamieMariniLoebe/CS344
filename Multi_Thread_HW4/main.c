/**************************************************
* Name: Jamie Loebe
* Course: CS344
* Date: 11-15-20
* Description: Multi-threaded program that uses 4
* threads in a producer-consumer approach
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

// Size of the buffers, hold no more than 50 lines of 1000 chars
#define MAX_LINES 50
#define CHARS 1000 //Account for null terminator
#define CHUNKSIZE 80

/* Buffers, shared resource between threads. */
char buffer_1[MAX_LINES*CHARS]; 
char buffer_2[MAX_LINES*CHARS]; 
char buffer_3[MAX_LINES*CHARS]; 

/* Pointers to buffers */
char* input_thread_ptr = buffer_1; 
char* newLineProc = buffer_2;
char* plusSignProc = buffer_3;

/* Flags to track buffer */
int buffer1_stopped = 0;
int buffer2_stopped = 0;
int buffer3_stopped = 0;

//Initialize the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Initialize condiiton variables
pthread_cond_t buffer1Updated = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer2Updated = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer3Updated = PTHREAD_COND_INITIALIZER;
pthread_cond_t stopProc = PTHREAD_COND_INITIALIZER;

/*Reads in lines of characters from the standard input*/
void* getInput(void* args){

    char* stop = "STOP\n"; //When to stop reading input
    size_t len = sizeof(buffer_1)/sizeof(buffer_1[0]); 
    ssize_t lineSize = 0;
    int flag = 0;
    int lineNum = 0;

    /*Read entire line from stdin, store in buffer_1*/
    while(buffer1_stopped == 0) 
    {
        lineSize = getline(&input_thread_ptr, &len, stdin);

        pthread_mutex_lock(&mutex);
        if(strcmp(input_thread_ptr, stop) == 0) {
            buffer1_stopped = 1;
        }
        else 
        {
            input_thread_ptr += lineSize; //Move to next idx
            len -= lineSize; //Decreasing buffer size
        }
        /*Signal that buffer1 has been updated with data*/
        pthread_cond_signal(&buffer1Updated);
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    /* Signal to stop process*/
    pthread_cond_signal(&stopProc);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

/*Replaces every line separator in the input by a space*/
void* replaceNewLines(void* args) {
    char* sourceptr = buffer_1;

    /*Wait for buffer1 data and then process*/
    while(!buffer2_stopped) {
        pthread_mutex_lock(&mutex);
        if(!buffer1_stopped) {
            pthread_cond_wait(&buffer1Updated, &mutex); //wait for input 
        }

        /*Process up to 80 chars, change '\n' to space*/
        for(int i=0; i<CHUNKSIZE && sourceptr < input_thread_ptr; i++) {
            if(*sourceptr == '\n') {
                *newLineProc = ' ';
            }
            else {
                *newLineProc = *sourceptr;
            }
            sourceptr++;
            newLineProc++;
        }
        if(buffer1_stopped == 1 && sourceptr == input_thread_ptr) {
            buffer2_stopped = 1;
        }
        pthread_cond_signal(&buffer2Updated); //signal buffer2 has data
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/*Replaces every pair of plus signs with '^' i.e., "++" replace with a "^"*/
void* replacePlusSigns(void* args){
    char* sourceptr = buffer_2;

    while(!buffer3_stopped) {
        pthread_mutex_lock(&mutex);
        if(!buffer2_stopped) {
            pthread_cond_wait(&buffer2Updated, &mutex); //wait for data from buffer2
        }

        // Process up to 80 chars
        for(int i=0; i<CHUNKSIZE && sourceptr < newLineProc; i++) {
            if(strncmp(sourceptr, "++", 2) == 0) { //Look for 2 plus signs
                *plusSignProc = '^';
                sourceptr++; //moving past 2nd +
                i++;
            }
            else {
                *plusSignProc = *sourceptr;
            }
            plusSignProc++;
            sourceptr++;
        }
        if(buffer2_stopped == 1 && sourceptr == newLineProc) {
            buffer3_stopped = 1;
        }
        pthread_cond_signal(&buffer3Updated);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/*Write this processed data to standard output as lines of exactly 80 characters*/
void* outputText(void* args){
    char* outputPtr = buffer_3;
    int i = 0;
    int totalSize = 0;

    /*Wait until buffer1 done*/
    do {
        pthread_mutex_lock(&mutex);
        if(buffer1_stopped) {
            break;
        }
        pthread_mutex_unlock(&mutex);
    } while(1);

    /* While still data to process */
    while(outputPtr < plusSignProc || !buffer3_stopped) {
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex);

        printf("%c", *outputPtr++); //print each char
        if(++totalSize % 80 == 0) { //Check if divisible by 80
            printf("\n");
        }

        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex);
    }
    pthread_mutex_unlock(&mutex);

    if(++totalSize % 80 != 1) {
        printf("\n"); //add newline character
    }
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
    
    return 0;
}