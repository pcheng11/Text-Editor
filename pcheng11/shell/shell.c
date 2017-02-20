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

static int exit_ = 0;
//static int clean = 0;
static pid_t child;
static pid_t child_b;


void intHandler(int r) {
    if (r == SIGINT) 
       exit_ = 0;
   kill(child, SIGINT);
  // printf("%d", child);
  // vector_erase(pid_info, 0);
   fflush(stdout);
}
void cleanup(int signal)
{
	while(waitpid((pid_t)(-1),0,WNOHANG) > 0) {}
		//fflush(stdout);

		//printf("yes\n");


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
	vector* pid_info = int_vector_create();
	vector* status_info = vector_create(string_copy_constructor, string_destructor, string_default_constructor);
	vector* command_info = vector_create(string_copy_constructor, string_destructor, string_default_constructor); 

   

	

	process a;
	pid_t main_pro = getpid();
	a.command = strdup(argv[0]);
	int *temp_1 = malloc(sizeof(pid_t));
	*temp_1 = (int)main_pro;
		
	a.status  = strdup(STATUS_RUNNING);
	vector_push_back(pid_info, temp_1);
	vector_push_back(status_info, a.status);
	vector_push_back(command_info, a.command);
	free(a.command);
	free(temp_1);
	free(a.status);

  	//stdin input main process: no need to create another process
  	if(argc == 1)
	{
		signal (SIGINT,intHandler);
		while(exit_ == 0)
		{
		
			char *directory = getcwd(NULL, 0);
			print_prompt(directory, main_pro);
			free(directory);
		
			char *buffer = NULL;
			size_t size = 0;
			size_t cd = getline(&buffer, &size, stdin);
			if((int)cd == -1)
			{
				for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					int r = *(int*)vector_get(pid_info,j);
					kill(r, SIGTERM);
					vector_erase(pid_info, j);
					vector_erase(command_info,j);
					vector_erase(status_info, j);

				}
				vector_destroy(pid_info);
				vector_destroy(command_info);
				vector_destroy(status_info);
				free(buffer);
				exit(0);
			}
		while(*buffer == '\n')
			{
				
				char *directory = getcwd(NULL, 0);
				print_prompt(directory, main_pro);
				free(directory);
				cd = getline(&buffer, &size, stdin);
			}
	
			size_t len = strlen(buffer);
		
			char **token_array;
			size_t num_tokens;
			token_array = strsplit(buffer, " \n&", &num_tokens);
		//see what is the command
		//ps
		

			if(strcmp(buffer, "ps\n") == 0)
			{
		 		for(int i = 0; i <(int) vector_size(status_info); i++)
		 		print_process_info( vector_get(status_info, i), *(int*)vector_get(pid_info,i), vector_get(command_info,i));
		
			}
		//exit
			else if(strcmp(buffer, "exit\n") == 0 ) 
			{
				for(size_t j = 1; j < vector_size(pid_info); j++)
				{
					int r = *(int*)vector_get(pid_info,j);
					kill(r, SIGINT);
					//printf("%d", r);
				}
				vector_destroy(pid_info);
				vector_destroy(command_info);
				vector_destroy(status_info);
				for(int i = 0; i<(int) num_tokens; i++)
   				{
  					free(token_array[i]);
  				}
  				free(token_array);
				free(buffer);
				return EXIT_SUCCESS;
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
					vector_erase(pid_info,remember);
					vector_erase(status_info,remember);
					vector_erase(command_info,remember);
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
				if(*a == '/')
				{
					char * b = get_full_path(a);
					DIR * re = opendir(b);
					if(re == NULL)
					{
						print_no_directory(b);
						free(b);
						free(re);
						
					}
					else
					{
						chdir(b);
						free(b);
						free(re);
					}
				}

				else
				{
					DIR * re = opendir(a);
					if(re== NULL)
					{
						print_no_directory(a);
					}
					else
					{
						chdir(a);
						free(re);
					}
				}

			}

			//externel command
			else 
			{
			//background
				char* loc = strstr(buffer, "&");

				if(loc != NULL)
				{
					*loc = '\0';
				//	token_array = strsplit(buffer, " \n", &num_tokens);
			//		buffer[len-1]= '\0';
					signal(SIGCHLD, cleanup);
					
				 	child_b = fork();
			 		
			 		
					if (child_b == -1) 
  					print_fork_failed();
  					if(child_b == 0) 
  					{ 
 		   				execvp(token_array[0], token_array);

    					print_exec_failed(buffer);

    					for(int i = 0; i<(int) num_tokens; i++)
   							{
  								free(token_array[i]);
  							}
  								free(token_array);
	
  								free(buffer);

    				
    					break;
   		 				
   					} 
  					else 
  					{ 
  					process b;
					b.command = strdup(buffer);
					int *temp_3 = malloc(sizeof(pid_t));
					*temp_3 = (int)child_b;
					b.status = strdup(STATUS_RUNNING);
					vector_push_back(pid_info, temp_3);
					vector_push_back(status_info, b.status);
					vector_push_back(command_info, b.command);
					free(temp_3);
					free(b.command);
					free(b.status);
  					print_command_executed(child_b);
  					
  	   				}
				}

				else
				{
					fflush(stdout);
					buffer[len-1]= '\0';
				//	signal(SIGCHLD, SIG_DFL);
				 	 child = fork();
				 	
  					if (child == -1) 
  						print_fork_failed();
  					if (child == 0) 
  					{ 
    					execvp(token_array[0], token_array);
    				
    					print_exec_failed(buffer);
    						for(int i = 0; i<(int) num_tokens; i++)
   							{
  								free(token_array[i]);
  							}
  								free(token_array);
	
  								free(buffer);

    							break;
  	 				} 
  					else 
  					{ 

  						process b;
						b.command = strdup(buffer);
						int *temp_3 = malloc(sizeof(pid_t));
						*temp_3 = (int)child;
						b.status = strdup(STATUS_RUNNING);
						vector_push_back(pid_info, temp_3);
						vector_push_back(status_info, b.status);
						vector_push_back(command_info, b.command);
						free(temp_3);
						free(b.command);
						free(b.status);

  						print_command_executed(child);
		
  						int status;
    					int return_value = waitpid(child, &status ,0);
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
					}
   				}		
   			}	
			for(int i = 0; i<(int) num_tokens; i++)
   			{
   			//puts("L");
  				free(token_array[i]);
  			}
  			free(token_array);
	
  			free(buffer);

	 	}
		
	}


	//file input
	else
	{
		FILE *file = fopen(argv[2], "r");
			
  		if(file == NULL)
  		{ // can not open
  			print_script_file_error();
  			fclose(file);
      	//	free(file);
      		return 0;
      		
  		}
//count line numbers
	
   		char *buffer = NULL;
    	size_t length = 0;
    	while (getline(&buffer, &length, file) != -1) 
		{

			pid_t main_pro = getpid();
		//get current directory
			char *directory = getcwd(NULL, 0);
			print_prompt(directory, main_pro);
			free(directory);

				while(*buffer == '\n')
			{
				
				char *directory = getcwd(NULL, 0);
				print_prompt(directory, main_pro);
				free(directory);
				getline(&buffer, &length, file);
			}

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
			token_array = strsplit(buffer, " &", &num_tokens);

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
				//	printf("%d", child);
				
				}
				vector_destroy(pid_info);
				vector_destroy(command_info);
				vector_destroy(status_info);
				for(int i = 0; i<(int) num_tokens; i++)
 	  			{
  					free(token_array[i]);
  				}
  				free(token_array);
		
  				free(buffer);
  				fclose(file);
			
			
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
				if(*a == '/')
				{
					char * b = get_full_path(a);
					DIR * re = opendir(b);
					if(re == NULL)
					{
						print_no_directory(b);
						free(b);
						free(re);
						
					}
					else
					{
						chdir(b);
						free(b);
						free(re);
					}
				}

				else
				{
					DIR * re = opendir(a);
					if(re== NULL)
					{
						print_no_directory(a);
					}
					else
					{
						chdir(a);
						free(re);
					}
				}

			}

	
		//externel command
			else 
			{
				//background
				char* loc = strstr(buffer, "&");
				if(loc != NULL)
				{
					*loc = '\0';
					//token_array = strsplit(buffer, " \n", &num_tokens);
				//	buffer[len-1]= '\0';

					signal(SIGCHLD, cleanup);
			
				

			 		child_b = fork();
			 		
  					if (child_b == -1) 
  						print_fork_failed();
 	 				if (child_b == 0) 
  					{ 
    					execvp(token_array[0], token_array);
    				
    					print_exec_failed(buffer);
    					for(int i = 0; i<(int) num_tokens; i++)
   							{
  								free(token_array[i]);
  							}
  								free(token_array);
	
  								free(buffer);

    							
    					break;
  	 				} 
  					else 
  					{ 
  					process b;
					b.command = strdup(buffer);
					int *temp_3 = malloc(sizeof(pid_t));
					*temp_3 = (int)child_b;
					b.status = strdup(STATUS_RUNNING);
					vector_push_back(pid_info, temp_3);
					vector_push_back(status_info, b.status);
					vector_push_back(command_info, b.command);
					free(temp_3);
					free(b.command);
					free(b.status);
  					print_command_executed(child_b);
					//exit(1);
   					}
				}

				else
				{	
					buffer[len-1]= '\0';
					child = fork();
			 		
				 	
  					if (child == -1) 
 	 					print_fork_failed();

  					if (child == 0) 
  					{ 
    					execvp(token_array[0], token_array);
    			
  	  					print_exec_failed(buffer);
  	  					for(int i = 0; i<(int) num_tokens; i++)
   							{
  								free(token_array[i]);
  							}
  								free(token_array);
	
  								free(buffer);

    	
    					break;
  	 				} 
  					else 
  					{ 

  						process b;
						b.command = strdup(buffer);
						int *temp_3 = malloc(sizeof(pid_t));
						*temp_3 = (int)child;
						b.status = strdup(STATUS_RUNNING);
						vector_push_back(pid_info, temp_3);
						vector_push_back(status_info, b.status);
						vector_push_back(command_info, b.command);
						free(temp_3);
						free(b.command);
						free(b.status);
  				
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
   			{
   				//puts("L");
  				free(token_array[i]);
  			}
  			free(token_array);
  		}
  	
  		
  		free(buffer);
   		fclose(file);
   		for(size_t j = 1; j < vector_size(pid_info); j++)
		{
			int child = *(int*)vector_get(pid_info,j);

			kill(child, SIGTERM);
			//printf("%d", child);
				
		}
		vector_destroy(pid_info);
		vector_destroy(command_info);
		vector_destroy(status_info);
	//	
   		exit(0);
	}

		//	exit(0);
		
		

			
	vector_destroy(pid_info);
	vector_destroy(command_info);
	vector_destroy(status_info);
	return 0;
 
}
