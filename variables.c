#include "variables.h"
#include <string.h>
#include <stdio.h>

struct Entry {
    char *key;
    char *value;
};

struct Entry variables[1000];
int numVariables = 0;

const char *lookup_variable(char *key) {
    for (int i = 0; i < numVariables; ++i) {
        if (strcmp(variables[i].key, key)) {
            return variables[i].value;
        }
    }
    return NULL;
}

void set_variable(char *key, char *value) {
    for (int i = 0; i < numVariables; ++i) {
        if (strcmp(variables[i].key, key) == 0) {
            variables[i].value = value;
            return;
        }
    }
    variables[numVariables].key = key;
    variables[numVariables].value = value;
    numVariables++;
    return;
}

void print_all_variables(void) {
    for (int i = 0; i < numVariables; ++i) {
        printf("%d - %s=%s\n", i + 1, variables[i].key, variables[i].value);
    }
}