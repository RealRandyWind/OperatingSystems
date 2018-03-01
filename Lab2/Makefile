NAME = iwish
CXX ?= g++
COMPILE_FLAGS = -std=c99 -pedantic -Wall -Wextra -g -DDEBUG
TESTS := $(patsubst %.in,%.out,$(wildcard tests/*.in))

.PHONY: all clean-all

all: main-build

main-build:
	$(CXX) -o $(NAME) $(NAME).c $(COMPILE_FLAGS)

test: build clean-test ./a.out $(TESTS)
	echo $(TESTS)
	cat tests.log

./tests/%.out: ./tests/%.in
	./a.out < $< > $@ || true
	(diff $@ $@.expected > $@.diff && echo $*: passed >> tests.log) || echo $*: faild >> tests.log

clean:
	rm -f ./$(NAME)

clean-test:
	rm -f ./tests/*.out ./tests/*.diff ./tests.log

clean-all: clean clean-test