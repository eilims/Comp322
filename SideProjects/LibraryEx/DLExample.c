#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main (int argc, char** argv)
{
   void* handle;
   //This is a symbol
   double (*sine)(double);
   char* error;
   
   //Open the math library
   handle = dlopen("libm.so.6", RTLD_LAZY);
   //Returns null if it fails. null void pointers are false
   if (!handle)
     {
	printf("Fail to open Library: %s", dlerror());
	exit(1);
     }
   
   //Load sine symbol with absolute value function
   sine = dlsym(handle, "fabs");
   //When error is NULL no error has occured
   //Otherwise dlerror returns a string containing info on fault
   if ((error = dlerror()) != NULL)
     {
	printf("Error: %s", error);
	exit(1);
     }
   
   printf("%f\n", (*sine)(-9.9));
   dlclose(handle);
   exit(0);
}
