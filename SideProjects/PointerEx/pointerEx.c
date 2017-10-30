#include <stdio.h>

int main(int argc, char* argv)
{
   int* test;
   int original = 5;
   test = &original;
   printf("Test: %d\n", test);
   printf("Original Address: %d\n", &original);
   printf("Orignial: %d\n", original);
   printf("Test Value: %d\n", *test);
   printf("Test Address: %d\n", &test);
   return 0;
}
