#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "variables.h"
#include "constants.h"
#include <stdio.h>
#include <pwd.h>

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
    for (char *ptr = line; *ptr != '\0'; ptr++) {
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
                break;

            case IN_WORD:
                if (contains(tokenDelimiter, ch)) {
                    *ptr = '\0';
                    printf("%d\n", argc);
                    argv[argc++] = word_start;
                    state = IN_DELIM;
                }
                break;
        }
        if (argc + 1 >= buf_size) {
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

bool isValidVar(char ch) {
    return isalnum(ch) || ch == '_';
}

bool isValidUser(char ch) {
    return isalnum(ch) || ch == '_' || ch == '-';
}

void strn_replace(char *str, char *rep, int idx, int len) {
    if (rep == NULL) {
        return;
    }
    size_t rep_len = strlen(rep);
    size_t str_len = strlen(str);
    char buffer[str_len - len + rep_len + 1];
    strncpy(buffer, str, idx);
    buffer[idx] = '\0';
    strcat(buffer, rep);
    strcat(buffer, str + idx + len);
    strcpy(str, buffer);
    return;
}

int variable_substitution(const char **line_ptr, bool ignore_squotes) {
    enum states {
        IN_VARIABLE, IN_OTHER, IN_SQUOTE, IN_TILDE
    } state = IN_OTHER;
    if (line_ptr == NULL || &line_ptr == NULL) {
        return NULL_ARG;
    }
    char *word_start = NULL;
    bool finished = false;
    int size = BUF_SIZE;
    char* line = *line_ptr;
    for (char *ptr = line; ; ptr++) {
        char ch = (unsigned char) *ptr;
        if (finished) {
            break;
        }
        finished = *ptr == '\0';
        switch (state) {
            case IN_OTHER:
                if (ignore_squotes && ch == '\'') {
                    state = IN_SQUOTE;
                    word_start = ptr + 1;
                    continue;
                } else if (ch == '$') {
                    state = IN_VARIABLE;
                    word_start = ptr + 1;
                    continue;
                } else if (ch == '~') {
                    state = IN_TILDE;
                    word_start = ptr + 1;
                    continue;
                }
                state = IN_OTHER;
                continue;

            case IN_SQUOTE:
                if (ignore_squotes && ch == '\'') {
                    state = IN_OTHER;
                }
                continue;

            case IN_VARIABLE:
                if (!isValidVar(ch)) {
                    char query[ptr - word_start + 1];
                    strncpy(query, word_start, ptr - word_start);
                    query[ptr - word_start] = '\0';
                    char *val = lookup_variable(query);
                    bool val_is_null = val == NULL;
                    if (val_is_null) {
                        val = "";
                    }
                    int offset = strlen(val) - (ptr - word_start) - 1;
                    if (strlen(line) + offset >= size) {
                        size *= 2;
                        int cur_idx = ptr - line;
                        int word_start_idx = word_start - line;
                        char *reallocated_line = realloc(line, size);
                        if (reallocated_line != NULL) {
                            line = reallocated_line;
                            ptr = line + cur_idx;
                            word_start = line + word_start_idx;
                            //printf("REALLOC: SUCC\n");
                        } else {
                            //printf("REALLOC: FAIL\n");
                            return MALLOC_ERR;
                        }
                    }
                    strn_replace(line, val, word_start - line - 1, ptr - word_start + 1);
                    ptr += offset;
                    if (ch == '$') {
                        state = IN_VARIABLE;
                        word_start = ptr + 1;
                    } else if (ch == '~') {
                        state = IN_TILDE;
                        word_start = ptr + 1;
                    } else {
                        state = IN_OTHER;
                    }
                }
                continue;

            case IN_TILDE:
                if (!isValidUser(ch)) {
                    char *rep = NULL;
                    if (ptr == word_start) {
                        rep = lookup_variable("HOME");
                    } else {
                        char query[ptr - word_start + 1];
                        strncpy(query, word_start, ptr - word_start);
                        query[ptr - word_start] = '\0';
                        struct passwd *result = getpwnam(query);
                        if (result == NULL) {
                            printf("%s: no such user or named directory: %s\n", SHELL_NAME, query);
                            return NULL_USER;
                        }
                        rep = result -> pw_dir;
                    }
                    int offset = strlen(rep) - (ptr - word_start) - 1;
                    if (strlen(line) + offset >= size) {
                        size *= 2;
                        int cur_idx = ptr - line;
                        int word_start_idx = word_start - line;
                        char *reallocated_line = realloc(line, size);
                        if (reallocated_line != NULL) {
                            line = reallocated_line;
                            ptr = line + cur_idx;
                            word_start = line + word_start_idx;
                            //printf("REALLOC: SUCC\n");
                        } else {
                            //printf("REALLOC: FAIL\n");
                            return MALLOC_ERR;
                        }
                    }
                    strn_replace(line, rep, word_start - line - 1, ptr - word_start + 1);
                    ptr += offset;
                    if (ch == '$') {
                        state = IN_VARIABLE;
                        word_start = ptr + 1;
                    } else if (ch == '~') {
                        state = IN_TILDE;
                        word_start = ptr + 1;
                    } else {
                        state = IN_OTHER;
                    }
                }
                continue;
        }
    }
    *line_ptr = line;
    return 0;
}


char **shellSplit(char *line) {
    const char *TOKEN_DELIMITER = " \t\n";
    char **argv = split(line, TOKEN_DELIMITER, true);
    return argv;
}
