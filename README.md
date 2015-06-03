`jit_brainfuck`
==============
A JIT compiler for brainfuck on x86_64

How to run:
----------
Currently, `jit_brainfuck` runs on OS X and linux 64 bits, and a Dockerfile for [docker](https://www.docker.com/) is provided.

To build and run natively:

	make
	./brain [--intepreter|--jit] script.fck


The [interpreter](lib/interpreter.c) reads one character at the time from the brainfuck script, execute the corresponding operation on the `instruction_pointer` or `cell_pointer`.

The [jit compiler](lib/compiler.c) reads the whole file and generates x64 assembly corresponding to the brainfuck instructions. It does so in one pass, without any optimisation.


Example
-------
The generated assembly from [hello script](scripts/hello.fck) looks like this.

It matches 1:1 with the brainfuck instructions:

       ; set up register for memory handing
	   0x100037000:	movabs rdx,0x100803200

	   0x10003700a:	add    BYTE PTR [rdx],0x1
	   0x10003700d:	add    BYTE PTR [rdx],0x1
	   0x100037010:	add    BYTE PTR [rdx],0x1
	   0x100037025:	add    BYTE PTR [rdx],0x1
	   ... snip ...

	   ; start of loop
	   0x100037028:	jmp    0x10003708f
	   0x10003702d:	add    rdx,0x1
	   0x100037031:	add    BYTE PTR [rdx],0x1
	   0x100037034:	add    BYTE PTR [rdx],0x1
	   ... snip ...
	   0x100037088:	sub    rdx,0x1

	   ; end loop
	   0x10003708c:	sub    BYTE PTR [rdx],0x1
	   0x10003708f:	cmp    BYTE PTR [rdx],0x0
	   0x100037092:	jne    0x10003702d

	   0x10003709f:	add    BYTE PTR [rdx],0x1

	   ; make a write syscall
	   0x1000370a2:	mov    eax,0x200018d
	   0x1000370a7:	mov    edi,0x1
	   0x1000370ac:	mov    rsi,rdx
	   0x1000370af:	push   rdx
	   0x1000370b0:	mov    edx,0x1
	   0x1000370b5:	syscall 
	   0x1000370b7:	pop    rdx
       ... snip ... 

	   ; end of program
	   0x100037135:	ret  

License
-------
BSD 3-clause. See [LICENSE](LICENSE).
