#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>

void error(char *msg)
{
   
       perror(msg);
       exit(0);
}


int main(int argc, char *argv[])
{
   
   int sockfd, portno, n;
   
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   printf("Client is working\n");
   
   char buffer[256];
       if (argc < 3) 
     {
	
	fprintf(stderr,"usage %s hostname port\n", argv[0]);
	exit(1);
     }
   
   //Reading parameters
   portno = atoi(argv[2]);
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) error("ERROR opening socket");
   
   //connecting to localhost
   server = gethostbyname(argv[1]);
   if (server == NULL) 
     {
	fprintf(stderr,"ERROR, no such host\n");
	exit(1);
     }
   
   //zero out server address
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   //set address configuration
   serv_addr.sin_family = AF_INET; //use internet protocol
   bcopy((char *)server->h_addr, 
	 (char *)&serv_addr.sin_addr.s_addr,
	 server->h_length); //magic?
   serv_addr.sin_port = htons(portno); //set port number
   
   //connect to server on appropriate port
   if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");
   
   //get number of numbers and send it to the server
   printf("Please enter amoutn of number to sum: ");
   int number_count;
   scanf("%d", &number_count);
   n = write(sockfd,&number_count,sizeof(number_count));
   if (n < 0) error("ERROR writing to server");
   
   //read all numbers
   int i;
   for (i = 0; i < number_count; i++)
     {
	int number;
	printf("Please enter a number:\n");
	scanf("%d", &number);
	n = write(sockfd,&number,sizeof(number));
	if (n < 0) error("ERROR writing to server");
	
     }
   
   //Recieve the sum for practice
   int sum;
   n = read(sockfd,&sum,sizeof(sum));
   if (n < 0) error("ERROR reading from socket");
   printf("The sum is (Client): %d\n",sum);
   
   //Close socket for practice
   close(sockfd);
   
   exit(0);
   return 0;
}
