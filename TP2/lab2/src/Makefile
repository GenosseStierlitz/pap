all: triangular_matrix bubble 

triangular_matrix: triangular_matrix.o
	gcc -fopenmp -o triangular_matrix triangular_matrix.o

triangular_matrix.o: triangular_matrix.c
	gcc -c -O2 -fopenmp triangular_matrix.c 

bubble: bubble.o
	gcc -fopenmp -o bubble bubble.o

bubble.o: bubble.c
	gcc -c -O2 -fopenmp bubble.c 


clean:
	rm -f triangular_matrix triangular_matrix.o bubble bubble.o *~
