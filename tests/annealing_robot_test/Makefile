OBJ_DIRS = ../../logging
OBJS = $(addsuffix /obj/*.o, $(OBJ_DIRS))
SRC = test_annealing_robot.cpp fake_actions.cpp simulated_annealing_on_pid.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../.. -g
LDFLAGS =

BUILD = bin
EXEC = $(BUILD)/test

ADDITIONAL = logs


all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

$(EXEC): $(OBJ_DIRS) $(SRC) $(BUILD) $(ADDITIONAL)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

$(BUILD):
	@mkdir -p $@

$(ADDITIONAL):
	@mkdir -p $@

clean-soft:
	@rm -rf $(BUILD)
	@rm -rf $(ADDITIONAL)

clean: clean-soft
	@for dir in $(OBJ_DIRS) ; do \
        make -C $$dir clean ; \
    done


ifeq ($(wildcard $(OBJ_DIRS)/obj/*.o),)
.PHONY: $(OBJ_DIRS)
endif
