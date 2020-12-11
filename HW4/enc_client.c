#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

// Error function used for reporting issues
void error(const char *msg) 
{ 
  perror(msg); 
  exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, int portNumber, char* hostname)
{
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;

  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  struct hostent* hostInfo = gethostbyname(hostname); 

  if (hostInfo == NULL) 
  { 
    fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
    exit(0); 
  }

  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char*) &address->sin_addr.s_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
}

/* Confirm key has approved chars + not shorter than plaintext */
void checkChars(char *textverify, char *charArr) 
{
  // text to verify, array of acceptable chars, length of plaintext

  int fLen = strlen(textverify);
  int arrLen = strlen(charArr);
  int i,j,valid;

  /* Loop thru each char of key, check for validity */
  for(i=0; i<fLen; i++) //Looping thru each char in key
  {
  valid = 0;
    for(j=0; j<arrLen; j++) //Looping thru each char of acceptable chars
    {
      if(textverify[i] == charArr[j]) 
      {
        valid = 1;
      }
    }
    if(valid == 0) {
      fprintf(stderr, "Bad character(s). Exiting....\n");
      exit(1);
    }
  }
}

// Referece to Stackoverflow: https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c/3747128#3747128 

char *createBuffer(char* fileName, long *fileSize) 
{ 
  FILE *fp = NULL;
  long lSize = 0L;
  char* buffer = NULL;
  
  fp = fopen (fileName , "r");
  if(!fp) 
  {
     perror(fileName);
     exit(1);
  }

  /* int fseek(FILE *stream, long int offset, int whence */

  fseek(fp, 0L, SEEK_END); // move pointer to end of file
  lSize = ftell(fp); // ftell() --> get curr val of file position
  *fileSize = lSize; // Setting pointer to fileSize
  rewind( fp ); // Returning back to beginning of file

  /* allocate memory for entire content */
  buffer = (char *) calloc(lSize+1, sizeof(char));

  if(!buffer) 
  {
    fclose(fp); // close file
    fprintf(stderr, "memory alloc error\n"); //Error output
    exit(1);
  }

  /* read file into the buffer */
  // size_t fread(void * buffer, size_t size, size_t count, FILE * stream)
  
  if(1!=fread(buffer, lSize, 1, fp)) // Read in 1 chunk of lsize bytes
  {
    fclose(fp);
    free(buffer);
    fprintf(stderr, "Error\n");
    exit(1);
  }

  if(buffer[lSize - 1] == '\n') 
  {
    buffer[lSize - 1] = '\0'; // input null terminator in place of newline
    *fileSize -= 1; // decrement filesize
  }

  fclose(fp);
  return buffer;
}

int main(int argc, char *argv[]) 
{
  int socketFD, portNumber, charsWritten, charsRead;
  struct sockaddr_in serverAddress;
  char buffer[80000];
  char* eId = "Encrypt";
  long textLen = 0;
  long keyLen = 0;
  char goodChars[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  int bytesSent = 0;
  int bytesLeft = 0;

  // Check usage & args
  if (argc < 3) 
  { 
    fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
    exit(0); 
  }

  char *pText = createBuffer(argv[1], &textLen);
  char *key = createBuffer(argv[2], &keyLen);

  if(keyLen < textLen) 
  {
    fprintf(stderr,"Error: Key smaller than plaintext. Exiting....\n"); 
    exit(1);
  }

  /* check files for valid input */
  checkChars(key, goodChars);
  checkChars(pText, goodChars);

  // Create a socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0);

  if (socketFD < 0)
  {
    error("CLIENT: ERROR opening socket");
  }

   // Set up the server address struct
  setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
  {
    error("CLIENT: ERROR connecting");
  }
  //printf("Connected....\n");

  // sending eId
  int charSent = send(socketFD, eId, sizeof(eId), 0);
  //printf("Sending '%s'\n", eId);

  // receiving eId
  int charRecv = recv(socketFD, buffer, sizeof(eId), 0);
  //printf("Receiving '%s'\n", buffer);

  if(charRecv != sizeof(eId) || strncmp(eId, buffer, sizeof(eId))) 
  {
    fprintf(stderr, "CLIENT - BAD IDENTIFIER\n");
    close(socketFD);
    return 1;
  }

  char size[10] = {}; 
  sprintf(size, "%d", textLen);

  /******************** SEND SIZE OF PLAINTEXT ************************/
  charSent = send(socketFD, size, sizeof(size), 0);
  //printf("Sending size of Plaintext: '%s'\n\n", size);

  memset(buffer, 0, sizeof(buffer));

  /******************** GET SIZE OF PLAINTEXT BACK ************************/
  //charRecv = recv(socketFD, buffer, sizeof(size), 0);
  //printf("Receiving size of Plaintext: '%s'\n\n", buffer);

  charSent = 0;
  //printf("%d\n", textLen);
  bytesLeft = textLen;
  //printf("BYTES SENT: %d\n", bytesSent);
  //printf("BYTES LEFT BEFORE SEND PLAINTEXT: %d\n", bytesLeft);
  const char *txtPtr = pText;
  int index = 0;

  while(bytesLeft > 0)
  {
    /******************** SEND PLAINTEXT ************************/
    charSent = send(socketFD, txtPtr, sizeof(pText), 0);
    //printf("Sending PLAINTEXT: %s....\n\n", txtPtr);

    /******************** GET PLAINTEXT BACK ************************/
    if((charRecv = recv(socketFD, buffer, sizeof(buffer), 0)) >= 0) 
    {
      //printf("Receiving Plaintext Size: '%s'\n\n", buffer);
    }

    //printf("Bytes Left: %d\n", bytesLeft);

    bytesLeft -= charSent;
    txtPtr += charSent;

    //printf("Bytes Left END: %d\n", bytesLeft);
  }

  char sizeKey[10] = {}; 
  sprintf(sizeKey, "%d", keyLen);

  /******************** SEND SIZE OF KEY ************************/
  charSent = send(socketFD, sizeKey, sizeof(sizeKey), 0);
  //printf("Sending key size: '%s'\n\n", sizeKey);

  memset(buffer, '\0', sizeof(buffer));

  /******************** GET SIZE OF KEY BACK ************************/
  charRecv = recv(socketFD, buffer, sizeof(sizeKey), 0);
  printf("Receiving key size: '%s'\n\n", sizeKey);
  
  bytesLeft = keyLen;
  const char* keyPtr = key;


  //while(bytesLeft > 0)
  //{
    /******************** SEND KEY ************************/
    charSent = send(socketFD, keyPtr, sizeof(key), 0);
    printf("Sending KEY:....'%s'\n\n", keyPtr);

    //sleep(5);

    //memset(buffer, '\0', sizeof(buffer));
    //sleep(5);

    /******************** GET KEY BACK ************************/
    //charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
    //printf("Receiving key: '%s'\n\n", buffer);
    sleep(10);  

    bytesLeft -= charSent;
    keyPtr += charSent;
  //}

  //charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
  //printf("Final Data: '%s'\n", buffer);

  //while(bytesLeft > 0)
  //{
    /******************** GET DATA SIZE ************************/
    //charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
    //int dataSize = atoi(buffer);
    //printf("Size: %d", dataSize);
    //printf("Receiving key: '%s'\n", buffer);
    //sleep(10);  

    //bytesLeft -= charSent;
    //keyPtr += charSent;
  //}

  char encData[72000];

  //while(dataSize > 0)
  //{
    /******************** GET DATA ************************/
  charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
  printf("Final Data: '%s'\n", buffer);
  //strcat(encData, buffer);
    //dataSize -= charRecv;
  //}

  //printf("Final Data: '%s'\n", encData);

  //charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
  //printf("Size: %d\n", buffer);

  //bytesLeft = atoi(buffer);
  //printf("Size: %d\n", bytesLeft);

  //memset(buffer, '\0', sizeof(buffer));

  //bytesLeft = keyLen;

    //charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
    //bytesLeft -= charRecv;

  //memset(buffer, '\0', sizeof(buffer));

    //charRecv = recv(socketFD, buffer, sizeof(buffer), 0);
    //bytesLeft -= charRecv;

  //printf("Buffer: %d\n", buffer);

  //printf("'%s'\n", buffer);

  //printf("Successful!\n");

  close(socketFD);

  // Get input message from user
  //printf("CLIENT: Enter text to send to the server, and then hit enter: ");

  // Clear out the buffer array
  //memset(buffer, '\0', sizeof(buffer));

  // Get input from the user, trunc to buffer - 1 chars, leaving \0
  //fgets(buffer, sizeof(buffer) - 1, stdin);

  // Remove the trailing \n that fgets adds
  //buffer[strcspn(buffer, "\n")] = '\0'; 

  // Send message to server
  // Write to the server
  //charsWritten = send(socketFD, buffer, strlen(buffer), 0); 

  /*if (charsWritten < 0)
  {
    error("CLIENT: ERROR writing to socket");
  }
  
  if (charsWritten < strlen(buffer))
  {
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  } 
  */

  // Get return message from server
  // Clear out the buffer again for reuse
  //memset(buffer, '\0', sizeof(buffer));
  
  // Read data from the socket, leaving \0 at end
  //charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);

  /*(if (charsRead < 0)
  {
    error("CLIENT: ERROR reading from socket");
  } 
  */
  
  //printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

  // Close the socket
  close(socketFD); 
  return 0;
}