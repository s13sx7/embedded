TARGET    := blink
BUILD_DIR := build

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size
OBJDUMP := arm-none-eabi-objdump

CPU := -mcpu=cortex-m3 -mthumb

CFLAGS  := $(CPU) -std=c11 -Wall -Wextra -g -O0
CFLAGS  += -ffunction-sections -fdata-sections -ffreestanding

LDFLAGS := $(CPU) -T stm32f103c8.ld -nostdlib
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

OBJS := $(BUILD_DIR)/main.o $(BUILD_DIR)/startup.o

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/main.o: src/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/startup.o: src/startup.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --reset write $< 0x08000000

flash-ocd: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $< verify reset exit"

disasm: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -d $< | less

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean flash flash-ocd disasm
