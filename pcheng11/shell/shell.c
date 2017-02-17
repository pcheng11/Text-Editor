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
 #include <sys/types.h>
 #include <signal.h>
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

// This is the default constructor function for string element.
// Use this as a default constructor callback in vector.
void *string_default_constructor(void) {
  // A single null byte
  return calloc(1, sizeof(char));
}


int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
	//input is not a file;

	if(argc > 3 || argc == 2 || (argc == 3 && getopt(argc, argv, "f") == -1))
	{
		print_usage();
		return 0;
	}
	
// create 3 vectors to keep track of process info
	vector *pid_info = int_vector_create();
	vector *status_info = vector_create(string_copy_constructor, string_destructor, string_default_constructor);
	vector *command_info = vector_create(string_copy_constructor, string_destructor, string_default_constructor);
	

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
			process a;
			a.command = argv[0];
			int *temp_1 = malloc(sizeof(a.pid));
			*temp_1 = (int)main_pro;

			
			//稍后需要判断
			a.status = STATUS_RUNNING;
			vector_push_back(pid_info, temp_1);
			vector_push_back(status_info, a.status);
			vector_push_back(command_info, a.command);
		
		//get stdin
		char *buffer = NULL;
		size_t size = 0;
		getline(&buffer, &size, stdin);
		char* tell = strtok(buffer, " ");
		//see what is the command
		//ps
		if(strcmp(buffer, "ps\n") == 0)
		{
		 print_process_info( vector_get(status_info, 0), *(int*)vector_get(pid_info,0), vector_get(command_info,0));

		}
		
		
		//kill
		else if(strcmp(buffer, "kill\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
		}
		else if(strcmp(tell, "kill") == 0)
		{
			char* a = strdup(tell + 5);
			int exist = 0;
			int i = atoi(a);
		//	printf("%d", i);
			int remember;
			for(size_t j = 0; j < vector_size(pid_info); j++)
			{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					exist = 1;
					remember = j;
					break;
				}

			}
			if(exist == 0)
			{
				print_no_process_found(i);
			}
			else
			{
				kill(i, SIGTERM);
				print_killed_process(*(int*)vector_get(pid_info, remember), vector_get(command_info, remember));
			}
		}

		//stop
		else if(strcmp(buffer, "stop\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
		}
		else if(strcmp(tell, "stop") == 0)
		{
			char* a = strdup(tell + 5);
			int exist = 0;
			int i = atoi(a);
			//printf("%d", i);
			int remember;
			for(size_t j = 0; j < vector_size(pid_info); j++)
			{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					exist = 1;
					remember = j;
					break;
				}

			}
			if(exist == 0)
			{
				print_no_process_found(i);
			}
			else
			{
				kill(i, SIGSTOP);
				vector_set(status_info, remember, STATUS_STOPPED);
				print_stopped_process(*(int*)vector_get(pid_info, remember), vector_get(command_info, remember));
			}
		}
		//cont
		else if(strcmp(buffer, "cont\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
		}
		else if(strcmp(tell, "cont") == 0)
		{
			char* a = strdup(tell + 5);
			int exist = 0;
			int i = atoi(a);
			//printf("%d", i);
			int remember;
			for(size_t j = 0; j < vector_size(pid_info); j++)
			{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					exist = 1;
					remember = j;
					break;
				}

			}
			if(exist == 0)
			{
				print_no_process_found(i);
			}
			else
			{
				kill(i, SIGCONT);
				vector_set(status_info, remember, STATUS_RUNNING);
				
			}
		}

		//cd
		else if(strcmp(buffer, "cd\n") == 0)
			print_no_directory("");
		
		else if(strcmp(tell, "cd") == 0)
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
	else
	{

		int temp = 0;
		while(!temp)
		{	
		
			FILE *file = fopen(argv[2], "r");
			
  			if(file == NULL)
  			{ // can not open
  				
      			print_script_file_error();
      			free(file);
      			return 0;
      		
  			}
//count line numbers
			else
  			{
   		  		char *buffer = NULL;
    			size_t length = 0;
    			ssize_t read;
    		
    			while ((read = getline(&buffer, &length, file)) != -1) 
    			{

    					int len = strlen(buffer);
    					pid_t main_pro = getpid();
						//get current directory
						char *directory = getcwd(NULL, 0);
						print_prompt(directory, main_pro);
						free(directory);
		//stock process
						process a;
						a.command = argv[0];
						int *temp_1 = malloc(sizeof(a.pid));
						*temp_1 = (int)main_pro;

			
			
						a.status = STATUS_RUNNING;
						vector_push_back(pid_info, temp_1);
						vector_push_back(status_info, a.status);
						vector_push_back(command_info, a.command);
		
		//get stdin
						char* tell = strtok(buffer, " ");
		//see what is the command
		//ps
					if(strcmp(buffer, "ps\n") == 0)
					{
					print_command(buffer);
		 			print_process_info( vector_get(status_info, 0), *(int*)vector_get(pid_info,0), vector_get(command_info,0));

					}
		
		
		//kill
				else if(strcmp(buffer, "kill\n") == 0)
				{
					buffer[len] = '\0';
					print_command(buffer);
					
					print_invalid_command(buffer);
				}

				else if(strcmp(tell, "kill") == 0)
				{
					buffer[len] = '\0';
					print_command(buffer);

					char* a = strdup(tell + 5);
					int exist = 0;
					int i = atoi(a);
		//	printf("%d", i);
					int remember;
					for(size_t j = 0; j < vector_size(pid_info); j++)
					{
						if(*(int*)vector_get(pid_info,j) == i)
						{
							exist = 1;
							remember = j;
							break;
						}

					}
					if(exist == 0)
					{
					print_no_process_found(i);
					}
					else
					{
						kill(i, SIGTERM);
						print_killed_process(*(int*)vector_get(pid_info, remember), vector_get(command_info, remember));
					}
				}

		//stop
				else if(strcmp(buffer, "stop\n") == 0)
			{

				buffer[len] = '\0';
					print_command(buffer);
				print_invalid_command(buffer);
			}
		else if(strcmp(tell, "stop") == 0)
		{
			buffer[len] = '\0';
					print_command(buffer);

			char* a = strdup(tell + 5);
			int exist = 0;
			int i = atoi(a);
			//printf("%d", i);
			int remember;
			for(size_t j = 0; j < vector_size(pid_info); j++)
			{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					exist = 1;
					remember = j;
					break;
				}

			}
			if(exist == 0)
			{
				print_no_process_found(i);
			}
			else
			{
				kill(i, SIGSTOP);
				vector_set(status_info, remember, STATUS_STOPPED);
				print_stopped_process(*(int*)vector_get(pid_info, remember), vector_get(command_info, remember));
			}
		}
		//cont
		else if(strcmp(buffer, "cont\n") == 0)
		{
			buffer[len] = '\0';
					print_command(buffer);

			print_invalid_command(buffer);
		}
		else if(strcmp(tell, "cont") == 0)
		{
			buffer[len] = '\0';
					print_command(buffer);

			char* a = strdup(tell + 5);
			int exist = 0;
			int i = atoi(a);
			//printf("%d", i);
			int remember;
			for(size_t j = 0; j < vector_size(pid_info); j++)
			{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					exist = 1;
					remember = j;
					break;
				}

			}
			if(exist == 0)
			{
				print_no_process_found(i);
			}
			else
			{
				kill(i, SIGCONT);
				vector_set(status_info, remember, STATUS_RUNNING);
				
			}
		}

		//cd
		else if(strcmp(buffer, "cd\n") == 0)
		{
			buffer[len] = '\0';
					print_command(buffer);
			print_no_directory("");
		}
		
		else if(strcmp(tell, "cd") == 0)
		{
			buffer[len] = '\0';
					print_command(buffer);
					
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
  				free(buffer);
   				fclose(file);
			}
		}
	}

}
return 0;
 
}
