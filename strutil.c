#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool contains(char *str, char ch) {
    if (str == NULL) {
        return false;
    }
    for (char *cur_ch = str; *cur_ch != '\0'; cur_ch++) {
        if (*cur_ch == ch) {
            return true;
        }
    }
    return false;
}

char **split_strtok(const char *line, const char *tokenDelimiter) {
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

char **split(const char *line, const char *tokenDelimiter, bool ignore_quotes) {
    int buf_size = 32;
    char **argv = (char **) malloc(buf_size * sizeof(char *));
    if (argv == NULL) {
        return NULL;
    }
    int argc = 0;
    enum states {
        IN_DELIM, IN_WORD, IN_SQUOTE, IN_DQUOTE
    } state = IN_DELIM;
    char *word_start = NULL;
    char *buffer = line;//strdup(line);
    for (char *ptr = buffer; *ptr != '\0'; ptr++) {
        char ch = (unsigned char) *ptr;

        switch (state) {
            case IN_DELIM:
                if (contains(tokenDelimiter, ch)) {
                    continue;
                }

                if (ignore_quotes && ch == '"') {
                    state = IN_DQUOTE;
                    word_start = ptr + 1;
                    continue;
                } else if (ignore_quotes && ch == '\'') {
                    state = IN_SQUOTE;
                    word_start = ptr + 1;
                    continue;
                }
                state = IN_WORD;
                word_start = ptr;
                continue;

            case IN_DQUOTE:
                if (ignore_quotes && ch == '"') {
                    *ptr = '\0';
                    argv[argc++] = word_start;
                    state = IN_DELIM;
                }
                continue;

            case IN_SQUOTE:
                if (ignore_quotes && ch == '\'') {
                    *ptr = '\0';
                    argv[argc++] = word_start;
                    state = IN_DELIM;
                }
                continue;

            case IN_WORD:
                if (contains(tokenDelimiter, ch)) {
                    *ptr = '\0';
                    argv[argc++] = word_start;
                    state = IN_DELIM;
                }
                continue;
        }
        if (argc >= buf_size) {
            buf_size *= 2;
            argv = (char **) realloc(argv, buf_size * sizeof(char *));
            if (argv == NULL) {
                return NULL;
            }
        }
    }
    if (state != IN_DELIM) {
        argv[argc++] = word_start;
    }
    argv[argc] = NULL;
    return argv;
}

char **shellSplit(char *line) {
    const char *TOKEN_DELIMITER = " \t\n";
    char **argv = split(line, TOKEN_DELIMITER, true);
    return argv;
}
