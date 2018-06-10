#ifndef FAKE_PROCESS_H
#define FAKE_PROCESS_H

#include <iostream>
#include <stdlib.h>
#include <mpi.h>

class FakeProcess {
public:
    FakeProcess(int rank, bool token, double rate = 2.0f):
        _rank{rank},
        _token{token},
        _rate{rate},
        _timesUsedResource{0}
    {
        srand(0);
    }

    bool useResource() {
        if (_token) {
            std::cout << "Process " << _rank << " used " << ++_timesUsedResource << std::endl;
        } else {
            std::cout << "Process " << _rank << " waiting for token..." << std::endl;
        }
        return true;
    }

    bool doOtherStuff() {
        std::cout << "Process " << _rank << " doing other stuff " << std::endl;
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
    unsigned _timesUsedResource;
};

#endif
