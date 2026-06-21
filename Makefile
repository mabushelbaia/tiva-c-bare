# Firmware for the TM4C123GH6PM (EK-TM4C123GXL), built bare-metal with
# arm-none-eabi-gcc and flashed with lm4flash. No vendor SDK to install: register
# access goes through the vendored CMSIS device headers (device/, struct style
# like GPIOF->DATA), so there is nothing external to point the build at.

TARGET := blinky

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

# device/startup_TM4C123.c is our CMSIS-style startup (vector table + reset
# handler) with weak, named handlers, so the app overrides any vector just by
# defining e.g. TIMER1A_Handler (see src/main.c). device/system_TM4C123.c
# provides SystemInit / SystemCoreClock (clock + FPU setup), called from the
# reset handler. external/printf is mpaland's tiny printf (a submodule), tuned by
# external/printf_config.h; --gc-sections drops it until something calls it (then
# provide a _putchar).
SOURCES := src/main.c \
           device/startup_TM4C123.c \
           device/system_TM4C123.c \
           external/printf/printf.c

BUILD   := build
OBJ_DIR := $(BUILD)/obj
BIN_DIR := $(BUILD)/bin

OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)
DEPS    := $(OBJECTS:.o=.d)

AXF := $(BIN_DIR)/$(TARGET).axf
BIN := $(BIN_DIR)/$(TARGET).bin
MAP := $(BIN_DIR)/$(TARGET).map

MCU_FLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
# TM4C123GH6PM selects the device in the CMSIS dispatch header (device/TM4C123.h).
# PRINTF_INCLUDE_CONFIG_H makes external/printf pick up external/printf_config.h.
DEFINES   := -DTM4C123GH6PM -DPRINTF_INCLUDE_CONFIG_H

# Registers come from the vendored CMSIS device headers (struct style:
# GPIOF->DATA, SYSCTL->RCGCGPIO via #include "TM4C123.h"), which also bring the
# Cortex-M intrinsics (NVIC_EnableIRQ(), __NOP(), ...). The device/core and
# printf headers are third-party, so -isystem keeps their warnings out of our
# -Wall -Wextra -Werror build.
CFLAGS  := $(MCU_FLAGS) -std=gnu11 -Wall -Wextra -Werror \
           -ffunction-sections -fdata-sections -MMD -MP \
           -Isrc -Iexternal \
           -isystem device -isystem device/cmsis_core -isystem external/printf \
           $(DEFINES) -Og -g
LDFLAGS := $(MCU_FLAGS) -nostartfiles -specs=nosys.specs -T linker.ld \
           -Wl,--gc-sections -Wl,-Map=$(MAP)

# Our own C only - not the vendored printf or the device/ CMSIS + startup files.
FORMAT_FILES := $(shell find src -name '*.c' -o -name '*.h')

.PHONY: all flash size format format-check clean

all: $(BIN)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(AXF): $(OBJECTS) linker.ld
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	@$(SIZE) $@

$(BIN): $(AXF)
	$(OBJCOPY) -O binary $< $@

flash: $(BIN)
	lm4flash $(BIN)

size: $(AXF)
	@$(SIZE) $(AXF)

format:
	@clang-format -i $(FORMAT_FILES)

format-check:
	@clang-format --dry-run --Werror $(FORMAT_FILES)

clean:
	@rm -rf $(BUILD)

-include $(DEPS)
