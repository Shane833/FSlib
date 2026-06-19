ifeq ($(OS), Windows_NT)
	SHELL=powershell.exe
endif
CFLAGS=-Wall -Wextra -Wpedantic -ggdb -O2 -std=c23 -Isrc -DNDEBUG -DDEBUG_FILE
#CFLAGS=-Wall -Wextra -Wpedantic -ggdb -O2 -std=c23 -Isrc 
SANITIZEW=-fsanitize=undefined -fsanitize-trap
SANITIZEL=-fsanitize=address

SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,%.o, $(SRC))

TEST_SRC = $(wildcard tests/*.c)
TESTS = $(patsubst %.c,%, $(TEST_SRC))

windows: compilew testw

linux: compilel testl

compilew: CFLAGS+=$(SANITIZEW)
compilew: src/path.h src/file.h $(OBJ)

compilel: CFLAGS+=$(SANITIZEL)
compilel: src/path.h src/file.h $(OBJ)

testw: CFLAGS+=$(OBJ)
testw: $(TESTS)
	powershell -ExecutionPolicy Bypass -File ./run_tests.ps1

testl: CFLAGS+=$(OBJ)
testl: $(TESTS)
	./run_tests.sh

cleanw: 	
	-rm src/*.o 
	-rm tests/*.exe

cleanl:
	-rm $(OBJ)
	-rm $(TESTS)
