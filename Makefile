# Имена
TARGET := firmware
BUILD_DIR := build

# Инструменты
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

# Флаги компиляции
CFLAGS := -mcpu=cortex-m3 -mthumb -std=c11
CFLAGS += -Wall -Wextra -g -O0
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding
CFLAGS += -Iinclude  # ← ВАЖНО: путь к заголовочным файлам

# Флаги линковки
LDFLAGS := -mcpu=cortex-m3 -mthumb
LDFLAGS += -T stm32f103c8.ld -nostdlib
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# Объектные файлы
OBJS := $(BUILD_DIR)/main.o $(BUILD_DIR)/startup.o $(BUILD_DIR)/clock.o

# Цель по умолчанию
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

# Компиляция C файлов
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция ассемблера
$(BUILD_DIR)/startup.o: src/startup.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Создание бинарника
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Создание директории
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Прошивка через st-flash
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x8000000

# Прошивка через OpenOCD
flash-ocd: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $< verify reset exit"

# Очистка
clean:
	rm -rf $(BUILD_DIR)

# Дизассемблер
disasm: $(BUILD_DIR)/$(TARGET).elf
	arm-none-eabi-objdump -d $< > $(BUILD_DIR)/$(TARGET).dis
	@echo "Disassembly saved to $(BUILD_DIR)/$(TARGET).dis"

# Показать карту памяти
map: $(BUILD_DIR)/$(TARGET).map
	@echo "=== Memory Map ==="
	@cat $< | grep -E "^\.(text|data|bss|rodata)"

.PHONY: all clean flash flash-ocd disasm map
