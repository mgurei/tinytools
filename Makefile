# Project name
PROJECT := tinytools

# Compiler and tools
CC := gcc
AR := ar
CFLAGS := -Wall -Wextra -std=c11 -I./include
ARFLAGS := rcs

# Output directories
BUILD_DIR := build
LIB_DIR := lib
TEST_DIR := $(BUILD_DIR)/tests

# Source files
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

# Test files
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(TEST_SRCS:tests/%.c=$(TEST_DIR)/%)

# Library name
LIB_NAME := lib$(PROJECT).a

# Platform selection
PLATFORM ?= linux  # Default platform

ifeq ($(PLATFORM),arduino)
    CC := avr-gcc
    CFLAGS += -mmcu=atmega4809  # R4 Minima uses ATmega4809
    CFLAGS += -DPLATFORM_ARDUINO
else ifeq ($(PLATFORM),raspberry)
    CFLAGS += -DPLATFORM_RASPBERRY
else
    CFLAGS += -DPLATFORM_LINUX
endif

# Debug build
ifdef DEBUG
    CFLAGS += -g -DDEBUG
endif

# Targets
.PHONY: all clean test

all: dirs $(LIB_DIR)/$(LIB_NAME)

dirs:
	@mkdir -p $(BUILD_DIR) $(LIB_DIR) $(TEST_DIR)

$(BUILD_DIR)/%.o: src/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIB_NAME): $(OBJS)
	@echo "Creating library $@..."
	@$(AR) $(ARFLAGS) $@ $^

# Test targets
test: dirs $(LIB_DIR)/$(LIB_NAME) $(TEST_BINS)
	@echo "Running tests..."
	@for test in $(TEST_BINS); do \
        echo "Running $${test#$(TEST_DIR)/}..."; \
        if $$test; then \
            echo "PASS: $${test#$(TEST_DIR)/}"; \
        else \
            echo "FAIL: $${test#$(TEST_DIR)/}"; \
            exit 1; \
        fi; \
    done

$(TEST_DIR)/%: tests/%.c $(LIB_DIR)/$(LIB_NAME)
	@echo "Compiling test $<..."
	@$(CC) $(CFLAGS) $< -L$(LIB_DIR) -l$(PROJECT) -o $@

clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) $(LIB_DIR)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the library (default)"
	@echo "  test     - Build and run tests"
	@echo "  clean    - Remove build artifacts"
	@echo "  help     - Show this help message"
	@echo "\nAvailable platforms:"
	@echo "  linux    - Build for Linux (default)"
	@echo "  raspberry- Build for Raspberry Pi"
	@echo "  arduino  - Build for Arduino"
	@echo "\nOptions:"
	@echo "  DEBUG=1  - Enable debug build"
