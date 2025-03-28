#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"


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

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {

	if (cmd_line == NULL || cmd_buff == NULL) {
		return ERR_MEMORY;
	}
	
	while (*cmd_line == ' ') {
		cmd_line++;
	}
	
	char *cmd_end = cmd_line + strlen(cmd_line) - 1;
	while (cmd_end > cmd_line && *cmd_end == ' ') {
		*cmd_end = '\0';
		cmd_end--;
	}

	memset(cmd_buff, 0, sizeof(cmd_buff_t));
	
	cmd_buff->_cmd_buffer = strdup(cmd_line);
	if (cmd_buff->_cmd_buffer == NULL) {
		return ERR_MEMORY;
	}

	char *cmd_pointer = cmd_buff->_cmd_buffer;
	cmd_buff->argc = 0;
	char *token;
	while (*cmd_pointer != '\0') {
		if (*cmd_pointer == ' ') {
			cmd_pointer++;
		}
		else if (*cmd_pointer == '"') {
			cmd_pointer++;
			token = cmd_pointer;
			while (*cmd_pointer && *cmd_pointer != '"') {
				cmd_pointer++;
			}
			if (*cmd_pointer == '"') {
				*cmd_pointer = '\0';
				cmd_pointer++;
			}
			cmd_buff->argv[cmd_buff->argc++] = token;
		}
		else {
			token = cmd_pointer;
			while (*cmd_pointer && *cmd_pointer != ' ') {
				cmd_pointer++;
			}
			if (*cmd_pointer != '\0') {
				*cmd_pointer = '\0';
				cmd_pointer++;
			}
			cmd_buff->argv[cmd_buff->argc++] = token;
		}
	}
	cmd_buff->argv[cmd_buff->argc] = NULL;
	return OK;
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

		 rc = build_cmd_buff(cmd_buff, &cmd); 
		 if (rc != OK) {
			 fprintf(stderr, "error: failed to parse command\n");
			 continue;
		 }

		 if (cmd.argc == 0) {
			 continue;
		 }

		 if (exec_built_in_cmd(&cmd) == BI_EXECUTED) {
			 continue;
		 }

		 int f_result = fork();
		 int status;

		 if (f_result < 0) {
			 perror("fork error");
			 continue;
		 }

		 if (f_result == 0) {
			 execvp(cmd.argv[0], cmd.argv);
			 perror("execvp error");
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
