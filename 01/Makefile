# assembler, linker, and flags
AS = nasm
LD = ld
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386

# source and object files
SRC = conway.asm
OBJ = $(SRC:.asm=.o)
BIN = conway

# default target (make)
all: $(BIN)

# rule to build the executable
$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $<

# rule to assemble source files
%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

# clean target
clean:
	rm -f $(OBJ) $(BIN)

# run target
run: $(BIN)
	./$(BIN)

.PHONY: all clean run
