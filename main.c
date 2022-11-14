#include "shell.h"

void sig_handler(int sig);
int execute(char **args, char **front);

/**
* sig_handler - creates a prompt after a signal
* @sig: signal
*/
void sig_handler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
*execute - excecute a command in a child process 
* @args: arguments array
* @front: double pointer to the beginning of args
* Return: erro? corresponding error code; otherwise,
* exit value of the last command
*/
int execute(char **args, char **front)
{
	pid_t child_pid;
	int status, flag = 0, ret = 0;
	char *command = args[0];

	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		command = get_location(command);
	}

	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (create_error(args, 126));
		else
			ret = (create_error(args, 127));
	}
	else
	{
		child_pid = fork();

		if (child_pid == -1)
		{
			if (flag)
				free(command);
			perror("Child error : ");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(command, args, environ);
			if (errno == EACCES)
				ret = (create_error(args, 126));
			free_env();
			free_args(args, front);
			free_alias_list(aliases);
			_exit(ret);
		}
		else
		{
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}
	if (flag)
		free (command);
	return (ret);
}
