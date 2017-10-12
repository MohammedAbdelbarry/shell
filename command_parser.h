#ifndef COMMAND_PARSER_H   /* Include guard */
#define COMMAND_PARSER_H

#include "command.h"

/**
 * Takes a pointer to the scanned line, parses it
 * and returns the parsed command.
 * @param command_ptr A pointer to the scanned input line
 * @return A struct Command representing the parsed command
 */
struct Command parse_command(char **command_ptr);


#endif // COMMAND_PARSER_H