#include "shell.h"

/**
 * execute_shell - Core shell loop responsible for executing commands.
 * @info: A struct containing parameters and return values.
 * @args: An array of arguments passed from the main function.
 *
 * Return: 0 if successful, 1 on error, or another error code.
 */
int execute_shell(info_t *info, char **args)
{
	ssize_t input_read = 0;
	int built_in_status = 0;

	while (input_read != -1 && built_in_status != -2)
	{
		clear_info(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		input_read = get_input(info);
		if (input_read != -1)
		{
			set_info(info, args);
			/* Check for and execute builtin commands */
			built_in_status = search_builtin(info);
			if (built_in_status == -1)
				 /* Search for external commands if not a builtin */
				search_command(info);
		}
		else if (interactive(info))
			_putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (built_in_status == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (built_in_status);
}

/**
 * search_builtin - Searches for a builtin command.
 * @info: Struct that carries function parameters and returns info.
 *
 * Return: -1 if no builtin is found,
 *         0 if a builtin is successfully executed,
 *         1 if a builtin is found but fails to execute,
 *         -2 if the builtin signals an exit.
 */
int search_builtin(info_t *info)
{
	int idx, built_in_result = -1;
	builtin_table builtin_list[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (idx = 0; builtin_list[idx].type; idx++)
		if (_strcmp(info->argv[0], builtin_list[idx].type) == 0)
		{
			info->line_count++;
			/* Execute the matched builtin command */
			built_in_result = builtin_list[idx].func(info);
			break;
		}
	return (built_in_result);
}

/**
 * search_command - Locates a command within the PATH environment.
 * @info: Struct that holds function parameters and returns info.
 *
 * Return: Nothing (void).
 */
void search_command(info_t *info)
{
	char *cmd_path = NULL;
	int arg_idx, non_delim_count;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (arg_idx = 0, non_delim_count = 0; info->arg[arg_idx]; arg_idx++)
		if (!is_delim(info->arg[arg_idx], " \t\n"))
			non_delim_count++;
	if (!non_delim_count)
		return;

	cmd_path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (cmd_path)
	{
		info->path = cmd_path;
		execute_command(info); /* Execute command found in PATH */
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			execute_command(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * execute_command - Creates a new process to execute the command.
 * @info: Struct containing parameters and return data.
 *
 * Return: Nothing (void).
 */
void execute_command(info_t *info)
{
	pid_t process_id;

	process_id = fork(); /* Create a child process */
	if (process_id == -1)
	{
		/* IMPLEMENT ERROR HANDLING FUNCTION */
		perror("Error:");
		return;
	}
	if (process_id == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* IMPLEMENT ERROR HANDLING FUNCTION */
	}
	else
	{
		wait(&(info->status)); /* Parent process waits for the child to finish */
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}
