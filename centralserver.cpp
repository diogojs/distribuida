#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>


#include "fakeprocess2.h"

int main(int argc, char** argv) {
    int size, rank;
    MPI_Status st;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Process %d started, out of %d processes.\n", rank, size);

    unsigned int token = 0;
    FakeProcess proc{rank, 0};

    // Server/Lider
    if (rank == 0) {
        // Notify all processes that server is on
        printf("Initializing server...\n");
        double t1 = MPI_Wtime();
        double t2 = MPI_Wtime();
        while (t2 < t1+3) {
            t2 = MPI_Wtime();
        }
        for (unsigned i = 1; i < size; ++i) {
            MPI_Send(&token, 1, MPI_UNSIGNED, i, TAG_SEND, MPI_COMM_WORLD);
        }
        bool running = true;
        token = 1;
        std::vector<int> queue{};
        unsigned msg = 0;
        while (running) {
            // Aguarda pedidos
            printf("Server listening...\n");
            MPI_Recv(&msg, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
            int enquirer = st.MPI_SOURCE;
            int tag = st.MPI_TAG;
            if (tag == TAG_ASK) {
                if (token && queue.size() == 0) {
                    printf("Server sending token to %d\n", enquirer);
                    MPI_Send(&token, 1, MPI_UNSIGNED, enquirer, TAG_SEND, MPI_COMM_WORLD);
                    token = 0;
                }
                else {
                    printf("Server put %d in queue.\n", enquirer);
                    queue.push_back(enquirer);
                }
            }
            if (tag == TAG_GIVEBACK) {
                printf("Server received token back from %d\n", enquirer);
                token = 1;
            }
            if (queue.size() > 0 && token) {
                printf("Server sending token to %d\n", queue.front());
                MPI_Send(&token, 1, MPI_UNSIGNED, queue.front(), TAG_SEND, MPI_COMM_WORLD);
                queue.erase(queue.begin());
                token = 0;
            }
            if (tag == TAG_EXIT) {
                printf("Server shutting down.\n");
                running = false;
            }
        }
    }

    // Other processes
    else {
        while (true) {
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