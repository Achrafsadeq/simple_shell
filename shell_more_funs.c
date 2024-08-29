#include "shell.h"

/**
 * tokenize_input - Breaks a string into tokens
 * @input: The string to tokenize
 *
 * Return: An array of tokens
 */
char **tokenize_input(char *input)
{
    int capacity = BUFFER_SIZE, index = 0;
    char **tokens = malloc(capacity * sizeof(char*));
    char *segment;

    if (!tokens)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    segment = strtok(input, TOKEN_DELIM);
    while (segment != NULL)
    {
        tokens[index] = segment;
        index++;

        if (index >= capacity)
        {
            capacity += BUFFER_SIZE;
            tokens = realloc(tokens, capacity * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }

        segment = strtok(NULL, TOKEN_DELIM);
    }
    tokens[index] = NULL;
    return tokens;
}

/**
 * process_command - Processes and executes a command
 * @tokens: Array of command arguments
 *
 * Return: 1 if the shell should continue, 0 if it should exit
 */
int process_command(char **tokens)
{
    int i;
    const char *builtin_commands[] = {
        "cd", "help", "exit", "env"
    };
    int (*builtin_functions[]) (char **) = {
        &change_directory,
        &display_help,
        &terminate_shell,
        &show_environment
    };

    if (tokens[0] == NULL)
        return 1;

    for (i = 0; i < 4; i++)
    {
        if (strcmp(tokens[0], builtin_commands[i]) == 0)
            return (*builtin_functions[i])(tokens);
    }

    return execute_program(tokens);
}

/**
 * execute_program - Launches an external program
 * @tokens: Array of command arguments
 *
 * Return: Always returns 1 to continue shell execution
 */
int execute_program(char **tokens)
{
    pid_t child_pid;
    int status;
    char *program_path;

    child_pid = fork();
    if (child_pid == 0)
    {
        /* Child process */
        program_path = find_executable(tokens[0]);
        if (program_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", tokens[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(program_path, tokens, environ) == -1)
        {
            perror("execve");
        }
        exit(EXIT_FAILURE);
    }
    else if (child_pid < 0)
    {
        /* Fork error */
        perror("fork");
    }
    else
    {
        /* Parent process */
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
 * find_executable - Locates the full path of a command
 * @command: The command to locate
 *
 * Return: The full path of the command, or NULL if not found
 */
char *find_executable(char *command)
{
    char *path_var, *path_copy, *dir, *full_path;
    int command_len, dir_len;
    struct stat file_stat;

    path_var = getenv("PATH");
    if (path_var)
    {
        path_copy = strdup(path_var);
        command_len = strlen(command);
        dir = strtok(path_copy, ":");
        while (dir != NULL)
        {
            dir_len = strlen(dir);
            full_path = malloc(command_len + dir_len + 2);
            sprintf(full_path, "%s/%s", dir, command);

            if (stat(full_path, &file_stat) == 0)
            {
                free(path_copy);
                return full_path;
            }
            free(full_path);
            dir = strtok(NULL, ":");
        }
        free(path_copy);
        if (stat(command, &file_stat) == 0)
        {
            return strdup(command);
        }
    }
    return NULL;
}
