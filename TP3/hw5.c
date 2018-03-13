#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    double mess[10];
    int my_rank;
    int size;
    char name[256];

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
        for (int i = 0; i < 10; i++){
            mess[i] = (double) (10-i);
        }
        //printf("sod : %d\n\n", sizeof(double));    
        double start = MPI_Wtime();
        MPI_Send(mess, 10, MPI_DOUBLE, 1, 99, MPI_COMM_WORLD);
        MPI_Recv(mess, 10, MPI_DOUBLE, 1, 99, MPI_COMM_WORLD, &status);
        
        printf("Printing received message: \n\t");
        for (int i = 0; i<10; i++){
            printf("%F ", mess[i]); 
        }
        printf("\nRTT: %F s\n", MPI_Wtime() - start);
        
    }

    if (my_rank == 1){
        MPI_Recv(mess, 10, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &status);
        for (int i = 0; i<10; i++){
            mess[i] = mess[i]*2.0;
            //printf("%F ", mess[i]); 
        }
        MPI_Send(mess, 10, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
        //printf("\n");
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
