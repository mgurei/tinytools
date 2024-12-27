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

# Source files
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

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
.PHONY: all clean

all: dirs $(LIB_DIR)/$(LIB_NAME)

dirs:
	@mkdir -p $(BUILD_DIR) $(LIB_DIR)

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIB_NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
