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

int findchar(char a[],char s)
{
	int i=0;
	while(a[i]!='\0')
	{
    printf("%s\n", a[i]);
		if(s==a[i]) 
    {
		  return 1;
    }
		i++;
	}
	return 0;
}

char *cipherFunc(char pText[], char key[]) {

  int len = strlen(pText);
  int index = 0;
  int count = 0;
  char alpha[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  int numText[70000] = {}; // Hold char number values
  int numKey[70000] = {}; // Hold char number values

  int cipherNums[70000] = {}; 
  char *cipherChars = NULL;
  cipherChars = malloc(70000 * sizeof(cipherChars));

  cipherChars = malloc(70000 * sizeof(char));

  int *charVal = 0;

  while(pText[index] != '\0') 
  {
    /*if((strchr(alpha, pText[index])) == NULL) 
    {
      printf("Error....\n");
      break;
    } */
    numText[count] = (int)(strchr(alpha, pText[index]) - alpha);
    index++;
    count++;
  }

  //printf("\n");
  index = 0;
  count = 0;

  while(key[index] != '\0') 
  {
    /*if((strchr(alpha, key[index])) == NULL) 
    {
      printf("Error....\n");
      break;
    } */

    /*printf("Key index: %c\n", key[index]);
    if(findchar(alpha, key[index]) == 0) {
      printf("Error....\n");
      break;
    }*/

    numKey[count] = (int)(strchr(alpha, key[index]) - alpha);
    index++;
    count++;
  }
  printf("\n");

  index = 0;
  int total = 0; //Hold total after adding 2 indexes

  //Add key and message
  while(index < len) {
    total = numText[index] + numKey[index];
    total = total % 26;

    cipherNums[index] = total;
    index++;
  }
  printf("\n");
  index = 0;

  /* convert encrypted nums to corresponding chars */
  while(index < len) {
    cipherChars[index] = alpha[cipherNums[index]];
    //printf("Char Final: %c\n", cipherChars[index]);
    index++;
  }
  return cipherChars;
}


int main(int argc, char *argv[])
{
  int connectionSocket, charsRead;
  char buffer[100000]; //Plaintext file has more than 256**
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

    int bytesRead;
    int textSize;

    //if fork successful
    if(spawnpid == 0) {

      // check to make sure child process is communicating with enc_client
      //int confirm = send(connectionSocket, data, sizeof(data), 0); //Sending random data to confirm correct connection

      //Receiving eid
      charsRead = recv(connectionSocket, buffer, sizeof(eId), 0);
      //printf("Receiving....'%s'\n", buffer);

      //If not correct connection
      if(charsRead != sizeof(eId) || strncmp(buffer, eId, charsRead) != 0) {
        fprintf(stderr, "SERVER - BAD IDENTIFIER\n");
        close(connectionSocket);
        exit(1);
      }

      //Sending eid
      int charSent = send(connectionSocket, buffer, sizeof(eId), 0);
      //printf("Sending '%s'\n", buffer);

      memset(buffer, '\0', sizeof(buffer));
      
      /******************** READ IN SIZE OF PLAINTEXT ************************/
      charsRead = recv(connectionSocket, buffer, sizeof(buffer), 0);
      //printf("Receiving size of Plaintext: '%s'\n\n", buffer);

      //printf("Buffer: %s\n", buffer);
      textSize = atoi(buffer);

      /******************** SEND BACK SIZE OF PLAINTEXT ************************/
      //Sending size of plaintext back
      charSent = send(connectionSocket, buffer, sizeof(buffer), 0);
      //printf("Sending size of Plaintext back: '%s'\n\n", buffer);


      int bytesRec = 0;
      int bytesLeft = textSize;
      int index = 0;
      char *bufPtr = buffer;
      char pText[70000];
      char key[70000];
      int total = 0;

      /******************** GET/RECEIVE PLAINTEXT ************************/
      while(bytesLeft > 0) 
      {
        /******************** GET PLAINTEXT ************************/
        charsRead = recv(connectionSocket, buffer, sizeof(buffer), 0);
        //printf("Receiving Plaintext: '%s'\n\n", buffer);

        //memset(buffer, '\0', sizeof(buffer));


        /******************** SEND BACK PLAINTEXT ************************/
        
        charSent = send(connectionSocket, buffer, sizeof(buffer), 0);

        strcat(pText, buffer);

        memset(buffer, '\0', sizeof(buffer));

        bytesLeft -= charsRead;
        //printf("Bytes Received: %d\n", bytesRec);
        //printf("Bytes Left: %d\n", bytesLeft);
      }
      //printf("pText: '%s'\n\n", pText);

      memset(buffer, '\0', sizeof(buffer));


      /******************** READ IN SIZE OF KEY ************************/
      //Read in SIZE of key
      charsRead = recv(connectionSocket, buffer, sizeof(buffer), 0);
      //printf("Receiving size of Key: '%s'\n\n", buffer);

      int keySize = atoi(buffer);
      //printf("keySize: %d\n", keySize);

      /******************** SEND BACK SIZE OF KEY ************************/
      charSent = send(connectionSocket, buffer, sizeof(buffer), 0);
      //printf("Sending size of key back: '%s'\n\n", buffer);


      bytesLeft = keySize;
      //printf("KeySize before ReceiveKey Func: %d\n", bytesLeft);

      //******************** GET/SEND KEY ************************/

      //while(bytesLeft > 0) 
      //{

        //******************** GET KEY ************************/
      charsRead = recv(connectionSocket, buffer, sizeof(buffer), 0);
        //printf("Receiving Key: '%s'\n\n", buffer);


        /******************** SEND BACK KEY ************************/
      charSent = send(connectionSocket, buffer, sizeof(buffer), 0);
      printf("Sending Key back: '%s'\n\n", buffer);

      strcat(key, buffer);

        //memset(buffer, '\0', sizeof(buffer));

        //bytesLeft -= charsRead;
        //printf("Bytes Received: %d\n", bytesRec);
        //printf("Bytes Left: %d\n", bytesLeft);
      //}

      //printf("Key: '%s'\n", key);
      //charSent = send(connectionSocket, key, sizeof(key), 0);
      //printf("Sending....'%s'\n", key);

      //sleep(5);

      char *encryptData = cipherFunc(pText, key);
      charSent = send(connectionSocket, encryptData, sizeof(encryptData), 0);

      char *dataPtr = encryptData;
      printf("EncData: '%s'\n", encryptData);
      printf("DataPtr: '%s'\n", dataPtr);

      int dataLen = strlen(encryptData);

      //charSent = send(connectionSocket, buffer, sizeof(buffer), 0);
      //printf("Sending data back: '%s'\n\n", buffer);

      //memset(buffer, '\0', sizeof(buffer));

      //dataLen -= charsRead;
      //printf("Bytes Received: %d\n", bytesRec);
      //printf("Bytes Left: %d\n", dataLen);

      //char dataSize[10] = {}; 
      //sprintf(dataSize, "%d", dataLen);

      //printf("'%s'\n", dataLen);
      //charSent = send(connectionSocket, dataSize, sizeof(buffer), 0);

      //while(dataLen > 0) 
      //{

        /******************** SEND ENCRYPTED DATA ************************/
        
      //}


      //send size of encrypted data
      //charSent = send(connectionSocket, dataSize, sizeof(dataSize), 0);

      //printf("Encrypt: '%s'\n", encryptData);

      //charSent = send(connectionSocket, keySize, sizeof(encryptData), 0);
      //bytesLeft = dataLen;

      /*while(bytesLeft > 0) {
        charSent = send(connectionSocket, dataPtr, sizeof(encryptData), 0);
        //printf("Encrypt sending....'%s'\n", dataPtr);
        dataPtr += charSent;
        //bytesLeft -= charSent;
      } */

      //charsRead= recv(connectionSocket, buffer, sizeof(buffer), 0);

      /*
      //Sending size of plaintext back
      charSent = send(connectionSocket, buffer, sizeof(buffer), 0);
      printf("Sending '%s'\n", buffer);

      //int textSize = sizeof(buffer);
*/

      //ENCRYPT TEXT HERE

      //Sending back ciphertext
      //charSent = send(connectionSocket, buffer, sizeof(buffer), 0);
      //printf("Sending '%s'\n", buffer);*/




      close(connectionSocket);
    }

      /*

      Child now receives plaintext and a key from enc_client via the connected socket.
      NOTE: The key passed in must be at least as big as the plaintext.

      // Read the client's message from the socket

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
