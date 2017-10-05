#include "constants.h"
#include "command_parser.h"
#include "strutil.h"
#include "variables.h"
#include <stdio.h>
#include <string.h>
#include <wordexp.h>


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
    parsedCommand.isBackground = false;
    if (commandLen == 0) {
        parsedCommand.type = COMMENT;
        return parsedCommand;
    }
    wordexp_t expandedLine;
    int ret = wordexp(command, &expandedLine, WRDE_SHOWERR);
    char **argv = expandedLine.we_wordv;//shellSplit(command);
    switch(ret) {
        case WRDE_BADCHAR:
            printf("%s: error, invalid characters", SHELL_NAME);
            parsedCommand.type = COMMENT;
            return parsedCommand;
        case WRDE_SYNTAX:
            printf("%s: error, invalid syntax", SHELL_NAME);
            parsedCommand.type = COMMENT;
            return parsedCommand;
    }
    if (argv == NULL) {
        printf("%s: failed to allocate memory\n", SHELL_NAME);
        parsedCommand.argv = NULL;
        parsedCommand.argc = 0;
        //wordfree(&expandedLine);
        return parsedCommand;
    }

    if (*argv == NULL || strlen(*argv) == 0) {
        parsedCommand.type = COMMENT;
        //wordfree(&expandedLine);
        return parsedCommand;
    }

    parsedCommand.argv = argv;
    parsedCommand.argc = expandedLine.we_wordc;
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
    //wordfree(&expandedLine);
    return parsedCommand;
}
