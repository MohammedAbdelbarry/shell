#include "environment.h"
#include "variables.h"
#include <stdlib.h>
#include <limits.h>
#include <zconf.h>
#include <libgen.h>

void setup_environment() {
    char buffer[PATH_MAX];
    readlink("/proc/self/exe", buffer, PATH_MAX);
    set_variable("PATH", getenv("PATH"), false);
    set_variable("HOME", getenv("HOME"), false);
    set_variable("USER", getenv("USER"), false);
    set_variable("PWD", getenv("PWD"), false);
    set_variable("SHELL_HOME", dirname(buffer), false);
}