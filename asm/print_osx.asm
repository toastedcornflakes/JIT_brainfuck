BITS 64

mov rax, 0x200018d	 ; write_nocancel is used by libc
mov rdi, 1           ; to stdout
mov rsi, rdx         ; rdx is the address of the one character string [rdx] (doesn't need a NUL byte since we specify the length of the message)
push rdx             ; save rdx to stack
mov rdx, 1           ; size of message
syscall             
pop rdx              ; restore rdx
