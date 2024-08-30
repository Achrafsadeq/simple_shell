#include "shell_core.h"

Alias aliases[MAX_ALIASES];
int alias_count = 0;

int builtin_alias(char **args) {
    int i;

    if (args[1] == NULL) {
        /* Print all aliases */
        for (i = 0; i < alias_count; i++) {
            printf("%s='%s'\n", aliases[i].name, aliases[i].value);
        }
        return 1;
    }

    for (i = 1; args[i] != NULL; i++) {
        char *equals = strchr(args[i], '=');
        if (equals) {
            /* Define or update alias */
            *equals = '\0';
            add_or_update_alias(args[i], equals + 1);
        } else {
            /* Print specific alias */
            char *value = get_alias_value(args[i]);
            if (value) {
                printf("%s='%s'\n", args[i], value);
            } else {
                fprintf(stderr, "alias: %s not found\n", args[i]);
            }
        }
    }

    return 1;
}

char *get_alias_value(const char *name) {
    int i;
    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            return aliases[i].value;
        }
    }
    return NULL;
}

void add_or_update_alias(const char *name, const char *value) {
    int i;
    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            free(aliases[i].value);
            aliases[i].value = strdup(value);
            return;
        }
    }

    if (alias_count < MAX_ALIASES) {
        aliases[alias_count].name = strdup(name);
        aliases[alias_count].value = strdup(value);
        alias_count++;
    } else {
        fprintf(stderr, "alias: maximum number of aliases reached\n");
    }
}
