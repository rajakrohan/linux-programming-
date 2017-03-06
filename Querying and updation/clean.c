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

typedef struct
{
    char First_Name[20];
    char last_Name[20];
    int roll;
    float CG;    
} student;


int main()
{
	int cl = 0;
	int el = 0;
	int len = 102;
	key_t key = ftok("X.c", 357);
	if(key == -1)
	{
		perror("\nError");
		exit(-1);
	}
	/*key_t key2 = ftok("X.c", 126);
	if(key2 == -1){
		perror("\nError");
		exit(-1);
	}
	key_t keyS = ftok("X.c", 7691);
	if(keyS == -1){
		perror("\nError");
		exit(-1);
	}*/

	int shmid = shmget(key, 101*sizeof(student), 0777);
	if(shmid == -1){
		perror("\nError");
	}else el++;

	
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("\nError");
	}else cl++;


	/*int shmflags = shmget(key2, 3*sizeof(int), 0777);
	if(shmflags == -1){
		perror("\nError");
	}else el++;

	
	if(shmctl(shmflags, IPC_RMID, 0) == -1){
		perror("\nError");
	}else cl++;
	*/
	printf("\nCleaned %d/%d accessable shared memory segments.\n",cl,el);

	/*cl=el=0;
	int s_rec = semget(keyS, len, 0777);
	if(s_rec == -1){
		perror("\nError");
	}else el++;

	if(semctl(s_rec, IPC_RMID, 0) < 0){
		perror("\nError");
	}else cl++;

	printf("\nCleaned %d/%d accessable Semaphone set(s).\n",cl,el);
	*/

	return 0;
}