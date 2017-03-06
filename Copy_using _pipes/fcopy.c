#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 100

int main(int arg,char* argv[3])
{
  int fd[2];    /* Input and output file descriptors */
    
    char buffer[BUF_SIZE];
    int pid;
  fd[0]= open (argv [1], O_RDONLY);

    if (fd[0] == -1)
     {
            perror ("open");
            return 2;
     }

    /* Create output file descriptor */
    fd[1]= open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fd[1] == -1)
    {
        perror("open");
        return 3;
    }
    			int pfd1[2],pfd2[2];//array pipe1 and pipe2
    			char buffer2[2]="0";
				pipe(pfd1); //pipe 1
				pipe(pfd2); //pipe 2
				int retc,retp;
pid=fork();
if(pid==0)
{           close( pfd1[1]);
			close( pfd2[0] );
			while((retc=read (pfd1[0], &buffer, BUF_SIZE))>0)
			{
				buffer[retc]=0;
				//printf("child --- %s\n",buffer);
			write(fd[1],&buffer,retc);
			
				
					
				write(pfd2[1],&buffer2,2);
				if (retc<100)
					printf("successfully copied\n");		

			
			}
			if(retc==-1)
			{
				buffer2[0]="-1";
				write(pfd2[1],&buffer2,2);
				printf("error\n");	
			}

                      
}
else
{
	 close( pfd1[0] );
	 close( pfd2[1] );
	while((retp=read (fd[0], &buffer, BUF_SIZE))>0)
	{

		buffer[retp]=0;
		//printf("parent --- %s\n",buffer);
		write(pfd1[1],&buffer,retp);
		read(pfd2[0],&buffer2,2);
		//printf("buf2----%s\n",buffer2);
		if(buffer2=="-1" || retp==-1)				
			{
				printf("Error\n");
				break;
				
			}
		if(buffer2=="0")
		continue;		
	}
        
}

close(fd[0]);
close(fd[1]);
}
