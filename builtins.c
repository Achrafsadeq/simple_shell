#include "shell.h"

/**
 * builtin_exit - Exit the shell with a given status
 * @args: Array of command arguments
 *
 * Return: Does not return, exits the program
 */
int builtin_exit(char **args)
{
if (args[1] != NULL)
{
int status = atoi(args[1]);
exit(status);
}
exit(EXIT_SUCCESS);
}

/**
 * builtin_env - Print the current environment
 * @args: Array of command arguments (unused)
 *
 * Return: Always returns 1 to continue the shell
 */
int builtin_env(char **args)
{
	int i = 0;

	(void)args;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (1);
}

/**
 * builtin_setenv - Set an environment variable
 * @args: Array of command arguments
 *
 * Return: Always returns 1 to continue the shell
 */
int builtin_setenv(char **args)
{
	if (args[1] == NULL || args[2] == NULL)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (1);
	}
	if (setenv(args[1], args[2], 1) != 0)
		perror("setenv");
	return (1);
}

/**
 * builtin_unsetenv - Unset an environment variable
 * @args: Array of command arguments
 *
 * Return: Always returns 1 to continue the shell
 */
int builtin_unsetenv(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (1);
	}
	if (unsetenv(args[1]) != 0)
		perror("unsetenv");
	return (1);
}

/**
 * builtin_cd - Change the current directory
 * @args: Array of command arguments
 *
 * Return: Always returns 1 to continue the shell
 */
int builtin_cd(char **args)
{
	char *dir = args[1];
	char cwd[1024];

	if (dir == NULL)
		dir = getenv("HOME");
	else if (strcmp(dir, "-") == 0)
		dir = getenv("OLDPWD");

	if (chdir(dir) != 0)
	{
		perror("cd");
	}
	else
	{
		getcwd(cwd, sizeof(cwd));
		setenv("OLDPWD", getenv("PWD"), 1);
		setenv("PWD", cwd, 1);
	}
	return (1);
}
