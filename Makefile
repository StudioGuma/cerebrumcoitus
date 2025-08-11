CC = gcc
PROGRAM_NAME = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(PROGRAM_NAME))

CC_FLAGS += -O2 -Wall -Werror -Wextra

all:
	$(CC) $(CC_FLAGS) $(PROGRAM_NAME) -o $(TARGET)

clean:
	rm $(TARGET)

