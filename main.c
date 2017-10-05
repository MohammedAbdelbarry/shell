#include "environment.h"
#include "variables.h"
#include "commands.h"
#include "command.h"
#include "command_parser.h"
#include "constants.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
//#include <stdlib.h>
//#include "strutil.h"

void start(bool read_from_file);
void shell_loop(bool input_from_file);

int main(int argc, char *argv[])
{
	/*char* s = NULL;
    size_t buf_size = 512;
	while (true) {
        int num = getline(&s, &buf_size, stdin);
		char** argv = shellSplit(s);
        if (argv == NULL) {
            printf("Err");
        } else {
            int i = 0;
            while(*(argv + i) != NULL) {
                printf("%s ", *(argv + i));
                i++;
            }
        }
        printf("\n");
        */
    setup_environment();
    // any other early configuration should be here
    if( argc > 1 ){
        start(true);
    }
    else{
        start(false);
    }

    return 0;
}

void start(bool read_from_file)
{
	cd(""); // let shell starts from home
    /*print_all_variables();
    char cwd[512];
    char* str = getcwd(cwd, 512);
    printf("CWD=%s\n", str);*/
    if(read_from_file){
		// file processing functions should be called from here

		shell_loop(true);
	}
	else{
		shell_loop(false);
	}
}

void execute_program(struct Command command) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(command.argv[0], command.argv);
        printf("ERROR");
        abort();
    } else if (pid > 0) {
        if (!command.isBackground) {
             wait(0);
        }
    }
}

void execute_cd(struct Command command) {
    if (command.argc == 2) {
        cd(command.argv[1]);
        printf("%s\n", command.argv[1]);
    } else if (command.argc == 1) {
        cd("");
    } else {
        printf("cd: invalid number of arguments\n");
    }
}

void execute_echo(struct Command command) {
    for (int i = 1 ; i < command.argc ; i++) {
        printf("%s ", command.argv[i]);
    }
    printf("\n");
}

void shell_loop(bool input_from_file)
{
	bool from_file = input_from_file;
    char* line = NULL;
    size_t buf_size = 512;
    while(true){
		if(from_file){
			//read next instruction from file

			// if end of file {from_file = false; continue;}
		}
		else{
            char* pwd = lookup_variable("PWD");
            printf("%s>%s> ", SHELL_NAME, pwd);
            int retCode = getline(&line, &buf_size, stdin);
            if (retCode < 1) {
                continue;
            }
            line[retCode - 1] = '\0';
        }

		//parse your command here
        struct Command parsedCommand = parse_command(line);
		//execute your command here
        switch(parsedCommand.type) {
            case PROGRAM:
                execute_program(parsedCommand);
                break;
            case CD:
                execute_cd(parsedCommand);
            case ECHO:
                execute_echo(parsedCommand);
            case COMMENT:
            default:
                break;
        }
        //free(line);
        //free(parsedCommand.argv);
		/*
			you don't need to write all logic here, a better practice is to call functions,
			each one contains a coherent set of logical instructions
		*/
	}
}
