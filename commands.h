#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

#include "command.h"

/**
	- This function should be responsible for implementing the "cd" shell command
*/
void cd(char *path);

/**
	- This function should be responsible for implementing the "echo" shell command
	- Typically should lookup any variable included in the message
*/
void echo(struct Command command);

void execute_cd(struct Command command);

#endif // COMMANDS_H_