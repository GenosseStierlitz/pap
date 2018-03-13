#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int my_rank;
    int size;
    char name[256];

    char *token;
    double big_message[666];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    gethostname(name, 256);
    /*
    if (my_rank % 2 == 0){
        printf("My rank is  better, it's %d\n", my_rank);
        printf("\tI run on %s\n", name);
    }
    else {
        printf("My rank is %d\n", my_rank);
        printf("\tI run on %s\n", name);
    }
    printf ("Total number of processes is %d\n", size);
    */

    if (my_rank == 0){
        *token = 0;
        MPI_Send(token, 1, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
        MPI_Recv(token, 1, MPI_CHAR, size - 1, 99, MPI_COMM_WORLD, &status);
        printf("Received token: %d\n", *token);
    }

    else {
        MPI_Recv(token, 1, MPI_CHAR, (my_rank + size - 1) % size, 99, MPI_COMM_WORLD, &status);
        *token = *token + my_rank;
        MPI_Send(token, 1, MPI_CHAR, (my_rank + 1) % size, 99, MPI_COMM_WORLD);
        //*token = *token;
    } 


    /*
    if (my_rank == i) {
        strcpy(msg, "Hello !");
        MPI_Send(msg, strlen(msg), MPI_CHAR, 1, 99, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(msg, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        printf("I received %s!\n", msg);
    }
    */
    MPI_Finalize();
}
