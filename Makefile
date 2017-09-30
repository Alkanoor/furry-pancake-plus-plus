OBJ_DIR_TEST = obj/tests
SRC_DIR_TEST = tests
SRC_TEST = $(wildcard $(SRC_DIR_TEST)/*.cpp)
OBJ_TEST := $(patsubst %.cpp, $(OBJ_DIR_TEST)/%.o, $(notdir $(SRC_TEST)))

VPATH = .
VPATH = tests

RELEASE_DIR = bin
EXEC = $(RELEASE_DIR)/test

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -fPIC
LDFLAGS = -lpthread -fPIC
LDFLAGS_LIB = -lpthread -fPIC -shared


all: createDir $(EXEC)

$(EXEC): $(OBJ_TEST)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR_TEST)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

test: all
	./$(EXEC)

createDir:
	@mkdir -p $(OBJ_DIR_TEST)
	@mkdir -p $(RELEASE_DIR)

clean :
	@rm -rf $(OBJ_DIR_TEST)
	@rm -rf $(RELEASE_DIR)
