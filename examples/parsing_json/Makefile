# C compiler
CC := gcc

# Debugging included `-g`
CFLAGS := -g

# Extra layer of protection
ALL_CFLAGS := -Wall -Wextra -pedantic-errors -O2

# ================================ #

# Additional libraries that need to be searched for function definitions
LDFLAGS := -lSDL2 -lSDL2_image

# Executable file name
BIN := a.out

# ================================================================ #

# Building the executable with a static library
$(BIN): main.c
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $^ ../../libSanchoPanza.a $(LDFLAGS)

# ================================================================ #

.PHONY: clean

clean:
	rm -rf $(OBJDIR) ./*.a ./*.o ./*.out
