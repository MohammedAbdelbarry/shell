#include <string.h>
#include <stdlib.h>
#include "file_processing.h"
#include "variables.h"
#include "constants.h"

/* 
	history file section
*/
FILE *history_file;
FILE *log_file;
FILE *batch_file;

void open_history_file() {
    char *shell_home = lookup_variable("SHELL_HOME");
    char *history_path = "/.history";
    size_t buffer_len = strlen(shell_home) + strlen(history_path) + 1;
    char *buffer = (char *) malloc(buffer_len * sizeof(char));
    strcpy(buffer, shell_home);
    strcat(buffer, history_path);
    history_file = fopen(buffer, "a+");
    free(buffer);
}

FILE *get_history_file() {
    return history_file;
}

void close_history_file() {
    fclose(history_file);
}


/* 
	log file section
*/
void open_log_file() {
    char *shell_home = lookup_variable("SHELL_HOME");
    char *log_path = "/shell.log";
    size_t buffer_len = strlen(shell_home) + strlen(log_path) + 1;
    char *buffer = (char *) malloc(buffer_len * sizeof(char));
    strcpy(buffer, shell_home);
    strcat(buffer, log_path);
    log_file = fopen(buffer, "a+");
    free(buffer);
}

FILE *get_log_file() {
    return log_file;
}

void close_log_file() {
    fclose(log_file);
}


/* 
	CommandsBatch file section
*/
void open_commands_batch_file(const char *file_path) {
    batch_file = fopen(file_path, "r");
}

FILE *get_commands_batch_file() {
    return batch_file;
}

void close_commands_batch_file() {
    fclose(batch_file);
}

void fputline(FILE *file, char *line) {
    if (file == NULL) {
        fprintf(stderr, "%s: failed to write to history file", SHELL_NAME);
    }
    fprintf(file, "%s\n", line);
}
