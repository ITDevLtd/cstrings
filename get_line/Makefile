CFLAGS+=-fPIC
CXXFLAGS+=-fPIC -std=c++11
LDLIBS+=-lgtest -lgtest_main -lpthread
LDFLAGS+=-L/usr/lib/

test_get_line: test_get_line.o get_line.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@

get_line.o: get_line.h

.PHONY: all
all: test_get_line

.PHONY: clean
clean:
	@$(RM) *.o test_get_line

