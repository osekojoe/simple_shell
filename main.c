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
* execute - excecute a command in a child process
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
		free(command);
	return (ret);
}

/**
* main - simple UNIX command interpreter
* @argc: number of arguments
* @argv: array of pointers to the arguments
* Return: Value of last command
*/
int main(int argc, char **argv[])
{
	int ret = 0, retn;
	int *exe_ret = &retn;
	char *prompt = "$ ", *newline = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, sig_handler);

	*exe_ret = 0;
	environ = _copyenv();

	if (!environ)
		exit(-100);
	if (argc != 1)
	{
		ret = proc_file_commands(argv[1], exe_ret);
		free_env();
		free_alias_list(aliases);

		return (*exe_ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handle_args(exe_ret);
		free_env();
		free_alias_list(aliases);

		return (*exe_ret);
	}

	while (1)
	{
		write(STD_FILENO, prompt, 2);
		ret = handle_args(exe_ret);
		if (ret == ENF_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, newline, 1);
			free_env();
			free_alias_list(aliases);
			exit(*exe_ret);
		}
	}
	free_env();
	free_alis_list(aliases);

	return (*exe_ret);
}
