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
#define P(s) semop(s, &pop, 1) 
#define V(s) semop(s, &vop, 1) 

typedef struct
{
    char First_Name[20];
    char last_Name[20];
    int roll;
    float CG;    
} student;


int main(int argc, char *argv[])
{
	
   int i,j,len;
student *s_shared;

    key_t key = ftok("X.c", 357);
	if(key == -1)
	{
		perror("\nError");
		exit(-1);
	}

int shmid = shmget(key, 101*sizeof(student), 0777|IPC_CREAT|IPC_EXCL);
	if(shmid == -1)
	{
		perror("\nError");
		exit(-1);
	}








int mute;
  key_t key2 = ftok("X.c", 357);
	if(key2 == -1)
	{
		perror("\nError");
		exit(-1);
	}
mute = semget(key2, 1, 0777|IPC_CREAT); //shared semaphore -mute

 semctl(mute, 0, SETVAL, 1);

    struct sembuf pop, vop ;
    pop.sem_num = vop.sem_num = 0;
    pop.sem_flg = vop.sem_flg = 0;
    pop.sem_op = -1 ; vop.sem_op = 1 ;






s_shared=(student*)shmat(shmid, 0, 0);


    FILE* fin = fopen(argv[1], "r"); 

     if(fin== NULL) 
   {
      perror("Error in opening file");
      return(-1);
   }  



    for(i=0;!feof(fin);i++)
    fscanf(fin, "%s %s %d %f",s_shared[i].First_Name,s_shared[i].last_Name,&(s_shared[i].roll),&(s_shared[i].CG));
//printf("\n\ni=%d\n\n",i);

len = i-1;

s_shared[len].roll = -1;


//printf("\n\n%d\n\n",len);
rewind(fin);

 for(i=0;i < len;i++)
   printf("%s %s %d %f\n",s_shared[i].First_Name,s_shared[i].last_Name,s_shared[i].roll,s_shared[i].CG);
	
    
    fclose(fin);

    while(1)
    {
    	sleep(5);
    	if(s_shared[len].roll==-2)//memory updated
    	{
    		P(mute);
    		FILE* fin2 = fopen(argv[1], "w");
    		for(i=0;i<len;i++)
    		{
    			fprintf(fin2,"%s %s %d %f\n",s_shared[i].First_Name,s_shared[i].last_Name,s_shared[i].roll,s_shared[i].CG);
    			printf("%s %s %d %f\n",s_shared[i].First_Name,s_shared[i].last_Name,s_shared[i].roll,s_shared[i].CG);
    		}
    		printf("\nsuccessfully updated\n");
    		fclose(fin2);
    		V(mute);
    	}
    }
    return 0;
}