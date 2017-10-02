CC := gcc
CFLAGS := -Wall -ansi -pedantic -std=c99

SRC_DIR := ./src
INC_DIR := ./include
OBJ_DIR := ./obj
TEST_DIR := ./test

INCLUDES := -I $(INC_DIR) -I C:/MinGW/include
SOURCES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(TEST_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET := test_containers

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo [COMPILE] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INCLUDES)

$(TARGET): $(OBJECTS)
	@echo [INFO] Creating Executable [$(TARGET)]
	@echo [LINK]
	@$(CC) -o $@ $^ $(INCLUDES)
	@./$(TARGET)
