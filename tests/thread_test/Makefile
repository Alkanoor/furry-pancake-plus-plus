OBJ_DIRS = ../../logging ../../threads
OBJS = $(addsuffix /obj/*.o, $(OBJ_DIRS))
SRC = test_thread.cpp

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
	echo $(OBJ_DIRS)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

../../logging:
	make -C $@ obj

../../threads:
	make -C $@ obj

$(BUILD):
	@mkdir -p $@

$(ADDITIONAL):
	@mkdir -p $@

clean :
	@rm -rf $(BUILD)
	@rm -rf $(ADDITIONAL)
	@for dir in $(OBJ_DIRS) ; do \
        make -C $$dir clean ; \
    done


ifeq ($(wildcard ../../threads/obj/*.o),)
ifeq ($(wildcard ../../logging/obj/*.o),)
.PHONY: $(OBJ_DIRS)
endif
endif
