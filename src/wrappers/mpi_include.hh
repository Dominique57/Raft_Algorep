#pragma once

#include <mpi.h>
#include "rpc/rpcResponse.hh"

namespace MPI {
    /**
     * @brief Send a RPC message with Isend to by a non-blocking call.
     * @param rpc       RPC message to send
     * @param dest      ID of the receiver, can be set to -1
     * @param tag       optional message tag
     * @param comm      optional group to send the message to.
     */
    void Send_Rpc(const Rpc::Rpc &rpc, const int& dest, const int tag = 0, const MPI_Comm comm = MPI_COMM_WORLD);

    /**
     * @brief Check if any message has been sent and waiting to be received.
     * @param src       ID of the sender, can be set to -1
     * @param tag       optional message tag
     * @param comm      optional group to send the message to
     * @return The length of the waiting message timeout, 0 if there is none.
     */
    size_t AnyMessageWaiting(const int& src, const int tag = 0, const MPI_Comm comm = MPI_COMM_WORLD);

    /**
     * @brief Receives a waiting RPC message if there is one.
     * @param src       ID of the sender, can be set to -1
     * @param tag       optional message tag
     * @param comm      optional group to send the message to
     * @return A unique smart pointer to the RPC message.
     */
    std::unique_ptr<Rpc::RpcResponse>
    Recv_Rpc(const int& src, const int tag = 0, const MPI_Comm comm = MPI_COMM_WORLD);

    /**
     * @brief Receives a waiting RPC message but times out if there is none.
     * @param src       ID of the sender, can be set to -1
     * @param timeout   timeout limit before receiving the message
     * @param tag       optional message tag
     * @param comm      optional group to send the message to
     * @return A unique smart pointer to the RPC message if one was received before the timeout.
     */
    std::unique_ptr<Rpc::RpcResponse>
    Recv_Rpc_Timeout(const int& src, const long& timeout, const int tag = 0, const MPI_Comm comm = MPI_COMM_WORLD);
}
