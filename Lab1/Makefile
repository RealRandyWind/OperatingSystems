CXX = gcc
LOG = logs.cmpl
COMPILE_FLAGS = -std=c99 -pedantic -Wall -Wextra -g

.PHONY: all clean

all: duplicates execute ring

duplicates: duplicates.c
	$(CXX) -o duplicates duplicates.c $(COMPILE_FLAGS) 2>> duplicates.$(LOG)

execute: execute.c
	$(CXX) -o execute execute.c $(COMPILE_FLAGS) 2>> execute.$(LOG)

ring: ring.c
	$(CXX) -o ring ring.c $(COMPILE_FLAGS) 2>> ring.$(LOG)

clean:
	rm -f ./ring ./execute ./duplicates ./ring.$(LOG) ./execute.$(LOG) ./duplicates.$(LOG)