#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>

//Global Variables

//Pipe Setup
//Send data on currentTemp and setTemp
int tempToParent[2];
int parentTempReq[2];
int tempToChild[2];
//Send data on whether to exit or not
int status[2];
//Send data on whether to cool or heat
int direction[2];
//Prevent child from spamming reads and causing a lock
int allowRead[2];


//Variables for temperature operation
double currentTemp = 75.0;
double setTemp = 75.0;

//0 => cool, else heat
int cool = 1;

//1 => continue, 0 => exit
int quit = 1;

int childStatus = 1;

int request = 0;

//Start Code
void interface()
{
   int choice;
   
   //Send Set temperature to child process
   close(tempToChild[0]);
   write(tempToChild[1], &setTemp, sizeof(setTemp));
   
   //Send current cool/heat setting
   write(direction[1], &cool, sizeof(cool));
   
  
   printf("Set Temperature: %f\n", setTemp);
   printf("Current Temperature: %f\n", currentTemp);
   printf("Main Menu:\n");
   printf("1) Refresh Current Temperature\n");
   printf("2) Change Mode\n");
   printf("3) Change Set Temperature\n");
   printf("4) Quit\n");
   
   //Get input
   scanf("%d", &choice);
   switch (choice)
     {	
      case 1:
	//Reprint main menu
	break;

      case 2:
	printf("Mode Menu\n");
	printf("Current Mode: %d\n", cool);
	printf("0) Heat\n");
	printf("1) Cool\n");
	scanf("%d", &cool);
	break;
	
      case 3:
	printf("Enter new Temperature\n");
	scanf("%lf", &setTemp);
	break;
	
      case 4:
	quit = 0;	
	break;
     }
   
    //Send current status
   write(status[1], &quit, sizeof(quit));
   
   //Request Temperature Update
   write(parentTempReq[1], &request, sizeof(request)); 
	
   //Loop to make sure we have the most up to date temperature
   read(tempToParent[0], &currentTemp , sizeof(currentTemp));
   
   printf("\n");
}

double changeTemperature()
{
   //Read current set temperature
   close(tempToChild[1]);
   read(tempToChild[0], &setTemp, sizeof(setTemp)); 
 
   //Read current mode setting
   close(direction[1]);
   read(direction[0], &cool, sizeof(cool));
   
   while (!request) 
     {
	sleep(2);
	//Determine whether to cool or heat the home
	if (cool) 
	  {
	     //Cool
	     if (setTemp < currentTemp)
	       {
		  currentTemp -= 0.1; 
	       }	
	  }
	else 
	  {
	     if (setTemp > currentTemp)
	       {
		  currentTemp += 0.1;
	       }	
	  }
	
	//Read if parent is requesting temperature
	close(parentTempReq[1]);
	read(parentTempReq[0], &request, sizeof(request));
	
	if (request) 
	  {
	     //Send current temperature
	     close(tempToParent[0]);
	     write(tempToParent[1], &currentTemp, sizeof(currentTemp));
	  }
	
     }
   request = 0;
 
   //Read current status
   close(status[1]);
   read(status[0], &quit, sizeof(quit));
  
}
   

int main(int argc, char** argv)
{
   
   
   //Pipe Setup
   //Pipe declaration
   pipe(tempToParent);
   pipe(tempToChild);
   pipe(status);
   pipe(direction);
   pipe(parentTempReq);
   
   //Create child to change temperature
   pid_t pid = getpid();
   pid_t cpid = fork();
   assert(cpid >= 0);
   
   //Exit if error
   if(cpid == -1)
     {
	printf("Error! Please restart the application\n");
	quit = 0;
     }
   else if (cpid == 0)
     {
	//Set parentTempReq to be a nonblocking read
	int flags = fcntl(parentTempReq[0], F_GETFD);
	flags |= O_NONBLOCK;
	int retval = fcntl(parentTempReq[0], F_SETFL, flags);
	if (retval == -1) 
	  {
	     quit = 0;
	  }
	
     }
   
   
   while (quit)
     {
	if (cpid > 0)
	  {
	    request = 1;
	    interface();
	  }
	else 
	  {
	     changeTemperature();
	  }
	
		
     }
   
   //Closing program statements
   if (cpid >0)
     {
	int parentStatus;
	wait(&parentStatus);
     }
   else 
     {
	exit(1);
     }
   system("ps -H");
   exit(0);
   return 0;
}
