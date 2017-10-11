#include "environment.h"
#include "variables.h"
#include "commands.h"
#include "command_parser.h"
#include "constants.h"
#include "strutil.h"
#include "file_processing.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
//#include <stdlib.h>
//#include "strutil.h"

void start(bool read_from_file);

void shell_loop(bool input_from_file);

void sigchld_handler(int sig);

int main(int argc, char *argv[]) {
    setup_environment();
    open_history_file();

//    char *test = malloc(512);
//    strcpy(test, "$HOME/$USER/~mohammed~");
//    printf("%s\n", test);
//    variable_substitution(test, false);
//    printf("%s\n", test);
    // any other early configuration should be here
    if (argc > 1) {
        if (argc > 2) {
            printf("%s: error, too many arguments\n", SHELL_NAME);
            return 1;
        }
        open_commands_batch_file(argv[1]);
        if (get_commands_batch_file() == NULL) {
            printf("%s: error, batch file not found\n", SHELL_NAME);
            return 404;
        }
        start(true);
    } else {
        start(false);
    }
    destroy_all_variables();
    close_history_file();
    //close_log_file();
    if (argc > 1) {
        close_commands_batch_file();
    }
    return 0;
}

void sigchld_handler(int sig) {
    pid_t pid;
    pid = wait(0);
    printf("%s: child %d done.\n", SHELL_NAME, pid);
    fflush(stdout);
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

void execute_program(struct Command command) {
    signal(SIGCHLD, SIG_DFL);
    fflush(get_history_file());
    fflush(stdin);
    pid_t pid = fork();
    if (pid == 0) {
        if (command.isBackground) {

        }
        const char *PATH = strdup(lookup_variable("PATH"));
        char **split_path = split(PATH, ":", false);
        int counter = 0;
        execv(command.argv[0], command.argv);
        while (split_path[counter] != NULL) {
            char file_path[strlen(split_path[counter]) + strlen(command.argv[0]) + 2];
            sprintf(file_path, "%s/%s", split_path[counter], command.argv[0]);
            execv(file_path, command.argv);
            counter++;
        }
        if (errno == EACCES) {
            printf("%s: permission denied: %s\n", SHELL_NAME, command.argv[0]);
        } else {
            printf("%s: command not found: %s\n", SHELL_NAME, command.argv[0]);
        }
        free(PATH);
        free(split_path);
        abort();
    } else if (pid > 0) {
        if (!command.isBackground) {
            wait(0);
        } else {
            signal(SIGCHLD, sigchld_handler);
            printf("%s: child pid: %d\n", SHELL_NAME, getpid());
            fflush(stdout);
        }
    }
}

void execute_assignment(struct Command command, bool export) {
    char temp[512];
    if (!export) {
        strcpy(temp, command.argv[0]);
    } else {
        strcpy(temp, command.argv[1]);
    }
    size_t len = strlen(temp);
    bool found = false;
    for (int i = 1; i < len - 1; i++) {
        if (temp[i] == '=') {
            temp[i] = '\0';
            set_variable(temp, temp + i + 1, export);
            found = true;
            break;
        }
    }
    if (!found && export) {
        char* val = lookup_variable(command.argv[1]);
        if (val != NULL) {
            set_variable(command.argv[1], val, export);
        }
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
        line[strlen(line) - 1] = '\0';
        char* orig_line = strdup(line);
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
            free(parsedCommand.argv); //SEGABRT WHEN EMPTY
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
