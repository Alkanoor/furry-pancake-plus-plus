TESTS = first_test


all:
	@for dir in $(TESTS) ; do \
        make -C  $$dir ; \
    done

clean:
	@for dir in $(TESTS) ; do \
        make -C  $$dir clean ; \
    done
