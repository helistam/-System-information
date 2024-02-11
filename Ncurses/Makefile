SRC_DIR =.
OBJ_DIR = .
BIN_DIR = .
INC_DIR =.

EXE = $(BIN_DIR)/main
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC			= gcc
CPPFLAGS	= -I$(INC_DIR)
CFLAGS		= -W -Wall -Wno-unused-parameter -Wno-unused-variable -pedantic
LDFLAGS		=
LDLIBS 		=-lncurses -lX11 -pthread

.PHONY: build clean

build: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@
clean:
	$(RM) $(EXE) $(OBJ)

