VAL = valgrind --leak-check=full --show-leak-kinds=all

CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic

a8: a8.c
	$(CC) $(CFLAGS) -o a8 a8.c

run: a8
	./a8 graph.txt queries.txt

testmemory: a8
	$(VAL) ./a8 graph.txt queries.txt

clean:
	rm -f a8 *.o memcheck.txt *~
