CC = gcc
PROGRAM_NAME = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(PROGRAM_NAME))

CC_FLAGS += -Os -Wall -Werror -Wextra -pedantic-errors

all:
	$(CC) $(CC_FLAGS) $(PROGRAM_NAME) -o $(TARGET)

clean:
	rm -f $(TARGET)

