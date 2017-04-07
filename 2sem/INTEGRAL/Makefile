CC              =  gcc
CFLAGS          = -Wall -g -O0 -std=c11
LIBS            = -lm -lpthread

SOURCES         = $(EXECUTABLE).c
OBJECTS         = $(SOURCES:.c=.o)
EXECUTABLE      = integral


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

-include $(wildcard *.d)

make_clean: all clean

clean:
	rm -f *.o *.d

clean_all:
	rm -f *.o *.d $(EXECUTABLE)



.PHONY: all clean