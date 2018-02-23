NAME ?= ex5
TEST_IN := 
CXX ?= g++
COMPILE_FLAGS = -std=c99 -pedantic -Wall -Wextra -g

.PHONY: all clean

all: main-build

main-build:
	$(CXX) -o $(NAME) $(NAME).c $(COMPILE_FLAGS)

clean:
	rm -f ./$(NAME).c ./$(NAME)