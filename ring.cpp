#include <mpi.h>
#include <stdio.h>

#include "fakeprocess.h"

int main(int argc, char** argv) {
    int size, rank;
    unsigned int duration = 10;
    MPI_Status st;

    if(argc == 2){
        duration = atoi(argv[1]);
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Process %d started, out of %d processes.\n", rank, size);

    unsigned int token = 0;
    FakeProcess proc{rank, 0};

    double t1 = MPI_Wtime();
    double t2 = MPI_Wtime();
    while (t2 < t1+1) {
        t2 = MPI_Wtime();
    }

    if (rank == 0) {
        token = 1;
        proc.token(token);
        printf("Process %d sending token to %d.\n", rank, (rank+1));
        MPI_Send(&token, 1, MPI_UNSIGNED, (rank+1)%size, 0, MPI_COMM_WORLD);
    }

    for (int i = 1; i < duration; i++) {
        int r = ((rank-1)<0)?(size-1):(rank-1);
        MPI_Recv(&token, 1, MPI_UNSIGNED, r, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
        proc.token(token);
        proc.update();

        if (token) {
            printf("Process %d sending token to %d.\n", rank, (rank+1)%size);
            MPI_Send(&token, 1, MPI_UNSIGNED, (rank+1)%size, 0, MPI_COMM_WORLD);
            proc.token(0);
        }        
    }

    MPI_Finalize();
    return 0;
}