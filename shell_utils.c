#include "shell_core.h"

char **tokenize_input(char *input, const char *delim)
{
    int capacity = MAX_BUFFER, index = 0;
    char **result = malloc(capacity * sizeof(char*));
    char *segment;

    if (!result) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    segment = strtok(input, delim);
    while (segment != NULL) {
        result[index++] = segment;

        if (index >= capacity) {
            capacity += MAX_BUFFER;
            result = realloc(result, capacity * sizeof(char*));
            if (!result) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }

        segment = strtok(NULL, delim);
    }
    result[index] = NULL;
    return result;
}

char **parse_logical_operators(char *input)
{
    int capacity = MAX_BUFFER, index = 0;
    char **result = malloc(capacity * sizeof(char*));
    char *segment;
    char *remaining = input;

    if (!result) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while ((segment = strtok_r(remaining, "&|", &remaining))) {
        result[index++] = segment;

        if (index >= capacity) {
            capacity += MAX_BUFFER;
            result = realloc(result, capacity * sizeof(char*));
            if (!result) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }

        if (remaining[0] == '&') {
            result[index++] = "&&";
            remaining++;
        } else if (remaining[0] == '|') {
            result[index++] = "||";
            remaining++;
        }
    }
    result[index] = NULL;
    return result;
}

int process_command(char **tokens)
{
    int i;
    const char *builtin_commands[] = {
        "cd", "help", "exit", "env", "setenv", "unsetenv"
    };
    int (*builtin_functions[]) (char **) = {
        &builtin_change_directory, &builtin_display_help, &builtin_terminate,
        &builtin_show_env, &builtin_set_env, &builtin_unset_env
    };

    if (tokens[0] == NULL)
        return 1;

    for (i = 0; i < 6; i++) {
        if (strcmp(tokens[0], builtin_commands[i]) == 0)
            return (*builtin_functions[i])(tokens);
    }

    return execute_external(tokens);
}

int execute_external(char **tokens)
{
    pid_t child_pid;
    int status;
    char *cmd_path;

    child_pid = fork();
    if (child_pid == 0) {
        cmd_path = locate_command(tokens[0]);
        if (cmd_path == NULL) {
            fprintf(stderr, "%s: command not found\n", tokens[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(cmd_path, tokens, environ) == -1) {
            perror("Execution error");
        }
        exit(EXIT_FAILURE);
    } else if (child_pid < 0) {
        perror("Fork error");
    } else {
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

char *locate_command(const char *cmd)
{
    char *path_var, *path_copy, *dir, *full_path;
    int cmd_len, dir_len;
    struct stat file_info;

    path_var = getenv("PATH");
    if (path_var) {
        path_copy = strdup(path_var);
        cmd_len = strlen(cmd);
        dir = strtok(path_copy, ":");
        while (dir != NULL) {
            dir_len = strlen(dir);
            full_path = malloc(cmd_len + dir_len + 2);
            sprintf(full_path, "%s/%s", dir, cmd);

            if (stat(full_path, &file_info) == 0) {
                free(path_copy);
                return full_path;
            } else {
                free(full_path);
                dir = strtok(NULL, ":");
            }
        }
        free(path_copy);
        if (stat(cmd, &file_info) == 0)
            return strdup(cmd);
        return NULL;
    }
    return NULL;
}
