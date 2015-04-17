CFLAGS=-g -O0 -Ilib -std=gnu99 -m64 -Wno-pointer-sign -DPLAYGROUND_CELLS=30000 -DNDEBUG

SOURCES=$(wildcard lib/**/*.c lib/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

ASM=$(wildcard asm/*.asm)
ASMBIN=$(patsubst %.asm,%.bin,$(ASM))

TARGET=brain

all: $(TARGET) tags

$(TARGET): $(OBJECTS)

asm: $(ASMBIN)

%.bin: %.asm
	nasm -f bin $< -o $@

tags: $(SOURCES)
	@(ctags -R --exclude=asm . || true)

clean:
	@rm -rf $(TARGET) $(TARGET).dSYM $(OBJECTS) $(ASMBIN) tags peda-*

.PHONY: tags clean asm

