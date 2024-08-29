#include "shell.h"

/**
 * fill_memory - Populates a block of memory with a specific byte
 * @dest: Pointer to the memory region to be filled
 * @byte: The byte value used for filling the memory
 * @count: Number of bytes to be filled with the value
 *
 * Return: Pointer to the filled memory area (dest)
 */
char *fill_memory(char *dest, char byte, unsigned int count)
{
	unsigned int j; /* Loop iterator */

	for (j = 0; j < count; j++)
		dest[j] = byte; /* Assign the byte value to each position */
	return (dest); /* Return the pointer to the filled memory */
}

/**
 * release_memory - Deallocates an array of strings
 * @array: Array of strings to be freed
 */
void release_memory(char **array)
{
	char **backup = array; /* Backup pointer */

	if (!array)
		return; /* Exit if the array is NULL */
	while (*array)
		free(*array++); /* Free each string in the array */
	free(backup); /* Free the backup pointer */
}

/**
 * resize_memory - Adjusts the size of a memory block
 * @ptr: Pointer to the previously allocated memory block
 * @current_size: Size in bytes of the existing memory block
 * @new_size: Size in bytes of the new memory block
 *
 * Return: Pointer to the resized memory block, or NULL if allocation fails
 */
void *resize_memory(void *ptr, unsigned int current_size,
		unsigned int new_size)
{
	char *new_ptr; /* Pointer for the new memory block */

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL); /* Free memory if new_size is 0 */
	if (new_size == current_size)
		return (ptr); /* Return the original block if sizes are equal */

	new_ptr = malloc(new_size); /* Allocate new memory block */
	if (!new_ptr)
		return (NULL);

	current_size = current_size < new_size ? current_size : new_size;
	while (current_size--)
		new_ptr[current_size] = ((char *)ptr)[current_size];
	free(ptr); /* Free the original memory block */
	return (new_ptr); /* Return pointer to the new memory block */
}
