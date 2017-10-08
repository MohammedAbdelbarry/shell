#include "constants.h"
#include "command_parser.h"
#include "strutil.h"
#include "variables.h"
#include <stdio.h>
#include <string.h>


bool isAssignment(char *command) {
    size_t commandLen = strlen(command);
    for (int i = 1; i < commandLen - 1; i++) {
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
    } else if (argc > 1 && strcmp(argv[1], "&") == 0) {
        argv[1] = argv[2];
        return true;
    }
    return false;
}

struct Command parse_command(char *command) {
    const char COMMENT_CHAR = '#';
    struct Command parsedCommand;
    parsedCommand.argv = NULL;
    parsedCommand.argc = 0;
    parsedCommand.type = COMMENT;
    parsedCommand.error_code = 0;
    // pre processing
    size_t commandLen = strlen(command);
    for (unsigned int i = 0; i < commandLen; i++) {
        if (command[i] == COMMENT_CHAR) {
            command[i] = '\0';
            commandLen = i;
            break;
        }
    }
    parsedCommand.isBackground = false;
    if (commandLen == 0) {
        return parsedCommand;
    }

    int ret = variable_substitution(command, true);

    parsedCommand.argv = shellSplit(command);

    if (ret == NULL_USER) {
        parsedCommand.error_code = NULL_USER;
        return parsedCommand;
    }

    if (parsedCommand.argv == NULL) {
        printf("%s: failed to allocate memory\n", SHELL_NAME);
        return parsedCommand;
    }

    if (*parsedCommand.argv == NULL || strlen(*parsedCommand.argv) == 0) {
        return parsedCommand;
    }

    int counter = 0;
    while (parsedCommand.argv[counter] != NULL) {
//        if (parsedCommand.argv[counter][0] == '$') {
//            parsedCommand.argv[counter] = lookup_variable(parsedCommand.argv[counter] + 1);
//            parsedCommand.argv[counter] = parsedCommand.argv[counter] == NULL ? "" : parsedCommand.argv[counter];
//        } else if (parsedCommand.argv[counter][0] == '~') {
//            if (strlen(parsedCommand.argv[counter]) == 1) {
//                parsedCommand.argv[counter] = lookup_variable("HOME");
//            } else {
//                char buffer[512];
//                sprintf(buffer, "/home/%s", parsedCommand.argv[counter] + 1);
//                parsedCommand.argv[counter] = strdup(buffer);
//            }
//        }
        counter++;
    }
    parsedCommand.argv = parsedCommand.argv;
    parsedCommand.argc = counter;
    if (strcmp(parsedCommand.argv[0], "cd") == 0) {
        parsedCommand.type = CD;
    } else if (strcmp(parsedCommand.argv[0], "echo") == 0) {
        parsedCommand.type = ECHO;
    } else if (strcmp(parsedCommand.argv[0], "pwd") == 0) {
        parsedCommand.type = PWD;
    } else if (strcmp(parsedCommand.argv[0], "exit") == 0) {
        parsedCommand.type = EXIT;
    } else if (strcmp(parsedCommand.argv[0], "history") == 0) {
        parsedCommand.type = HISTORY;
    } else if ((strcmp(parsedCommand.argv[0], "printenv") == 0) ||
            (strcmp(parsedCommand.argv[0], "env") == 0 && parsedCommand.argc == 1)) {
        parsedCommand.type = PRINTENV;
    } else if (strcmp(parsedCommand.argv[0], "export") == 0) {
        parsedCommand.type = EXPORT;
    } else if (parsedCommand.argc == 1) {
        if (isAssignment(parsedCommand.argv[0])) {
            parsedCommand.type = EXPRESSION;
        } else {
            parsedCommand.type = PROGRAM;
            parsedCommand.isBackground = isBackground(parsedCommand.argv, parsedCommand.argc);
        }
    } else {
        parsedCommand.type = PROGRAM;
        parsedCommand.isBackground = isBackground(parsedCommand.argv, parsedCommand.argc);
    }
    return parsedCommand;
}
