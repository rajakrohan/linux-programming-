#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>

#define limit 100
typedef struct _pipes{
	char *arg[20];

	} argarray;


char* gets(char *str);

int main()
{
	char *input,*first,*second,*third,*curwd,*inputcopy,*context,*remainder1,*file1, *file2;
	input = (char*) malloc(limit* (sizeof(char)));
	inputcopy = (char*) malloc(limit* (sizeof(char)));
	curwd = (char*) malloc(limit* (sizeof(char)));
	file1=(char*)malloc(limit * sizeof(char));
	file2=(char*)malloc(limit * sizeof(char));
	char delimiter[] = " ";
	int i,j;
	double diff;
	struct dirent *dirread;
	DIR *direct;
	struct stat status,status1;
	int fstatus,fstatus1;
	
	
	
	
	FILE *f1, *f2, *fp = NULL;
	argarray p[50];
	while(1)
	{
		//takes the command into curwd
		int background=0;
		printf("%s>", getcwd(curwd,limit));
		gets(input);
		int inputLength = strlen(input);
		char *inputCopy = (char*) calloc(inputLength + 1, sizeof(char));
		strncpy(inputCopy, input, inputLength);
		//parsing the input command
		first = strtok_r (inputCopy, delimiter, &context);
		second = strtok_r (NULL, delimiter, &context);
		
		remainder1 = context;

		//command : cd - changes the directory
		if(strcmp(first,"cd")==0)
		{
			fp = fopen("test.txt","a");			
			fwrite(first, 1, sizeof(first), fp);		
			fprintf(fp, "\n");
			fclose(fp);
			if(second==NULL)
				chdir("/home");
							else
			{
				int error=chdir(second);
				if(error==-1)
					perror("Error: ");
			}
		}

		//command : pwd - prints the current directory
		else if(strcmp(first,"pwd")==0)
		{
			fp = fopen("test.txt","a");			
			fwrite(first, 1, sizeof(first), fp);
			fprintf(fp, "\n");
			fclose(fp);			
			printf("%s\n",curwd);
		}


		//command : mkdir - creates a directory
		else if(strcmp(first,"mkdir")==0)
		{
			fp = fopen("test.txt","a");			
			fwrite(first, 1, sizeof(first), fp);
			fprintf(fp, "\t");
			fclose(fp);
			fp = fopen("test.txt","a");			
			fwrite(second, 1, sizeof(second), fp);
			fprintf(fp, "\n");
			fclose(fp);
			int error=mkdir(second,S_IRWXU);
			if(error==-1)
				perror("Error: ");
		}


		//command : rmdir - removes the directory
		else if(strcmp(first,"rmdir")==0)
		{
			fp = fopen("test.txt","a");			
			fwrite(first, 1, sizeof(first), fp);
			fprintf(fp, "\t");
			fclose(fp);
			fp = fopen("test.txt","a");			
			fwrite(second, 1, sizeof(second), fp);
			fprintf(fp, "\n");
			fclose(fp);
			int error=rmdir(second);
			if(error==-1)
				perror("Error: ");
		}

		


		//command : ls - lists the files in the current directory
		else if(strcmp(first, "ls")==0)
		{
			fp = fopen("test.txt","a");			
			fwrite(first, 1, sizeof(first), fp);
			fprintf(fp, "\n");
			fclose(fp);
			if(second==NULL)
			{
				direct=opendir(curwd);
				if(direct==NULL)
				perror("Error:");
				else
				{
					while((dirread=readdir(direct))!=NULL)
					printf("%s\n", dirread->d_name);
					closedir(direct);
					printf("\n");
				}
			}

			else
			{
				if(strcmp(second,"-l")==0)
				{
					direct=opendir(curwd);
					if(direct==NULL)
					perror("Empty Directory.");
					else
					{
						while((dirread=readdir(direct))!=NULL)
						{
							fstatus=stat(dirread->d_name, &status);
							if(fstatus>=0)
							{
								printf( (S_ISDIR(status.st_mode)) ? "d" : "-");
								printf( (status.st_mode & S_IRUSR) ? "r" : "-");
								printf( (status.st_mode & S_IWUSR) ? "w" : "-");
								printf( (status.st_mode & S_IXUSR) ? "x" : "-");
								printf( (status.st_mode & S_IRGRP) ? "r" : "-");
								printf( (status.st_mode & S_IWGRP) ? "w" : "-");
								printf( (status.st_mode & S_IXGRP) ? "x" : "-");
								printf( (status.st_mode & S_IROTH) ? "r" : "-");
								printf( (status.st_mode & S_IWOTH) ? "w" : "-");
								printf( (status.st_mode & S_IXOTH) ? "x" : "-");
								char* timestr;
								timestr=ctime(&status.st_atime);
								printf(" %d\t%d\t%s\t%s\n",(int)status.st_nlink,(int)status.st_size,timestr,dirread->d_name);
							}
						}
					}
					closedir(direct);
				}
			}
		}

		//command: cp to copy file
		else if(strcmp(first,"cp")==0)
			{
				third = strtok_r (NULL, delimiter, &context);
				 int inputFd, outputFd, openFlags;
     mode_t filePerms;
     ssize_t numRead;
     char buf[1024];
     inputFd = open(second, O_RDONLY);
    
 
     openFlags = O_CREAT | O_WRONLY | O_TRUNC;
     filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                 S_IROTH | S_IWOTH;    
     outputFd = open(third, openFlags, filePerms);
     
 
     /* Transfer data until we encounter end of input or an error */
 
     while ((numRead = read(inputFd, buf, 1024)) > 0)
         if (write(outputFd, buf, numRead) != numRead)
             //fatal("couldn't write whole buffer");
     
 
   close(inputFd);
        
    close(outputFd);
         

				}


		
		
		//command : exit - exit from shell
		else if(strcmp(first,"exit")==0)
		{
			fp = fopen("test.txt","a");			
			fwrite(first, 1, sizeof(first), fp);
			fprintf(fp, "\n");
			fclose(fp);
			exit(0);
		}
		





		//running executable file and allowing background execution
		else
		{
			char *arg[50], *tempstr1;
			tempstr1=(char*) calloc(remainder1 + 1, sizeof(char));
			int pfd[50][2];
			int inputnum;
			
			char *path;
			if(first[strlen(first)-1]=='&')
			{
				first[strlen(first)-1]='\0';
				background=1; //background execution
				arg[0]=first;
				arg[1]=NULL;
			}
			else
			{
				arg[0]=first;
				arg[1]=second;
				inputnum=2;
				if(second!=NULL)
				{
					char* temp;
					//parsing the command
					do
					{
						temp=strtok_r(remainder1,delimiter,&tempstr1);	
						remainder1=tempstr1;
						arg[inputnum]=temp;
						if(temp==NULL)
							break;
						if(temp[strlen(temp)-1]=='&')
						{
							temp[strlen(temp)-1]='\0';
							arg[inputnum]=temp;
							background=1;
							break;
						}
						inputnum++;
					}while(temp!=NULL);
				}
			}
			



			int ik;
			int pid;
			int yesfileinputoutput=0;
			int yespipes=0;
	
			//redirection of the output of one command to the input of another by using the "|" symbol
	
				for(ik=0; ik<inputnum-1;ik++)
				{
					//the symbol "|" is found in the command 
					if(strcmp(arg[ik],"|")==0)
					{
						yespipes=1;					
						break;
					}
				}


				if(yespipes==1 && inputnum>2)
				{
					//printf("ASD");
					yespipes=0;
					i=0;
					j=0;
					//store the arguments of each executable file separately
					for(ik=0; ik<inputnum; ik++)
					{
						if(strcmp(arg[ik],"|")==0)
						{
											
							p[i].arg[j]=NULL;
							i++;
							j=0;
							yespipes++;
						}
						else
						{
							p[i].arg[j]=arg[ik];
							j++;
						}
					}

					//create pipes for the number of executable files
					
					//redirecting the output of one to input of the other and running the executable files
					//int pid1;
					int pclink;
					for(i=yespipes; i<0; i--)
					{
						pclink = pfd[1]; // link to write descriptor for next iteration
						if(pipe(pfd) == -1) continue;

						pid=fork();//child processes
						//if(i == 0) pid1 = pid;										
						if(pid==0)
						{
							//first file - the output of which is the input of the second one
							if(i==0)
							{
								dup2(pclink, 1); 
								
								if(execvp(p[i].arg[0], p[i].arg))
              			 		{
                					puts(strerror(errno));
                					exit(127);
                				}
                				exit(-1);
							}
							//last file - takes the input from the previous file
							else if(i==yespipes)
							{
								dup2(pfd[0],0);
								
								if(execvp(p[i].arg[0], p[i].arg))
              			 		{
                					puts(strerror(errno));
                					exit(127);
                				}
                				exit(-1);

							}
					//inbetween files - takes the input from previous one and gives the output to the next one
							else
							{
								dup2(pclink, 1);
								dup2(pfd[0], 0);
								if(execvp(p[i].arg[0], p[i].arg))
	              			 	{
	                				puts(strerror(errno));
	                				exit(127);
	                			}
	                			exit(-1);
							}
							
                		}

					}
					
					waitpid(pid,&fstatus,0);	//parent waits for each child to execute			
				}
				
				//redirection of output of the program to a file and reading the input from a file using ">" and "<" symbols
				else
				{
					int fd1, fd2;

					for(ik=0; ik<inputnum-1;ik++)
					{	
						//the symbols "<" or ">" are found
						if(strcmp(arg[ik],"<")==0 || strcmp(arg[ik], ">")==0)
						{
							yesfileinputoutput=1;
							break;
						}
					}

					if(yesfileinputoutput==1)
					{
						//redirection of output to a file
						if(strcmp(arg[inputnum-2],">")==0)
						{
							//if reading of the input from the file is not requested
							if((inputnum<4) || ((strcmp(arg[inputnum-4],"<")!=0) && (inputnum>4)))
							{
								pid=fork();
								//child process
								if(pid==0)
								{
									fd1=open(arg[inputnum-1],O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //open the file to which output has to be written
									close(1); //close stdout
									dup(fd1);
									close(fd1);
									arg[inputnum-2]=NULL;
									arg[inputnum-1]=NULL;
									if( execvp(arg[0], arg))
	              					{
	                					puts(strerror(errno));
	                					exit(127);
	                				}

								}
								//parent process
								else
								{
									waitpid(pid,&fstatus,0);
								}
							}
							//reading input from file and redirection of output to another file
							else if(strcmp(arg[inputnum-4],"<")==0)
							{
								pid=fork();
								//child process
								if(pid==0)
								{
									fd2=open(arg[inputnum-3],O_RDONLY);  //open the file from which input has to be taken
									close(0);  //close stdin
									dup(fd2);
									close(fd2);
									fd1=open(arg[inputnum-1],O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);  //open the file to which output has to be written
									close(1); //close stdout
									dup(fd1);
									close(fd1);
									arg[inputnum-4]=arg[inputnum-3]=arg[inputnum-2]=arg[inputnum-1]=NULL;
									if( execvp(arg[0], arg))
	              					{
	                					puts(strerror(errno));
	                					exit(127);
	                				}

								}
								//parent process
								else
								{
									waitpid(pid,&fstatus,0);
								}

							}
						}

						//reading input from a file
						else if(strcmp(arg[inputnum-2],"<")==0)
						{
							//no redirection of output to a file
							if((inputnum<4) || ((strcmp(arg[inputnum-4],">")!=0) && (inputnum>4)))
							{
								pid=fork();
								//child process
								if(pid==0)
								{
									fd2=open(arg[inputnum-1],O_RDONLY);  //open the file from which input has to be taken
									close(0);  //close stdin
									dup(fd2);
									close(fd2);
									arg[inputnum-2]=arg[inputnum-1]=NULL;
									if( execvp(arg[0], arg))
	              					{
	                					puts(strerror(errno));
	                					exit(127);
	                				}
								}
								//parent process
								else
								{
									waitpid(pid,&fstatus,0);
								}
							}
							//reading input from a file and redirecting output to a file
							else if(strcmp(arg[inputnum-4],">")==0)
							{
								pid=fork();
								//child process
								if(pid==0)
								{
									fd2=open(arg[inputnum-1],O_RDONLY); //open the file from which input has to be taken
									close(0);  //close stdin
									dup(fd2);
									close(fd2);
									fd1=open(arg[inputnum-1],O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //open the file to which output has to be written
									close(1);  //close stdout
									dup(fd1);
									close(fd1);
									arg[inputnum-1]=arg[inputnum-4]=arg[inputnum-3]=arg[inputnum-2]=NULL;
									if( execvp(arg[0], arg))
	              					{
	                					puts(strerror(errno));
	                					exit(127);
	                				}
								}
								//parent process
								else
								{
									waitpid(pid,&fstatus,0);
								}
							}
						}
					}
					//no redirection of output to a file or reading input from a file
					else
					{
						pid=fork();
						if(pid==0)
						{
						//executing files
                			if( execvp(arg[0], arg))
                			{
                				puts(strerror(errno));
                				exit(127);
                			}
						}
						else
						{
							if(background!=1)
							waitpid(pid,&fstatus,0);
						}
						background=0;
					}
				

				}
		
		}
	}
	return 0;
}