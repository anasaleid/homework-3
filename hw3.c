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
  int currentStdout = dup(1);
  //print prompt
  printf("CS361 >");
  //read line from terminal
  fgets(line, 500, stdin);

  //break the string up into words
  char *word = strtok(line, " \n");
  int i = 0;
  while (strcmp(word, "exit") != 0) {
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
    //get next word
    word = strtok(NULL, " \n");

    if(word == NULL || strcmp(word, ";") == 0)
	{
		int newFile;
		if (redirect == 1)
		{
			newFile = open(argsarray[i], O_RDWR | O_CREAT | O_TRUNC, 0666);
			dup2(newFile, 1);
		}
		else if (redirect == 2)
		{
			newFile = open(argsarray[i], O_RDWR | O_CREAT, 0666);
			dup2(newFile, 0);
		}

		int pid = fork();
		if (pid == 0){
			
			const char arg[10];
			if (redirect == 1)
			{
				execlp(argsarray[i-2], arg,  NULL );
			}
			else if( redirect == 2)
			{
				char buf[100];
				FILE * filePointer = fopen(argsarray[i-2], "r");
				fgets(buf, 101, filePointer);
				execlp(buf, arg, NULL);
			}
			else
			{
				execlp(argsarray[i], arg,  NULL);
			}
		}
		else {
			int status;
			wait(&status);
			printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
		}

		if(word == NULL)
		{
			dup2(currentStdout, 1);	
			printf("CS361 >");
			fgets(line, 500, stdin);
			word = strtok(line, " \n");
		}
				
		redirect = 0;
	}


  	i = i+1; 
  }
}
