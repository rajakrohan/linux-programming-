#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
     char* args[]={"./shell",NULL};
     pid_t f;
     f=fork();
     if(f==0)
     {
          //child calling/executing shell.c
          execvp(args[0],args);
          
     }    
     else
     {    //parent waits for child to complete
          wait(NULL);
          exit(0);
     }
}