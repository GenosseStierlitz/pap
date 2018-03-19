all: sipslash sipslash_break sipslash_32 sipslash_search twine_perm


twine_perm: twine_perm.o
	gcc -o twine_perm twine_perm.o

twine_perm.o: twine_perm.c
	gcc -c -O0  twine_perm.c -Wall -pedantic -std=c99


sipslash_search: sipslash_search.o
	gcc -o sipslash_search sipslash_search.o

sipslash_search.o: sipslash_search.c
	gcc -c -O0  sipslash_search.c -Wall -pedantic -std=c99

sipslash: sipslash.o
	gcc -o sipslash sipslash.o

sipslash.o: sipslash.c
	gcc -c -O0  sipslash.c -Wall -pedantic -std=c99

sipslash_32: sipslash_32.o
	gcc -o sipslash_32 sipslash_32.o

sipslash_32.o: sipslash_32.c
	gcc -c -O0  sipslash_32.c -Wall -pedantic -std=c99

sipshash_break: sipshash_break.o
	gcc -o sipshash_break sipshash_break.o

sipshash_break.o: sipshash_break.c
	gcc -c -O0  sipshash_break.c -Wall -pedantic -std=c99
clean:
	rm -f sipslash.o sipslash sipslash_break.o sipslash_break sipslash_32.o sipslash_32 sipslash_search sipslash_search.o twine_perm.o twine_perm *~
