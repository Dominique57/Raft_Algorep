// Created by dominique on 10/22/21.

#include "node.hh"

#include <spdlog/spdlog.h>
#include <config/globalConfig.hh>
#include <rpc/message.hh>

bool Node::update_follower(std::unique_ptr<Rpc::RpcResponse> &rpc, FollowerCycleState &cycleState) {
    spdlog::info("Follower: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
    MPI::Send_Rpc(Rpc::Message("Reply"), rpc->senderId);
    return true;
}

bool Node::update_leader(std::unique_ptr<Rpc::RpcResponse> &rpc, LeaderCycleState &cycleState) {
    spdlog::info("Leader: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
    return false;
}

bool Node::update_candidate(std::unique_ptr<Rpc::RpcResponse> &rpc, CandidateCycleState &cycleState) {
    spdlog::info("Candidate: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
    cycleState.voteCount += 1;
    if (cycleState.voteCount > GlobalConfig::size / 2) {
        state = STATE::LEADER;
        spdlog::info("Candidate: elected leader !");
        return true;
    }
    return false;
}

void Node::pre_update(local_state_t &variant, int &timer) {
    if (state == STATE::FOLLOWER) {
        variant = FollowerCycleState();
        timer = (std::rand() % 150) + 150;
    } else if (state == STATE::LEADER) {
        variant = LeaderCycleState();
        timer = 100; // FIXME
        auto rpc = Rpc::Message("HEARTBEAT");
        for (auto dst = 0; dst < GlobalConfig::size; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    } else if (state == STATE::CANDIDATE) {
        timer = 200; // FIXME
        variant = CandidateCycleState{.voteCount = 1};
        auto rpc = Rpc::Message("VOTE4ME");
        for (auto dst = 0; dst < GlobalConfig::size; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    } else {
        throw std::logic_error("NODE>UPDATE> invalid state !");
    }
}

void Node::post_update(bool hasTimedOut) {
    if (state == STATE::FOLLOWER && hasTimedOut) {
        spdlog::info("Follower timed out");
        state = STATE::CANDIDATE;
    }
}

void Node::update() {
    // Set timeout beginning
    auto start = std::chrono::steady_clock::now();
    local_state_t variant;
    int timer; // FIXME: more elegant solution

    pre_update(variant, timer);

    std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
    bool leaveCycle = false;
    do {
        auto cur = std::chrono::steady_clock::now();
        long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
        long timeToWait = timer - countTime;
        rpcResponse = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeToWait);
        if (rpcResponse != nullptr) {
            // Received a message
            if (state == STATE::FOLLOWER) {
                leaveCycle = update_follower(rpcResponse, std::get<Node::FollowerCycleState>(variant));
            } else if (state == STATE::LEADER) {
                leaveCycle = update_leader(rpcResponse, std::get<Node::LeaderCycleState>(variant));
            } else if (state == STATE::CANDIDATE) {
                leaveCycle = update_candidate(rpcResponse, std::get<Node::CandidateCycleState>(variant));
            } else {
                throw std::logic_error("NODE>UPDATE> invalid state !");
            }
        }
    } while (rpcResponse != nullptr && !leaveCycle);

    post_update(rpcResponse == nullptr);
}

void Node::start() {
    while (true) {
        update();
    }
}