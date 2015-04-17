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

	   0x100037000:	movabs rdx,0x100803200
	   0x10003700a:	add    BYTE PTR [rdx],0x1
	   0x10003700d:	add    BYTE PTR [rdx],0x1
	   0x100037010:	add    BYTE PTR [rdx],0x1
	   0x100037013:	add    BYTE PTR [rdx],0x1
	   0x100037016:	add    BYTE PTR [rdx],0x1
	   0x100037019:	add    BYTE PTR [rdx],0x1
	   0x10003701c:	add    BYTE PTR [rdx],0x1
	   0x10003701f:	add    BYTE PTR [rdx],0x1
	   0x100037022:	add    BYTE PTR [rdx],0x1
	   0x100037025:	add    BYTE PTR [rdx],0x1
	   0x100037028:	jmp    0x10003708f
	   0x10003702d:	add    rdx,0x1
	   0x100037031:	add    BYTE PTR [rdx],0x1
	   0x100037034:	add    BYTE PTR [rdx],0x1
	   0x100037037:	add    BYTE PTR [rdx],0x1
	   0x10003703a:	add    BYTE PTR [rdx],0x1
	   0x10003703d:	add    BYTE PTR [rdx],0x1
	   0x100037040:	add    BYTE PTR [rdx],0x1
	   0x100037043:	add    BYTE PTR [rdx],0x1
	   0x100037046:	add    rdx,0x1
	   0x10003704a:	add    BYTE PTR [rdx],0x1
	   0x10003704d:	add    BYTE PTR [rdx],0x1
	   0x100037050:	add    BYTE PTR [rdx],0x1
	   0x100037053:	add    BYTE PTR [rdx],0x1
	   0x100037056:	add    BYTE PTR [rdx],0x1
	   0x100037059:	add    BYTE PTR [rdx],0x1
	   0x10003705c:	add    BYTE PTR [rdx],0x1
	   0x10003705f:	add    BYTE PTR [rdx],0x1
	   0x100037062:	add    BYTE PTR [rdx],0x1
	   0x100037065:	add    BYTE PTR [rdx],0x1
	   0x100037068:	add    rdx,0x1
	   0x10003706c:	add    BYTE PTR [rdx],0x1
	   0x10003706f:	add    BYTE PTR [rdx],0x1
	   0x100037072:	add    BYTE PTR [rdx],0x1
	   0x100037075:	add    rdx,0x1
	   0x100037079:	add    BYTE PTR [rdx],0x1
	   0x10003707c:	sub    rdx,0x1
	   0x100037080:	sub    rdx,0x1
	   0x100037084:	sub    rdx,0x1
	   0x100037088:	sub    rdx,0x1
	   0x10003708c:	sub    BYTE PTR [rdx],0x1
	   0x10003708f:	cmp    BYTE PTR [rdx],0x0
	   0x100037092:	jne    0x10003702d
	   0x100037098:	add    rdx,0x1
	   0x10003709c:	add    BYTE PTR [rdx],0x1
	   0x10003709f:	add    BYTE PTR [rdx],0x1
	   0x1000370a2:	mov    eax,0x200018d
	   0x1000370a7:	mov    edi,0x1
	   0x1000370ac:	mov    rsi,rdx
	   0x1000370af:	push   rdx
	   0x1000370b0:	mov    edx,0x1
	   0x1000370b5:	syscall 
	   0x1000370b7:	pop    rdx
	   0x1000370b8:	add    rdx,0x1
	   0x1000370bc:	add    BYTE PTR [rdx],0x1
	   0x1000370bf:	mov    eax,0x200018d
	   0x1000370c4:	mov    edi,0x1
	   0x1000370c9:	mov    rsi,rdx
	   0x1000370cc:	push   rdx
	   0x1000370cd:	mov    edx,0x1
	   0x1000370d2:	syscall 
	   0x1000370d4:	pop    rdx
	   0x1000370d5:	add    BYTE PTR [rdx],0x1
	   0x1000370d8:	add    BYTE PTR [rdx],0x1
	   0x1000370db:	add    BYTE PTR [rdx],0x1
	   0x1000370de:	add    BYTE PTR [rdx],0x1
	   0x1000370e1:	add    BYTE PTR [rdx],0x1
	   0x1000370e4:	add    BYTE PTR [rdx],0x1
	   0x1000370e7:	add    BYTE PTR [rdx],0x1
	   0x1000370ea:	mov    eax,0x200018d
	   0x1000370ef:	mov    edi,0x1
	   0x1000370f4:	mov    rsi,rdx
	   0x1000370f7:	push   rdx
	   0x1000370f8:	mov    edx,0x1
	   0x1000370fd:	syscall 
	   0x1000370ff:	pop    rdx
	   0x100037100:	mov    eax,0x200018d
	   0x100037105:	mov    edi,0x1
	   0x10003710a:	mov    rsi,rdx
	   0x10003710d:	push   rdx
	   0x10003710e:	mov    edx,0x1
	   0x100037113:	syscall 
	   0x100037115:	pop    rdx
	   0x100037116:	add    BYTE PTR [rdx],0x1
	   0x100037119:	add    BYTE PTR [rdx],0x1
	   0x10003711c:	add    BYTE PTR [rdx],0x1
	   0x10003711f:	mov    eax,0x200018d
	   0x100037124:	mov    edi,0x1
	   0x100037129:	mov    rsi,rdx
	   0x10003712c:	push   rdx
	   0x10003712d:	mov    edx,0x1
	   0x100037132:	syscall 
	   0x100037134:	pop    rdx
	   0x100037135:	ret  

License
-------
BSD 3-clause. See [LICENSE](LICENSE).
