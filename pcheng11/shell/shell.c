/**
 * Machine Problem: Shell
 * CS 241 - Fall 2016
 */
#include "format.h"
#include "shell.h"
#include "vector.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
typedef struct process {
  char *command;
  char *status;
  pid_t pid;
} process;



int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
	//input is not a file;

	//create a status tracker
	
	
	
	

	int exit_ = 0;

	while(exit_ == 0)
  {

  	//stdin input main process: no need to create another process
  	if(argc == 1)
	{

		//vector *proc = vector_create((void*)process_copy_constructor, (void*)process_destructor,(void*) process_default_constructor);

		int temp = 0;
		while(!temp)
	  {
		pid_t main_pro = getpid();
		//get current directory
		char *directory = getcwd(NULL, 0);
		print_prompt(directory, main_pro);
		free(directory);
		//stock process
			process *a = NULL;
			a->command = argv[0];
			
			a->pid = (int)main_pro;
			
			//稍后需要判断
			a->status = STATUS_RUNNING;
			vector_push_back(proc, a);
		//get stdin
		char *buffer = NULL;
		size_t size = 0;
		getline(&buffer, &size, stdin);
		//see what is the command
		if(strcmp(buffer, "cd\n") == 0)
			print_no_directory("");

			

		if(strcmp(buffer, "ps\n") == 0)
		{
			// print_process_info( (vector_get(proc, 0)->status, vector_get(proc,0)->pid, vector_front(proc,0)->command));

		}
		else
		{
			char *temp_dir = strdup(buffer + 3);
			size_t a = strlen(temp_dir);
			temp_dir[a-1] = '\0';
			//printf("%s", temp_dir);

			// dir does not exist

			if(opendir(temp_dir)== NULL)
			{
				print_no_directory(temp_dir);
			}
			else
				chdir(temp_dir);

		}
	 }
		
	}
	//file input
	if(getopt(argc, argv, "f") != -1)
	{

	}
	  }
  return 0;
}
