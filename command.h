#include <stdbool.h>
#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H
enum CommandType {
    CD,
    ECHO,
    PWD,
    COMMENT,
    EXPRESSION,
    PROGRAM
};
struct Command {
    char* command;
    int argc;
    char** argv;
    bool isBackground;
    enum CommandType  type;
};
#endif //SHELL_COMMAND_H
