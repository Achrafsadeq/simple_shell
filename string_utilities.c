#include "shell.h"

/**
 * str_copy_limit - Copies a string up to a given number of characters.
 * @dest: Pointer to the destination buffer.
 * @src: Pointer to the source string.
 * @n: Maximum number of characters to copy.
 *
 * Return: Returns a pointer to the destination string.
 */
char *str_copy_limit(char *dest, char *src, int n)
{
	int x, y;
	char *output = dest; /* Store destination address */

	x = 0;
	while (src[x] != '\0' && x < n - 1)
	{
		dest[x] = src[x];
		x++;
	}
	if (x < n)
	{
		y = x;
		while (y < n) /* Fill with null characters */
		{
			dest[y] = '\0';
			y++;
		}
	}
	return (output); /* Return result */
}

/**
 * str_append_limit - Appends one string to another.
 * @dest: Destination string.
 * @src: Source string to append.
 * @n: Maximum number of bytes to append.
 *
 * Return: Pointer to the concatenated string.
 */
char *str_append_limit(char *dest, char *src, int n)
{
	int m, z;
	char *result = dest; /* Store destination address */

	m = 0;
	z = 0;
	while (dest[m] != '\0') /* Find end of destination */
		m++;
	while (src[z] != '\0' && z < n) /* Append characters */
	{
		dest[m] = src[z];
		m++;
		z++;
	}
	if (z < n)
		dest[m] = '\0'; /* Null-terminate */
	return (result); /* Return result */
}

/**
 * str_find_char - Searches for the first occurrence of a character in str.
 * @s: Pointer to the string.
 * @c: Character to locate.
 *
 * Return: Pointer to the first occurrence, or NULL.
 */
char *str_find_char(char *s, char c)
{
	do {
		if (*s == c) /* Character found */
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
