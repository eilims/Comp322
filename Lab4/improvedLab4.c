#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//Semaphore declarations
//A customer is waiting
sem_t customer_sem;
//The barber finishes a haircut
sem_t barber_sem;
//Customer left store
sem_t leaving_sem;


//Conditional Variable declarations
//Barber is/is not cutting hair
pthread_cond_t barber_cv;


//Mutex declarations
//Lock on seats
pthread_mutex_t seat_mutex;
//Lock on barber
pthread_mutex_t barber_mutex;
//Lock for orderly printing
pthread_mutex_t print_mutex;
//Lock for keeping track of visitors
pthread_mutex_t count_mutex;

//Global variables
//Maximum of 20 barbers
int barber_working[20];
//Number of currently available waiting spots
int free_seat_count;
//Maximum number of waiting spots
int max_seat_count;
//Number of barbers
int barber_count;
//Number of customers
int customer_count;
//Number of customers who have entered the store
int customers_visited;





//Customer request for a haircut
void requestHaircut()
{ 
	//Getting the haircut
	//Must wait for the barber to finish
	sem_wait(&barber_sem);

	//Note that a customer has been serviced
	pthread_mutex_lock(&count_mutex);
	customers_visited++;
	pthread_mutex_unlock(&count_mutex);
	
	//Print out a confirmation message
	time_t t;
	time(&t);
	pthread_mutex_lock(&print_mutex);
	printf("\n---Customer %d hair is cut. The time is %s", syscall(SYS_gettid), ctime(&t));
	pthread_mutex_unlock(&print_mutex);
	
	//Needed to ensure that the barber does not leave early
	sem_post(&leaving_sem);
}

void noBarbers()
{
   pthread_mutex_lock(&print_mutex);
   printf("\n---No more seats! I am Customer %d\n", syscall(SYS_gettid));
   pthread_mutex_unlock(&print_mutex);
   
   //Note that a customer has been missed
   pthread_mutex_lock(&count_mutex);
   customers_visited++;
   pthread_mutex_unlock(&count_mutex);
}


//Customer thread function
void* customer_run(void* arg)
{
   int check_in_time = (rand()%10) + 1;
   pthread_mutex_lock(&print_mutex);
   printf("Customer %d! I will appear in %d seconds\n", syscall(SYS_gettid), check_in_time);
   pthread_mutex_unlock(&print_mutex);
   //Sleep until check in time
   sleep(check_in_time);
     
   //Lock on seat
   pthread_mutex_lock(&seat_mutex);
    
   //Check if there are no seats 
   //Seats are still free
   if (free_seat_count > 0)
     {
	//One seat is now occupied
	free_seat_count--;
	
	pthread_mutex_unlock(&seat_mutex);
	//State that a customer sat down
	sem_post(&customer_sem);
	
	//Lock barber to check if he is busy
	pthread_mutex_lock(&barber_mutex);
	
	//Debugging statement
        printf("\nCustomer %d is sitting in seat %d\n", syscall(SYS_gettid), (free_seat_count + 1));
	
	int i;
	//Allows for any barber to cut hair by applying a generic barber
	int free_barber = 0;
	//Wait until the barber is not working
	while(!free_barber)
	  {
	     //unlocks and relocks mutex when called
	     pthread_cond_wait(&barber_cv, &barber_mutex);
	     
	     for (i = 0; i < barber_count; i++)
	       {
		  free_barber += barber_working[i];
	       }
	  }
	pthread_mutex_unlock(&barber_mutex);
	requestHaircut();
	   
     }
   
   //Seats are all filled
   else 
     {
	pthread_mutex_unlock(&seat_mutex);
	noBarbers();
     }

}


//Barber thread function
void* barber_run(void* arg)
{
   int cut_time = (rand()%3) + 1;
   pthread_mutex_lock(&print_mutex);
   printf("\nBarber %d has arrived! It takes me %d seconds to cut hair\n\n", syscall(SYS_gettid), cut_time);
   pthread_mutex_unlock(&print_mutex);
   
   while(customers_visited < customer_count)
     {
	
	pthread_mutex_unlock(&count_mutex);
	//Wait until at least one customer arrivers
	pthread_mutex_lock(&print_mutex);
	printf("Barber %d is waiting for a customer\n", syscall(SYS_gettid));
	pthread_mutex_unlock(&print_mutex);
	sem_wait(&customer_sem);

	//Skip waiting if there are no more customers
	pthread_mutex_lock(&count_mutex);
	int customers_done = customers_visited < customer_count; 
	pthread_mutex_unlock(&count_mutex);
	
	if (customers_done) 
	  {
	     
	     //Remove one thread from the seat
	     pthread_mutex_lock(&seat_mutex);
	     free_seat_count++;
	     pthread_mutex_unlock(&seat_mutex);
	     
	     //Notify all waiting threads that the barber is busy
	     pthread_mutex_lock(&barber_mutex);
	     barber_working[syscall(SYS_gettid)%barber_count] = 1;
	     pthread_cond_signal(&barber_cv);
	     pthread_mutex_unlock(&barber_mutex);
	     
	     //Sleep for cutting time
	     sleep(cut_time);
	     //pthread_mutex_lock(&print_mutex);
	     //printf("\nBarber %d cut hair\n", syscall(SYS_gettid));
	     //pthread_mutex_unlock(&print_mutex);
	     
	     
	     //Notify one thread that the barber is done
	     pthread_mutex_lock(&barber_mutex);
	     barber_working[syscall(SYS_gettid)%barber_count] = 0;
	     pthread_cond_signal(&barber_cv);
	     pthread_mutex_unlock(&barber_mutex);
	     
	     //Allow customer to leave
	     //Makes the customer wait until the barber is done
	     sem_post(&barber_sem);	
		
	     //Noticed the customer left
	     sem_wait(&leaving_sem);
	  }
	
	//Lock mutex to aqcuire current count
	pthread_mutex_lock(&count_mutex);
     }
   //Unlock mutex when we leave
   pthread_mutex_unlock(&count_mutex);
   
   //Debugging
   printf("Barber %d has left\n", syscall(SYS_gettid));
   //int sem_count = -100;
   //int cust_count = -100;
   
   //int sem1 = sem_getvalue(&leaving_sem, &sem_count);
   //int sem2 = sem_getvalue(&customer_sem, &cust_count);
   //if (sem1 == 0)
   //  {
   //	printf("Remaining blocked threads leaving %d\n", sem_count);
   //  }
   //if(sem2 == 0)
   //  {
   //	printf("Remaining blocked threads customer %d\n", cust_count);
   //  }
   
   //Other barbers may be trapped waiting
   //These statements ensure they are released
   sem_post(&customer_sem);
   //system("ps -x");
}


int main(int argc, char** argv)
{
   if (argc == 4)
     {
       
	customers_visited = 0;
	
	//Seed for random values
	srand(time(NULL));
	
	//endptr for parsing
	char* endptr;
	
	//Inital value parsing
	barber_count = (int)strtol(argv[1], &endptr, 10);
	customer_count = (int)strtol(argv[2], &endptr, 10);
	free_seat_count = (int)strtol(argv[3], &endptr, 10);
	
	max_seat_count = free_seat_count;
	
	printf(" Barbers %d\n Customers %d\n Free Seats %d \n", barber_count, customer_count, free_seat_count);
	
	//Count mutex
	pthread_mutex_init(&count_mutex, NULL);
	
	//Print mutex
	pthread_mutex_init(&print_mutex, NULL);
	
	//Semaphore definistion
	sem_init(&customer_sem, 0, 0);
	sem_init(&barber_sem, 0, 0);
	sem_init(&leaving_sem, 0, 0);
	
	//Threads declarations
	pthread_t barber[barber_count];
	pthread_t customer[customer_count];
   
	//Barber initialization
	pthread_mutex_init(&barber_mutex, NULL);
	pthread_cond_init(&barber_cv, NULL);
	
	//Seat initialization
	pthread_mutex_init(&seat_mutex, NULL);
	
	
	//index for for loops
	int i;
	
	//Barber thread creation
	int test = barber_count;
	for (i = 0; i < test; i++)
	  {
	     printf("%d\n",i);
	     pthread_create(&barber[i], NULL, &barber_run, NULL);
	     barber_working[i] = 1;
	  }
	
	//Customer thread creation
	for (i = 0; i < customer_count; i++)
	  {
	     pthread_create(&customer[i], NULL, &customer_run, NULL);
	  }
	
      
	//Customer thread creation
	for (i = 0; i < customer_count; i++)
	  {
	     pthread_join(customer[i], NULL);
	  } 
	
	//Make sure threads finish
	for (i = 0; i < test; i++)
	  {
	     pthread_join(barber[i], NULL);
	  }
	
     }
   else 
     {
	printf("Please enter the number of: barbers. customers, and free seats respectively as arguments\n");
     }
   
   //system("ps -x");
   return 0;
}




