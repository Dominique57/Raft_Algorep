#include "candidateCycle.hh"

#include "runners/node.hh"
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
        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool CandidateCycle::handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) {
        Log::recieve(STATE::CANDIDATE, rpc->rpc->Type(), rpc->senderId);
        if (check_always_should_stop(rpc))
            return true;

        if (rpc->rpc->Type() == Rpc::TYPE::REQUEST_VOTE_RESPONSE) {
            auto voteRes = static_cast<Rpc::RequestVoteResponse*>(rpc->rpc.get());
            if (voteRes->voteGranted && !hasVotedForMe[rpc->senderId - GlobalConfig::nb_node_min]) {
                hasVotedForMe[rpc->senderId - GlobalConfig::nb_node_min] = true;
                voteCount += 1;
                if (voteCount > (GlobalConfig::nb_node_max - GlobalConfig::nb_node_min + 1) / 2) {
                    spdlog::info("Candidate has been elected leader !");
                    changeNextState(STATE::LEADER);
                    return true;
                }
            }
        } else if(rpc->rpc->Type() == Rpc::TYPE::APPEND_ENTRIES) {
            // If appendEntry from a leader in same or higher term, accept it as our leader
            // Re-inject rpc: don't loose appendEntries request and wait for the next heartbeat
            auto appendEntries = static_cast<Rpc::AppendEntries*>(rpc->rpc.get());
            if (appendEntries->term >= node.term) {
                changeNextState(STATE::FOLLOWER);
                node.rpcReciever.reinject_rpc(std::move(rpc));
                return true;
            }
        }
        return false;
    }

    void CandidateCycle::handle_client_request(std::unique_ptr<Rpc::RpcResponse>) {
    }

    void CandidateCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut)
            spdlog::info("Candidate timed out");
        if (*nextState == STATE::LEADER) {
            node.initLeader();
        }
    }
}
