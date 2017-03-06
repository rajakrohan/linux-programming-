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


int main()
{
	int i,yroll;
	float yCGPA;
student *s_shared;
   key_t key = ftok("X.c", 357);
	if(key == -1)
	{
		perror("\nError");
		exit(-1);
	}



	int s_flag,flag=0;
do{
	s_flag = shmget(key,101*sizeof(student), 0777);
	if(s_flag == -1)
	{
		if(errno != ENOENT)
		{
			perror("\nError");
			exit(-1);
		}
		
	}
}while(s_flag == -1);



s_shared=(student*)shmat(s_flag, 0, 0);








int mute;

   key_t key2 = ftok("X.c", 357);
	if(key2 == -1)
	{
		perror("\nError");
		exit(-1);
	}




	mute = semget(key2, 1, 0777);   //shared semaphore -mute
    struct sembuf pop, vop ;
    pop.sem_num = vop.sem_num = 0;
    pop.sem_flg = vop.sem_flg = 0;
    pop.sem_op = -1 ; vop.sem_op = 1 ;







int in,length;
 for(i=0;s_shared[i].roll!=-1;i++);
length=i;

while(1)
	{

			printf("\n\nEnter number:\n");
			printf("1.Search\n");
			printf("2.update\n");
			printf("3.Exit\n");
			scanf("%d",&in);
			if(in==3)
			  {
				exit(0);
			  }
			if(in==1)
			{	int match=0;
				printf("Enter Roll_no to be searched: ");
				scanf("%d",&yroll);

			 	for(i=0;s_shared[i].roll!=-1;i++)
			 	{
			 	if(s_shared[i].roll==yroll)
			 	{
			 			 printf("%s %s %d %f",s_shared[i].First_Name,s_shared[i].last_Name,s_shared[i].roll,s_shared[i].CG);
			 			match=1;
			 	} 	

			 	}
			 	if(match==0)
			 	{
			 		printf("\nNO match found\n");
			 	}
			}
			if(in==2)
			{ P(mute);
				printf("Enter corresponding Roll_no: ");
				scanf("%d",&yroll);

			 for(i=0;s_shared[i].roll!=-1;i++)
			 {
			 	if(s_shared[i].roll==yroll)
			 	{     
			 			 printf("%s %s %d %f",s_shared[i].First_Name,s_shared[i].last_Name,s_shared[i].roll,s_shared[i].CG);
			 			 printf("\nupdate CGPA: ");
			 			 scanf("%f",&yCGPA);
			 			 s_shared[i].CG=yCGPA;
			 			 	s_shared[length].roll=-2;
			 			 
			 	} 	

			 }
			 V(mute);
			}
			
	}

}