BITS 64

db 'prout'
yolo:
db 'abaaaaaaaaaaaaaaaa'

mov al, [rdx]
test eax,eax
jnz yolo
