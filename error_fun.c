#include "shell.h"

/**
 * _eputs - Outputs a string to stderr
 * @str: Pointer to the string to be output
 *
 * Description: This function takes a string and sends it char by char
 * to the standard error stream.
 *
 * Return: This function does not return any value.
 */
void _eputs(char *str)
{
	int index = 0;

	if (!str)
		return;
	while (str[index] != '\0')
	{
		_eputchar(str[index]);
		index++;
	}
}

/**
 * _eputchar - Writes a character to the standard error stream
 * @c: Character to be written
 *
 * Description: This function writes a single character to the standard
 * error stream.
 * It handles buffering and flushes the buffer when necessary.
 *
 * Return: On success, returns 1. On failure, returns -1.
 */
int _eputchar(char c)
{
	static int index;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(2, buf, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buf[index++] = c;
	return (1);
}

/**
 * _putfd - Writes a character to a specified file descriptor
 * @c: The character to be written
 * @fd: The file descriptor to which the character should be written
 *
 * Description: This function writes a single character to the given file desc
 * and manages buffering to optimize write operations.
 *
 * Return: 1 on success. On failure, returns -1 and sets errno as appropriate.
 */
int _putfd(char c, int fd)
{
	static int index;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || index >= WRITE_BUF_SIZE)
	{
		write(fd, buf, index);
		index = 0;
	}
	if (c != BUF_FLUSH)
		buf[index++] = c;
	return (1);
}

/**
 * _putsfd - Sends a string to a specified file descriptor
 * @str: Pointer to the string to be sent
 * @fd: The file descriptor where the string will be written
 *
 * Description: This function iterates over a string and writes each
 * character to the specified file descriptor,
 * while tracking the number of characters written.
 *
 * Return: Returns the total number of characters written.
 */
int _putsfd(char *str, int fd)
{
	int index = 0;

	if (!str)
		return (0);
	while (*str)
	{
		index += _putfd(*str++, fd);
	}
	return (index);
}
