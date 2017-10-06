#include <stdlib.h>
#include <string.h>

char **split(const char *line, const char *tokenDelimiter) {
    int buf_size = 32;
    char **argv = (char **) malloc(buf_size * sizeof(char *));
    if (argv == NULL) {
        return NULL;
    }
    int argc = 0;
    char *token = strtok(line, tokenDelimiter);
    while (token != NULL) {
        argv[argc++] = token;
        token = strtok(NULL, tokenDelimiter);
        if (argc >= buf_size) {
            buf_size *= 2;
            argv = (char **) realloc(argv, buf_size * sizeof(char *));
            if (argv == NULL) {
                return NULL;
            }
        }
    }
    argv[argc] = NULL;
    return argv;
}

char **shellSplit(char *line) {
    const char *TOKEN_DELIMITER = " \t\n";
    char **argv = split(line, TOKEN_DELIMITER);
    return argv;
}

