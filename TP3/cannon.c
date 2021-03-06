#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>

void print_array_int(int array[], int len){
    printf("[");
    int i;
    for (i = 0; i < len; i++){
        printf("%d; ", array[i]);
    }
    printf("]\n");
}


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

void matrix_copy(int m1[], int m2[], int size){
    int i;
    for(i = 0; i < size*size; i++){
        m2[i] = m1[i];
    }
}

MPI_Comm *create_grid(int nnodes, int ndims, int dims[], MPI_Comm comm_old, int periods[], int reorder){
    // ex: 12 2 {3, 4} MPI_COMM_WORLD {0, 0} false
    printf("Basic Succes\n");
    MPI_Dims_create(nnodes, ndims, dims);
    MPI_Comm *comm_cart = malloc(sizeof(MPI_Comm));
    if (MPI_Cart_create(comm_old, ndims, dims, periods, reorder, comm_cart) == MPI_SUCCESS){
        printf("Succes\n");
    }
    else {
        printf("Error MPI Cart\n");
    }
    return comm_cart; 

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

void matrix_sum(int *A, int *B, int size){
    /*Stores A+B in A*/
    int i, j;
    for(i = 0; i < size*size; i++){
        A[i] = A[i] + B[i];
    }
}

int main(int argc, char *argv[]){
    int my_rank;
    int my_torus_rank;
    int size;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int nnodes = size;
    int q = (int) sqrt(size); // number of lines in the matrix
    int ndims = 2;
    int dims[2] =  {q, q};
    int periods[2] = {1, 1};
    int reorder = 0;

    MPI_Comm *comm_cart = create_grid(nnodes, ndims, dims, MPI_COMM_WORLD,periods, reorder);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_torus_rank);

    /*--------Data------------*/

    /*dimension of an individual block*/
    int n = 3; 

    /*blocks*/
    int A[n*n];
    int B[n*n];
    int C[n*n];
    int CC[n*n];

    /*filling the matrix*/
    int i;
    for (i = 0; i < n*n; i++){
        A[i] = i + my_rank;
        B[i] = i - my_rank;
        C[i] = 0;
    }


    int coords[ndims]; 
    MPI_Cart_coords(*comm_cart, my_torus_rank, ndims, coords);

    // testing
    printf("Process %d's coords: ", my_rank);
    print_array_int(coords, ndims);
    //still for testing: reconstruct the big matrices at process 0 and print them
    if (my_rank == 0){
        int *MA = (int *) malloc(n*n*q*q*sizeof(int));
        int *MB = (int *) malloc(n*n*q*q*sizeof(int));
        int *MC = (int *) malloc(n*n*q*q*sizeof(int));
        int rank;
        int i, j;
        for (rank = 0; rank < size; rank++){
            int coords_rank[ndims]; 
            MPI_Cart_coords(*comm_cart, rank, ndims, coords_rank);
            int I = coords_rank[0];
            int J = coords_rank[1];
            for (i = 0; i < n; i++){
                for (j = 0; j < n; j++){
                    MA[(i+I*n)*n*q + j+J*n] = (i*n + j) + rank; // TODO: parametrize it by an arbitrary function of rank and i & j
                    MB[(i+I*n)*n*q + j+J*n] = (i*n + j) - rank;
                }
            }
        }
        printf("MA = \n");
        print_matrix(MA, n*q);
        printf("MB = \n");
        print_matrix(MB, n*q);
        matrix_product(MA, MB, MC, n*q);
        printf("MA*MB = \n");
        print_matrix(MC, n*q);
    }

    int src, dest;
    // Preskewing of A
    MPI_Cart_shift(*comm_cart, 1, coords[0], &src, &dest);
    MPI_Sendrecv_replace(A, n*n, MPI_INT, src, 99, dest, 99, *comm_cart, &status);

    // Preskewing of B
    MPI_Cart_shift(*comm_cart, 0, coords[1], &src, &dest);
    MPI_Sendrecv_replace(B, n*n, MPI_INT, src, 99, dest, 99, *comm_cart, &status);
     
    // iterations of Cannon algorithm 
    int k;
    for(k = 0; k < q; k++){
        //Perform local computations
        matrix_product(A, B, CC, n);
        matrix_sum(C, CC, n);

        // horizontal shift of A
        MPI_Cart_shift(*comm_cart, 1, 1, &src, &dest);
        MPI_Sendrecv_replace(A, n*n, MPI_INT, src, 99, dest, 99, *comm_cart, &status);

        // vertical shift of B
        MPI_Cart_shift(*comm_cart, 0, 1, &src, &dest);
        MPI_Sendrecv_replace(B, n*n, MPI_INT, src, 99, dest, 99, *comm_cart, &status);
    }
    
    // Postskewing of A
    MPI_Cart_shift(*comm_cart, 1, coords[0], &src, &dest);
    MPI_Sendrecv_replace(A, n*n, MPI_INT, dest, 99, src, 99, *comm_cart, &status);

    // Postskewing of B
    MPI_Cart_shift(*comm_cart, 0, coords[1], &src, &dest);
    MPI_Sendrecv_replace(A, n*n, MPI_INT, dest, 99, src, 99, *comm_cart, &status);

    printf("C at block %dx%d\n", coords[0], coords[1]);
    print_matrix(C, n);

    MPI_Finalize();
    
}
