#include "environment.h"
#include "variables.h"
#include <stdlib.h>

void setup_environment() {
    set_variable("PATH", getenv("PATH"));
    set_variable("HOME", getenv("HOME"));
    set_variable("USER", getenv("USER"));
    set_variable("PWD", getenv("PWD"));
    set_variable("SHELL_HOME", getenv("PWD"));
}