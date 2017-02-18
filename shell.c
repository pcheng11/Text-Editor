/**
 * Machine Problem: Shell
 * CS 241 - Fall 2016
 */
#include "format.h"
#include "shell.h"
#include "vector.h"
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
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

int exit_ = 0;

void intHandler(int r) {
    if (r == SIGINT) 
       exit_ = 0;
   fflush(stdout);
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
	

	process a;
			pid_t main_pro = getpid();
			a.command = argv[0];
			int *temp_1 = malloc(sizeof(a.pid));
			*temp_1 = (int)main_pro;
		
			
			//稍后需要判断
			a.status = STATUS_RUNNING;
			vector_push_back(pid_info, temp_1);
			vector_push_back(status_info, a.status);
			vector_push_back(command_info, a.command);



  	//stdin input main process: no need to create another process
  	if(argc == 1)
	{
				
		//vector *proc = vector_create((void*)process_copy_constructor, (void*)process_destructor,(void*) process_default_constructor);

		signal (SIGINT,intHandler);
		while(exit_ == 0)
	  {
		
		//get current directory
		char *directory = getcwd(NULL, 0);
		print_prompt(directory, main_pro);
		free(directory);
		//stock process
			
			
		
		//get stdin
		char *buffer = NULL;
		size_t size = 0;
		size_t cd = getline(&buffer, &size, stdin);
	//	size_t len = strlen(buffer);
	
		char **token_array;
		size_t num_tokens;
		token_array = strsplit(buffer, " \n", &num_tokens);
		//see what is the command
		//ps
		

		if(strcmp(buffer, "ps\n") == 0)
		{
		 for(int i = 0; i <(int) vector_size(status_info); i++)
		 print_process_info( vector_get(status_info, i), *(int*)vector_get(pid_info,i), vector_get(command_info,i));

		}
		//exit
		else if(strcmp(buffer, "exit\n") == 0 || (int)cd == -1 )
		{
			//signal(SIGINT, intHandler);
			exit_= 1;
		}

		//kill
		else if(strcmp(buffer, "kill\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
		}
		else if(strcmp(token_array[0], "kill") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
		else if(strcmp(token_array[0], "stop") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
		else if(strcmp(token_array[0], "cont") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
		
		else if(strcmp(token_array[0], "cd") == 0 && num_tokens == 2)
		{
			char *temp_dir = token_array[1];
				//size_t a = strlen(temp_dir);
			//temp_dir[a-1] = '\0';
			//printf("%s", temp_dir);

			// dir does not exist

			if(opendir(temp_dir)== NULL)
			{
				print_no_directory(temp_dir);
			}
			else
				chdir(temp_dir);

		}
		//externel command
		//ls
		else if(strcmp(buffer, "/bin/ls\n") == 0 || strcmp(buffer, "ls\n") == 0)
		{
				//puts("L");
			 	pid_t child = fork();
		

  				if (child == -1) 
  					print_fork_failed();
  				if (child == 0) 
  				{ /* I have a child! */
    				execvp(token_array[0], token_array);
    			
    				print_exec_failed(buffer);
    				break;
   				} 
  				else 
  				{ 
  				print_command_executed(child);
			 	//puts("L");
			 /*	process b;
			 	buffer[len-1] = '\0';
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		*/
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();
					//exit(1);
   				}
		}
		//exit(0);

		else if(strcmp(token_array[0], "echo") == 0 )
		{
		
			pid_t child = fork();
				if (child == -1) 
  				print_fork_failed();
  				if (child == 0) 
  				{ 
    				execvp(token_array[0], token_array);
    				print_exec_failed(buffer);
    				break;
   				} 
  				else 
  				{ 
  				print_command_executed(child);
			 	//puts("L");
			 	/*process b;
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		*/
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();
					//exit(1);
   				}

			
		}

		else if(strcmp(token_array[0], "sleep") == 0 )
		{
		
			pid_t child = fork();
				if (child == -1) 
  				print_fork_failed();
  				if (child == 0) 
  				{ 
    				execvp(token_array[0], token_array);
    				print_exec_failed(buffer);
    				break;
   				} 
  				else 
  				{ 
  				print_command_executed(child);
			 	//puts("L");
			 	/*process b;
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		*/
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();
					//exit(1);
   				}

			
		}
		else
		{
			print_invalid_command(buffer);
		}

		//externel command
		//ls

	 }
		
	}


	//file input
	else
	{
		
	//	int temp = 0;
		//while(temp!=1)
	//{	
		
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
    			
			
    		
    	while (getline(&buffer, &length, file) != -1) 
    {

		pid_t main_pro = getpid();
		//get current directory
		char *directory = getcwd(NULL, 0);
		print_prompt(directory, main_pro);
		free(directory);
		int len = strlen(buffer);
		char *r_temp = strdup(buffer);
		if(r_temp[len-1] == '\n')
		r_temp[len-1] = '\0';
		print_command(r_temp);
		free(r_temp);
		
		//get stdin
		if(buffer[len-1] == '\n')
		buffer[len-1] = '\0';
		char **token_array;
		size_t num_tokens;
		token_array = strsplit(buffer, " ", &num_tokens);

		//see what is the command
		//ps
		if(strcmp(buffer, "ps") == 0)
		{
		  for(int i = 0; i <(int) vector_size(status_info); i++)
		 print_process_info( vector_get(status_info, i), *(int*)vector_get(pid_info,i), vector_get(command_info,i));

		}
		
		
		//kill
		else if(strcmp(buffer, "kill") == 0)
		{
			
			print_invalid_command(buffer);
		}
		else if(strcmp(token_array[0], "kill") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
				kill(i, SIGTERM);
				print_killed_process(*(int*)vector_get(pid_info, remember), vector_get(command_info, remember));
				vector_erase(pid_info,remember);
				vector_erase(status_info,remember);
				vector_erase(command_info,remember);

			}
		}

		//stop
		else if(strcmp(buffer, "stop") == 0)
		{
			//buffer[4] = '\0';
			print_invalid_command(buffer);
		}
		else if(strcmp(token_array[0], "stop") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
		else if(strcmp(buffer, "cont") == 0)
		{
			//buffer[4] = '\0';
			print_invalid_command(buffer);
		}
		else if(strcmp(token_array[0], "cont") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
		//exit
		else if(strcmp(buffer, "exit") == 0 )
		{
			//signal(SIGINT, intHandler);
			exit_ = 1;
		}
		else if(strcmp(token_array[0], "cont") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
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
		else if(strcmp(buffer, "cd") == 0)
			print_no_directory("");
		
		else if(strcmp(token_array[0], "cd") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
		
			//printf("%s", temp_dir);

			// dir does not exist

			if(opendir(a)== NULL)
			{
				print_no_directory(a);
			}
			else
				chdir(a);

		}

		//externel command
		//ls
		else if(strcmp(buffer, "/bin/ls") == 0 || strcmp(buffer, "ls") == 0)
		{
				//puts("L");
			 	pid_t child = fork();
		

  				if (child == -1) 
  					print_fork_failed();
  				if (child == 0) 
  				{ /* I have a child! */
    				execvp(token_array[0], token_array);
    			
    				print_exec_failed(buffer);
    				break;
   				} 
  				else 
  				{ 
  				print_command_executed(child);
			 	//puts("L");
			 /*	process b;
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		*/
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();
					//exit(1);
   				}
		}
		//exit(0);

		else if(strcmp(token_array[0], "echo") == 0)
		{
				//puts("L");
			 	pid_t child = fork();
			//printf("%s", token_array[1]);

  				if (child == -1) 
  					print_fork_failed();
  				if (child == 0) 
  				{ /* I have a child! */
  					
    				execvp(token_array[0], token_array);
    			
    				print_exec_failed(buffer);
    				break;
   				} 
  				else 
  				{ 
  				print_command_executed(child);
			 	//puts("L");
			 	/*process b;
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		*/
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();
					//exit(1);
   				}
		}
		else if(strcmp(token_array[0], "sleep") == 0 )
		{
		
			pid_t child = fork();
				if (child == -1) 
  				print_fork_failed();
  				if (child == 0) 
  				{ 
    				execvp(token_array[0], token_array);
    				print_exec_failed(buffer);
    				break;
   				} 
  				else 
  				{ 
  				print_command_executed(child);
			 	//puts("L");
			 	/*process b;
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		*/
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();
					//exit(1);
   				}

			
		}
		else
		{
			print_invalid_command(buffer);
		}

  	}
  		
  				free(buffer);
   				fclose(file);
   				exit(0);
		}
		//	exit(0);
		
	}


return 0;
 
}
