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
*
*
*
*/
int execute(char **args, char **front)
{
	
}
