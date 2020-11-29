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

//Referece to Stackoverflow: https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c/3747128#3747128 

char* createBuffer(char* fileName, int* fileSize) {
  FILE *fd = NULL;
  long lineSize = 0L;
  char* buffer = NULL;

  fp = fopen (fileName , "r" );
  if(!fp) {
     perror(fileName);
     exit(1);
  }

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  fileSize = lSize;
  rewind( fp );

  /* allocate memory for entire content */
  buffer = calloc( 1, lSize+1 );
  if( !buffer ) {
    fclose(fp);
    fputs("memory alloc fails",stderr);
    exit(1);
  }

  /* copy the file into the buffer */
  if(1!=fread( buffer , lSize, 1 , fp)) {
    fclose(fp);
    free(buffer);
    fputs("entire read fails",stderr);
    exit(1);
  }

  /* do your work here, buffer is a string contains the whole text */
  fclose(fp);
  return buffer;
}

int main(int argc, char *argv[]) 
{
  int socketFD, portNumber, charsWritten, charsRead;
  struct sockaddr_in serverAddress;
  char buffer[256];
  char* eId = "Encrypt";

  // Check usage & args
  if (argc < 3) 
  { 
    fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
    exit(0); 
  }

  FILE *fd = fopen(argv[2], "r");
  int byte;

  char* key = getline(&buffer, sizeof(buffer), fd);

  while(c = fgetc(fd) )

  // Create a socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0);

  if (socketFD < 0)
  {
    error("CLIENT: ERROR opening socket");
  }

   // Set up the server address struct
  setupAddressStruct(&serverAddress, atoi(argv[2]), argv[1]);

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
  {
    error("CLIENT: ERROR connecting");
  }
  printf("Connected....\n");

  int charSent = send(socketFD, eId, sizeof(eId), 0);
  printf("Sending....\n");

  int charRecv = recv(socketFD, buffer, sizeof(eId), 0);
  printf("Receiving....\n");

  if(charRecv != sizeof(eId) || strncmp(eId, buffer, sizeof(eId))) {
    fprintf(stderr, "CLIENT - BAD IDENTIFIER\n");
    close(socketFD);
    return 1;
  }

  printf("Successful!\n");

  close(socketFD);

  /*// Get input message from user
  printf("CLIENT: Enter text to send to the server, and then hit enter: ");

  // Clear out the buffer array
  memset(buffer, '\0', sizeof(buffer));

  // Get input from the user, trunc to buffer - 1 chars, leaving \0
  fgets(buffer, sizeof(buffer) - 1, stdin);

  // Remove the trailing \n that fgets adds
  buffer[strcspn(buffer, "\n")] = '\0'; 

  // Send message to server
  // Write to the server
  charsWritten = send(socketFD, buffer, strlen(buffer), 0); 

  if (charsWritten < 0)
  {
    error("CLIENT: ERROR writing to socket");
  }
  
  if (charsWritten < strlen(buffer))
  {
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }

  // Get return message from server
  // Clear out the buffer again for reuse
  memset(buffer, '\0', sizeof(buffer));
  
  // Read data from the socket, leaving \0 at end
  charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);

  if (charsRead < 0)
  {
    error("CLIENT: ERROR reading from socket");
  }
  
  printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
*/
  // Close the socket
  close(socketFD); 
  return 0;
}