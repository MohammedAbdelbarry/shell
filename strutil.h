//
// Created by mohammed on 10/5/17.
//

#ifndef SHELL_STRUTIL_H
#define SHELL_STRUTIL_H

bool contains(char *str, char ch);

char **split(const char *line, const char *tokenDelimiter, bool ignore_quotes);

char **split_strtok(const char *line, const char *tokenDelimiter, bool ignore_quotes);

void strn_replace(char *str, char *rep, int idx, int len);

int variable_substitution(const char **line_ptr, bool ignore_squotes);

char **shellSplit(char *line);

#endif //SHELL_STRUTIL_H
