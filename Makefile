# Object files location. Object files will be placed in this directory during compilation
OBJDIR := objects

# Full names of object files
OBJECTS	:= $(addprefix $(OBJDIR)/, core.o cJSON.o Window.o Application.o Timer.o Manager.o Grid.o)

# ================================================================ #

# C compiler
CC := gcc

# We are only interested in object files without linking them into an executable `-c`
# Debugging included `-g`
CFLAGS := -g -c

# Extra layer of protection
ALL_CFLAGS := $(CFLAGS) -Wall -Wextra -pedantic-errors -fPIC -O2

# ================================ #

# Additional libraries that need to be searched for function definitions
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf

# ================================ #

# Using an archiver to produce a static library
AR := ar

# Archiver flags
ARFLAGS := -r -s

# ================================ #

# *nix operating system support only
OS_NAME := $(shell uname -s)

# Generating a list of all header files in the `include` directory and its subdirectories
INCLUDE := $(wildcard include/*/*.h)

# Library name
LIB_NAME := SanchoPanza

# ================================ #

# If the operating system is Linux, set the variables
ifeq ($(OS_NAME), Linux)
# The prefix for library names
	LIB_PREFIX := lib
# The suffix for static library files
    LIB_SUFFIX := .a
# The suffix for dynamic library files
    DLL_SUFFIX := .so
else ifeq ($(OS_NAME), Darwin)
# The prefix for library names
    LIB_PREFIX := lib
# The suffix for static library files
    LIB_SUFFIX := .a
# The suffix for dynamic library files
    DLL_SUFFIX := .dylib
else
    $(error Unsupported operating system)
endif

# ======== #

# Constructing the name of static library
TARGET_STATIC := $(LIB_PREFIX)$(LIB_NAME)$(LIB_SUFFIX)

# Constructing the name of static library
TARGET_SHARED := $(LIB_PREFIX)$(LIB_NAME)$(DLL_SUFFIX)

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Setting the value of the variable CORE to the path of the `core.c`
CORE := $(addprefix source/core/, core.c)

# Setting the value of the variable cJSON to the path of the `cJSON.c`
cJSON := $(addprefix source/core/, cJSON.c)

# Setting the value of the variable WINDOW to the path of the `window.c`
WINDOW := $(addprefix source/Window/, window.c)

# Setting the value of the variable APPLICATION to the path of the `application.c`
APPLICATION := $(addprefix source/Application/, application.c)

# Setting the value of the variable TIMER to the path of the `timer.c`
TIMER := $(addprefix source/Timer/, timer.c)

# Setting the value of the variable MANAGER to the path of the `manager.c`
MANAGER := $(addprefix source/InputManager/, manager.c)

# Setting the value of the variable GRID to the path of the `manager.c`
GRID := $(addprefix source/Grid/, grid.c)
# ================================================================ #
# ================================================================ #
# ================================================================ #

# Define a target
all: $(TARGET_STATIC) $(TARGET_SHARED)

# Building a static library
$(TARGET_STATIC): $(OBJECTS)
	$(AR) $(ARFLAGS) $(TARGET_STATIC) $^

#Building a shared library
$(TARGET_SHARED): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

# ================================ #

# Building the `core.o` object file from the CORE
$(OBJDIR)/core.o: $(CORE) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# Building the `cJSON.o` object file from the cJSON
$(OBJDIR)/cJSON.o: $(cJSON) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# Building the `Window.o` object file from the WINDOW
$(OBJDIR)/Window.o: $(WINDOW) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# Building the `Application.o` object file from the APPLICATION
$(OBJDIR)/Application.o: $(APPLICATION) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# Building the `Timer.o` object file from the APPLICATION
$(OBJDIR)/Timer.o: $(TIMER) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# Building the `Manager.o` object file from the MANAGER
$(OBJDIR)/Manager.o: $(MANAGER) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# Building the `Grid.o` object file from the GRID
$(OBJDIR)/Grid.o: $(GRID) $(INCLUDE)
	$(CC) $(ALL_CFLAGS) $(CFLAGS) -o $@ $<

# ================================================================ #
# ================================================================ #
# ================================================================ #

# Creating a directory if it does not exist. *nix
$(shell mkdir -p $(OBJDIR))

# ================================ #
# Executing the commands regardless of whether a file named clean exists or not
.PHONY: clean

clean:
	rm -rf $(OBJS) $(OBJDIR) $(TARGET_STATIC) $(TARGET_SHARED)
