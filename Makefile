CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g
TARGET  = myshell

all: $(TARGET)

$(TARGET): shell.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean