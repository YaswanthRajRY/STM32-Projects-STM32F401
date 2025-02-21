# target
TARGET = main

# start address for flashing
FLASH_ADDR = 0x08008000
# debug build
DEBUG = 1

# optimization
OPT = -Og

#######################################
# paths^M
#######################################
# build path
BUILD_DIR = build

######################################
# source^M
######################################
# C sources
C_SOURCES = $(wildcard src/*.c)

# ASM sources
ASM_SOURCES = startup/startup_stm32f401ccux.s

#######################################
# binaries^M
#######################################
PREFIX = arm-none-eabi-
GCC_PATH = /usr/bin

CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =	-DSTM32F401xC

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =	-Idriver/Core/ \
				-Idriver/Device/ \
				-Iinc

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = linker/STM32F401CCUX_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

.PHONY: all

#######################################
# flash stm32
#######################################
# Flashing command
FLASH_CMD = st-flash write $(BUILD_DIR)/$(TARGET).bin $(FLASH_ADDR)

# Make flash target
flash: $(BUILD_DIR)/$(TARGET).bin
	$(FLASH_CMD)

.PHONY: flash

#######################################
# debug
#######################################
# Debugging command
DEBUG_CMD = arm-none-eabi-gdb $(BUILD_DIR)/$(TARGET).elf

# Make debug target
debug: $(BUILD_DIR)/$(TARGET).elf
	$(DEBUG_CMD)

.PHONY: debug

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

.PHONY: clean

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
