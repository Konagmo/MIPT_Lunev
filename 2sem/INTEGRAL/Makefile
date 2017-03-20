CC = gcc
CFLAGS = -g -Wall -Werror -pthread

SOURCES = integral.c $(EXECUTABLE).c
OBJECTS = $(SOURCES:.c = .o)
EXECUTABLE = integral

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o:%.c
	$(CC) -c -MD $(CFLAGS) $(GCOV_FLAGS) -o $@ $<

-include $(wildcard *.d)

valgrind:
	valgrind ./$(EXECUTABLE)

clean:
	rm -f *.o *.d main.out