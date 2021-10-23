#pragma once

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>

#include "mpi.h"

class Node {
public:
    explicit Node(unsigned int rank) : rank_(rank) {}

    void send_message();

    void receive_message();

protected:
    unsigned rank_;

    static bool
    MPI_Recv_Timeout(void *data, int count, MPI_Datatype datatype, int source, int tag,
                     MPI_Comm communicator, MPI_Status *status, unsigned long timeout);
};

