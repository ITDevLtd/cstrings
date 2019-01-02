.PHONY: all
all: test_file_io

.PHONY: clean
clean:
	@$(RM) *.o test_file_io

test_file_io: CFLAGS+=-fPIC
test_file_io: CXXFLAGS+=-fPIC
test_file_io: LDLIBS+=-lgtest -lgtest_main -lpthread
test_file_io: LDFLAGS+=-L/usr/lib/
test_file_io: test_file_io.o file_io.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@

file_io.o: file_io.h
