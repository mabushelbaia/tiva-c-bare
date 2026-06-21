# Firmware for the TM4C123GH6PM (EK-TM4C123GXL), built with arm-none-eabi-gcc
# against the TivaWare SDK and flashed with lm4flash.

TARGET := blinky

# TivaWare isn't vendored. Default to the standard TI install location; $(HOME)
# keeps it user-agnostic. Override on the command line or in the environment if
# yours is elsewhere or a different version.
TIVAWARE_PATH ?= $(HOME)/ti/TivaWare_C_Series-2.2.0.295

# Targets that don't touch the SDK skip the check below.
NO_SDK_GOALS := clean format format-check
ifneq ($(filter-out $(NO_SDK_GOALS),$(or $(MAKECMDGOALS),all)),)
ifeq ($(wildcard $(TIVAWARE_PATH)/driverlib/gcc/libdriver.a),)
$(error set TIVAWARE_PATH to your TivaWare install (got '$(TIVAWARE_PATH)'))
endif
endif

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

# device/startup_TM4C123.c is our CMSIS-style startup (vector table + reset
# handler) with weak, named handlers so the app overrides any vector just by
# defining e.g. TIMER1A_Handler. device/system_TM4C123.c provides SystemInit /
# SystemCoreClock (clock + FPU setup), called from the reset handler.
SOURCES := src/main.c device/startup_TM4C123.c device/system_TM4C123.c

BUILD   := build
OBJ_DIR := $(BUILD)/obj
BIN_DIR := $(BUILD)/bin

OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)
DEPS    := $(OBJECTS:.o=.d)

AXF := $(BIN_DIR)/$(TARGET).axf
BIN := $(BIN_DIR)/$(TARGET).bin
MAP := $(BIN_DIR)/$(TARGET).map

MCU_FLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
# PART_/TARGET_IS_ select the TivaWare driverlib variant; TM4C123GH6PM picks the
# device in the CMSIS dispatch header (device/TM4C123.h).
DEFINES   := -DPART_TM4C123GH6PM -DTARGET_IS_TM4C123_RB1 -DTM4C123GH6PM

# Two register-access styles are available, pick per source file (don't mix both
# in one .c): TivaWare driverlib (-I$(TIVAWARE_PATH), the *.h under inc/driverlib)
# and the CMSIS struct style (GPIOF->DATA, device/TM4C123GH6PM.h). The CMSIS
# device/core headers are vendored, so -isystem keeps their warnings out of our
# -Wall -Wextra -Werror build.
CFLAGS  := $(MCU_FLAGS) -std=gnu11 -Wall -Wextra -Werror \
           -ffunction-sections -fdata-sections -MMD -MP \
           -Isrc -I$(TIVAWARE_PATH) -isystem device -isystem device/cmsis_core \
           $(DEFINES) -Og -g
LDFLAGS := $(MCU_FLAGS) -nostartfiles -specs=nosys.specs -T linker.ld \
           -Wl,--gc-sections -Wl,-Map=$(MAP) -L$(TIVAWARE_PATH)/driverlib/gcc
LDLIBS  := -ldriver

# Our own C only - not the vendored printf or the device/ CMSIS + startup files.
FORMAT_FILES := $(shell find src -name '*.c' -o -name '*.h')

.PHONY: all flash size format format-check clean

all: $(BIN)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(AXF): $(OBJECTS) linker.ld
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@
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
