#include <stdio.h>
#include <string.h>

int main (int argc, char** argv)
{
   if (argc != 1) 
     {
	printf("Not + 1: %d\n", strlen(argv[1]));
	printf("+ 1: %d\n", strlen(argv[1]) + 1);
     }
   
   
}
