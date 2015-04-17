BITS 64

mov rax, 0x200018d	 ; write_nocancel is used by libc
mov rdi, 1           ; to stdout
mov rsi, rdx         ; rdx is the address of the one character string [rdx] (forgetting the NUL byte)
push rdx             ; save rdx to stack
mov rdx, 1           ; size of message
syscall             
pop rdx              ; restore rdx
