#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
   time_t startTime = time(0);
   pid_t cpid = getpid();
   pid_t pid = getpid();
   printf("Parent PID: %d\n", cpid);
   int i;
   for (i = 1; cpid != 0 &&  i < argc; i++)
     {
	cpid = fork();
	if (cpid == 0) 
	  {
	    printf("Filename: %s             PID: %d\n", argv[i], getpid());
	    sleep(1);
	    exit(1);
	  } else if (cpid < 0) 
	  {
	     printf("Error");
	     exit(0);
	  }
      }
   int status;
   int count = 0;
   while (count < (argc-1))
     {
	printf("Status: %d", status);
	wait(&status);
	count++;
     }
   
   if (cpid = pid)
     {
	printf("%d\n", pid);
	system("ps -H");
     }
   
   time_t endTime = time(0);
   printf("Running time: %ld\n", (endTime - startTime));
   exit(0);
   return 0;
}
