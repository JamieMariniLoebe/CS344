#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Error function used for reporting issues
void error(const char *msg) 
{
  perror(msg);
  exit(1);
} 

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, int portNumber)
{
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;

  // Store the port number
  address->sin_port = htons(portNumber);
  
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}


int main(int argc, char *argv[])
{
  int connectionSocket, charsRead;
  char buffer[248]; //Plaintext file 5 has more than 256**
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);
  pid_t spawnpid = -5; //Initializing for fork
  int numConnects = 0; //Num of connections
  const char* eId = "Encrypt";

  // Check usage &\n a;rgs
  if (argc < 2) 
  { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (listenSocket < 0) 
  {
    error("ERROR opening socket");
  }

  // Set up the address struct for the server socket
  setupAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    error("ERROR on binding");
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5); 
  
  // Accept a connection, blocking if one is not available until one connects (MAX: 5)
  // use semaphore/mutex while loop
  while(1)
  {
    // Accept the connection request which creates a connection socket
    connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); 

    if (connectionSocket < 0)
    {
      error("ERROR on accept");
    }

    //Child process will encrypt ciphertext
    spawnpid = fork();

    //if fork successful
    if(spawnpid == 0) {

      // check to make sure child process is communicating with enc_client
      //int confirm = send(connectionSocket, data, sizeof(data), 0); //Sending random data to confirm correct connection

      charsRead = recv(connectionSocket, buffer, sizeof(eId), 0);

      //If not correct connection
      if(charsRead != sizeof(eId) || strncmp(buffer, eId, charsRead) != 0) {
        fprintf(stderr, "SERVER - BAD IDENTIFIER\n");
        close(connectionSocket);
        return 1;
      }

      int charSent = send(connectionSocket, buffer, sizeof(eId), 0);
      close(connectionSocket);
    }

      /*
      //printf("SERVER: Connected to client running at host %d port %d\n", ntohs(clientAddress.sin_addr.s_addr), ntohs(clientAddress.sin_port));

      Child now receives plaintext and a key from enc_client via the connected socket.
      NOTE: The key passed in must be at least as big as the plaintext.
      memset(buffer, '\0', 256);

      // Read the client's message from the socket
      charsRead = recv(connectionSocket, buffer, , 0); 

      if (charsRead < 0)
      {
        error("ERROR reading from socket");
      }

      // Child will now write back the ciphertext to the enc_client process that it is connected to via the same connected socket. 

      printf("SERVER: I received this from the client: \"%s\"\n", buffer);

      // Send a Success message back to the client
      charsRead = send(connectionSocket, "I am the server, and I got your message", 39, 0); 

      if (charsRead < 0)
      {
        error("ERROR writing to socket");
      }

      // Close the connection socket for this client
      close(connectionSocket); 
    }

    //If in parent
    else if (spawnpid > 0)
    {
      childpid[numConnects] = spawnpid;
    }
    
    else {
      //Error handling
    }

  // Close the listening socket
  close(listenSocket); */
    }
  return 0;
}