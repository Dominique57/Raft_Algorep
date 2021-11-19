#pragma once

#include <spdlog/spdlog.h>

#include "rpc/rpc.hh"
#include "runners/node.hh"
#include "config/globalConfig.hh"

namespace Log {
    inline void recieve(const Node::STATE& state, const Rpc::TYPE& type, const int& senderId)
    {
        std::string stateName = Node::getStateName(state);

#ifndef NDEBUG
        spdlog::info("{}: Received {} from {}", stateName, Rpc::getTypeName(type), senderId);
#endif
        (void) type;
        (void) senderId;


        (void) stateName;
    }


    inline void recieve_leader_response(const int& leaderId)
    {
#ifndef NDEBUG
        spdlog::info("Client {} : Received REQUEST_LEADER_RESPONSE from {}", GlobalConfig::rank, leaderId);
#endif
        (void) leaderId;
    }
}
