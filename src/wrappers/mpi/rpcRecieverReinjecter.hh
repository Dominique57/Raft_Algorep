#pragma once

#include <wrappers/mpi_include.hh>
#include <queue>

namespace Rpc {

    /// Handle a FIFO cache to reinject message in the next Recv_Rpc wrapper method call
    class RpcRecieverReinjecter {
    public:
        /**
         * @brief Constructor
         */
        RpcRecieverReinjecter()
            : reinjected_messages_() {}

        /**
         * @brief Add a new existing RPC in the cache
         * @param message_
         */
        void reinject_rpc(std::unique_ptr<RpcResponse> message_) {
            reinjected_messages_.emplace(std::move(message_));
        }

        /**
         * @brief Around MPI::Recv_Rpc that sends cached message in priority and blocks until a message is available
         * @param src       ID of the sender, can be set to -1
         * @param tag       optional message tag
         * @param comm      optional group to send the message to
         * @return A unique smart pointer to the RPC message.
         */
        std::unique_ptr<RpcResponse> get_rpc_blocking(int src, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

        /**
         * @brief Around MPI::Recv_Rpc that sends cached message in priority and returns null if no message is available
         * @param src       ID of the sender, can be set to -1
         * @param tag       optional message tag
         * @param comm      optional group to send the message to
         * @return A unique smart pointer to the RPC message.
         */
        std::unique_ptr<RpcResponse> get_rpc(int src, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

        /**
         * @brief Wrapper Around MPI::Recv_Rpc_Timeout that sends cached message in priority and returns null if no message is available until timeout ms
         * @param src       ID of the sender, can be set to -1
         * @param timeout   timeout limit before receiving the message (in milliseconds)
         * @param tag       optional message tag
         * @param comm      optional group to send the message to
         * @return A unique smart pointer to the RPC message if one was received from the cache before the timeout.
         */
        std::unique_ptr<RpcResponse> get_rpc_timeout(int src, long timeout, int tag = 0, MPI_Comm comm = MPI_COMM_WORLD);

    protected:
        /// FIFO cache that store messages
        std::queue<std::unique_ptr<RpcResponse>> reinjected_messages_;
    };
}
