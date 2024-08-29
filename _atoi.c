#include "shell.h"

/**
 * interactive - Checks if the shell is running in interactive mode.
 * @info: Pointer to the struct containing relevant information.
 *
 * Return: 1 if in interactive mode, 0 otherwise.
 */
int interactive(info_t *info)
{
	/* Check if input is from a terminal and file descriptor is valid */
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delim - Determines if a character matches any in the delimiter string.
 * @c: The character to evaluate.
 * @delim: The string containing delimiter characters.
 * Return: 1 if the character is found in the delimiter string, 0 if not.
 */
int is_delim(char c, char *delim)
{
	/* Loop through the delimiter string to find a match */
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 * _isalpha - Verifies if the input is an alphabetic character.
 * @c: The character to be checked.
 * Return: 1 if the character is a letter, 0 if not.
 */
int _isalpha(int c)
{
	/* Check if the character is within alphabetic ranges */
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * _atoi - Converts a string to its integer value.
 * @s: The input string to convert.
 * Return: The converted integer, or 0 if no numbers are found.
 */
int _atoi(char *s)
{
	int i, direction = 1, state = 0, result_output;
	unsigned int result = 0;

	/* Iterate through the string to parse the integer value */
	for (i = 0; s[i] != '\0' && state != 2; i++)
	{
		if (s[i] == '-')
			direction *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			state = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (state == 1)
			state = 2;
	}

	/* Apply the sign to the result and return the output */
	if (direction == -1)
		result_output = -result;
	else
		result_output = result;

	return (result_output);
}
