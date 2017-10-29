#include <stdio.h>
#include <stdlib.h>


int main (int argc, char **argv)
{
   int i;
   
   i = atoi (argv[1]);
   printf("The entered number fits %d times into 100\n" , intDivide(i));
   return 0;
}
