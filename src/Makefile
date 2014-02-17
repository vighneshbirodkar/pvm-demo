CC=gcc
CFLAGS=-c -Wall -O3 -g -llibbmp
SOURCES=demo.c bitmap.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=demo.out

all: $(EXECUTABLE) $(HEADERS)


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
