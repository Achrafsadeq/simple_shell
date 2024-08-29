#include "shell.h"

int shell_cd(char **args)
{
    char *dir = args[1] ? args[1] : getenv("HOME");
    char cwd[PATH_MAX];

    if (strcmp(dir, "-") == 0)
    {
        dir = getenv("OLDPWD");
        if (!dir)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        printf("%s\n", dir);
    }

    if (chdir(dir) != 0)
    {
        perror("cd");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)))
    {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", cwd, 1);
    }
    else
    {
        perror("getcwd");
    }
    return 1;
}

int shell_help(char **args)
{
    (void)args;
    char *builtin_str[] = {"cd", "help", "exit", "env", "setenv", "unsetenv"};

    printf("Simple Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (int i = 0; i < 6; i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int shell_exit(char **args)
{
    int status = args[1] ? atoi(args[1]) : 0;
    exit(status);
}

int shell_env(char **args)
{
    (void)args;
    for (int i = 0; environ[i]; i++)
    {
        printf("%s\n", environ[i]);
    }
    return 1;
}

int shell_setenv(char **args)
{
    if (!args[1] || !args[2])
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }

    if (setenv(args[1], args[2], 1) != 0)
    {
        fprintf(stderr, "Failed to set environment variable\n");
        return 1;
    }
    return 1;
}

int shell_unsetenv(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }

    if (unsetenv(args[1]) != 0)
    {
        fprintf(stderr, "Failed to unset environment variable\n");
        return 1;
    }
    return 1;
}
