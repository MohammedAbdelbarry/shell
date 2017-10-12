#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

#include "command.h"

/**
 * Changes the current working directory of the shell
 * to the directory given by the parameter path.
 * @param path The path of the new working directory
 */
void cd(char *path);

/**
 * Prints the args of the echo command to stdout.
 * @param command The parsed echo command
 */
void echo(struct Command command);

/**
 * Gets the path from the parsed command and passes it to cd.
 * @param command The parsed cd command
 */
void execute_cd(struct Command command);

/**
 * Prints the current working directory of the
 * shell to stdout.
 */
void pwd();

/**
 * Prints the history of the shell.
 */
void history();

/**
 * Prints all the environment variables if command.argc == 1
 * else prints the value of each argument.
 * @param command The parsed printenv command
 */
void printenv(struct Command command);

#endif // COMMANDS_H_