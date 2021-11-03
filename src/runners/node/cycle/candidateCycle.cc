#include "candidateCycle.hh"
#include <spdlog/spdlog.h>

#include "runners/node.hh"
#include "config/globalConfig.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void CandidateCycle::pre_cycle() {
        // New term
        node.term += 1;

        // Vote for itself
        node.votedFor = GlobalConfig::rank;
        voteCount += 1;

        // Broadcast request vote
        auto rpc = Rpc::RequestVote(node.term, GlobalConfig::rank);
        for (auto dst = 0; dst < GlobalConfig::nb_node; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool CandidateCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        Log::recieve(STATE::CANDIDATE, rpc->rpc->Type(), rpc->senderId);
        if (check_always_should_stop(rpc))
            return true;

        voteCount += 1;
        if (voteCount > GlobalConfig::nb_node / 2) {
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
