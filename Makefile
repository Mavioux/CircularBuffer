CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -g
CXXFLAGS := -Wall -Wextra -g
LDFLAGS :=

ifeq ($(OS),Windows_NT)
    RM := del /Q /S
    MKDIR := mkdir
    MAIN_TARGET := main.exe
    PRODUCER_CONSUMER_TARGET := producer_consumer.exe
    TEST_TARGET := test_runner.exe
else
    RM := rm -rf
    MKDIR := mkdir -p
    MAIN_TARGET := main
    PRODUCER_CONSUMER_TARGET := producer_consumer
    TEST_TARGET := test_runner
endif

# Source and object directories
SRCDIR := src
OBJDIR := obj
TESTDIR := unittest

# Include directory
INCLUDES := -I$(SRCDIR)

# Source files and objects for main
MAIN_SRC := $(SRCDIR)/main.c $(SRCDIR)/circular_buffer.c
MAIN_OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/$(SRCDIR)/%.o, $(MAIN_SRC))

# Source files and objects for producer_consumer
PRODUCER_CONSUMER_SRC := $(SRCDIR)/producer_consumer.c $(SRCDIR)/circular_buffer.c
PRODUCER_CONSUMER_OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/$(SRCDIR)/%.o, $(PRODUCER_CONSUMER_SRC))

# Source files for tests
TEST_SRCS := $(SRCDIR)/$(TESTDIR)/circular_buffer_test.cpp
EXTRA_TEST_OBJS := $(OBJDIR)/$(SRCDIR)/circular_buffer.o

.PHONY: all clean test

# Default target
all: $(MAIN_TARGET) $(PRODUCER_CONSUMER_TARGET)

# Build the main program
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CC) $(MAIN_OBJS) -o $(MAIN_TARGET) $(LDFLAGS)

# Build the producer_consumer program
$(PRODUCER_CONSUMER_TARGET): $(PRODUCER_CONSUMER_OBJS)
	$(CC) $(PRODUCER_CONSUMER_OBJS) -o $(PRODUCER_CONSUMER_TARGET) $(LDFLAGS)

# Build the test executable
test: $(TEST_TARGET)

$(TEST_TARGET): $(EXTRA_TEST_OBJS)
	$(CXX) $(TEST_SRCS) $(EXTRA_TEST_OBJS) -o $(TEST_TARGET) -lgtest -lgtest_main -pthread $(INCLUDES)

# Compile source files into object files (generic rule)
$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	$(RM) $(OBJDIR)
	$(RM) $(MAIN_TARGET) $(PRODUCER_CONSUMER_TARGET) $(TEST_TARGET)
