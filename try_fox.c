#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>

int get_matrix(int size,int rand1[size][size], int rand2[size][size]){

  //printf("Enter size \n" );
  //scanf("%d\n", &size );
  srand(time(NULL));
  for(int o = 0; o<size; o++){
    for(int i = 0; i<size; i++){
      rand1[o][i] = rand();
    }
  }
  //srand(time(NULL));
  for(int o = 0; o<size; o++){
      for(int i = 0; i<size; i++){
          rand2[o][i] = rand();
        }
      }
      printf("Print the randomly generated matrices\n");

      for (int row=0; row<size; row++)
      {

          for(int columns=0; columns<size; columns++)
              {
                rand1[row][columns]=rand1[row][columns]%10;
                printf("%d     ", rand1[row][columns]);
              }
          printf("\n");
       }

       printf("-----------------------------------------\n");

       for (int row=0; row<size; row++)
       {

           for(int columns=0; columns<size; columns++)
               {
                 rand2[row][columns]=rand2[row][columns]%10;
                 printf("%d     ", rand2[row][columns]);
               }
           printf("\n");
        }
}


int fox(int size,int A[size][size],int B[size][size],int C[size][size],int argc, char *argv[]){
  int my_rank=0;
  int r,c;
  //int *arr;
  int *buf_diag=(int*)malloc(size*sizeof(int));
  int A2[size][size];
  for (int row=0; row<size; row++)
  {
      for(int columns=0; columns<size; columns++)
          {
            A2[row][columns]=0;
            C[row][columns]=0;
          }
    }

  //using array of pointers
  int **arr = (int **)malloc(size * sizeof(int *));
  int x=0,y=0;
  for (int i=0; i<size; i++)
        arr[i] = (int *)malloc(size * sizeof(int));//stores diagonal of Array A
  //printf("SIZE %d",size);
  //printf("NEXT ROUNDS size=%d\n",size);

  for(int i=0;i<size;i++){
    int count=0;
    x=0;
    y=i;
    while(count<=size-1){
      arr[i][count]=A[x][y];
      //printf("%d %d",x,y);
      x=(x+1)%size;
      y=(y+1)%size;
      //printf("\n" );
      count++;
    }
  }
  int sz;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &sz);
      printf("sz=%d\n",sz );
for (int k = 0; k < size; k++) {
  for (int i = 0; i < size; i++) {
    buf_diag[i]=arr[k][i];
    printf("Buf[%d]=%d at k=%d \n",i,buf_diag[i],k);
  }
  MPI_Bcast(buf_diag, size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if(my_rank<=size-1){
    r=my_rank/size;
    c=my_rank%size;
    //C[r][c]=C[r][c]+A[r][c]*B[r][c];
    C[r][c]=C[r][c]+buf_diag[c]*B[r][c];
    printf("\nRow=%d Col=%d Val=%d Rank=%d\n",r,c,C[r][c],my_rank);
    //shift B
    int temp[size];
    for (int i = 0; i < size; i++) {
      temp[i]=B[0][i];
    }


    for(int i =1; i<=size - 1; i++) {
      for(int z = 0; z < size; z++) {
          B[i-1][z] = B[i][z];
        }
      }

      for (int i = 0; i < size; i++) {
        B[size-1][i]=temp[i];
      }
    //shift B ends here, working fine
  }

}


/*printf("THE DIAGONALS>>>>\n");
    for (int i = 0; i <  size; i++){
         for (int j = 0; j < size; j++){
            printf("%d ", arr[i][j]);
          }
    printf("\n" );
  }*/

  //MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
//  printf("Rank %d\n",my_rank );

free(*arr);
free(buf_diag);
MPI_Finalize();
return 0;

}




//processes= size*size
//diagonal= % size

int main(int argc, char *argv[]){
    int my_rank;
    int my_torus_rank;


    //accept size=2

    int i, o;

    int size= *argv[1]- 48;
    int rand1[size][size],rand2[size][size];
// the next is the block size for the diagonal
    int C[size][size];

    printf("SIZE NOW=%d \n",size );
    char name[256]; //machine's name

    get_matrix(size,rand1,rand2);

    gethostname(name, 256);
    //**let number of Processes be 16
    //**MPI_Status status;

    fox(size,rand1,rand2,C,argc,argv);

  //  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //**MPI_Comm_size(MPI_COMM_WORLD, &size);



     printf("THE RESULTS>>>>\n");
         for (int i = 0; i <  size; i++){
              for (int j = 0; j < size; j++){
                 printf("%d ", C[i][j]);
               }
         printf("\n" );
       }
     //printf(" MOD TESTING%d\n",(0-1)%size );
     return 0;
}
