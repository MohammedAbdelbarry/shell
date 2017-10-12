#include "variables.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Entry {
    char *key;
    char *value;
};

struct Entry variables[1000];
int numVariables = 0;

const char *lookup_variable(char *key) {
    for (int i = 0; i < numVariables; ++i) {
        if (strcmp(variables[i].key, key) == 0) {
            return variables[i].value;
        }
    }
    return getenv(key);
}

void set_variable(char *key, char *value, bool export) {
    bool found = false;
    bool env_found = getenv(key) != NULL;
    if (env_found) {
        setenv(key, value, true);
    }
    for (int i = 0; i < numVariables; ++i) {
        if (strcmp(variables[i].key, key) == 0) {
            char *old_value = variables[i].value;
            variables[i].value = strdup(value);
            free(old_value);
            found = true;
        }
    }
    if (export && !env_found) {
        setenv(key, value, true);
    } else if (!found) {
        variables[numVariables].key = strdup(key);
        variables[numVariables].value = strdup(value);
        numVariables++;

    }
    return;
}

void print_all_variables() {
    for (int i = 0; i < numVariables; ++i) {
        printf("%d - %s = %s\n", i + 1, variables[i].key, variables[i].value);
    }
}

void destroy_all_variables() {
    for (int i = 0; i < numVariables; ++i) {
        free(variables[i].key);
        free(variables[i].value);
    }
    numVariables = 0;
}