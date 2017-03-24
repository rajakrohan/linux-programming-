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


int main()
{	
	
	srand(time(NULL));
    int i,j,*Aqueue;

    key_t key = ftok("B.c", 357);
	if(key == -1)
	{
		perror("\nError-B not yet started");
		exit(-1);
	}

   int shmid = shmget(key, 11*sizeof(int), 0777);

   if(shmid == -1)
	{
		perror("\nError");
		exit(-1);
	}
	
	int PS,CS;

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

	PS = semget(key2, 1, 0777);   //shared semaphore -mute
	CS = semget(key3, 1, 0777); 

	

    struct sembuf pop, vop ;
    pop.sem_num = vop.sem_num = 0;
    pop.sem_flg = vop.sem_flg = 0;
    pop.sem_op = -1 ; vop.sem_op = 1 ;


    Aqueue=(int *)shmat(shmid, 0, 0);

    int in=0;
    
    while (1)
    {
    	if(kill(Aqueue[10],0)==-1)
    	{	
    		printf("\nserver stopped\nKilling client ------ \n" );
    		exit(0);
    	}

        P(PS);

	    sleep(rand()%3);
		Aqueue[in] = (rand()%11)-5;
		printf(" %d \n",Aqueue[in] );
		in = (in + 1) % 10;
		
		V(CS);
  
	}


}

