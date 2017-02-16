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
typedef struct process {
  char *command;
  char *status;
  pid_t pid;
} process;

void *string_copy_constructor(void *elem) {
  char *str = elem;
  assert(str);
  return strdup(str);
}

// This is the destructor function for string element.
// Use this as destructor callback in vector.
void string_destructor(void *elem) { free(elem); }

void *string_default_constructor(void) {
  // A single null byte
  return calloc(1, sizeof(char));
}

int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
	//input is not a file;

	//create a status tracker
	
	vector *com = vector_create(string_copy_constructor, string_destructor, string_default_constructor);
	
	vector *stat = vector_create(string_copy_constructor, string_destructor, string_default_constructor);

	vector *p = vector_create(int_copy_constructor, int_destructor, int_default_constructor);
	

	int exit_ = 0;

	while(exit_ == 0)
  {

  	//stdin input
  	if(argc == 1)
	{
		int temp = 0;
		while(!temp)
	  {
		pid_t main_pro = getpid();
		char *directory = getcwd(NULL, 0);
		print_prompt(directory, main_pro);
		free(directory);
		//stock process
			process *a;
			a->command = argv[0];
			vector_push_back(com, a->command);
			a->pid = (int)main_pro;
			vector_push_back(p, &(int)a->pid);
			//稍后需要判断
			a->status = STATUS_RUNNING;
			vector_push_back(stat, a->status);
		//get stdin
		char *buffer = NULL;
		size_t size = 0;
		getline(&buffer, &size, stdin);
		//see what is the command
		if(strcmp(buffer, "cd\n") == 0)
			print_no_directory("");

			

		if(strcmp(buffer, "ps\n") == 0)
		{
			void print_process_info(&vector_front(stat), vector_font(p), &vector_front(com));

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
