#include "shell_1.h"
/**
 * main - creates a simple shell
 *
 *
 * Return: 0
 */
char *lsh_read_line();
char **lsh_split_line(char *line);
void lsh_exec(char **args);
int main(void)
{
	char *line;
	char **tokens;

	while (1)
	{
		printf("$ ");
		line = lsh_read_line();
		tokens = lsh_split_line(line);
		if (tokens[0] != NULL)
		{
			lsh_exec(tokens);
		}
		free(tokens);
		free(line);
	}
}
/**
 * lsh_read_line - Parses user input
 *
 * Return: The string stored
 */
char *lsh_read_line()
{
	char *buffer = NULL;
	size_t n = 0;

	if (getline(&buffer, &n, stdin) == -1)
	{
		if (feof(stdin))
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}
	return (buffer);
}
/**
 * lsh_split_line - splits strings into arguments and commands
 * @line: the string to be split
 *
 * Return: the tokenized string
 */
char **lsh_split_line(char *line)
{
	int length = 0;
	int capacity = 16;
	char **tokens = malloc(sizeof(char *) * capacity);
	char *delimiters = "\t\r\n\a";
	char *token = strtok(line, delimiters);

	while (token)
	{
		tokens[length] = token;
		token = strtok(NULL, delimiters);
		length++;
	}

	tokens[length] = NULL;
	return (tokens);
}
/**
 * lsh_exec - executes the command and arguments
 * @args: the arguments to be passed
 *
 * Return: 0
 */
void lsh_exec(char **args)
{
	pid_t childpid;

	childpid = fork();
	if (childpid == 0)
	{
		execve(args[0], args, NULL);
		perror("./shell");
		exit(1);
	}
	else if (childpid > 0)
	{
		int status;

		do {
			waitpid(childpid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	else
	{
		perror("lsh");
	}
}
