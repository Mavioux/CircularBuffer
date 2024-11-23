CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -g
CXXFLAGS := -Wall -Wextra -g
LDFLAGS :=

ifeq ($(OS),Windows_NT)
    RM := del /Q /S
    MKDIR := mkdir
    TARGET := heart_monitor.exe
    TEST_TARGET := test_runner.exe
else
    RM := rm -rf
    MKDIR := mkdir -p
    TARGET := heart_monitor
    TEST_TARGET := test_runner
endif

# Source and object directories
SRCDIR := src
OBJDIR := obj
TESTDIR := unittest

# Include directory
INCLUDES := -I$(SRCDIR)

# Source files for main application
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/$(SRCDIR)/%.o, $(SRCS))

# Source files for tests
TEST_SRCS := $(SRCDIR)/$(TESTDIR)/circular_buffer_test.cpp
# We only need the circular_buffer object file for the test
EXTRA_TEST_OBJS := $(OBJDIR)/$(SRCDIR)/circular_buffer.o

.PHONY: all clean test

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Build the test executable
test: $(TEST_TARGET)

$(TEST_TARGET): $(EXTRA_TEST_OBJS)
	$(CXX) $(TEST_SRCS) $(EXTRA_TEST_OBJS) -o $(TEST_TARGET) -lgtest -lgtest_main -pthread $(INCLUDES)

# Compile source files into object files (for main application)
$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	$(RM) $(OBJDIR)
	$(RM) $(TARGET) $(TEST_TARGET)