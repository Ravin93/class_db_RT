# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC_DIR = src
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/repl.c $(SRC_DIR)/btree.c
OBJ = $(SRC:.c=.o)
TARGET = db_program

# Règles
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
