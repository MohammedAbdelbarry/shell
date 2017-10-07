#include "environment.h"
#include "variables.h"
#include <stdlib.h>

void setup_environment() {
    set_variable("PATH", getenv("PATH"), false);
    set_variable("HOME", getenv("HOME"), false);
    set_variable("USER", getenv("USER"), false);
    set_variable("PWD", getenv("PWD"), false);
    set_variable("SHELL_HOME", getenv("PWD"), false);
}