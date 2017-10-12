//
// Created by mohammed on 10/5/17.
//

#ifndef SHELL_STRUTIL_H
#define SHELL_STRUTIL_H

/**
 * Checks if a string contains a given character.
 * @param str The string to be searched
 * @param ch The character to search for
 * @return true if str contains ch and false otherwise
 */
bool contains(char *str, char ch);
/**
 * Splits a string by given delimiters.
 * @param line The line to be split
 * @param tokenDelimiter A list of delimiters used in splitting
 * @param ignore_quotes A boolean indicating whether or not delimiters between
 * double or single quotes will be ignored
 * @param allow_quotes_in_words A boolean that if true will allow quotes in words to be
 * treated as part of the word and not a special character "ex.: It's a quote"
 * @return A list of strings containing the split strings.
 */
char **split(const char *line, const char *tokenDelimiter, bool ignore_quotes, bool allow_quotes_in_words);
/**
 * Splits a string around given delimiters using the strtok function.
 * @deprecated Deprecated for the introduction of th split function which is safer
 * and more accurate
 * @param line The line to be split
 * @param tokenDelimiter A list of delimiters used in splitting
 * @param ignore_quotes A boolean indicating whether or not delimiters between
 * double or single quotes will be ignored
 * @return
 */
char **split_strtok(const char *line, const char *tokenDelimiter, bool ignore_quotes);
/**
 * Replaces the substring in str starting at idx and has a length
 * len with the rep string.
 * Assumes that the str buffer has enough allocated memory for the replacement.
 * @param str The string that will have some of its contents replaced
 * @param rep The replacement string
 * @param idx The starting index of the substring that will be replaced
 * @param len The length of the substring that will be replaced
 */
void strn_replace(char *str, char *rep, int idx, int len);

/**
 * Performs the variable and tilde expansion shell operation.
 * @param line_ptr A pointer to the line to be expanded
 * @param ignore_squotes A boolean that if true will not
 * expand variables and tildes inside single quotes
 * @return 0 if the line was expanded successfully
 *         NULL_ARG if line_ptr = NULL or *line_ptr = NULL
 *         NULL_USER if ~user expansion didn't find the given
 *                   user
 *         MALLOC_ERR if the function failed to allocate enough memory
 *                    for the line expansion
 */
int variable_substitution(const char **line_ptr, bool ignore_squotes);

/**
 * Splits a given shell command line.
 * @param line The given shell command line
 * @return A list of strings containing the command line args
 */
char **shellSplit(char *line);

/**
 * Checks if the given string is a valid assignment string
 * Accepted variable names are [A-Za-z_][A-Za-z0-9_]*
 * @param command The string to be checked
 * @return true if the string represents a valid assignment
 * and false otherwise
 */
bool isAssignment(char *command);

//char **splitAssignment(char *command);

#endif //SHELL_STRUTIL_H
