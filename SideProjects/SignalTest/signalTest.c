#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

sig_atomic_t count;

void handler(int signal_number)
{
   count++;
}


int main(int argc, char** argv)
{
   //Create struct to allow for the easy reporogramming of signal handler
   struct sigaction sigchild;
   //Clear sigchild. Good to start with blank template
   memset(&sigchild, 0, sizeof(sigchild));
   //Set handler as callback function
   sigchild.sa_handler = &handler;
   //Using function sigaction to set handler for reception of SIGUSR1 signal 
   sigaction(SIGUSR1, &sigchild, NULL);
   
   //For loop to increment count
   int i;
   for (i = 0; i < 5; i++)
     {
	raise(SIGUSR1);
     }
   
   printf("%d\n", count);
   
   return 0;
}
