#include "shell.h"

int main(void)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t read_size;
    char **commands;
    int status = 1;

    while (status)
    {
        if (isatty(STDIN_FILENO))
            printf("($) ");

        read_size = getline(&input, &input_size, stdin);

        if (read_size == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        commands = split_line(input, ";");
        for (int i = 0; commands[i] != NULL; i++)
        {
            status = execute_command_chain(commands[i]);
            if (status == 0)
                break;
        }
        free(commands);
    }

    free(input);
    return (0);
}
