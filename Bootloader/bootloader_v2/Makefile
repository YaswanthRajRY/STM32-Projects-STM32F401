CC=arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
MCU=cortex-m4
FLASH_ADDR = 0x08000000

INCLUDE_PATHS=  -I/mnt/c/Users/yaswa/Desktop/Embedded_System/STM32-Projects-STM32F401/Bootloader/bootloader_v2/driver/Core \
				-I/mnt/c/Users/yaswa/Desktop/Embedded_System/STM32-Projects-STM32F401/Bootloader/bootloader_v2/driver/Device

CFLAGS= -c -mcpu=$(MCU) -mthumb -std=gnu11 -Wall -O0 -ffreestanding -fno-builtin -g $(INCLUDE_PATHS)
LDFLAGS= -nostdlib -T STM32F401CCUX_FLASH.ld -Wl,-Map=boot.map -lgcc -g

all:boot.bin

boot.bin:boot.elf
	$(OBJCOPY) -O binary $< $@

bootloader.o: bootloader.c
	$(CC) $(CFLAGS) -o $@ $^

boot.elf: bootloader.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.map *.bin

flash:boot.bin
	st-flash write boot.bin $(FLASH_ADDR)

debug:
	gdb-multiarch boot.elf