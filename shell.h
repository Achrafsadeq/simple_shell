#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>    /* Standard I/O functions */
#include <stdlib.h>   /* General utilities:memory management, program control*/
#include <string.h>   /* String handling functions */
#include <unistd.h>   /* POSIX API for system calls like fork, exec, etc. */
#include <sys/wait.h> /* Wait for process state changes */
#include <sys/types.h>/* Defines data types used in system calls */
#include <limits.h>   /* Implementation-defined constants, like PATH_MAX */
#include <fcntl.h>    /* File control options for operations like open, close*/
#include <errno.h>    /* Error number definitions for handling errors */
#include <sys/stat.h> /* Defines structure of data returned by stat functions*/

/* Constants for buffer sizes during read/write operations */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH     -1

/* Command chaining types for the shell */
#define CMD_NORM  0
#define CMD_OR    1
#define CMD_AND   2
#define CMD_CHAIN 3

/* Flags used in number conversion functions */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED  2

/* Flags for using different getline implementations */
#define USE_GETLINE 0
#define USE_STRTOK  0

/* Definitions related to command history */
#define HIST_FILE ".simple_shell_history"
#define HIST_MAX  4096

extern char **environ;

/**
 * struct liststr - Node structure for a singly linked list
 * @num: Integer value stored in the node
 * @str: String data associated with the node
 * @next: Pointer to the next node in the list
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - Encapsulates shell execution context and command data
 *                   for standardized function interfaces
 * @arg: Raw command line input from user
 * @argv: Tokenized command arguments
 * @path: Resolved path of executable
 * @argc: Number of command arguments
 * @line_count: Total lines processed
 * @err_num: Last error code
 * @linecount_flag: Toggle for line counting
 * @fname: Name of current script file
 * @env: Environment variables as linked list
 * @environ: Array representation of environment
 * @history: Command history structure
 * @alias: Command aliases structure
 * @env_changed: Indicator for environment modifications
 * @status: Exit status of previous command
 * @cmd_buf: Buffer for handling command chains
 * @cmd_buf_type: Logical operator for command chaining
 * @readfd: Input stream file descriptor
 * @histcount: Total number of history entries
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;
	char **cmd_buf; /* Buffer for handling command chains */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct builtin - Represents a built-in command and its associated function.
 * @type: The flag indicating the built-in command.
 * @func: The function pointer associated with the built-in command.
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* shell_executor.c */
/* Prototype: Main shell loop */
int execute_shell(info_t *info, char **args);
/* Prototype: Locate builtin command */
int search_builtin(info_t *info);
/* Prototype: Search for a command in PATH */
void search_command(info_t *info);
/* Prototype: Create a process to run command */
void execute_command(info_t *info);
#endif /* End of SHELL_H */
