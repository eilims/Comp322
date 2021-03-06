/* A simple server in the internet domain using TCP
 *    The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg)
{
   
       perror(msg);
       exit(1);
}


int main(int argc, char *argv[])
{
   
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   if (argc < 2) 
     {
	fprintf(stderr,"ERROR, no port provided\n");
	exit(1);
     }
   
   printf("Socket Ready\n");
   //Create socket on the server side localhost
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) error("ERROR opening socket");
   printf("Success! Socket Opened!\n");
   
   //clear the server address and set configurations
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET; //setting protocol method (internet)
   serv_addr.sin_addr.s_addr = INADDR_ANY; //setting addresses to look at
   serv_addr.sin_port = htons(portno); //setting port number
   
   //bind socket to a port number
   if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");
 
   //tell server to pay attention to this particular socket
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   //accept incoming data from this new client (they are all queued up)
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
   
   if (newsockfd < 0) error("ERROR on accept");
   
   //Read number of incoming numbers
   int number_count = 0;
   n = read(newsockfd,&number_count,sizeof(number_count));
   if (n < 0) error("ERROR reading from socket");
   
   
   //Read all numbers and sum them up 
   int i;
   int sum = 0;
   for (i = 0; i < number_count; i++)
     {
	int number;
	n = read(newsockfd,&number,sizeof(number));
	
	if (n < 0) error("ERROR reading from socket");
   	
	printf("Recieved Number: %d\n",number);
	sum += number;
	
     }
   
   //Print results
   printf("The result is (Server): %d\n", sum);
   
   //Sens back the data for fun
   n = write(newsockfd,&sum,sizeof(sum));
   if (n < 0) error("ERROR sending to client");
   
   //Sleep to prevent the server from missing the client closing ack
   sleep(1);
   //good practive to close socket when done
   close(newsockfd);
   //must close server
   close(sockfd);
   exit(0);
   return 0; 
}
