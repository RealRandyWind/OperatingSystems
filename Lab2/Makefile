CXX = gcc
LOG = logs.cmpl
_CMPL = _cmpl.rm
COMPILE_FLAGS = -std=c99 -pedantic -Wall -Wextra -g
APPS = $(patsubst %.c,%,$(wildcard ./*.c))

all: $(APPS)

%: %.c
	@date > $@.$(LOG)
	@($(CXX) -o $@ $< $(COMPILE_FLAGS) 2>> $@.$(LOG) && echo $@": succeded") || echo $@": failed"

clean:
	rm -f *.$(LOG) $(APPS)