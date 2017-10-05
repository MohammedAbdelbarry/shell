#include "commands.h"
#include "variables.h"
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void cd(char* path)
{
    if (strlen(path) == 0) {
        path = lookup_variable("HOME");
    }
    DIR* dir = opendir(path);
    if (dir != NULL) {
        // Directory exists
        closedir(dir);
        char real_path[PATH_MAX];
        realpath(path, real_path);
        set_variable("PWD", real_path);
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
    for (int i = 1 ; i < command.argc ; i++) {
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
    const char* pwd = lookup_variable("PWD");
    printf("%s\n", pwd);
}