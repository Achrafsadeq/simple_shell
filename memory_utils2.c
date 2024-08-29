#include "shell.h"

/**
 * release_and_nullify - releases memory allocated to a pointer and sets
 * @ptr: a double pointer pointing to the memory to be freed
 *
 * This function deallocates the memory pointed to by the provided pointer and
 * sets the pointer itself to NULL to prevent dangling references.
 *
 * Return: returns 1 if the memory was successfully freed, otherwise returns 0.
 */
int release_and_nullify(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
