#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){
	int redirect = 0;
  //create some space for our strings
  char line[500];
  char argsarray[20][100];

  //print prompt
  printf("CS361 > ");
  //read line from terminal
  fgets(line, 500, stdin);

  //break the string up into words
  char *word = strtok(line, " \n");
  int i = 0;
  while (strcmp(word, "exit") != 0) {
    printf("word: %s\n", word);
    //copy a word to the arg array
    strcpy(argsarray[i], word);
	if (strcmp(argsarray[i], ">") == 0)
	{
		redirect = 1;
	}
	else if (strcmp(argsarray[i], "<") == 0)
	{
		redirect = 2;
	}
	else if (strcmp(argsarray[i], ";") == 0)
	{
		redirect = 3;
	}
    //get next word
    word = strtok(NULL, " \n");

    if(word == NULL || strcmp(word, ";") == 0)
	{
		if (redirect == 1)
		{
			int newFile = open(argsarray[i], O_RDWR | O_CREAT, 0666);
			dup2(newFile, 1);
		}
		else if (redirect == 2)
		{
			int newFile = open(argsarray[i], O_RDWR | O_CREAT, 0666);
			dup2(newFile, 0);
		}

		int pid = fork();
		if (pid == 0) {
			printf("I am the child!  I have pid %d.\n", getpid());
			
			const char arg[10];
			if (redirect == 1 || redirect == 2)
			{
				execlp(argsarray[i-2], arg,  NULL );
			}
			else if (redirect == 3)
			{
				execlp(argsarray[i - 1], arg,  NULL);
			}
			else
			{
				execlp(argsarray[i], arg,  NULL);
			}
			exit(6);
		}
		else {
			printf("I am the parent.  I am waiting for my child %d to die.\n", pid);
			int status;
			wait(&status);
			printf("My child has died with status %d. :(\n", WEXITSTATUS(status));
		}

		if(word == NULL)
		{
			printf("CS361 > ");
			fgets(line, 500, stdin);
			word = strtok(line, " \n");
		}		
		redirect = 0;
	}
  	i = i+1; 
  }
}
