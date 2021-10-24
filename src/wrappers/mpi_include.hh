// Created by dominique on 10/24/21.

#pragma once

#include <mpi.h>
#include <rpc/rpcResponse.hh>

namespace MPI {
    void Send_Rpc(const Rpc::Rpc &rpc, int dest, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    size_t AnyMessageWaiting(int source, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    std::unique_ptr<Rpc::RpcResponse> Recv_Rpc(int src, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    std::unique_ptr<Rpc::RpcResponse>
    Recv_Rpc_Timeout(int src, long timeout, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);
}