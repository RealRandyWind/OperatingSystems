NAME = iwish
CXX = gcc
LOG = $(Name).logs.cmpl
COMPILE_FLAGS = -std=c99 -pedantic -Wall -Wextra -g -DDEBUG
TESTS := $(patsubst %.in,%.out,$(wildcard tests/*.in))

.PHONY: all clean-all

all: main-build

main-build:
	$(CXX) -o $(NAME) $(NAME).c $(COMPILE_FLAGS) 2>> $(LOG)

test: build clean-test ./a.out $(TESTS)
	echo $(TESTS)
	cat tests.log

./tests/%.out: ./tests/%.in
	./a.out < $< > $@ || true
	(diff $@ $@.expected > $@.diff && echo $*: passed >> tests.log) || echo $*: faild >> tests.log

clean:
	rm -f ./$(NAME) $(NAME).logs.cmpl

clean-test:
	rm -f ./tests/*.out ./tests/*.diff ./tests.log

clean-all: clean clean-test