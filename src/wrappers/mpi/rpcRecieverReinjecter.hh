// Created by dominique on 10/27/21.

#pragma once

#include <wrappers/mpi_include.hh>
#include <queue>

namespace Rpc {

    /// Handle a FIFO cache to reinject message in the next Recv_Rpc wrapper method call
    class RpcRecieverReinjecter {
    public:
        /// Constructor
        RpcRecieverReinjecter()
            : reinjected_messages_() {}

        /// Add a new existing rpc in the cache
        void reinject_rpc(std::unique_ptr<RpcResponse> message_) {
            reinjected_messages_.emplace(std::move(message_));
        }

        /// Wrapper Around MPI::Recv_Rpc that sends cached message in priority
        std::unique_ptr<RpcResponse> get_rpc(int src, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

        /// Wrapper Around MPI::Recv_Rpc_Timeout that sends cached message in priority
        std::unique_ptr<RpcResponse> get_rpc_timeout(int src, long timeout, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    protected:
        /// FIFO cache that store messages
        std::queue<std::unique_ptr<RpcResponse>> reinjected_messages_;
    };
}
