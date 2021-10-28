// Created by dominique on 10/28/21.

#include "candidateCycle.hh"

#include <runners/node.hh>
#include <config/globalConfig.hh>
#include <spdlog/spdlog.h>

namespace Node {

    void CandidateCycle::pre_cycle() {
//        term += 1;  // FIXME: access node state
//        votedFor = GlobalConfig::rank; // FIXME: access node state
        voteCount += 1;

        auto rpc = Rpc::RequestVote(0, GlobalConfig::rank); // FIXME term
        for (auto dst = 0; dst < GlobalConfig::size; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool CandidateCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        spdlog::info("Candidate: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
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