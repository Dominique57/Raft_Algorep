// Created by dominique on 10/24/21.

#pragma once

#include <mpi.h>
#include <rpc/rpc.hh>

namespace MPI {
    bool Recv_Timeout(void *data, int count, MPI_Datatype datatype, int source,
                          int tag, MPI_Comm communicator, MPI_Status *status,
                          unsigned long timeout);

    size_t AnyMessageWaiting(int source, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    void Send_Rpc(const Rpc& rpc, int dest, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    Rpc Recv_Rpc(int src, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);
}