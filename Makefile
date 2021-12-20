CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: tsp

tsp: tsp.o
	$(CC) -lm -o tsp  *.o

tsp.o:  *.c *.h
	$(CC) $(CFLAGS) -c *.c

format:
	clang-format -i -style=file *.[ch]

debug:
	$(CC) $(CFLAGS) -g *.c
clean:
	rm -f tsp *.o

