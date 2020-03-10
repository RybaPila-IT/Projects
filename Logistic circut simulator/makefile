CC=gcc
CFlags=-pedantic -Wall -Wextra -O2
manual_src=manual.h manual.c
input_src=input.h input.c
logicboard_src=logicboard.h logicboard.c

all: main.o input.o logicboard.o manual.o
	$(CC) $^ -o main

main.o: $(manual_src) $(input_src) $(logicboard_src) main.c
	$(CC) -c $(CFlags) main.c

input.o: $(input_src)
	$(CC) -c $(CFlags) input.c

manual.o: $(manual_src)
	$(CC) -c $(CFlags) manual.c

logicboard.o: $(logicboard_src)
	$(CC) -c $(CFlags) logicboard.c

clean:
	rm -f *.o main
