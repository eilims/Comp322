#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int count = 0;

void* print6(void* arg)
{
   count++;
   printf("Thread Void* Address: %d\n", arg);
   printf("Thread1 Int Address: %d\n", (int*)arg);
   printf("Thread1 Int Value: %d\n", *(int*)arg);
}

//We may only pass a void pointer
void* print5(void* arg)
{
   count++;
   //We cast then dereference to get our true value with no errors 
   printf("Thread0 Void* Address: %d\n", arg);
   printf("Thread0 Int* Address: %d\n", (int**)arg);
   printf("Thread0 Int Address: %d\n", (int*)*(int**)arg);
   printf("Thread0 Int Value: %d\n", *(int*)*(int**)arg);
   //We must cast the return type to void*.
   //It is generic so we have no issues
   //Upon return we must cast back into what we need
   //Doubles are 8 bytes
   //Pointers are 4 bytes
   //We have to convert first then we are able to cast
   double n = 10.01;
   double* q = &n;
   //We wont actually use this return as we cannot return a local variable
   return (void*)q;
}

int main(int argc, char** argv)
{
   //Testing void pointers
   //Create function pointer for pthread to execute
   //Must use void* to account for any return type
   void* (*fg)();
   //Assign function
   fg = print5;
   //End void pointer testing
   
   printf("Start thread creation\n");
   //Declare pthread object
   pthread_t pthread0;
   pthread_t pthread1;
   //Prep argument list for function
   //Value
   int argument = 5;
   //Pointer to value
   int* argumentP = &argument;
   printf("Parent: %d\n", argumentP);
   //The thread object we will be using,
   //attribute config,
   //function to execute(function pointer),
   //argument list (must be a pointer) 
   //if we pass a pointer => single pointer, int
   //if we pass an address of a pointer => double pointer, single pointer, int
   int error0 = pthread_create(&pthread0, NULL, &print5, &argumentP);
   printf("Counter: %d\n", count);
   //Example of single pointer
   int error1 = pthread_create(&pthread1, NULL, &print6, argumentP);
   printf("Counter: %d\n", count);
   if (error0||error1)
     {
	printf("Thread did not start\n");
	return 1;
     }
   pthread_join(pthread0, NULL);
   printf("Counter: %d\n", count);
   pthread_join(pthread1, NULL);
   printf("Counter: %d\n", count);
   printf("Im the main method");
   return 0;
}