#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
   pid_t pid = getpid();
   
   pid_t cpid = fork();
   if (cpid == 0)
     {
	char* temp[3];
	temp[0] = "./server";
	temp[1] = argv[2];
	temp[2] = NULL;
	int error = execvp("./server", temp);
	if (error)
	  {
	     printf("Error has occured for server :(\n");
	  }
	
	exit(0);
     }
   printf("Server started\n");
   
   
   cpid = fork();
   if (cpid == 0)
     {
	char* temp[4];
	temp[0] = "./client";
	temp[1] = argv[1];
	temp[2] = argv[2];
	temp[3] = NULL;
	int error = execvp("./client", temp);
	if (error)
	  {
	     printf("Error has occured for client :(\n");
	  }
       
	exit(0);
     }
   printf("Client started\n");

   int i;
   for (i = 0; i < 2; i++)
     {
	int returnValue;
	wait(&returnValue);
     }
   
   
   
   return 0;
}
