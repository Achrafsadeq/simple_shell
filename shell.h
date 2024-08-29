#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"

extern char **environ;

char *custom_getline(void);
char **split_line(char *line);
int execute(char **args);
int launch(char **args);
int builtin_exit(char **args);
int builtin_env(char **args);
int builtin_setenv(char **args);
int builtin_unsetenv(char **args);
int builtin_cd(char **args);
char *get_location(char *command);

#endif
