#include <stdio.h>


//Arrays are passed as pointers
//There is no way to get the size of the array unless it is passed in or stored somewhere
//You can store it in a struct or allocate data + 1 and store the length in therex
void arrayMod(int array[])
{
   printf("Function Array Address %d\n", array);
   int i;
   for(i = 0; i < sizeof(*array)/sizeof(int); i++)
     {
	array[i] = i;
	printf("Index %d Value %d\n", i, array[i]);
     }
}


int main(int argc, char** argv)
{
   
   int array[4];
   arrayMod(array);
   printf("Main Array Address %d\n", array);
   int i;
   for(i = 0; i < sizeof(array)/sizeof(int); i++)
     {
	array[i] = i;
	printf("Index %d Value %d\n", i, array[i]);
     }
   
}
