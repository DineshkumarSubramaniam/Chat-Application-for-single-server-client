/* Program to demonstrate the server in socket */

/* init_addr() */
#include <arpa/inet.h> 
/*  Official name of the host. */
#include <netdb.h>
/* standard input output file */
#include <stdio.h>
#include <stdlib.h>
/* gets function */
#include <string.h>
/* bzero() */
#include <strings.h> 
/* binding */
#include <sys/socket.h>
//read(), write(), close()
#include <unistd.h>

#define PORT 4040
int count_client = 0;


int main()
{
	/* variable and structure declaration's */
	int serverSocket;
	int newSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	/* Declaration of pid_t */
	pid_t childpid;


	/* socket create and verification */
	/* domain, type, protocol */
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Server Socket is created.\n");

	/* erases the data in the n bytes of the memory starting at the location pointed to by s Arguments -> structure, size */
	bzero(&serverAddr, sizeof(serverAddr));

	// assign IP, PORT
	// AF_INET -> Address from the Internet
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	/* Binding newly created socket to given IP and verification */
	/* BIND is a nameserver service responsible for performing domain-name-to-IP conversion on Linux-based DNS servers */
	if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))!=0)
	{
		printf("Error in binding.\n");
		exit(1);
	}
	printf("Binded to port %d\n", PORT);


	/* Now server is ready to listen and verification */
	/* 1 ->socket that will be used to accept incoming connection requests using accept */
	/* 2 -> specifies the maximum number of connections the kernel should queue for this socket */
	if(listen(serverSocket, 10) == 0)
	{
		printf("Server Listening....\n");
	}

	else
	{
		printf("Server failed to accept client \n");
	}


	/* Loop designed for chat between client and server */
	while(1)
	{
		/* Accept the data packet from client and verification */
		newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);

		/* variable to count no.of client connection */
		count_client++;
		if(newSocket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0)
		{
			close(serverSocket);

			while(1)
			{
				/* This function is used to set all the socket structures with null values */
				bzero(buffer, sizeof(buffer));

				/* read the message from client and copy it in buffer */
				read (newSocket,buffer,sizeof(buffer));

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


				/* print buffer which contains the client contents */
				printf("From client %d: %s\t To client %d : ",count_client, buffer,count_client);

				/* This function is used to set all the socket structures with null values */
				bzero(buffer, sizeof(buffer));
				int n = 0;

				/* Copy server message in the buffer */
				/* if char contains \n means jump to client */
				while ((buffer[n++] = getchar()) != '\n');

				/* And sending that buffer to client */
				write(newSocket, buffer, sizeof(buffer)); 


				/* Condition to check exit from server */
				if(strncmp("exit",buffer,4)==0)
				{
					printf("Server exit\n");
					break;
				}
			}
		}

	}

	close(newSocket);


	return 0;
}
