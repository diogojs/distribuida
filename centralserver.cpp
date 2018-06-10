#include <mpi.h>
#include <stdio.h>
#include <vector>

#include "fakeprocess2.h"

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

    // Server/Lider
    if (rank == 0) {
        bool running = true;
        token = 1;
        std::vector<int> queue{};
        while (running) {
            // Aguarda pedidos
            MPI_Recv(&token, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
            int enquirer = st.MPI_SOURCE;
            int tag = st.MPI_TAG;
            if (tag == TAG_ASK) {
                if (token) {
                    MPI_Send(&token, 1, MPI_UNSIGNED, enquirer, TAG_SEND, MPI_COMM_WORLD);
                    token = 0;
                }
                else {
                    queue.push_back(enquirer);
                }
            }
            if (tag == TAG_GIVEBACK) {
                queue.erase(queue.begin());
                token = 1;
            }
            if (queue.size() > 0 && token) {
                MPI_Send(&token, 1, MPI_UNSIGNED, queue.front(), TAG_SEND, MPI_COMM_WORLD);
                queue.erase(queue.begin());
                token = 0;
            }
            if (tag == TAG_EXIT)
                running = false;
        }
    }

    // Other processes
    else {
        for (int i = 1; i < duration; i++) {
            proc.update();
        }
    }

    // Last process tells lider to exit
    if (rank == size-1) {
        MPI_Send(&token, 1, MPI_UNSIGNED, 0, TAG_EXIT, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}