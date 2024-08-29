#include "shell_core.h"

int main(void)
{
    char *user_input = NULL;
    size_t input_capacity = 0;
    ssize_t chars_read;
    char **command_list;
    char **command_tokens;
    int shell_active = 1;
    int i, cmd_result;

    while (shell_active) {
        if (isatty(STDIN_FILENO))
            printf("$ ");

        chars_read = getline(&user_input, &input_capacity, stdin);

        if (chars_read == -1) {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        user_input[strcspn(user_input, "\n")] = '\0';

        command_list = tokenize_input(user_input, ";");
        for (i = 0; command_list[i] != NULL; i++) {
            char **logical_ops = parse_logical_operators(command_list[i]);
            int j;
            int last_result = 1;

            for (j = 0; logical_ops[j] != NULL; j++) {
                if (strcmp(logical_ops[j], "&&") == 0) {
                    if (last_result == 0)
                        break;
                    continue;
                } else if (strcmp(logical_ops[j], "||") == 0) {
                    if (last_result != 0)
                        break;
                    continue;
                }

                command_tokens = tokenize_input(logical_ops[j], DELIMITER_TOKENS);
                cmd_result = process_command(command_tokens);
                free(command_tokens);

                last_result = cmd_result;
            }

            free(logical_ops);

            if (cmd_result == 0)
                break;
        }
        free(command_list);
    }

    free(user_input);
    return 0;
}
