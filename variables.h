#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

#include <stdbool.h>

/**
 * Looks up a key in the variable table and returns its value
 * if it exists.
 * @param key The key to be looked up
 * @return The value of the key if it exists and false otherwise
 */
const char *lookup_variable(char *key);

/**
 * Sets up the value of the key in the lookup table
 * to the given value.
 * @param key The key to be set
 * @param value The value of the key
 * @param export A parameter indicating whether or
 * not this key represents an environment variable.
 */
void set_variable(char *key, char *value, bool export);


/**
 * Prints the keys and values of all lookup table entries.
 */
void print_all_variables();

/**
 * Destroys all variables in the lookup table.
 * Used to clear memory before terminating.
 */
void destroy_all_variables();

#endif // VARIABLES_H_