//
// Created by mohammed on 10/5/17.
//

#ifndef SHELL_STRUTIL_H
#define SHELL_STRUTIL_H

bool contains(char *str, char ch);

char **split(const char *line, const char *tokenDelimiter, bool ignore_quotes);

char **split_strtok(const char *line, const char *tokenDelimiter, bool ignore_quotes);

char **shellSplit(char *line);

#endif //SHELL_STRUTIL_H
