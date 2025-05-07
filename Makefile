# Makefile for AVR ATmega328P

# Compiler and Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = sudo avrdude

# Project specific
TARGET = main
SRC = $(TARGET).c

# MCU and CPU Frequency
MCU_GCC = atmega328p
F_CPU = 16000000

# Common Compiler Flags (applied to all builds)
COMMON_CFLAGS = -mmcu=$(MCU_GCC) -DF_CPU=$(F_CPU)UL -Wall -Wextra

# AVRDUDE Configuration
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = /dev/ttyUSB0 # Adjust if your port is different
AVRDUDE_BAUD = 115200
AVRDUDE_MCU = ATMEGA328P # avrdude often uses uppercase for MCU
AVRDUDE_UPLOAD_FLAGS = -V -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUD) -D -U flash:w:$(TARGET).hex

# --- Build and Upload Targets for Different Optimization Levels ---

# Default target: build and upload with -Os (optimize for size)
default: CFLAGS = $(COMMON_CFLAGS) -Os
default: all
	@echo ">>> Default: Uploading -Os (Optimize for Size) Version <<<"
	sleep 1
	$(AVRDUDE) $(AVRDUDE_UPLOAD_FLAGS)
	@echo ">>> Upload Complete <<<"

# Debug build: no optimization (-O0), include debug symbols (-g)
debug: CFLAGS = $(COMMON_CFLAGS) -O0 -g
debug: all
	@echo ">>> Debug: Uploading -O0 (No Optimization, Debug Symbols) Version <<<"
	sleep 1
	$(AVRDUDE) $(AVRDUDE_UPLOAD_FLAGS)
	@echo ">>> Upload Complete <<<"

# Release build with -Os (Optimize for size)
release-os: CFLAGS = $(COMMON_CFLAGS) -Os
release-os: all
	@echo ">>> Release-Os: Uploading -Os (Optimize for Size) Version <<<"
	sleep 1
	$(AVRDUDE) $(AVRDUDE_UPLOAD_FLAGS)
	@echo ">>> Upload Complete <<<"

# Release build with -O1 (Basic optimizations)
release-o1: CFLAGS = $(COMMON_CFLAGS) -O1
release-o1: all
	@echo ">>> Release-O1: Uploading -O1 (Basic Optimizations) Version <<<"
	sleep 1
	$(AVRDUDE) $(AVRDUDE_UPLOAD_FLAGS)
	@echo ">>> Upload Complete <<<"

# Release build with -O2 (More optimizations)
release-o2: CFLAGS = $(COMMON_CFLAGS) -O2
release-o2: all
	@echo ">>> Release-O2: Uploading -O2 (More Optimizations) Version <<<"
	sleep 1
	$(AVRDUDE) $(AVRDUDE_UPLOAD_FLAGS)
	@echo ">>> Upload Complete <<<"

# Release build with -O3 (Aggressive optimizations - may increase code size)
release-o3: CFLAGS = $(COMMON_CFLAGS) -O3
release-o3: all
	@echo ">>> Release-O3: Uploading -O3 (Aggressive Optimizations) Version <<<"
	sleep 1
	$(AVRDUDE) $(AVRDUDE_UPLOAD_FLAGS)
	@echo ">>> Upload Complete <<<"

# --- Generic Build Steps (called by the targets above) ---
all: $(TARGET).hex
	@echo "Build complete with CFLAGS: $(CFLAGS)"

# Rule to build .hex from .elf
$(TARGET).hex: $(TARGET).elf
	@echo "Creating HEX: $@"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Rule to build .elf from .o
$(TARGET).elf: $(TARGET).o
	@echo "Linking: $@"
	$(CC) -mmcu=$(MCU_GCC) $< -o $@

# Rule to build .o from .c
# This uses the CFLAGS variable, which is set differently by the specific targets.
$(TARGET).o: $(SRC)
	@echo "Compiling: $< with flags: $(CFLAGS)"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET).o $(TARGET).elf $(TARGET).hex
	@echo "Cleanup complete."

# Declare phony targets
.PHONY: default debug release-os release-o1 release-o2 release-o3 all clean