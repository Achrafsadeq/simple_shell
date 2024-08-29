#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static int (*builtin_func[])(char **) = {
	&builtin_exit,
	&builtin_env,
	&builtin_setenv,
	&builtin_unsetenv,
	&builtin_cd
};

static char *builtin_str[] = {
	"exit",
	"env",
	"setenv",
	"unsetenv",
	"cd"
};

/**
 * num_builtins - Get the number of builtin functions
 *
 * Return: The number of builtin functions
 */
int num_builtins(void)
{
	return (sizeof(builtin_str) / sizeof(char *));
}

/**
 * execute - Execute builtin or external commands
 * @args: Array of command arguments
 *
 * Return: 1 if the shell should continue running, 0 if it should terminate
 */
int execute(char **args)
{
	int i;

	if (args[0] == NULL)
		return (1);

	for (i = 0; i < num_builtins(); i++)
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
			return ((*builtin_func[i])(args));
	}

	return (launch(args));
}

/**
 * launch - Launch an external program
 * @args: Array of command arguments
 *
 * Return: Always returns 1 to continue the shell
 */
int launch(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		char *command_path = get_location(args[0]);

		if (command_path == NULL)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
		if (execv(command_path, args) == -1)
			perror("execv");
		free(command_path);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return (1);
}
