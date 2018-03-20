#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>

void print_array_int(int array[], int len){
    printf("[");
    for (int i = 0; i < len; i++){
        printf("%d; ", array[i]);
    }
    printf("]\n");
}

MPI_Comm *create_grid(int nnodes, int ndims, int dims[], MPI_Comm comm_old, int periods[], int reorder){
    // ex: 12 2 {3, 4} MPI_COMM_WORLD {0, 0} false
    MPI_Dims_create(nnodes, ndims, dims);
    MPI_Comm *comm_cart;
    MPI_Cart_create(comm_old, ndims, dims, periods, reorder, comm_cart);
    return comm_cart; 

}

int main(int argc, char *argv[]){
    int my_rank;
    int size;
    int mess[10];
    
    char name[256]; //machine's name
    gethostname(name, 256);

    //MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    int nnodes = 12;
    int ndims = 2;
    int dims[2] = {3, 4};
    int periods[2] = {0, 0};
    int reorder = 0;

    MPI_Comm *newcomm = MPI_COMM_WORLD(nnodes, ndims, dims, MPI_COMM_WORLD,periods, reorder);
    int coords[ndims]; 
    MPI_Cart_coords(&newcomm, my_rank, ndims, coords);
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
