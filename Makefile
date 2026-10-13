CC := gcc

CFLAGS := -std=c11 -Wall -Wextra -g -Iinclude -MMD -MP

SRC := src/main.c src/led_sim.c
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

TARGET := blink_sim

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

rebuild: clean all

.PHONY: all run clean rebuild

-include $(DEP)
