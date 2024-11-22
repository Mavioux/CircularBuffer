# Makefile

CC := gcc
CFLAGS := -Wall -Wextra -g
LDFLAGS :=

ifeq ($(OS),Windows_NT)
    RM := del /Q
    TARGET := heart_monitor.exe
else
    RM := rm -f
    TARGET := heart_monitor
endif

# Source directory
SRCDIR := src
# Object directory (optional, can be the same as SRCDIR or separate)
OBJDIR := obj

SRCDIR := src
# List of source files
SRCS := $(wildcard $(SRCDIR)/*.c)
# Replace source file extensions with .o and store in OBJDIR
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

.PHONY: all clean

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) # Ensure OBJDIR exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	$(RM) $(OBJS) $(TARGET)
	@if [ -d $(OBJDIR) ]; then rmdir $(OBJDIR); fi