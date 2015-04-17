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

	int fd = -1;
	char *script_path = argv[argc - 1];

	check(argc == 2 || argc == 3, "Usage: %s [--interpreter|--JIT] script", argv[0]);
	
	if(strcmp(argv[1], "--interpreter") == 0) {
		debug("Using interpreter");
		use_interpreter = 1;
	} else {
		debug("Using JIT compiler");
	}

	fd = open(script_path, O_RDWR, 0);
	check(fd != -1, "Couldn't open script %s", script_path);

	// get size of brainfuck instructions file
	struct stat st;
	check(fstat(fd, &st) == 0, "stat");
	size_t command_size = st.st_size;

	debug("command_size: %zu", command_size);
	debug("File descriptor: %d", fd);

	// mmap the brainfuck instructions file to hand it to the interpreter/compiler
	command_file = mmap(NULL, command_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
	close(fd);
	if (command_file == MAP_FAILED) {
		log_err("MAP_FAILED, exiting");
		exit(-1);
	}
	debug("mmaped %s at %p", script_path, command_file);

	if(use_interpreter) {
		interpreter_main_loop(command_file, command_file + command_size);
	} 
	else {
		jit_run(command_file, command_size);
	}

error:
	if(command_file) {
		munmap(command_file, command_size);
	}
}

