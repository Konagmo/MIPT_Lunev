CC = gcc
CFLAGS = -g -Wall -o0 -Werror
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

SOURCES = list.c $(EXECUTABLE).c
OBJECTS = $(SOURCES:.c = .o)
EXECUTABLE = test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $@ $^

%.o:%.c
	$(CC) -c -MD $(CFLAGS) $(GCOV_FLAGS) -o $@ $<

-include $(wildcard *.d)

valgrind:
	valgrind ./$(EXECUTABLE)

gcov:
	gcov list.gcno

clean:
	rm -f *.o *.d 