#include "shell.h"

/**
 * split_line - splits a line into tokens using a delimiter
 * @line: the line to be split
 * Return: an array of tokens
 */
char **split_line(char *line)
{
	int bufsize = BUFFER_SIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token, *saveptr;

	if (!tokens)
	{
		fprintf(stderr, "split_line: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok_r(line, TOKEN_DELIM, &saveptr);
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
				fprintf(stderr, "split_line: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok_r(NULL, TOKEN_DELIM, &saveptr);
	}
	tokens[position] = NULL;
	return (tokens);
}
