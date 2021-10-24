// Created by dominique on 10/22/21.

#include "node.hh"

#include <spdlog/spdlog.h>

void Node::start() {
    while (true) {
        int timeToWait = (std::rand() % 150)  + 150;
        auto rpcResponse = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeToWait);
        if (rpcResponse != nullptr) {
            // Received a message
            spdlog::info("Received message from {}", rpcResponse->senderId);
        } else {
            // Timeout
            spdlog::info("Time out after {} ms", timeToWait);
        }
    }
}