#include "commands.h"
#include "variables.h"
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include <string.h>

void cd(char* path)
{
    if (strlen(path) == 0) {
        path = lookup_variable("HOME");
    }
    DIR* dir = opendir(path);
    if (dir != NULL) {
        // Directory exists
        closedir(dir);
        set_variable("PWD", path);
        chdir(path);
    } else if (ENOENT == errno) {
        // Directory does not exist
        printf("cd: no such file or directory: %s\n", path);
    } else {
        // Failed to open directory
        printf("cd: failed to change directory\n");
    }
}  


void echo( const char* message )
{
	printf("%s\n", message);
}