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

    if (my_rank == 0){
        for (int j = 0; j < 10; j++){
            mess[j] = 10 - j;
        }
    }
    MPI_Bcast(mess, 10, MPI_INT, 0, MPI_COMM_WORLD);
    if (my_rank != 0){
        printf("Process %d on machine %s received.", my_rank, name);
        print_array_int(mess, 10);
    }
    MPI_Finalize();
}
