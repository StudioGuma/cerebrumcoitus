CC = gcc
PROGRAM_NAME = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(PROGRAM_NAME))

# DEBUG = true

CC_FLAGS = -Wall -Werror -Wextra
ifdef DEBUG
CC_FLAGS += -O0 -g
endif
ifndef DEBUG
CC_FLAGS += -O2
endif

all:
	$(CC) $(CC_FLAGS) $(PROGRAM_NAME) -o $(TARGET)

clean:
	rm $(TARGET)

