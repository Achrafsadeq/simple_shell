#ifndef SHELL_CORE_H
#define SHELL_CORE_H

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

#define MAX_BUFFER 1024
#define DELIMITER_TOKENS " \t\r\n\a"
#define BUFFER_SIZE 1024
#define MAX_ARGS 64

/* Function declarations */
char **tokenize_input(char *input, const char *delim);
char **parse_logical_operators(char *input);
int process_command(char **tokens);
int execute_external(char **tokens);
char *locate_command(const char *cmd);
int builtin_change_directory(char **tokens);
int builtin_display_help(char **tokens);
int builtin_terminate(char **tokens);
int builtin_show_env(char **tokens);
int builtin_set_env(char **tokens);
int builtin_unset_env(char **tokens);

/* Function declarations */
void execute_command(char *command);
int builtin_cd(char **args);
int builtin_exit(char **args);
int builtin_env(char **args);
int builtin_setenv(char **args);
int builtin_unsetenv(char **args);
int builtin_display_help(char **args);
extern char **environ;

#endif /* SHELL_CORE_H */
