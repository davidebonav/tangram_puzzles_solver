include project.conf

CC=gcc
CC_FLAGS=-c                               \
         -W                                   \
         -Wall                                \
         -pedantic                            \
		 -I $(shell pg_config --includedir)   \
		 -I ./include                         \
		 -L $(shell pg_config --libdir)
LN_FLAGS=-lpq

# sources
C_SRC=$(filter-out $(wildcard ./$(SRC_DIR)/_*.c), $(wildcard ./$(SRC_DIR)/*.c))
# includes
H_SRC=$(filter-out $(wildcard ./$(INCLUDE_DIR)/_*.h), $(wildcard ./$(INCLUDE_DIR)/*.h))
# binaries
OBJ=$(subst .c,.o,$(subst $(SRC_DIR),$(BIN_DIR),$(C_SRC)))

#
# Compilation and linking
#
all: binFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ $(LN_FLAGS) -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./$(BIN_DIR)/%.o: ./$(SRC_DIR)/%.c ./$(INCLUDE_DIR)/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $(CC_FLAGS) $< -o $@
	@ echo ' '

./$(BIN_DIR)/main.o: ./$(SRC_DIR)/main.c $(H_SRC)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $(CC_FLAGS) $< -o $@
	@ echo ' '

reset_db:
	@ ./script/create_user_and_db.sh
	@ echo "Reset successful"

binFolder:
	@ mkdir -p $(BIN_DIR)

clean:
	@ rm -rf ./$(BIN_DIR)/*.o $(PROJ_NAME) *~
	@ rm -rf $(BIN_DIR)

.PHONY: all clean reset_db