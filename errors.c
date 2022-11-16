#include "shell.h"

int num_len(int num);
char *_itoa(int num);
int create_error(char **args, int err);

/**
* num_len - length of a number (digits)
* @num: number
* Return: num of digits
*/
int num_len(int num)
{
	unsigned int n;
	int len = 1;

	if (num < 0)
	{
		len++;
		n = num * -1;
	}
	else
	{
		n = num;
	}
	while (n > 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

/**
* _itoa - convert integer to string
* @num: integer
* Return: string
*/
char *_itoa(int num)
{
	char *buffer;
	int len = num_len(num);
	unsigned int n;

	buffer = malloc(sizeof(char) * (len + 1));
	if (!buffer)
		return (NULL);
	buffer[len] = '\0';

	if (num < 0)
	{
		n = num * -1;
		buffer[0] = '-';
	}
	else
	{
		n = num;
	}
	len--;

	while (n > 0)
	{
		buffer[len] = (n % 10) + '0';
		n = n / 10;
		len--;
	}

	return (buffer);
}

/**
* create_error - write custom error msg to stderr
* @args: arguments array
* @err: error value
* Return: error value
*/
int create_error(char **args, int err)
{
	char *error;

	switch (err)
	{
		case -1:
			error = error_env(args);
			break;
		case 1:
			error_1(args);
			break;
		case 2:
			if (*(args[0]) == 'e')
				error = error_2_exit(++args);
			else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
				error = error_2_syntax(args);
			else
				error = error_2_cd(args);
			break;
		case 126:
			error = error_126(args);
			break;
		case 127:
			error = error_127(args);
			break;
	}
	write(STDERR_FILENO, error, _strlen(error));

	if (error)
		free(error);

	return (err);
}
