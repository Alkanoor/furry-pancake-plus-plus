HELPER_DIRS = ../logging
OBJ_DIR = obj
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC))) $(wildcard $(SRC_DIR)/*.cpp)

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../logging/include


all: createDir $(HELPER_DIRS) $(OBJ)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(HELPER_DIRS):
	make -C $@

obj: createDir $(OBJ)

createDir:
	@mkdir -p $(OBJ_DIR)

clean :
	@rm -rf $(OBJ_DIR)


ifeq ($(wildcard $(HELPER_DIRS)/obj/*.o),)
.PHONY: $(HELPER_DIRS)
endif
