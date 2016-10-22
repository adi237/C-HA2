#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int cs531_system(char *command);


/*This is the Signal Handler function which has been registered with the Kernel
* Used in the child process to handle the 'SIGINT' signal
* @param signal-number
*/
void signal_handler(int signum)
{
  if (signum == SIGINT)
   printf("\nReceived signal'SIGINT'\n");
}

/*
*
* @param
*/
int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Please provide a command.\n");
		exit(EXIT_FAILURE);
	}
    int status;
    status = cs531_system(argv[1]);
    return status;
}

/*
* This function takes a pointer to character as input(which would contain the command to be executed) 
* and will execute the command.
* @param command - pointer to character
* @return - status of the command execution.
*/
int cs531_system(char *command)
{
    int status=0;
    pid_t child_id;
	pid_t c_id;
	    
    child_id = fork();
    
    //child logic
    if(child_id == (pid_t)0){
		execlp("/bin/sh", "sh", "-c", command, (char *) NULL);
        
        //If program reaches this statement it means that execlp has failed.
        printf("Command execution failed\n");
    }
    else{
        if(child_id < (pid_t)0)
        {
            fprintf(stderr,"Fork failed.\n");
			exit(EXIT_FAILURE);
        }
		else
		{
			//signal function to bind signalhandler.
			if (signal(SIGINT, signal_handler) == SIG_ERR)
				printf("\nCannot Handle Signal 'SIGINT'\n");
			
			c_id = wait(&status); /*Wait for child to complete.*/
			printf("Parent: Child %ld exited with status = %d\n",(long)c_id,status);
		}
        
        
    }
    return status;
}