PREFIX   = riscv-none-elf
CC       = /opt/xpack-riscv/bin/$(PREFIX)-gcc
OBJCOPY  = /opt/xpack-riscv/bin/$(PREFIX)-objcopy
MINICHLINK = /home/aayush/Projects/ch32fun/minichlink/minichlink
NEWLIB   = /opt/xpack-riscv/riscv-none-elf/include

TARGET   = firmware
SRCS     = startup.S main.c gpio.c uart.c

CFLAGS = -march=rv32ec -mabi=ilp32e -Os -Wall \
         -I$(NEWLIB) -nostdlib -ffreestanding

LDFLAGS = -T linker.ld -lgcc

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

flash: $(TARGET).bin
	$(MINICHLINK) -w $(TARGET).bin flash -b

clean:
	rm -f $(TARGET).elf $(TARGET).bin
