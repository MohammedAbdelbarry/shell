#include "constants.h"
#include "command_parser.h"
#include "strutil.h"
#include "variables.h"
#include <stdio.h>
#include <string.h>


bool isAssignment(char* command) {
    size_t commandLen = strlen(command);
    for (int i = 0 ; i < commandLen ; i++) {
        if (command[i] == '=' &&  i > 0) {
            return true;
        }
    }
    return false;
}

struct Command parse_command(char *command) {
    const char COMMENT_CHAR = '#';
    struct Command parsedCommand;
    // pre processing
    size_t commandLen = strlen(command);
    for (int i = 0 ; i < commandLen ; i++) {
        if (command[i] == COMMENT_CHAR) {
            command[i] = '\0';
            commandLen = i;
            break;
        }
    }
    if (commandLen == 0) {
        parsedCommand.type = COMMENT;
        return parsedCommand;
    }
    char **argv = shellSplit(command);
    if (argv == NULL) {
        printf("%s: failed to allocate memory\n", SHELL_NAME);
    }
    int counter = 0;
    while (argv[counter] != NULL) {
        if(argv[counter][0] == '$') {
            argv[counter] = lookup_variable(argv[counter] + 1);
            argv[counter] = argv[counter] == NULL ? "" : argv[counter];
        }
        counter++;
    }
    parsedCommand.argv = argv;
    parsedCommand.argc = counter;
    if(strcmp(argv[0], "cd") == 0) {
        parsedCommand.type = CD;
    } else if (strcmp(argv[0], "echo") == 0) {
        parsedCommand.type = ECHO;
    } else if (strcmp(argv[0], "pwd") == 0) {
        parsedCommand.type = PWD;
    } else if (parsedCommand.argc == 1) {
        if (isAssignment(argv[0])) {
            parsedCommand.type = EXPRESSION;
        } else {
            parsedCommand.type = PROGRAM;
        }
    } else {
        parsedCommand.type = PROGRAM;
    }
    return parsedCommand;
}
