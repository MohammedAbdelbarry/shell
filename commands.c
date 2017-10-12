#include "commands.h"
#include "variables.h"
#include "file_processing.h"
#include "constants.h"
#include "strutil.h"
#include <unistd.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

extern char **environ;

void cd(char *path) {
    bool empty = path == NULL || strlen(path) == 0;
    if (empty) {
        path = lookup_variable("HOME");
    }
    DIR *dir = opendir(path);
    if (dir != NULL) {
        // Directory exists
        closedir(dir);
        char real_path[PATH_MAX];
        realpath(path, real_path);
        set_variable("PWD", real_path, true);
        chdir(path);
    } else if (ENOENT == errno) {
        // Directory does not exist
        printf("cd: no such file or directory: %s\n", path);
    } else {
        // Failed to open directory
        printf("cd: failed to change directory\n");
    }
}


void echo(struct Command command) {
    for (int i = 1; i < command.argc; i++) {
        printf("%s ", command.argv[i]);
    }
    printf("\n");
}

void execute_cd(struct Command command) {
    if (command.argc == 2) {
        cd(command.argv[1]);
    } else if (command.argc == 1) {
        cd("");
    } else {
        printf("cd: invalid number of arguments\n");
    }
}

void pwd() {
    const char *pwd = lookup_variable("PWD");
    printf("%s\n", pwd);
}

void history() {
    rewind(get_history_file());
    char *line = (char *) malloc((BUF_SIZE + 1) * sizeof(char));
    int counter = 1;
    while (fgets(line, BUF_SIZE, get_history_file()) != NULL) {
        printf("\t%d\t%s", counter++, line);
    }
    free(line);
}

void printenv(struct Command command) {
    if (command.argc == 1) {
        char **ptr = environ;
        for (; *ptr != NULL; ++ptr) {
            printf("%s\n", *ptr);
        }
    } else {
        char **ptr = ++command.argv;
        for (; *ptr != NULL; ++ptr) {
            char *val = getenv(*ptr);
            if (val != NULL) {
                printf("%s\n", val);
            }
        }
    }
}

void sigchld_handler(int sig) {
    pid_t pid;
    pid = wait(0);
    char buffer[512];
    sprintf(buffer, "child %d done", pid);
    printf("%s: %s\n", SHELL_NAME, buffer);

    log(get_log_file(), buffer, DEBUG);
    fflush(stdout);
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
        char **split_path = split(PATH, ":", false, true);
        int counter = 0;
        execv(command.argv[0], command.argv);
        while (split_path[counter] != NULL) {
            char file_path[strlen(split_path[counter]) + strlen(command.argv[0]) + 2];
            sprintf(file_path, "%s/%s", split_path[counter], command.argv[0]);
            execv(file_path, command.argv);
            counter++;
        }
        if (errno == EACCES) {
            fprintf(stderr, "%s: permission denied: %s\n", SHELL_NAME, command.argv[0]);
        } else {
            fprintf(stderr, "%s: command not found: %s\n", SHELL_NAME, command.argv[0]);
        }
        free(PATH);
        free(split_path);
        abort();
    } else if (pid > 0) {
        char buffer[512];
        sprintf(buffer, "child %d started: %s", pid, command.argv[0]);
        log(get_log_file(), buffer, DEBUG);
        if (!command.isBackground) {
            int status;
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            sprintf(buffer, "child %d done.", pid);
            log(get_log_file(), buffer, DEBUG);
        } else {
            signal(SIGCHLD, sigchld_handler);
            printf("%s: %s\n", SHELL_NAME, buffer);
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
    for (int i = 1; i < len; i++) {
        if (temp[i] == '=') {
            temp[i] = '\0';
            set_variable(temp, temp + i + 1, export);
            found = true;
            break;
        }
    }
    if (!found && export) {
        char *val = lookup_variable(command.argv[1]);
        if (val != NULL) {
            val = strdup(val);
            set_variable(command.argv[1], val, export);
            free(val);
        }
    }
}
