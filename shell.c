/**
 * Machine Problem: Shell
 * CS 241 - Fall 2016
 */
#include "format.h"
#include "shell.h"
#include "vector.h"
#include <unistd.h>
# include <string.h>
typedef struct process {
  char *command;
  char *status;
  pid_t pid;
} process;

int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
	//input is not a file;
	int exit_ = 0;
	while(exit_ == 0)
  {

  	if(getopt(argc, argv, "f") != -1)
	{
		int temp = 0;
		while(!temp)
	  {
		pid_t main_pro = getpid();
		char *directory = getcwd(NULL, 0);
		print_prompt(directory, main_pro);
		free(directory);
		//get stdin
		char *buffer = NULL;
		size_t size = 0;
		getline(&buffer, &size, stdin);
		//see what is the command
		if(strcmp(buffer, "cd\n") == 0)
			print_no_directory("");
	 }
		
	}

	else
	{

	}
	  }
  return 0;
}
