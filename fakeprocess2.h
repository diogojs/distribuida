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
        // Wait confirmation that Server is on
        if (_rank != 0) {
            std::cout << "P" << _rank << ": waiting for server startup." << std::endl;
            MPI_Recv(&_token, 1, MPI_UNSIGNED, 0, TAG_SEND, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "P" << _rank << ": working." << std::endl;
        }
    }

    bool useResource() {
        if (_token) {
            std::cout << "P" << _rank << ": used the resource." << std::endl;
            MPI_Send(&_token, 1, MPI_UNSIGNED, 0, TAG_GIVEBACK, MPI_COMM_WORLD);
            _token = 0;
        } else {
            std::cout << "P" << _rank << ": waiting for token..." << std::endl;
            unsigned ask = 1;
            MPI_Send(&ask, 1, MPI_UNSIGNED, 0, TAG_ASK, MPI_COMM_WORLD);
            MPI_Recv(&_token, 1, MPI_UNSIGNED, 0, TAG_SEND, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "P" << _rank << ": received token." << std::endl;
        }
        return true;
    }

    bool doOtherStuff() {
        //std::cout << "P" << _rank << ": doing other stuff." << std::endl;
        double t1 = MPI_Wtime();
        double t2 = MPI_Wtime();
        while (t2 < t1+0.02) {
            t2 = MPI_Wtime();
        }
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
