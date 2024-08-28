#include "shell.h"

/**
 * check_exec - Checks if the specified file is an executable command.
 * @info: Pointer to the info structure.
 * @path: File path to be checked.
 *
 * Return: Returns 1 if the file is executable, otherwise 0.
 */
int check_exec(info_t *info, char *path)
{
	struct stat stat_buf;

	(void)info;
	/* Ensure the path is valid and retrieve file status */
	if (!path || stat(path, &stat_buf))
		return (0);

	/* Check if the file is a regular file */
	if (stat_buf.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * copy_range - Extracts and duplicates a substring of characters.
 * @pathstr: The string representing the PATH.
 * @start: The starting index for the substring.
 * @stop: The stopping index for the substring.
 *
 * Return: Pointer to the buffer containing the duplicated characters.
 */
char *copy_range(char *pathstr, int start, int stop)
{
	static char buffer[1024];
	int idx_start = 0, idx_buf = 0;

	/* Copy characters from the specified range */
	for (idx_buf = 0, idx_start = start; idx_start < stop; idx_start++)
		if (pathstr[idx_start] != ':')
			buffer[idx_buf++] = pathstr[idx_start];
	buffer[idx_buf] = '\0'; /* Null-terminate the buffer */
	return (buffer);
}

/**
 * locate_cmd - Locates a command within the PATH environment variable.
 * @info: Pointer to the info structure.
 * @pathstr: The string representing the PATH environment variable.
 * @cmd: The command to be located.
 *
 * Return: Full path of the command if found, otherwise NULL.
 */
char *locate_cmd(info_t *info, char *pathstr, char *cmd)
{
	int idx = 0, pos = 0;
	char *full_path;

	/* Return NULL if PATH string is empty */
	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		/* Check if command is a valid executable in the current directory */
		if (check_exec(info, cmd))
			return (cmd);
	}
	while (1)
	{
		/* Check for the end of a directory path in PATH */
		if (!pathstr[idx] || pathstr[idx] == ':')
		{
			full_path = copy_range(pathstr, pos, idx);
			if (!*full_path)
				_strcat(full_path, cmd);
			else
			{
				_strcat(full_path, "/");
				_strcat(full_path, cmd);
			}
			/* Check if the constructed path is an executable */
			if (check_exec(info, full_path))
				return (full_path);
			if (!pathstr[idx])
				break;
			pos = idx;
		}
		idx++;
	}
	return (NULL); /* Command not found */
}
