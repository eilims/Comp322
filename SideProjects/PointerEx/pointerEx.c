#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
   int* test;
   int original = 5;
   test = &original;
   printf("Test: %d\n", test);
   printf("Original Address: %d\n", &original);
   printf("Orignial: %d\n", original);
   printf("Test Value: %d\n", *test);
   printf("Test Address: %d\n", &test);
   *test = 6;
   printf("Modified %d", original);
   int i;
   for (i = 0; i < argc; i++)
     {
	printf("Testing? %s\n", argv[i]);
     }
   exit(0);
   return 0;
}
