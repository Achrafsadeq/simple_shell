#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIM " \t\r\n\a"

/* Function prototypes */
char **tokenize_input(char *input);
int process_command(char **tokens);
int execute_program(char **tokens);
char *find_executable(char *command);
int change_directory(char **parameters);
int display_help(char **parameters);
int terminate_shell(char **parameters);
int show_environment(char **parameters);

/* Environment variables */
extern char **environ;

#endif /* SHELL_H */
