#include "shell.h"

/**
 * split_line - Splits a line into tokens
 * @line: The line to split
 *
 * Return: An array of tokens
 */
char **split_line(char *line)
{
	int bufsize = BUFFER_SIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOKEN_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += BUFFER_SIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TOKEN_DELIM);
	}
	tokens[position] = NULL;
	return (tokens);
}

/**
 * execute - Executes a command
 * @args: Array of arguments
 *
 * Return: 1 if the shell should continue running, 0 if it should terminate
 */
int execute(char **args)
{
	int i;
	char *builtin_str[] = {
		"cd",
		"help",
		"exit",
		"env"
	};
	int (*builtin_func[]) (char **) = {
		&shell_cd,
		&shell_help,
		&shell_exit,
		&shell_env
	};

	if (args[0] == NULL)
		return (1);

	for (i = 0; i < 4; i++)
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
			return ((*builtin_func[i])(args));
	}

	return (launch(args));
}

/**
 * launch - Launches a program
 * @args: Array of arguments
 *
 * Return: Always returns 1, to continue execution
 */
int launch(char **args)
{
	pid_t pid;
	int status;
	char *command_path;

	pid = fork();
	if (pid == 0)
	{
		/* Child process */
		command_path = get_location(args[0]);
		if (command_path == NULL)
		{
			fprintf(stderr, "%s: command not found\n", args[0]);
			exit(EXIT_FAILURE);
		}

		if (execve(command_path, args, environ) == -1)
		{
			perror("Error:");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		/* Error forking */
		perror("Error:");
	}
	else
	{
		/* Parent process */
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return (1);
}

/**
 * get_location - Gets the location of a command
 * @command: The command to locate
 *
 * Return: The path of the command or NULL if not found
 */

