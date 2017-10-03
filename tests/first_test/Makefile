OBJ_DIRS = ../../logging
OBJS = $(addsuffix /obj/*.o, $(OBJ_DIRS))
SRC = test_variadic_specialization.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../.. -g
LDFLAGS = -lpthread

BUILD = bin
EXEC = $(BUILD)/test
ADDITIONAL = logs


all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

$(EXEC): $(OBJ_DIRS) $(SRC) $(BUILD) $(ADDITIONAL)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

$(OBJ_DIRS):
	make -C $@ obj

$(BUILD):
	@mkdir -p $@

$(ADDITIONAL):
	@mkdir -p $@

clean :
	@rm -rf $(BUILD)
	@rm -rf $(ADDITIONAL)
	make -C $(OBJ_DIRS) clean


ifeq ($(wildcard $(OBJ_DIRS)/obj/*.o),)
.PHONY: $(OBJ_DIRS)
endif
