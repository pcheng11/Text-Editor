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
int clean_up = 0;

void intHandler(int r) {
    if (r == SIGINT) 
       exit_ = 0;
   fflush(stdout);
}
pid_t child;

void cleanup(int signal) {
	clean_up = 1;
  int status;
  waitpid(child, &status, 0);
  

	
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
		
			
		
			a.status = STATUS_RUNNING;
			vector_push_back(pid_info, temp_1);
			vector_push_back(status_info, a.status);
			vector_push_back(command_info, a.command);

			free(temp_1);

  	//stdin input main process: no need to create another process
  	if(argc == 1)
	{
				


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
		size_t len = strlen(buffer);
	
		char **token_array;
		size_t num_tokens;
		token_array = strsplit(buffer, " \n", &num_tokens);
		//see what is the command
		//ps
		

		if(strcmp(buffer, "ps\n") == 0)
		{
		 for(int i = 0; i <(int) vector_size(status_info); i++)
		 print_process_info( vector_get(status_info, i), *(int*)vector_get(pid_info,i), vector_get(command_info,i));
			for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
		}
		//exit
		else if(strcmp(buffer, "exit\n") == 0 || (int)cd == -1 )
		{


				
				for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					int child = *(int*)vector_get(pid_info,j);

					kill(child, SIGINT);
					printf("%d", child);
				
				}
					vector_clear(pid_info);
				vector_clear(command_info);
				vector_clear(status_info);
					for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
				
			
			return 0;
		}

		//kill
		else if(strcmp(buffer, "kill\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
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
				vector_erase(pid_info,remember);
				vector_erase(status_info,remember);
				vector_erase(command_info,remember);
			}
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
			
		}

		//stop
		else if(strcmp(buffer, "stop\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
		}
		else if(strcmp(token_array[0], "stop") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
			int exist = 0;
			int i = atoi(a);

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
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);

		}
		//cont
		else if(strcmp(buffer, "cont\n") == 0)
		{
			buffer[4] = '\0';
			print_invalid_command(buffer);
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
		}
		else if(strcmp(token_array[0], "cont") == 0 && num_tokens == 2)
		{
			char* a = token_array[1];
			int exist = 0;
			int i = atoi(a);
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
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
		}

		//cd
		else if(strcmp(buffer, "cd\n") == 0)
		{
			print_no_directory("");
				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
		}
		
		else if(strcmp(token_array[0], "cd") == 0 && num_tokens == 2)
		{
			char *temp_dir = token_array[1];

			if(opendir(temp_dir)== NULL)
			{
				print_no_directory(temp_dir);
			}
			else
				chdir(temp_dir);

				for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);

		}
//externel command
		else 
		{
			//background
			char* loc = strstr(buffer, "&");
			if(loc != NULL)
			{
				*loc = '\0';
				token_array = strsplit(buffer, " \n", &num_tokens);
				buffer[len-1]= '\0';
				signal(SIGCHLD, cleanup);

				if(clean_up == 1)
				{
					for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					vector_erase(pid_info, j);
					vector_erase(status_info, j);
					vector_erase(command_info, j);
				
				}
				
				}

			 	child = fork();
			 		process b;
			 		//buffer[len-1]= '\0';
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
				free(temp_3);
		

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
					//exit(1);
   				}
			}

			else
			{
				
				//signal
				signal(SIGCHLD, cleanup);
				if(clean_up == 1)
				{
					for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					vector_erase(pid_info, j);
					vector_erase(status_info, j);
					vector_erase(command_info, j);
				
				}
				
				}

			 	 child = fork();
			 		process b;
			 buffer[len-1]= '\0';
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
			free(temp_3);

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
		
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();

   				int i = child;

				int remember = 0;
				for(size_t j = 0; j < vector_size(pid_info); j++)
				{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					remember = j;
					break;
				}

				}
			
   				vector_erase(pid_info, remember );
				vector_erase(status_info, remember);
				vector_erase(command_info, remember);
					//exit(1);

   				}
   			}

   			for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
		}
	


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
				for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					int child = *(int*)vector_get(pid_info,j);

					kill(child, SIGINT);
					printf("%d", child);
				
				}
				vector_clear(pid_info);
				vector_clear(command_info);
				vector_clear(status_info);
				
			
			return 0;

			
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
		//externel command
		else 
		{
			//background
			char* loc = strstr(buffer, "&");
			if(loc != NULL)
			{
				*loc = '\0';
				token_array = strsplit(buffer, " \n", &num_tokens);
				buffer[len-1]= '\0';

				signal(SIGCHLD, cleanup);
				if(clean_up == 1)
				{
					for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					vector_erase(pid_info, j);
					vector_erase(status_info, j);
					vector_erase(command_info, j);
				
				}
				
				}

			 	 child = fork();
			 		process b;
			 		//buffer[len-1]= '\0';
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		
					free(temp_3);
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
					//exit(1);
   				}
			}

			else
			{
				signal(SIGCHLD, cleanup);
				//clean up
				if(clean_up == 1)
				{
					for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					vector_erase(pid_info, j);
					vector_erase(status_info, j);
					vector_erase(command_info, j);
				
				}
				
				}
			 	 child = fork();
			 		process b;
			 	buffer[len-1]= '\0';
				b.command = buffer;
				int *temp_3 = malloc(sizeof(b.pid));
				*temp_3 = (int)child;
				b.status = STATUS_RUNNING;
				vector_push_back(pid_info, temp_3);
				vector_push_back(status_info, b.status);
				vector_push_back(command_info, b.command);
		
				free(temp_3);
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
		
  				int status;
    			int return_value = waitpid(child , &status ,0);
   				if(return_value == -1 || !WIFEXITED(status))
   					print_wait_failed();

   				int i = child;

				int remember = 0;
				for(size_t j = 0; j < vector_size(pid_info); j++)
				{
				if(*(int*)vector_get(pid_info,j) == i)
				{
					remember = j;
					break;
				}

				}
			
   				vector_erase(pid_info, remember );
				vector_erase(status_info, remember);
				vector_erase(command_info, remember);
					//exit(1);
   				}
   			}
  		}
  		for(int i = 0; i<(int) num_tokens; i++)
  		free(token_array[i]);
  		free(token_array);
  	}
  		
  				free(buffer);
   				fclose(file);
   				for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					int child = *(int*)vector_get(pid_info,j);

					kill(child, SIGINT);
					printf("%d", child);
				
				}
				vector_clear(pid_info);
				vector_clear(command_info);
				vector_clear(status_info);
   				exit(0);
		}

		//	exit(0);
		
	}

	vector_clear(pid_info);
				vector_clear(command_info);
				vector_clear(status_info);
return 0;
 
}
