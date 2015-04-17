BITS 64

mov rax, 1	    ; write is syscall number 1
mov rdi, 1      ; write to stdout
mov rsi, rdx    ; rdx is the address of the one character string [rdx] (forgetting the NUL byte works)
push rdx        ; save rdx to stack
mov rdx, 1      ; size of message
syscall			; yay write
pop rdx         ; restore rdx
