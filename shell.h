#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

/* global environment */
extern char **environ;

/**
* struct list_s - struct defining a linked list
* @dir: directory path
* @next: point to the next struct list_s
*/
typedef struct list_s
{
	char *dir;
	struct lists *next;
} list_t;

/*main helpers */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **_strtok(char *line, char *delim);
char *get_location(char *command);
list_t *get_path_dir(char *path);
int execute(char **args, char **front);
void free_list(list_t *head);
char *_itoa(int num);

#endif
