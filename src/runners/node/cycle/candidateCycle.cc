// Created by dominique on 10/28/21.

#include "candidateCycle.hh"

#include <runners/node.hh>
#include <config/globalConfig.hh>
#include <spdlog/spdlog.h>

namespace Node {

    void CandidateCycle::pre_cycle() {
        // New term
        node.term += 1;

        // Vote for itself
        node.votedFor = GlobalConfig::rank;
        voteCount += 1;

        // Broadcast request vote
        auto rpc = Rpc::RequestVote(node.term, GlobalConfig::rank);
        for (auto dst = 0; dst < GlobalConfig::size; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool CandidateCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        spdlog::info("Candidate: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        if (check_always_should_stop(rpc))
            return true;

        voteCount += 1;
        if (voteCount > GlobalConfig::size / 2) {
            spdlog::info("Candidate has been elected leader !");
            changeNextState(STATE::LEADER);
            return true;
        }
        return false;
    }

    void CandidateCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut) {
            spdlog::info("Candidate timed out");
        }
    }

}