#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define P(s) semop(s, &pop, 1) 
#define V(s) semop(s, &vop, 1)

int global_count = 0;
int shmid;
int cl = 0;
int el = 0;

int MIN(int x, int y) 
{
	return (((x) < (y)) ? (x) : (y));
}

int *Bqueue;
int PS,CS;
void my_handler(int signum)
{	
	printf("Received a SIGINT, exitting\n");

	if(shmid == -1)
	{
		perror("\nError");
	}else el++;

	
	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		perror("\nError");
	}else cl++;

	
	printf("\nCleaned %d/%d accessable shared memory segments.\n",cl,el);

	cl=el=0;

	
	if(semctl(PS, IPC_RMID, 0) < 0){
		perror("\nError");
	}else cl++;


	if(semctl(CS, IPC_RMID, 0) < 0){
		perror("\nError");
	}else cl++;

	printf("\nCleaned %d accessable Semaphores.\n",cl);

	exit(-1);
}

int ticket=100;

pthread_mutex_t cnt_mutex,cnt_mutex2;
pthread_cond_t cnt_cond;
pthread_attr_t thread_attr;

struct threaditem
{
	pthread_t * point;
	int in;
};




void *book_ticket(void *t)
{

	struct threaditem *t_stuff=(struct threaditem *) t;

	//printf("thread value %d\n",t_stuff->in);
	int x=t_stuff->in;

	pthread_mutex_lock(&cnt_mutex); //locked

	if((ticket-x)>=0)
	{
		ticket=MIN(ticket-x,100);
		sleep(rand()%3);
		printf("Tickets available=%d\n",ticket);
	}
	else
	{
		sleep(rand()%3);
		printf("Request cannot be fulfilled \n");
	}
	pthread_mutex_unlock(&cnt_mutex); //signalled

	free(t_stuff->point);
	free(t_stuff);

	pthread_mutex_lock(&cnt_mutex2);
	global_count--;

	if(global_count <= 5) 
	pthread_cond_signal(&cnt_cond);
	//signal main to continue
	pthread_mutex_unlock(&cnt_mutex2);
}



int main()
{


	// initialize the mutex and the condition variable

	pthread_mutex_init(&cnt_mutex, NULL);
	pthread_cond_init(&cnt_cond, NULL);
	pthread_mutex_init(&cnt_mutex2, NULL);

    srand(time(NULL));
    int i,j;
   
    pthread_mutex_init(&cnt_mutex,NULL);
    
   
 

    key_t key = ftok("B.c", 357);
	if(key == -1)
	{
		perror("\nError");
		exit(-1);
	}

   shmid = shmget(key, 11*sizeof(int), 0777|IPC_CREAT|IPC_EXCL);

	if(shmid == -1)
	{
		perror("\nError");
		exit(-1);
	}

	

    key_t key2 = ftok("B.c", 1001);
	if(key2 == -1)
	{
		perror("\nError");
		exit(-1);
	}
	key_t key3 = ftok("B.c", 2001);
	if(key2 == -1)
	{
		perror("\nError");
		exit(-1);
	}

	PS = semget(key2, 1, 0777|IPC_CREAT|IPC_EXCL);   //shared semaphore -mute
	CS = semget(key3, 1, 0777|IPC_CREAT|IPC_EXCL); 

	semctl(CS, 0, SETVAL, 0); // B as consumer semaphores initialized to 0
	semctl(PS, 0, SETVAL, 10); // A as producer semaphores initialized to 10
    struct sembuf pop, vop ;
    pop.sem_num = vop.sem_num = 0;
    pop.sem_flg = vop.sem_flg = 0;
    pop.sem_op = -1 ; vop.sem_op = 1 ;

    Bqueue=(int *)shmat(shmid, 0, 0);

    Bqueue[10]=getpid(); //pid of server

	// register signal handler fo Ctrl-C
	signal(SIGINT, my_handler);

    int out=0;
    i=0;
    
   
     while (1) 
    {

    	P(CS);


    	pthread_mutex_lock(&cnt_mutex2);
    	if(global_count >= 10)
    	{
    		while (global_count>5)
    		{	
    			
    			pthread_cond_wait(&cnt_cond, &cnt_mutex2);
    			
    		}
    	
    	} 

    	global_count++;

    	pthread_mutex_unlock(&cnt_mutex2);    	

   		struct threaditem *stuff = (struct threaditem *)malloc(sizeof(struct threaditem));
   		pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t));

    	//printf("%d----\n",Bqueue[out]);
    	stuff->point=tid;
    	stuff->in=Bqueue[out];    	

	    pthread_create(tid, &thread_attr, book_ticket, (void *) stuff);
	    
		out = (out + 1) % 10;


		V(PS);

			

	}

	// clean up the mutex and condition variable
	pthread_mutex_destroy(&cnt_mutex);
	pthread_cond_destroy(&cnt_cond);
	pthread_mutex_destroy(&cnt_mutex2);
}