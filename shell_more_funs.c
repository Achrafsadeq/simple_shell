#include "shell.h"

char **split_line(char *line, const char *delim)
{
    int bufsize = BUFFER_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL)
    {
        tokens[position++] = token;

        if (position >= bufsize)
        {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command_chain(char *command)
{
    char **logical_ops = split_line(command, "&|");
    int prev_status = 1;

    for (int j = 0; logical_ops[j] != NULL; j++)
    {
        if (strcmp(logical_ops[j], "&&") == 0 && prev_status == 0)
            break;
        else if (strcmp(logical_ops[j], "||") == 0 && prev_status != 0)
            break;

        char **args = split_line(logical_ops[j], TOKEN_DELIM);
        int exec_status = execute(args);
        free(args);
        prev_status = exec_status;
    }

    free(logical_ops);
    return prev_status;
}

int execute(char **args)
{
    if (args[0] == NULL)
        return 1;

    char *builtin_str[] = {"cd", "help", "exit", "env", "setenv", "unsetenv"};
    int (*builtin_func[]) (char **) = {&shell_cd, &shell_help, &shell_exit, &shell_env, &shell_setenv, &shell_unsetenv};
    int num_builtins = sizeof(builtin_str) / sizeof(char *);

    for (int i = 0; i < num_builtins; i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);
    }

    return launch(args);
}

int launch(char **args)
{
    pid_t pid = fork();
    if (pid == 0) /* Child process */
    {
        char *command_path = get_location(args[0]);
        if (!command_path)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(command_path, args, environ) == -1)
            perror("Error");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) /* Error forking */
    {
        perror("Error");
    }
    else /* Parent process */
    {
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

char *get_location(char *command)
{
    char *path = getenv("PATH");
    if (!path)
        return NULL;

    char *path_copy = strdup(path);
    char *path_token = strtok(path_copy, ":");
    struct stat buffer;

    while (path_token)
    {
        char *file_path = malloc(strlen(path_token) + strlen(command) + 2);
        sprintf(file_path, "%s/%s", path_token, command);

        if (stat(file_path, &buffer) == 0)
        {
            free(path_copy);
            return file_path;
        }
        free(file_path);
        path_token = strtok(NULL, ":");
    }
    free(path_copy);

    return stat(command, &buffer) == 0 ? command : NULL;
}
