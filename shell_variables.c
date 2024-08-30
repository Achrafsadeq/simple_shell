#include "shell_core.h"
#include <ctype.h> /* For isalnum() */

extern int last_command_status;
extern pid_t shell_pid;

char *expand_variables(char *input) {
    char *result = malloc(strlen(input) * 2);
    char *src = input;
    char *dest = result;
    char *value;
    char *var_end;
    char tmp;

    while (*src) {
        if (*src == '$') {
            src++;
            if (*src == '?') {
                dest += sprintf(dest, "%d", last_command_status);
                src++;
            } else if (*src == '$') {
                dest += sprintf(dest, "%d", shell_pid);
                src++;
            } else if (*src == '{') {
                char *end = strchr(src, '}');
                if (end) {
                    *end = '\0';
                    value = getenv(src + 1);
                    if (value) {
                        dest += sprintf(dest, "%s", value);
                    }
                    src = end + 1;
                } else {
                    *dest++ = '$';
                }
            } else {
                var_end = src;
                while (isalnum(*var_end) || *var_end == '_') var_end++;
                tmp = *var_end;
                *var_end = '\0';
                value = getenv(src);
                if (value) {
                    dest += sprintf(dest, "%s", value);
                }
                *var_end = tmp;
                src = var_end;
            }
        } else {
            *dest++ = *src++;
        }
    }
    *dest = '\0';
    return result;
}
