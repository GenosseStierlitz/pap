#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>

void print_array_int(int array[], int len){
    printf("[");
    for (int i = 0; i < len; i++){
        printf("%d; ", array[i]);
    }
    printf("]\n");
}

void matrix_copy(int m1[], int2[], int size){
    for(int i = 0; i < size*size; i++){
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

int main(int argc, char *argv[]){
    int my_rank;
    int my_torus_rank;
    int size;
    //int mess[10];
    char *mess = malloc(1);



    
    char name[256]; //machine's name
    gethostname(name, 256);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int nnodes = size;
    int ndims = 2;
    int dims[2] = {(int) sqrt(size), (int) sqrt(size)};
    int periods[2] = {1, 1};
    int reorder = 0;
    MPI_Comm *newcomm = create_grid(nnodes, ndims, dims, MPI_COMM_WORLD,periods, reorder);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_torus_rank);


    /*--------Data------------*/
    /*dimension of an individual block*/
    int n = 5; 

    /*blocks*/
    int A[n*n];

    /*filling the matrix*/
    for (int i = 0; i < n; i++){
        A[i] = i + my_rank;
        B[i] = i - my_rank;
    }

    /*Buffer for the qth diagonal of A*/
    int AD[n*n];
    
    //testing only
    int q = 1;
    if (coords[1] - coords[0] == q || coords[1] - coords[0] == n - q){
        matrix_copy(A, AD, n);
        MPI_Bcast(mess, 10, MPI_INT, 0, MPI_COMM_WORLD);
    }
    for (int i = 0; i < n; i++){




    /*
    if (my_torus_rank == 4){
        *mess = 'a';
        MPI_Send(mess, 1, MPI_CHAR, 0, 99, *newcomm); 
    }
    if (my_torus_rank == 0){
        MPI_Recv(mess, 1, MPI_CHAR, 4, 99, *newcomm, &status);
        printf("process %d received %c\n", my_torus_rank, *mess);
    }
    */

    int coords[ndims]; 
    MPI_Cart_coords(*newcomm, my_torus_rank, ndims, coords);
    printf("Process %d's coords: ", my_rank);
    print_array_int(coords, ndims);
    /*
    printf("- process %d goes to sleep.\n", my_rank);
    sleep(my_rank);
    printf("+ process %d wakes up.\n", my_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Process %d on machine %s finished executing.\n", my_rank, name);
    */
    MPI_Finalize();
}
