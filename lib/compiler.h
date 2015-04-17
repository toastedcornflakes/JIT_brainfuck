#ifndef _compiler_h_
#define _compiler_h_

// shitty workaround for OS X.
#ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS MAP_ANON
#endif


void jit_run(unsigned char *command, size_t size);

#endif
