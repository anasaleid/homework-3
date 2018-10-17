#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

void int_Handler(int sig){

	printf("caught sigint");
}

void int_Handler2(int sig){
	printf("caught sigstp");
}

int main(){
  signal(SIGINT, int_Handler);
  signal(SIGTSTP, int_Handler2);	
  int redirect = 0; 
//create some space for our strings
  char line[500];
  char **argsarray = malloc(sizeof(char*) * 20);
  int currentStdout = dup(1);
  //print prompt
  printf("CS361 >");
  //read line from terminal
  fgets(line, 500, stdin);

  //break the string up into words
  char *word = strtok(line, " \n");
  int i = 0;
  char * fileName;

  while (strcmp(word, "exit") != 0) {  
  
  if (strcmp(word, ">") == 0)
  {
	redirect = 1;
	word = strtok(NULL, " \n");
	fileName = word;
  }
  else if (strcmp(word, "<") == 0)
  {
	redirect = 2;
	word = strtok(NULL, " \n");
        fileName = word;
  }
  else if(strcmp(word, ";") == 0)
  {
	word = strtok(NULL, " \n");
        int j;
        for(j = 0; j < i; j++)
        {
        	argsarray[j] = NULL;
        }
	i = 0;
	argsarray[i] = word;
  }
  else
  {
  	argsarray[i] = word;
  }
  i++;
  //get next word
  word = strtok(NULL, " \n");

  if(word == NULL || strcmp(word, ";") == 0)
	{
		int newFile;
		if (redirect == 1)
		{
			newFile = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0666);
			dup2(newFile, 1);
		}
		else if (redirect == 2)
		{
			newFile = open(fileName, O_RDWR | O_CREAT, 0666);
			dup2(newFile, 0);
		}

		int pid = fork();
		if (pid == 0){		
			if (redirect == 1)
			{ 
				execvp(argsarray[0], argsarray);
			}
			else if( redirect == 2)
			{
				char buf[100];
				FILE * filePointer = fopen(argsarray[i], "r");
				fgets(buf, 101, filePointer);
				execvp(buf, argsarray);
			}
			else
			{	
				execvp(argsarray[0], argsarray);
			}
		}
		else {
			int status;
			wait(&status);
			printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
		}

		if(word == NULL)
		{	
			int j;
			for(j = 0; j < i; j++)
			{
				argsarray[j] = NULL;
			}
			dup2(currentStdout, 1);	
			printf("CS361 >");
			fgets(line, 500, stdin);
			word = strtok(line, " \n");
		}
		i= 0;			
	}
  }
}
