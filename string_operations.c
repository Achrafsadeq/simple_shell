#include "shell.h"

/**
 * calculate_strlen - Calculates the length of a given string.
 * @str: Pointer to the string to be measured.
 *
 * Return: The length of the string as an integer.
 */
int calculate_strlen(char *str)
{
	int length = 0;

	if (!str) /* Check if the string is NULL */
		return (0);

	while (*str++)
		length++;
	return (length);
}

/**
 * compare_strings - Compares two strings in lexicographic order.
 * @str1: Pointer to the first string.
 * @str2: Pointer to the second string.
 *
 * Return: A negative value if str1 < str2, a positive value if str1 > str2,
 * or zero if str1 equals str2.
 */
int compare_strings(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2) /* Check for differing characters */
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	if (*str1 == *str2) /* Check if both strings ended together */
		return (0);
	else
		return (*str1 < *str2 ? -1 : 1);
}

/**
 * find_substring - Determines if a string starts with a specific substring.
 * @main_str: The main string to search in.
 * @sub_str: The substring to look for at the start of main_str.
 *
 * Return: A pointer to the next character in main_str if found,otherwise NULL.
 */
char *find_substring(const char *main_str, const char *sub_str)
{
	while (*sub_str) /* Iterate through the substring */
		if (*sub_str++ != *main_str++)
			return (NULL);
	return ((char *)main_str);
}

/**
 * concatenate_strings - Appends the source string to the destination string.
 * @destination: The destination buffer to which the string will be appended.
 * @source: The source string to be appended.
 *
 * Return: A pointer to the destination buffer.
 */
char *concatenate_strings(char *destination, char *source)
{
	char *result = destination; /* Save the start of the destination */

	while (*destination)
		destination++;
	while (*source)
		*destination++ = *source++;
	*destination = *source;
	return (result);
}
