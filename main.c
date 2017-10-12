#include "command_parser.h"
#include "commands.h"
#include "constants.h"
#include "environment.h"
#include "file_processing.h"
#include "strutil.h"
#include "variables.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void start(bool read_from_file);

void shell_loop(bool input_from_file);

void sigchld_handler(int sig);

int main(int argc, char *argv[]) {
    setup_environment();
    open_history_file();
    open_log_file();
    log(get_log_file(), "shell started", INFO);

    if (argc > 1) {
        if (argc > 2) {
            fprintf(stderr, "%s: error, too many arguments\n", SHELL_NAME);
            log(get_log_file(), "too many arguments", ERROR);
            return 1;
        }
        open_commands_batch_file(argv[1]);
        if (get_commands_batch_file() == NULL) {
            fprintf(stderr, "%s: error, batch file not found\n", SHELL_NAME);
            log(get_log_file(), "batch file not found", ERROR);
            return -1;
        }
        start(true);
    } else {
        start(false);
    }
    destroy_all_variables();
    close_history_file();
    log(get_log_file(), "shell exited", INFO);
    close_log_file();
    if (argc > 1) {
        close_commands_batch_file();
    }
    return 0;
}

void start(bool read_from_file) {
    cd(""); // let shell starts from home

    if (read_from_file) {
        // file processing functions should be called from here
        shell_loop(true);
    } else {
        shell_loop(false);
    }
}

void shell_loop(bool input_from_file) {
    bool from_file = input_from_file;
    int buf_size = BUF_SIZE;
    while (true) {
        char *line = (char *) malloc((buf_size + 2) * sizeof(char));
        bool terminateLoop = false;
        FILE *stream = NULL;
        if (from_file) {
            stream = get_commands_batch_file();
        } else {
            const char *pwd = lookup_variable("PWD");
            printf("%s>%s> ", SHELL_NAME, pwd);
            stream = stdin;
        }

        char *ret = fgets(line, buf_size + 2, stream);
        if (ret == NULL) {
            break;
        }
        if (strlen(line) == buf_size + 1) {
            log(get_log_file(), "Line is longer than 512 characters", ERROR);
            fflush(stdin);
            fprintf(stderr, "%s: error: line is longer than 512 characters\n", SHELL_NAME);
            continue;
        }
        line[strlen(line) - 1] = '\0';
        char *orig_line = strdup(line);
        if (from_file) {
            printf("%s\n", line);
        }
        //parse your command here
        struct Command parsedCommand = parse_command(&line);
        if (parsedCommand.error_code == NULL_USER) {
            continue;
        }
        //execute your command here
        switch (parsedCommand.type) {
            case PROGRAM:
                execute_program(parsedCommand);
                break;
            case CD:
                execute_cd(parsedCommand);
                break;
            case ECHO:
                echo(parsedCommand);
                break;
            case EXPRESSION:
                execute_assignment(parsedCommand, false);
                break;
            case EXPORT:
                execute_assignment(parsedCommand, true);
                break;
            case PRINTENV:
                printenv(parsedCommand);
                break;
            case PWD:
                pwd();
                break;
            case HISTORY:
                history();
                break;
            case EXIT:
                terminateLoop = true;
                break;
            case COMMENT:
            default:
                break;
        }
        if (parsedCommand.type != COMMENT) {
            // save command to history
            fputline(get_history_file(), orig_line);
        }
        if (parsedCommand.argv != NULL) {
            free(parsedCommand.argv);
        }

        free(line);
        free(orig_line);

        if (terminateLoop) {
            break;
        }
        /*
            you don't need to write all logic here, a better practice is to call functions,
            each one contains a coherent set of logical instructions
        */
    }
}
