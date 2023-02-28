/* Program to demonstrate the client in socket */


/* standard input output file */
#include <stdio.h>
#include <stdlib.h>
/* gets function */
#include <string.h>
#include <unistd.h>
/* binding */
#include <sys/socket.h>
#include <sys/types.h>
/* for connect between server and client */
#include <netinet/in.h>
/* init_addr() */
#include <arpa/inet.h>

#define PORT 4040

int main()
{
	/* variable's and structure declaration's */
	int clientSocket;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	/* socket create and verification */
	/* domain, type, protocol */
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	/* condition's to check whether the socket created or not */
	if(clientSocket < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	else
	{
		printf("Client Socket is created.\n");
	}

	/* erases the data in the n bytes of the memory starting at the location pointed to by s Arguments -> structure, size */
	bzero(&serverAddr, sizeof(serverAddr));

	// assign IP, PORT
	// AF_INET -> Address from the Internet
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* Connect the client socket to server socket */
	if( connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}

	else
	{
		printf("Connected to Server.\n");
	}

	/* Infinite loop to write */
	while(1)
	{
		/* used to erase the buffer, This function is used to set all the socket structures with null values */
		bzero(buffer, sizeof(buffer));
		printf("To server: ");
		int n=0;

		/* copy server message in the buffer */
		/* if char contains \n means jump to server */
		while ((buffer[n++] = getchar()) != '\n');

		/* And sending that buffer to server */
		write(clientSocket, buffer, sizeof(buffer));


		//condition to convert the lower case to upper cases received from client

		int i=0;
		for(i=0;buffer[i]!='\0';i++)
		{
			if (buffer[i] >= 'a' && buffer[i] <= 'z')
			{
				buffer[i]-=32;
			}
			else if(buffer[i] >= 'A' && buffer[i] <= 'Z')
			{
				buffer[i]+=32;
			}
		}
		printf("\tThe String Echoed from client : %s", buffer);


		/* used to erase the buffer, This function is used to set all the socket structures with null values */
		bzero(buffer, sizeof(buffer));

		/* read the message from server and copy it in buffer */
		read(clientSocket, buffer, sizeof(buffer));
		printf("From Server : %s", buffer);

	}
	// close the socket
	close(clientSocket);

}
