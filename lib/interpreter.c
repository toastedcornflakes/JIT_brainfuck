#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <interpreter.h>

#include <dbg.h>

// will fetch one brainfuck instruction at a time and execute it
void interpreter_main_loop(unsigned char *command_begin, unsigned char *command_end) {
	unsigned char *begin_cells = calloc(1, PLAYGROUND_CELLS);
	check_mem(begin_cells);
	
	unsigned char *cell_pointer = begin_cells;
	unsigned char *instruction_pointer = command_begin;


	/* safe loop version:
	while(instruction_pointer >= command_begin && instruction_pointer < command_end &&
			cell_pointer >= begin_cells && cell_pointer < begin_cells + PLAYGROUND_CELLS)
	*/


	// the loop has NO safety checks whatsoever
	while(instruction_pointer < command_end) {
		unsigned char command = *instruction_pointer;
		switch (command) {
			case '>':
				cell_pointer++;
				break;
			case '<':
				cell_pointer--;
				break;
			case '+':
				(*cell_pointer)++;
				break;
			case '-':
				(*cell_pointer)--;
				break;
			case '.':
				putchar(*cell_pointer);
				break;
			case',':
				// getchar returns an int
				*cell_pointer = (unsigned char)getchar();
				break;

				// control flow stuff
			case'[':
				// if *cell_pointer == 0, jump to CORRESPONDING (not next!) ]
				if (*cell_pointer == 0) {
					int count = 0;
					while (1) {
						if (*instruction_pointer == '[') {
							count++;
						} else if (*instruction_pointer == ']') {
							if(--count == 0) {
								break;
							}
						}
						instruction_pointer++;
					}
				}
				break;
			case ']':
				if (*cell_pointer != 0) {
					// jump to CORRESPONDING (not prev!) `[`
					int count = 0;
					while (1) {
						if (*instruction_pointer == ']') {
							count++;
						} else if (*instruction_pointer == '[') {
							if(--count == 0) {
								break;
							}
						}
						instruction_pointer--;
					}
				}
				break;
		}
		instruction_pointer++;
	}

error:
	if(begin_cells) {
		free(begin_cells);
	}

}
