#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>


void print_matrix(int *m, int size){
    int i,j;
    for(i = 0; i < size; i++){
        printf("[");
        for(j = 0; j < size; j++){
            printf("%d ", m[i*size + j]);
        }
        printf("]\n");
    }
    printf("\n");
}

void matrix_product(int * A, int * B, int * C, int size){
    /*Store A*B in C*/
    int i, j ,k;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            C[i*size + j] = 0;
            for(k = 0; k < size; k++){
                C[i*size + j] = C[i*size + j] + A[i*size + k]*B[k*size + j];
            }
        }
    }
}

int main(int argc, char *argv[]){
    int n; 
    int size = 25;
    int q = 5;
    int MA[n*n*q*q];
    int MB[n*n*q*q];
    int MC[n*n*q*q];
    int rank;
    int i, j;

    int t;
    for (rank = 0; rank < size; rank++){
        int I = rank / q; 
        int J = rank % q; 
        for (i = 0; i < n; i++){
            for (j = 0; j < n; j++){
                MA[(i+I*n)*n*q + j+J*n] = (i*n + j) + rank; // TODO: parametrize it by an arbitrary function of rank and i & j
                MB[(i+I*n)*n*q + j+J*n] = (i*n + j) - rank;
            }
        }
    }
    t = time(0);
    matrix_product(MA, MB, MC, n*q);
    t = time(0) - t;
    printf("%f\n", t);
}
