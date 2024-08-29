#include "shell.h"

/**
 * copy_string - replicates a string from source to destination
 * @destination: the target location for the string copy
 * @source: the original string to copy from
 *
 * Return: a pointer to the destination string
 */
char *copy_string(char *destination, const char *source)
{
	int index = 0;

	if (destination == source || source == NULL)
		return (destination);

	while (source[index] != '\0')
	{
		destination[index] = source[index];
		index++;
	}
	destination[index] = '\0';
	return (destination);
}

/**
 * duplicate_string - creates a new copy of a string
 * @input: the string to be duplicated
 *
 * Return: a pointer to the newly duplicated string
 */
char *duplicate_string(const char *input)
{
	int size = 0;
	char *new_string;

	if (input == NULL)
		return (NULL);

	const char *temp = input;

	while (*temp++)
		size++;

	new_string = malloc((size + 1) * sizeof(char));
	if (new_string == NULL)
		return (NULL);

	char *ptr = new_string;

	while (size-- > 0)
		*ptr++ = *input++;

	*ptr = '\0';
	return (new_string);
}

/**
 * print_string - outputs a string to stdout
 * @text: the string to be printed
 *
 * Return: Nothing
 */
void print_string(const char *text)
{
	int pos = 0;

	if (text == NULL)
		return;

	while (text[pos] != '\0')
	{
		output_char(text[pos]);
		pos++;
	}
}

/**
 * output_char - sends a character to stdout
 * @character: The character to output
 *
 * Return: On success returns 1.
 * On failure, returns -1 and sets errno accordingly.
 */
int output_char(char character)
{
	static int buffer_index;
	static char buffer[WRITE_BUF_SIZE];

	if (character == BUF_FLUSH || buffer_index >= WRITE_BUF_SIZE)
	{
		write(1, buffer, buffer_index);
		buffer_index = 0;
	}
	if (character != BUF_FLUSH)
		buffer[buffer_index++] = character;

	return (1);
}
