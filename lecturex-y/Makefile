TARGET = target
# need to install cs50 library(see: https://github.com/cs50/libcs50?tab=readme-ov-file#from-source-linux-and-mac)
# https://github.com/cs50/help50-deprecated/issues/24#issuecomment-422015970
.PHONY: run
run:
	clang $(ARG).c -o ./$(TARGET)/$(ARG) -lcs50 \
	&& ./$(TARGET)/$(ARG)

.PHONY: lint
lint:
	cppcheck --quiet .

CC := gcc
OBJS := main.o fizzbuzz.o
CFLAGS := -W -Wall

TEST_CFLAGS := $(CFLAGS)
TEST_INCDIR := -I/opt/homebrew/Cellar/cunit/2.1-3/include
TEST_LIBDIR := -L/opt/homebrew/Cellar/cunit/2.1-3/lib

TEST_LIB := -lcunit

all: main test

main: $(OBJS)

.PHONY: test
test: test.c fizzbuzz.c
	$(CC) $^ $(TEST_CFLAGS) $(TEST_INCDIR) $(TEST_LIBDIR) $(TEST_LIB) -o $@ && ./$@

$(OBJS): fizzbuzz.h 

.PHONY: clean
clean:
	-rm *.o 
	-rm main test

.DEFAULT_GOAL=all
