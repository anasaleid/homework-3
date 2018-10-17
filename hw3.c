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
  
//create some space for our strings
  char line[500];
  char **argsarray = malloc(sizeof(char*) * 20);
  int currentStdout = dup(1);
  int currentStdIn = dup(0);
  int newFile;
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
	word = strtok(NULL, " \n");
	fileName = word;
        newFile = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0666);
        dup2(newFile, 1);
  }
  else if (strcmp(word, "<") == 0)
  {
	word = strtok(NULL, " \n");
        fileName = word;
  	newFile = open(fileName, O_RDONLY , 0666);
        dup2(newFile, 0);
	
	fgets(line, 500, stdin);
	word = strtok(line, " \n");
	argsarray[i] = word;
	printf("WORD: %s\n", word);
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
		close(newFile);
		int pid = fork();
		if (pid == 0){			
			execvp(argsarray[0], argsarray);
		}
		else {
			int status;
			wait(&status);
			printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
		}

		if(word == NULL)
		{	
			int j;
			for(j = 0; j <= i; j++)
			{
				argsarray[j] = NULL;
			}	
			printf("CS361 >");
			dup2(currentStdout, 1);
                        dup2(currentStdIn, 0);
			fgets(line, 500, stdin);
			word = strtok(line, " \n");
		}
		i= 0;			
	
       }
  }
}

