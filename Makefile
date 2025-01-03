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

# Platform directories
PLATFORM_DIR := src/platform
PLATFORM_LINUX_DIR := $(PLATFORM_DIR)/linux
PLATFORM_ARDUINO_DIR := $(PLATFORM_DIR)/arduino
PLATFORM_FREERTOS_DIR := $(PLATFORM_DIR)/freertos

# Source files
SRCS := $(wildcard src/*.c)
PLATFORM_SRCS := $(wildcard $(PLATFORM_DIR)/*.c)

# Platform specific sources
ifeq ($(PLATFORM),arduino)
    PLATFORM_IMPL_SRCS := $(wildcard $(PLATFORM_ARDUINO_DIR)/*.c)
else ifeq ($(PLATFORM),raspberry)
    PLATFORM_IMPL_SRCS := $(wildcard $(PLATFORM_LINUX_DIR)/*.c)  # Using Linux implementation for RPi
else
    PLATFORM_IMPL_SRCS := $(wildcard $(PLATFORM_LINUX_DIR)/*.c)
endif

# Combine all sources
ALL_SRCS := $(SRCS) $(PLATFORM_SRCS) $(PLATFORM_IMPL_SRCS)
OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o) \
        $(PLATFORM_SRCS:$(PLATFORM_DIR)/%.c=$(BUILD_DIR)/platform/%.o) \
        $(PLATFORM_IMPL_SRCS:$(PLATFORM_DIR)/%.c=$(BUILD_DIR)/platform/%.o)

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
    CFLAGS += -DTT_TARGET_ARDUINO
else ifeq ($(PLATFORM),raspberry)
    CFLAGS += -DTT_TARGET_RASPBERRY
else
    CFLAGS += -DTT_TARGET_LINUX -DTT_CAP_MUTEX -DTT_CAP_THREADS
endif

# Debug build
ifdef DEBUG
    CFLAGS += -g -DDEBUG
endif

# Test flags
TEST_CFLAGS := $(CFLAGS) -I./tests

# Targets
.PHONY: all clean test

all: dirs $(LIB_DIR)/$(LIB_NAME)

dirs:
	@mkdir -p $(BUILD_DIR) $(LIB_DIR) $(TEST_DIR) \
        $(BUILD_DIR)/platform/linux \
        $(BUILD_DIR)/platform/arduino \
        $(BUILD_DIR)/platform/freertos

$(BUILD_DIR)/%.o: src/%.c
	@echo "Compiling $< with flags: $(CFLAGS)..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for platform/*.c files
$(BUILD_DIR)/platform/%.o: $(PLATFORM_DIR)/%.c
	@echo "Compiling platform file $< with flags: $(CFLAGS)..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for platform-specific implementation files
$(BUILD_DIR)/platform/%.o: $(PLATFORM_LINUX_DIR)/%.c
	@echo "Compiling Linux platform file $< with flags: $(CFLAGS)..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/platform/%.o: $(PLATFORM_ARDUINO_DIR)/%.c
	@echo "Compiling Arduino platform file $< with flags: $(CFLAGS)..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/platform/%.o: $(PLATFORM_FREERTOS_DIR)/%.c
	@echo "Compiling FreeRTOS platform file $< with flags: $(CFLAGS)..."
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
	@$(CC) $(TEST_CFLAGS) $< -L$(LIB_DIR) -l$(PROJECT) -o $@

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
