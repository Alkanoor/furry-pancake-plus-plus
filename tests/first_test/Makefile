SRC_DIRS = ../../logging
OBJS = $(addsuffix /obj/*.o, $(SRC_DIRS))
SRC = test_variadic_specialization.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I../.. -g
LDFLAGS = -lpthread

EXEC = test

.PHONY: $(SRC_DIRS)

$(EXEC): $(SRC_DIRS)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

$(SRC_DIRS):
	make -C $@ obj

run: $(EXEC)
	./$(EXEC)

clean :
	@rm -rf $(EXEC)
	@rm -rf logs
	make -C $(SRC_DIRS) clean
