#include "commands.h"
#include "variables.h"
#include "file_processing.h"
#include "constants.h"
#include <unistd.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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
    if (empty) {
        free(path);
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
        char** ptr = environ;
        for ( ; *ptr != NULL ; ++ptr) {
            printf("%s\n", *ptr);
        }
    } else {
        char** ptr = ++command.argv;
        for (; *ptr != NULL ; ++ptr) {
            char *val = getenv(*ptr);
            if (val != NULL) {
                printf("%s\n", val);
            }
        }
    }
}