// Created by dominique on 10/24/21.

#pragma once

#include "rpc.hh"

namespace Rpc {
    class RpcResponse {
    public:
        RpcResponse(std::unique_ptr<Rpc> &rpc, int senderId)
                : rpc(std::move(rpc)), senderId(senderId) {}

        std::unique_ptr<Rpc> rpc;
        int senderId;
    };
}
