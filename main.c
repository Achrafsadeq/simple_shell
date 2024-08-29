#include "shell.h"

/**
 * main - Entry point for the simple shell program
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    char *command_line = NULL;
    size_t buffer_size = 0;
    ssize_t chars_read;
    char **command_tokens;
    int shell_active = 1;

    while (shell_active)
    {
        if (isatty(STDIN_FILENO))
            printf("shell> ");

        chars_read = getline(&command_line, &buffer_size, stdin);

        if (chars_read == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Remove trailing newline */
        command_line[strcspn(command_line, "\n")] = '\0';

        command_tokens = tokenize_input(command_line);
        shell_active = process_command(command_tokens);

        free(command_tokens);
    }

    free(command_line);
    return (0);
}
