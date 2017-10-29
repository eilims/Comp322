#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv)
{
   extern char** environ;
   char** var;
   unsetenv("ALIOSHA");
   setenv("ALIOSHA", "alive", 0);
   for (var = environ; *var != NULL; ++var)
     {
	//var => memory address of memory address of start of char[]
	//*var => memory address of start of char []
	//**var => start of char[]
	printf("Index is: %d \n Variable is: %s\n", var, *var);
     }
   unsetenv("ALIOSHA");
   for (var = environ; *var != NULL; ++var)
     {
	printf("Index is: %d \n Letter: %d \n Variable is: %s\n", var, **var,  *var);
     }
   return 0;
}
