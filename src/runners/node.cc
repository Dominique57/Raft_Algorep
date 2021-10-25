// Created by dominique on 10/22/21.

#include "node.hh"

#include <spdlog/spdlog.h>
#include <config/globalConfig.hh>
#include <rpc/message.hh>

void Node::update_follower() {
    int timeToWait = (std::rand() % 150) + 150;
    auto rpcResponse = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeToWait);
    if (rpcResponse != nullptr) {
        // Received a message
        spdlog::info("Received {} from {}", Rpc::getTypeName(rpcResponse->rpc->Type()),
                     rpcResponse->senderId);
        MPI::Send_Rpc(Rpc::Message("Reply"), rpcResponse->senderId);
    } else {
        // Timeout
        spdlog::info("Time out after {} ms", timeToWait);
        state = STATE::CANDIDATE;
    }
}

void Node::update_leader() {
    // set timer curr et end(curr + 100ms)
    auto start = std::chrono::steady_clock::now();

    // ping everyone
    for (auto dst = 0; dst < GlobalConfig::size; ++dst) {
        if (dst == GlobalConfig::rank)
            continue;
        MPI::Send_Rpc(Rpc::Message("HEARTBEAT"), dst); // FIXME: use real heartbeat rpc
    }

    std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
    do {
        auto cur = std::chrono::steady_clock::now();
        long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
        long timeToWait = 100 - countTime; // FIXME: magic number (leader timeout)
        rpcResponse = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeToWait);
        if (rpcResponse != nullptr) {
            // Received a message
            spdlog::info("Leader: Received {} from {}", Rpc::getTypeName(rpcResponse->rpc->Type()),
                         rpcResponse->senderId);
        }
    } while (rpcResponse != nullptr);
}

void Node::update_candidate() {
    // set timer curr et end(curr + 100ms)
    auto start = std::chrono::steady_clock::now();

    // ping everyone
    for (auto dst = 0; dst < GlobalConfig::size; ++dst) {
        if (dst == GlobalConfig::rank)
            continue;
        MPI::Send_Rpc(Rpc::Message("VOTE4ME"), dst); // FIXME: use voteRequest rpc
    }

    std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
    long voteCount = 0;
    do {
        auto cur = std::chrono::steady_clock::now();
        long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
        long timeToWait = 200 - countTime; // FIXME: magic number (candidate timeout)
        rpcResponse = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeToWait);
        if (rpcResponse != nullptr) {
            // Received a message
            spdlog::info("Candidate: Received {} from {}", Rpc::getTypeName(rpcResponse->rpc->Type()),
                         rpcResponse->senderId);
            voteCount += 1;
            if (voteCount > GlobalConfig::size / 2) {
                state = STATE::LEADER;
                spdlog::info("Candidate: elected leader !");
                return;
            }
        }
    } while (rpcResponse != nullptr);
    spdlog::info("Candidate: failed election !");
}

void Node::start() {
    while (true) {
        if (state == STATE::FOLLOWER) {
            update_follower();
        } else if (state == STATE::LEADER) {
            update_leader();
        } else if (state == STATE::CANDIDATE) {
            update_candidate();
        } else {
            throw std::logic_error("NODE>UPDATE> invalid state !");
        }
    }
}