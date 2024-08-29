#include "shell.h"

/**
 * main - Program entry point.
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * This function initializes necessary data structures, sets up file desc,
 * and starts the shell program.
 *
 * Return: 0 if execution completes successfully; 1 if there is an error.
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	int file_descriptor = 2; /* Initialize file_descriptor to standard error */

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (file_descriptor)
		: "r" (file_descriptor));

	if (ac == 2)
	{
		/* Attempt to open the file specified by the command-line argument */
		file_descriptor = open(av[1], O_RDONLY);
		if (file_descriptor == -1)
		{
			if (errno == EACCES)
				exit(126); /* Permission denied */
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127); /* File not found */
			}
			return (EXIT_FAILURE); /* General error */
		}
		info->readfd = file_descriptor;
	}

	/* Populate environment variables and read history */
	populate_env_list(info);
	read_history(info);

	/* Start the shell */
	hsh(info, av);

	return (EXIT_SUCCESS); /* Successful execution */
}
