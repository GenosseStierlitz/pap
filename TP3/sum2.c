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

    int send_data = my_rank;
    int recv_data;

    MPI_Allreduce(&send_data, &recv_data, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("Process %d on machine %s received %d.\n", my_rank, name, recv_data);

    MPI_Finalize();
}
