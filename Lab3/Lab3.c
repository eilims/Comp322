#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>


int pd[2];

//Types
//2 = owner
//1 = group
//0 = general 
void checkPermissions(int st_mode, int type)
{
   //printf("st_mode : %d\n",(st_mode));
   
   //Getting permissions of st_mode for the user type
   st_mode = st_mode >> (3*type);
   
   if (st_mode & 1)
     {
	printf("Execute ");
     }
   if (st_mode & 2)
     {
	printf("Write ");
     }
   if (st_mode & 4)
     {
	printf("Read ");
     }
   printf("\n \n"); 
}



int main(int argc, char *argv[])
{
   //Pipe to control writing
   pipe(pd);
   
   //Keep track of running time
   time_t startTime = time(0);
   
   pid_t cpid = getpid();
   printf("Parent PID: %d\n", cpid);
   
   //Get current user's: ID and group
   int userID = getuid();
   int userGroupID = getgid();
   printf("UID: %d\n", userID);
   printf("GID: %d\n", userGroupID);
	     
   //Variable for loop
   int i;
   int c = 1;
   write(pd[1], &c, sizeof(c));
   
   //Exit loop when all files are accounted for with a unique process
   for (i = 1; i < argc; i++)
     {
	cpid = fork();
	//Child process
	if (cpid == 0) 
	  {
	    read(pd[0], &c, sizeof(c));
	    printf("\nFilename: %s\n", argv[i]);
	     
	    //For this file
	    //Declare and instantiate (struct stat) fileData in stack
	    struct stat fileData;
	    //Populates stat struct at address of fileData
	    stat(argv[i], &fileData);
	     
	    //For this file
	    //Declare and instantiate (struct passwd) password in stack
	    struct passwd* password;
	    //Populate passwd at address of password
	    // password is also a pointer
	    //getpwuid returns a pointer 
	    password = getpwuid(fileData.st_uid);
	    
	    printf("Directory: %s\n", password->pw_dir); 
	    
	    //Checking permissions
	    if (userID == fileData.st_uid)
	       {
		  printf("You have owner permissions: ");
		  checkPermissions(fileData.st_mode, 2);
	       }
	    else if (userGroupID == fileData.st_gid)
	       {
		  printf("You have group permissions: ");
		  checkPermissions(fileData.st_mode, 1);
	       }
	    else 
	       {
		  printf("You have general permissions: ");
		  checkPermissions(fileData.st_mode, 0);
	       }
	    write(pd[1], &c, sizeof(c));
	    //Change sleep time to verify functioning parallel process
	    sleep(1);
	    exit(1);
	  }
	//Error Catch
	else if (cpid < 0) 
	  {
	     printf("Error");
	     exit(0);
	  }
     }
   
   
   int status;
   int count = 0;
   
   //Wait until all children have exited
   while (count < (argc-1))
     {
	wait(&status);
	count++;
     }
   
   //Final print statement
   system("ps -H");
  
   //Conclude running time
   time_t endTime = time(0);
   printf("Running time: %ld\n", (endTime - startTime));
   //Close parent process
   exit(0);
   return 0;
}
