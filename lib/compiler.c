#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>

#include <utils/stack.h>

#include <compiler.h>
#include <dbg.h>

/* Conventions used in the generated code:
 %rip is the instruction pointer
 %rdx is the pointer to current memory cell (rdx wasn't chosen for any particular reason) */

typedef void (*jitted_code)();

/* Allocs a RWX page using mmap. */
void *alloc_executable_memory(size_t size) {
	void* ptr = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	check(ptr != MAP_FAILED && ptr != NULL, "Couldn't mmap RWX page");

	return ptr;

error:
	return NULL;
}


/* Convert brainfuck instruction to x64 code, returned in **code, with code size size in *size.
 Invalid brainfuck instructions (aka comments) returns NULL
 Note: asm files that generated the x64 hex are in the asm/ directory */
static void code_for_instruction(unsigned char instruction, unsigned char **code, size_t *size) {
	switch (instruction) {
		case '>':
			*code = "\x48\x83\xc2\x01"; // code for cc++, aka   add rdx, 1
			*size = 4;
			return;
		case '<':
			*code =  "\x48\x83\xea\x01"; // code for cc--, aka   sub rdx, 1
			*size = 4;
			return;

		case '+':
			*code = "\x80\x02\x01"; // code for (*cc)++, aka add [rdx], 1
			*size = 3;
			return;
		case '-':
			*code = "\x80\x2a\x01"; // (*cc)--
			*size = 3;
			return;
		case '.':
#ifdef __APPLE__
			// OS X syscalls are a bit different
			//*code = "\xb8\x04\x00\x00\x02\xbf\x01\x00\x00\x00\x48\x89\xd6\x52\xba\x01\x00\x00\x00\x0f\x05\x5a"; // write()
			  *code = "\xb8\x8d\x01\x00\x02\xbf\x01\x00\x00\x00\x48\x89\xd6\x52\xba\x01\x00\x00\x00\x0f\x05\x5a"; // write_nocancel()
			*size = 22;
#else
			*code = "\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x89\xd6\x52\xba\x01\x00\x00\x00\x0f\x05\x5a";   // print char stored at [rdx]
			*size = 22;
#endif
			return;
		default:
			*code = NULL;
			*size = -1;
	}
}

/* This function compiles and execute the `size` brainfucks instructions in `command`
*/
void jit_run(unsigned char *command, size_t size) {
	// create the data space of the program
	unsigned char *begin_cells = calloc(1, PLAYGROUND_CELLS);
	check_mem(begin_cells);
	
	unsigned char *big_chunk = alloc_executable_memory(4096 * 2);
	unsigned char *code = big_chunk;

	// add code to set rdx to address of playground
	code[0] = 0x48;
	code[1] = 0xba;
	code += 2;
	memcpy(code, &begin_cells, sizeof(unsigned char *));
	code += sizeof(unsigned char*);

	stack *jumpback_stack = stack_create(sizeof(unsigned char*));

	for (unsigned char *cur_command = command;  cur_command < command + size; cur_command++) {
		//debug("decoding command #%d", ++n);
		unsigned char *ins_code = NULL;
		size_t size;

		if (*cur_command == '[') {
			// will be filled later by the corresponding '['
			unsigned char base[5] = "\xe9 FLL"; 

			ins_code = base;
			size = 5;

			// note the current position (offset from start) for later use 
			unsigned char *next_ins = (unsigned char*)(code + size - big_chunk);
			stack_push(jumpback_stack, &next_ins);
		}
		else if (*cur_command == ']') {
			// TODO: check if stack is empty, which means
			check(!stack_empty(jumpback_stack), "Mismatched '[' or ']' in input");


			// jump is implemented in the form of jmp [RIP + const]
			unsigned char *addr;
			stack_pop(jumpback_stack, &addr);
			addr += (size_t)big_chunk;

			size = 9;
			// jump address relative to next instruction => needs to account for size of current jump instruction
			uint32_t jump_offset = addr - code - size; 
			debug("Making a %d bytes jump", jump_offset);
			unsigned char base[9] = "\x80\x3a\x00\x0f\x85    ";
			memcpy(&base[5], &jump_offset,4);

			ins_code = base;


			// now we also need to make the previous [ unconditionnaly jump here
			// just putting the current address there, relatively speaking
			uint32_t forward_offset = -(addr - code);
			memcpy(addr - 4, &forward_offset, 4);

		}
		else {
			// it's not a branch instruction, nothing special to do
			code_for_instruction(*cur_command, &ins_code, &size);
		}

		// copy instruction to executable page
		// ignoring comments
		if(ins_code) {
			memcpy(code, ins_code, size);
			code += size; 
		}
	}

	// add `ret` to exit cleanly:
	code[0] = 0xc3;
	code++;

	jitted_code func =  (jitted_code)big_chunk;

	debug("Running compiled code.");
	func();

error:
	if (begin_cells) {
		free(begin_cells);
	}
	return;
}

