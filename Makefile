WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 
VAL = valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all


TESTFALGS = 

SRCS = a8.c 
OBJS = $(SRCS:%.c=%.o)

a8: $(OBJS)
	$(GCC) $(TESTFALGS) $(OBJS) -o a8 -lm

.c.o:
	$(GCC) $(TESTFALGS) -c $*.c

testmemory: a8
	$(VAL) ./a8 graph.txt

#testing only input3 and output3

test1: a8
	./a8 graph.txt 
test2: a8
	./a8 graph43.txt 

clean: # remove all machine generated files
	rm -f a8 *.o output? *~