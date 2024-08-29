#include "shell.h"

/**
 * change_directory - Switches to a specified directory
 * @parameters: Command arguments (parameters[1] is the target directory)
 *
 * Return: 1 to continue shell execution, 0 to terminate
 */
int change_directory(char **parameters)
{
    if (parameters[1] == NULL)
    {
        fprintf(stderr, "cd: missing directory argument\n");
    }
    else
    {
        if (chdir(parameters[1]) == -1)
        {
            perror("cd");
        }
    }
    return 1;
}

/**
 * display_help - Shows available commands and usage information
 * @parameters: Command arguments (unused)
 *
 * Return: Always returns 1 to continue shell execution
 */
int display_help(char **parameters)
{
    const char *builtin_commands[] = {
        "cd", "help", "exit", "env"
    };
    int command_count = sizeof(builtin_commands) / sizeof(char *);
    (void)parameters;

    printf("Simple Shell - Available Commands:\n");
    for (int i = 0; i < command_count; i++)
    {
        printf("  %s\n", builtin_commands[i]);
    }
    printf("For other programs, consult the man pages.\n");
    return 1;
}

/**
 * terminate_shell - Exits the shell program
 * @parameters: Command arguments (unused)
 *
 * Return: Always returns 0 to signal shell termination
 */
int terminate_shell(char **parameters)
{
    (void)parameters;
    return 0;
}

/**
 * show_environment - Displays the current environment variables
 * @parameters: Command arguments (unused)
 *
 * Return: Always returns 1 to continue shell execution
 */
int show_environment(char **parameters)
{
    char **env = environ;
    (void)parameters;

    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
    return 1;
}
