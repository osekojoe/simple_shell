#include "shell.h"
/**
 * main - creates a simple shell
 *
 *
 * Return: 0
 */
char *_getline();
char **tokenize(char *line);
void _execute(char **args);
int main(void)
{
	char *line;
	char **tokens;

	while (1)
	{
		printf("$ ");
		line = _getline();
		tokens = tokenize(line);
		if (tokens[0] != NULL)
		{
			_execute(tokens);
		}
		free(tokens);
		free(line);
	}
}
/**
 * _getline - Parses user input
 *
 * Return: The string stored
 */
char *_getline()
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
 * tokenize - splits strings into arguments and commands
 * @line: the string to be split
 *
 * Return: the tokenized string
 */
char **tokenize(char *line)
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
 * _execute - executes the command and arguments
 * @args: the arguments to be passed
 *
 * Return: 0
 */
void _execute(char **args)
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
