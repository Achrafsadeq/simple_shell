#include "shell_core.h"

int builtin_change_directory(char **tokens)
{
    const char *target_dir = tokens[1];
    char current_dir[PATH_MAX];
    const char *home_dir = getenv("HOME");

    if (target_dir == NULL) {
        target_dir = home_dir;
    } else if (strcmp(target_dir, "-") == 0) {
        target_dir = getenv("OLDPWD");
        if (target_dir == NULL) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        printf("%s\n", target_dir);
    }

    if (chdir(target_dir) != 0) {
        perror("cd");
        return 1;
    }

    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", current_dir, 1);
    } else {
        perror("getcwd");
    }

    return 1;
}

int builtin_display_help(char **args)
{
    const char *builtin_commands[] = {
        "cd", "exit", "env", "setenv", "unsetenv", "help"
    };
    int i;
    int num_builtins = sizeof(builtin_commands) / sizeof(char *);

    if (args[1] == NULL) {
        printf("Simple Shell\n");
        printf("Type program names and arguments, and hit enter.\n");
        printf("The following are built in:\n");

        for (i = 0; i < num_builtins; i++) {
            printf("  %s\n", builtin_commands[i]);
        }

        printf("Use the man command for information on other programs.\n");
        return 1;
    }

    /* Add specific help for each builtin command if needed */

    return 0;
}
int builtin_terminate(char **tokens)
{
    int exit_code = 0;
    if (tokens[1] != NULL) {
        exit_code = atoi(tokens[1]);
    }
    exit(exit_code);
}

int builtin_show_env(char **tokens)
{
    int i = 0;
    (void)tokens;

    while (environ[i]) {
        printf("%s\n", environ[i]);
        i++;
    }
    return 1;
}

int builtin_set_env(char **tokens)
{
    if (tokens[1] == NULL || tokens[2] == NULL) {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }

    if (setenv(tokens[1], tokens[2], 1) != 0) {
        fprintf(stderr, "Failed to set environment variable\n");
        return 1;
    }

    return 1;
}

int builtin_unset_env(char **tokens)
{
    if (tokens[1] == NULL) {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }

    if (unsetenv(tokens[1]) != 0) {
        fprintf(stderr, "Failed to unset environment variable\n");
        return 1;
    }

    return 1;
}
