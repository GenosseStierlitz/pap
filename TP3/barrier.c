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

    printf("- process %d goes to sleep.\n", my_rank);
    sleep(my_rank);
    printf("+ process %d wakes up.\n", my_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Process %d on machine %s finished executing.\n", my_rank, name);

    MPI_Finalize();
}
