#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <dbg.h>
#include <interpreter.h>
#include <compiler.h>


int main(int argc, char **argv) {
	unsigned char *command_file = NULL;
	int use_interpreter = 0;

	FILE *script_file = NULL;
	char *script_path = argv[argc - 1];

	check(argc == 2 || argc == 3, "Usage: %s [--interpreter|--JIT] [script|-]", argv[0]);
	
	if(strcmp(argv[1], "--interpreter") == 0) {
		debug("Using interpreter");
		use_interpreter = 1;
	} else {
		debug("Using JIT compiler");
	}

	if(strcmp(script_path, "-") == 0) {
		script_file = stdin;
	} else {
		script_file = fopen(script_path, "r");
	}
	check(script_file != NULL, "Couldn't open script %s", script_path);

	// get size of brainfuck instructions file
	fseek(script_file, 0L, SEEK_END);
	size_t command_size = ftell(script_file);
	fseek(script_file, 0L, SEEK_SET);


	debug("command_size: %zu", command_size);

	// read instructions file to hand it to the interpreter/compiler
	command_file = malloc(command_size);
	check_mem(command_file);
	
	size_t read_bytes = fread(command_file, 1, command_size, script_file);
	check(read_bytes == command_size, "fread() failed to read the script");
	fclose(script_file);

	if(use_interpreter) {
		interpreter_main_loop(command_file, command_file + command_size);
	} 
	else {
		jit_run(command_file, command_size);
	}

error:
	if(command_file) {
		free(command_file);
	}
}

