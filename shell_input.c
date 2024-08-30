#include "shell_core.h"

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;
    char *comment;

    characters = getline(&line, &bufsize, stdin);

    if (characters == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    /* Remove comments */
    comment = strchr(line, '#');
    if (comment) {
        *comment = '\0';
    }

    return line;
}
