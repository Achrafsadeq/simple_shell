#include "shell.h"

/**
 * custom_getline - reads a line of input from the standard input
 * Return: pointer to the read line, or NULL on failure
 */
char *custom_getline(void)
{
	static char buffer[BUFFER_SIZE];
	static int buffer_pos;
	static int buffer_size;
	char *line = NULL;
	int line_size = 0;
	int line_pos = 0;

	while (1)
	{
		if (buffer_pos >= buffer_size)
		{
			buffer_size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			buffer_pos = 0;
			if (buffer_size <= 0)
			{
				return ((line_pos > 0) ? line : NULL);
			}
		}

		char c = buffer[buffer_pos++];

		if (line_pos >= line_size)
		{
			line_size += BUFFER_SIZE;
			line = realloc(line, line_size);
			if (!line)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}

		line[line_pos++] = c;

		if (c == '\n')
		{
			line[line_pos - 1] = '\0';
			return (line);
		}
	}
}
