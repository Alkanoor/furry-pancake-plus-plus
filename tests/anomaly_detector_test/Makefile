OBJ_DIRS = ../../logging ../../anomaly_detector
OBJS = $(addsuffix /obj/*.o, $(OBJ_DIRS))
SRC = test_anomaly_detector.cpp

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

../../anomaly_detector:
	make -C $@ obj

../../logging:
	make -C $@ obj

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


ifeq ($(wildcard ../../anomaly_detector/obj/*.o),)
ifeq ($(wildcard ../../logging/obj/*.o),)
.PHONY: $(OBJ_DIRS)
endif
endif
