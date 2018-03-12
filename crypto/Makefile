all: sipslash

sipslash: sipslash.o
	gcc -o sipslash sipslash.o

sipslash.o: sipslash.c
	gcc -c -O0  sipslash.c -Wall -pedantic -std=c99

clean:
	rm -f sipslash.o sipslash *~
