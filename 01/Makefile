# compilers, assembler, and flags
AS = nasm
CXX = g++
ASFLAGS = -f elf64
CXXFLAGS = -c `sdl2-config --cflags`
LDFLAGS = -no-pie `sdl2-config --libs`

# directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# source and object files
ASM_SRC = $(SRC_DIR)/conway.asm
CPP_SRC = $(SRC_DIR)/draw.cpp
ASM_OBJ = $(BUILD_DIR)/conway.o
CPP_OBJ = $(BUILD_DIR)/draw.o
OBJ = $(ASM_OBJ) $(CPP_OBJ)
BIN = $(BIN_DIR)/sim

# default target
all: $(BIN)

# build binary
$(BIN): $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) -lSDL2
	#$(CXX) $(LDFLAGS) $(OBJ) -o $@

# assemble asm file
$(ASM_OBJ): $(ASM_SRC) | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# compile cpp file
$(CPP_OBJ): $(CPP_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# create build and bin directories if they don't exist
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# clean target
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# run target
run: $(BIN)
	./$(BIN)

.PHONY: all clean run
