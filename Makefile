CFLAGS=-Wall -Wextra -Wpedantic -ggdb -O2 -std=c23 -Isrc -DNDEBUG #-DDEBUG_FILE

ifeq ($(OS), Windows_NT)
SHELL=powershell.exe
SANITIZE=-fsanitize=undefined -fsanitize-trap
else
SANITIZE=-fsanitize=address
endif

SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,%.o, $(SRC))

TEST_SRC = $(wildcard tests/*.c)
TESTS = $(patsubst %.c,%, $(TEST_SRC))

all: compile test

compile: CFLAGS+=$(SANITIZE)
compile: $(OBJ)

test: CFLAGS+=$(OBJ)
ifeq ($(OS), Windows_NT)
test: $(TESTS)
	powershell -ExecutionPolicy Bypass -File ./run_tests.ps1
else
test: $(TESTS)
	./run_tests.sh
endif

build:
	-mkdir build

ifeq ($(OS), Windows_NT)
clean:
	-powershell rm src/*.o
	-powershell rm tests/*.exe
else
clean:
	-rm $(OBJ)
	-rm $(TESTS)
endif
