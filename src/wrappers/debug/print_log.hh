#pragma once
#include "rpc/rpc.hh"
#include "runners/node.hh"

namespace Log {
    inline void recieve(Node::STATE state, Rpc::TYPE type, int senderId)
    {
        std::string stateName = Node::getStateName(state);

#ifndef NDEBUG
        spdlog::info("{}: Received {} from {}", stateName, Rpc::getTypeName(type), senderId);
#endif
        (void) type;
        (void) senderId;


        (void) stateName;
    }


    inline void recieve_leader_response(int leaderId)
    {
#ifndef NDEBUG
        spdlog::info("Client {} : Received REQUEST_LEADER_RESPONSE from {}", GlobalConfig::rank, leaderId);
#endif
        (void) leaderId;
    }
}
