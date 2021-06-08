CFLAGS = -ansi -pedantic -Wall -Werror -o3 -D_DEFAULT_SOURCE
CFLAGS += -g
CC = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
LIBRARY = liba.a

$(LIBRARY): $(OBJECTS)
#	ar rcs $(LIBRARY) *.o
	echo "ok"

clean:
	rm -f *.o $(LIBRARY)

stats:
	wc -l *.[h,c]
