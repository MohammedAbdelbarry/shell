#include "constants.h"
#include "command_parser.h"
#include "strutil.h"
#include "variables.h"
#include <stdio.h>
#include <string.h>
#include <wordexp.h>


bool isAssignment(char* command) {
    size_t commandLen = strlen(command);
    for (int i = 1 ; i < commandLen - 1 ; i++) {
        if (command[i] == '=') {
            return true;
        }
    }
    return false;
}

bool isBackground(char **argv, int argc) {
    size_t commandLen = strlen(argv[0]);
    if (argc > 0 && argv[0][commandLen - 1] == '&') {
        argv[0][commandLen - 1] = '\0';
        return true;
    } else if(argc > 1 && strcmp(argv[1], "&") == 0) {
        argv[1] = argv[2];
        return true;
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
    parsedCommand.isBackground = false;
    if (commandLen == 0) {
        parsedCommand.type = COMMENT;
        return parsedCommand;
    }
    //wordexp_t expandedLine;
    //int ret = wordexp(command, &expandedLine, WRDE_SHOWERR);
    char **argv = shellSplit(command);
    /*switch(ret) {
        case WRDE_BADCHAR:
            printf("%s: error, invalid characters", SHELL_NAME);
            parsedCommand.type = COMMENT;
            return parsedCommand;
        case WRDE_SYNTAX:
            printf("%s: error, invalid syntax", SHELL_NAME);
            parsedCommand.type
            return parsedCommand;
    }*/
    if (argv == NULL) {
        printf("%s: failed to allocate memory\n", SHELL_NAME);
        parsedCommand.argv = NULL;
        parsedCommand.argc = 0;
        return parsedCommand;
    }

    if (*argv == NULL || strlen(*argv) == 0) {
        parsedCommand.type = COMMENT;
        return parsedCommand;
    }

    int counter = 0;
    while (argv[counter] != NULL) {
        if (argv[counter][0] == '$') {
            argv[counter] = lookup_variable(argv[counter] + 1);
            argv[counter] = argv[counter] == NULL ? "" : argv[counter];
        } else if (argv[counter][0] == '~') {
            if (strlen(argv[counter]) == 1) {
                argv[counter] = lookup_variable("HOME");
            } else {
                char buffer[512];
                sprintf(buffer, "/home/%s", argv[counter] + 1);
                argv[counter] = strdup(buffer);
            }
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
    } else if (strcmp(argv[0], "exit") == 0) {
        parsedCommand.type = EXIT;
    } else if (parsedCommand.argc == 1) {
        if (isAssignment(argv[0])) {
            parsedCommand.type = EXPRESSION;
        } else {
            parsedCommand.type = PROGRAM;
            parsedCommand.isBackground = isBackground(argv, parsedCommand.argc);
        }
    } else {
        parsedCommand.type = PROGRAM;
        parsedCommand.isBackground = isBackground(argv, parsedCommand.argc);
    }
    //wordfree(&expandedLine);
    return parsedCommand;
}
