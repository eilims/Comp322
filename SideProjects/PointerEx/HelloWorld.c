#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv)
{
   printf("Hello World!\n");
   pid_t pid = getpid();
   pid_t cpid = fork();
   switch (cpid) 
     {
	
      case -1:
	//Error goes here
	printf("Forking Error\n");
	exit(1);
	break;
	
      case 0:
	//Children go here
	printf("Starting exec\n");
	//Name of the program list goes first
	//NULL must always terminate the array
	char* test[2];
	test[0] = "./pointersEx";
	test[1] = "testing";
	test[2] = NULL;
	//Program to execute and an argument list
	int error = execvp("./pointerEx", test);
	if (error = -1)
	  {
	     printf("Error in exec :(\n");
	  }
	
	break;
	
      default:
	//Parent goes here
	printf("Parent\n");
	int status;
	wait(&status);
	exit(0);
     }
   
   return 0;
}
