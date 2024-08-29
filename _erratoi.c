#include "shell.h"

/**
 * _erratoi - Transforms a string into an integer.
 * @s: The input string to be transformed.
 *
 * Return: If the string contains no digits, returns 0;
 * otherwise, returns the converted integer.
 *         Returns -1 in case of an error.
 */
int _erratoi(char *s)
{
	int index = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (index = 0; s[index] != '\0'; index++)
	{
		if (s[index] >= '0' && s[index] <= '9')
		{
			result *= 10;
			result += (s[index] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error - Outputs an error message.
 * @info: Structure containing parameters and return information.
 * @estr: A string that specifies the type of error.
 *
 * Return: Returns the converted number if the string contains digits,
 * otherwise returns 0; returns -1 on error.
 */
void print_error(info_t *info, char *estr)
{
	_eputs(info->fname);
	_eputs(": ");
	print_d(info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_d - Displays a decimal (integer) number in base 10.
 * @input: The number to print.
 * @fd: The file descriptor where the output is directed.
 *
 * Return: The total number of characters printed.
 */
int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int index, count = 0;
	unsigned int _abs_, curr;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	curr = _abs_;
	for (index = 1000000000; index > 1; index /= 10)
	{
		if (_abs_ / index)
		{
			__putchar('0' + curr / index);
			count++;
		}
		curr %= index;
	}
	__putchar('0' + curr);
	count++;

	return (count);
}

/**
 * convert_number - Converts a number to a string in a specified base.
 * @num: The number to convert.
 * @base: The base for conversion.
 * @flags: Flags to modify behavior of conversion.
 *
 * Return: A pointer to the converted string.
 */
char *convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign_char = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign_char = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign_char)
		*--ptr = sign_char;
	return (ptr);
}

/**
 * remove_comments - Strips comments by replacing the first instance of '#'.
 * @buf: The string to be modified.
 *
 * Return: Always returns 0.
 */
void remove_comments(char *buf)
{
	int index;

	for (index = 0; buf[index] != '\0'; index++)
		if (buf[index] == '#' && (!index || buf[index - 1] == ' '))
		{
			buf[index] = '\0';
			break;
		}
}
