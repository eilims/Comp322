#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//Conditional Variable declarations
//Barber is/is not cutting hair
pthread_cond_t barber_cv;

pthread_cond_t seat_cv;

//Mutex declarations
//Lock on seats
pthread_mutex_t seat_mutex;
//Lock on barber
pthread_mutex_t barber_mutex;

int barber_working[];

int free_seat_count;
int max_seat_count;

int barber_count;
int customer_count;


//Customer thread function
void* customer_run(void* arg)
{
   int check_in_time = (rand()%10) + 1;
   printf("Customer %d! I will appear in %d time\n", syscall(SYS_gettid), check_in_time);
   
   //Sleep until check in time
   sleep(check_in_time);
   
   //Lock on seat
   pthread_mutex_lock(&seat_mutex);
   //Seats are still free
   if (free_seat_count > 0)
     {
	//One seat is now occupied
	free_seat_count--;
	pthread_cond_signal(&seat_cv);
	pthread_mutex_unlock(&seat_mutex);
	
	//Lock barber to check if he is busy
	pthread_mutex_lock(&barber_mutex);
	//Wait until the barber is not working
	while(!barber_working[0])
	  {
	     //unlocks and relocks mutex when called
	     pthread_cond_wait(&barber_cv, &barber_mutex);
	  }
	//unlock barber
	pthread_mutex_unlock(&barber_mutex);
	printf("Customer %d hair is cut\n", syscall(SYS_gettid));
     }
   //Seats are all filled
   else 
     {
	pthread_mutex_unlock(&seat_mutex);
	printf("No more seats! I am Customer %d\n", syscall(SYS_gettid));
     }
   
}


//Barber thread function
void* barber_run(void* arg)
{
   int cut_time = (rand()%3) + 1;
   printf("Barber %d! It takes me %d time to cut hair\n", syscall(SYS_gettid), cut_time);
 
   
   //Remove one thread from the seat
   pthread_mutex_lock(&seat_mutex);
   free_seat_count++;
   pthread_mutex_unlock(&seat_mutex);
   
   //Notify all waiting threads that the barber is busy
   pthread_mutex_lock(&barber_mutex);
   barber_working[syscall(SYS_gettid)%barber_count] = 1;
   pthread_cond_broadcast(&barber_cv);
   pthread_mutex_unlock(&barber_mutex);
   
   //Sleep for cutting time
   sleep(cut_time);
   
   //Notify one thread that the barber is done
   pthread_mutex_lock(&barber_mutex);
   barber_working[syscall(SYS_gettid)%barber_count] = 0;
   pthread_cond_signal(&barber_cv);
   pthread_mutex_unlock(&barber_mutex);
   
}


int main(int argc, char** argv)
{
   if (argc == 4)
     {
	srand(time(NULL));
	//Inital value parsing
	barber_count = *argv[1] - '0';
	customer_count = *argv[2] - '0';
	free_seat_count = *argv[3] - '0';
	max_seat_count = free_seat_count;
	printf(" Barbers %d\n Customers %d\n Free Seats %d \n", barber_count, customer_count, free_seat_count);
   
	//Threads declarations
	pthread_t barber[barber_count];
	pthread_t customer[customer_count];
   
	//Barber initialization
	pthread_mutex_init(&barber_mutex, NULL);
	pthread_cond_init(&barber_cv, NULL);
	
	//Seat initialization
	pthread_mutex_init(&seat_mutex, NULL);
	pthread_cond_init(&seat_cv, NULL);
	
	
	//index for for loops
	int i;
	
	//Barber thread creation
	
	for (i = 0; i < barber_count; i++)
	  {
	     pthread_create(&barber[i], NULL, &barber_run, NULL);
	     barber_working[i] = 0;
	  }
	
	//Customer thread creation
	for (i = 0; i < customer_count; i++)
	  {
	     pthread_create(&customer[i], NULL, &customer_run, NULL);
	  }
	
	
	
	
	//Make sure threads finish
	for (i = 0; i < barber_count; i++)
	  {
	     pthread_join(barber[(syscall(SYS_gettid)%barber_count)], NULL);
	  }
	
	//Customer thread creation
	for (i = 0; i < customer_count; i++)
	  {
	     pthread_join(customer[i], NULL);
	  } 
	
	
	
     }
   else 
     {
	printf("Please enter the number of: barbers. customers, and free seats respectively as arguments\n");
     }
   
   
   return 0;
}




