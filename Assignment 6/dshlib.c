#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int execute_pipeline(command_list_t *clist) {
	int num_commands = clist->num;
	int pipes[num_commands - 1][2];  // Array of pipes
   pid_t pids[num_commands];        // Array to store process IDs

   for (int i = 0; i < num_commands - 1; i++) {
       if (pipe(pipes[i]) == -1) {
           perror("pipe");
			  return -1;
       }
   }

    // Create processes for each command
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
				return -1;
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
				exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
	 
	 for (int i = 0; i < num_commands; i++) {
		 waitpid(pids[i], NULL, 0);
	 }
}


int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	while (*cmd_line == SPACE_CHAR) {
		    cmd_line++;
	}

	int len = strlen(cmd_line);
	while (len > 0 && cmd_line[len - 1] == SPACE_CHAR) {
		    cmd_line[len - 1] = '\0';
			 len--;
	}
	if (len == 0) {
		return WARN_NO_CMDS;
	}

	int cmd_count = 0;
	char *cmd_pointer = NULL;
	char *arg_pointer = NULL;
	char *cmd = strtok_r(cmd_line, PIPE_STRING, &cmd_pointer);

	while (cmd != NULL && cmd_count < CMD_MAX) {
		memset(&clist->commands[cmd_count], 0, sizeof(command_t));
		clist->commands[cmd_count].argc = 0;

		char *arg = strtok_r(cmd, " ", &arg_pointer);

		while (arg != NULL) {
			if (clist->commands[cmd_count].argc >= CMD_ARGV_MAX - 1) {
				return ERR_CMD_OR_ARGS_TOO_BIG;
			}
			clist->commands[cmd_count].argv[clist->commands[cmd_count].argc++] = strdup(arg);
			arg = strtok_r(NULL, " ", &arg_pointer);
		}

		clist->commands[cmd_count].argv[clist->commands[cmd_count].argc] = NULL;
		cmd_count++;
		cmd = strtok_r(NULL, PIPE_STRING, &cmd_pointer);
	}

	if (cmd != NULL && cmd_count >= CMD_MAX) {
			return ERR_TOO_MANY_COMMANDS;
	}

	clist->num = cmd_count;
	return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
	if (strcmp(cmd->argv[0], "exit") == 0) {
		exit(0);
	}
	else if (strcmp(cmd->argv[0], "cd") == 0) {
		if (cmd->argc == 1) {
			return BI_EXECUTED;
		}
		if (chdir(cmd->argv[1]) != 0) {
				perror("cd");
				return BI_NOT_BI;
		}
		return BI_EXECUTED;
		}
	return BI_NOT_BI;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;
	 command_list_t clist;
	 cmd_buff_t cmd;

	 while(1) {
		 printf("%s", SH_PROMPT);
		 if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
			 printf("\n");
			 break;
		 }

		 cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
	
		 if (strlen(cmd_buff) == 0) {
			 printf("%s\n", CMD_WARN_NO_CMD);
			 continue;
		 }

		 rc = build_cmd_list(cmd_buff, &clist); 
		 if (rc != OK) {
			 fprintf(stderr, "error: failed to parse command\n");
			 continue;
		 }

		 int f_result = execute_pipeline(&clist);
		 int status;
		 if (f_result < 0) {
			 perror("fork error");
			 continue;
		 }
		 else if (f_result == 0) {
			 execvp(cmd.argv[0], cmd.argv);
			 perror("execvp");
			 exit(ERR_EXEC_CMD);
		 }
		 else {
			 wait(&status);
		 }

	 }
	 free(cmd_buff);


    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}
