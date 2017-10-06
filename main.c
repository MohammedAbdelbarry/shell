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
#include <limits.h>
//#include <stdlib.h>
//#include "strutil.h"

void start(bool read_from_file);

void shell_loop(bool input_from_file);

int main(int argc, char *argv[]) {
    /*char* s = NULL;
    size_t buf_size = 512;
    while (true) {
        int num = getline(&s, &buf_size, stdin);
        char** argv = shellSplit(s);
        if (argv == NULL) {
            printf("Err");
        } else {
            int i = 0;
            while(*(argv + i) != NULL) {
                printf("%s ", *(argv + i));
                i++;
            }
        }
        printf("\n");
        */
    setup_environment();
    open_history_file();
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
    close_history_file();
    return 0;
}

void start(bool read_from_file) {
    cd(""); // let shell starts from home
    /*print_all_variables();
    char cwd[512];
    char* str = getcwd(cwd, 512);
    printf("CWD=%s\n", str);*/
    if (read_from_file) {
        // file processing functions should be called from here

        shell_loop(true);
    } else {
        shell_loop(false);
    }
}

void execute_program(struct Command command) {
    fflush(get_history_file());
    pid_t pid = fork();
    if (pid == 0) {
        const char *PATH = lookup_variable("PATH");
        char **split_path = split(PATH, ":");
        int counter = 0;
        execv(command.argv[0], command.argv);
        while (*(split_path + counter) != NULL) {
            char file_path[PATH_MAX];
            sprintf(file_path, "%s/%s", *(split_path + counter), command.argv[0]);
            execv(file_path, command.argv);
            if (errno == EACCES) {
                printf("%s: permission denied: %s\n", SHELL_NAME, command.argv[0]);
            }
            counter++;
        }
        printf("%s: command not found: %s\n", SHELL_NAME, command.argv[0]);
        abort();
    } else if (pid > 0) {
        if (!command.isBackground) {
            wait(0);
        }
    }
}

void execute_assignment(struct Command command) {
    char temp[512];
    strcpy(temp, command.argv[0]);
    size_t len = strlen(temp);
    for (int i = 1; i < len - 1; i++) {
        if (temp[i] == '=') {
            temp[i] = '\0';
            set_variable(temp, temp + i + 1);
            break;
        }
    }
}

void shell_loop(bool input_from_file) {
    bool from_file = input_from_file;
    size_t buf_size = BUF_SIZE;
    char *line = (char *) malloc((buf_size + 1) * sizeof(char));
    while (true) {
        bool terminateLoop = false;
        FILE *stream;
        if (from_file) {
            //read next instruction from file

            // if end of file {from_file = false; continue;}
        } else {
            char *pwd = lookup_variable("PWD");
            printf("%s>%s> ", SHELL_NAME, pwd);
            stream = stdin;
        }

        char *ret = fgets(line, buf_size, stream);
        if (ret == NULL) {
            break;
        }
        line[strlen(line) - 1] = '\0';
        if (from_file) {
            printf("%s\n", line);
        }
        //parse your command here
        struct Command parsedCommand = parse_command(line);
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
                execute_assignment(parsedCommand);
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
            fputline(get_history_file(), line);
        }
        if (parsedCommand.argv != NULL) {
            free(parsedCommand.argv); //SEGABRT WHEN EMPTY
        }
        if (terminateLoop) {
            break;
        }
        /*
            you don't need to write all logic here, a better practice is to call functions,
            each one contains a coherent set of logical instructions
        */
    }
    free(line);
}
