// Created by dominique on 10/24/21.

#pragma once

#include "rpc.hh"

namespace Rpc {

    /// Container representing a rpc with metadata such as the sender id.
    class RpcResponse {
    public:
        /**
         * @brief Constructor
         * @param rpc           The RPC message.
         * @param senderId      The sender id.
         */
        RpcResponse(std::unique_ptr<Rpc> &rpc, int senderId)
                : rpc(std::move(rpc)), senderId(senderId) {}


        /// The parsed Rpc data
        std::unique_ptr<Rpc> rpc;

        /// The id of the sender
        int senderId;
    };
}
