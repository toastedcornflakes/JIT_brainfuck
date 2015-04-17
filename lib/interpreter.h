#ifndef _interpreter_h_
#define _interpreter_h_

// Will fetch one instruction at the time and execute it
void interpreter_main_loop(unsigned char *command_begin, unsigned char *command_end);

#endif
