#ifndef FAKE_PROCESS_H
#define FAKE_PROCESS_H

#include <iostream>
#include <stdlib.h>
#include <mpi.h>

#include "globals.h"

class FakeProcess {
public:
    FakeProcess(int rank, bool token, double rate = 0.2):
        _rank{rank},
        _token{token},
        _rate{rate}
    {
        srand(0);
    }

    bool useResource() {
        if (_token) {
            std::cout << "Process " << _rank << " used the resource." << std::endl;
            MPI_Send(&token, 1, MPI_UNSIGNED, 0, TAG_GIVEBACK, MPI_COMM_WORLD);
        } else {
            std::cout << "Process " << _rank << " waiting for token..." << std::endl;
            MPI_Recv(&token, 1, MPI_UNSIGNED, 0, TAG_SEND, MPI_COMM_WORLD, &st);
        }
        return true;
    }

    bool doOtherStuff() {
        std::cout << "Process " << _rank << " doing other stuff." << std::endl;
        return false;
    }

    bool update() {
        double chance = (double)rand()/(double)(RAND_MAX);
        if (chance < _rate) { // Wants to access resource
            return useResource();
        } else {
            return doOtherStuff();
        }
    }

    bool token() {
        return _token;
    }

    void token(bool t) {
        _token = t;
    }

private:
    int _rank;
    bool _token;
    double _rate;
};

#endif
