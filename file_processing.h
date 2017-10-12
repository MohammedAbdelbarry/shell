#ifndef FILE_PROCESSING_H_   /* Include guard */
#define FILE_PROCESSING_H_

#include <stdio.h>
#include "log_types.h"

/**
 * Opens the history file located at
 * "$SHELL_HOME/.history".
 */
void open_history_file();

/**
 * Gets a pointer to the history file.
 * @return A pointer to the history file
 */
FILE *get_history_file();

/**
 * Closes the history file.
 */
void close_history_file();

/**
 * Opens the log file located at
 * "$SHELL_HOME/shell.log".
 */
void open_log_file();

/**
 * Gets a pointer to the log file.
 * @return A pointer to the log file
 */
FILE *get_log_file();

/**
 * Closes the log file.
 */
void close_log_file();

/**
 * Opens the batch file given by the file_path parameter
 * for reading.
 * @param file_path The path of the batch file
 */
void open_commands_batch_file(const char *file_path);

/**
 * Gets a pointer to the batch file.
 * @return A pointer to the batch file
 */
FILE *get_commands_batch_file();

/**
 * Closes the batch file.
 */
void close_commands_batch_file();

/**
 * Prints a line to the given file.
 * @param file The file to which the line will be printed
 * @param line The line that will be printed
 */
void fputline(FILE *file, char *line);

/**
 * Prints a log message of a given log level into a given file.
 * @param file The file to which the message will be printed
 * @param message The message to be printed to the file
 * @param level The log level of the message
 */
void log(FILE *file, char *message, enum LogLevel level);

#endif // FILE_PROCESSING_H_