#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
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
    size_t buffer_len = strlen(shell_home) + strlen(SHELL_NAME) + strlen("/.log") + 1;
    char *buffer = (char *) malloc(buffer_len * sizeof(char));
    sprintf(buffer, "%s/%s.log", shell_home, SHELL_NAME);
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
    char real_path[PATH_MAX];
    realpath(file_path, real_path);
    printf("%s\n", real_path);
    batch_file = fopen(real_path, "r");
    if (batch_file != NULL) {
        log(get_log_file(), "batch file opened successfully", DEBUG);
    } else {
        log(get_log_file(), "failed to open batch file", ERROR);
    }
}

FILE *get_commands_batch_file() {
    return batch_file;
}

void close_commands_batch_file() {
    fclose(batch_file);
}

void fputline(FILE *file, char *line) {
    if (file == NULL) {
        log(get_log_file(), "failed to write to file", ERROR);
        return;
    }
    fprintf(file, "%s\n", line);
}

void log(FILE *file, char *message, enum LogLevel level) {

    if (file == NULL) {
        fprintf(stdout, "%s: failed to write to file", SHELL_NAME);
        return;
    }

    const static char *log_level_string[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

    char buff[20];
    struct tm *parsed_time;

    time_t now = time(0);
    parsed_time = localtime(&now);

    strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", parsed_time);
    fprintf(file, "%s [%s] %s: %s\n", buff, log_level_string[level], SHELL_NAME, message);
}