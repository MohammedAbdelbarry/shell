#ifndef FILE_PROCESSING_H_   /* Include guard */
#define FILE_PROCESSING_H_

#include <stdio.h>

/*
	history file basic functions' prototypes
*/
void open_history_file();

FILE *get_history_file();

void close_history_file();

/* 
	log file basic functions' prototypes
*/
void open_log_file();

FILE *get_log_file();

void close_log_file();

/* 
	CommandsBatch file basic functions' prototypes
*/
void open_commands_batch_file(const char *file_path);

FILE *get_commands_batch_file();

void close_commands_batch_file();

/**
 * File Manipulation.
 */
void fputline(FILE *file, char *line);

#endif // FILE_PROCESSING_H_