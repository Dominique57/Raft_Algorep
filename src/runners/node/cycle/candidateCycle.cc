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

    bool CandidateCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        Log::recieve(STATE::CANDIDATE, rpc->rpc->Type(), rpc->senderId);
        if (check_always_should_stop(rpc))
            return true;

        voteCount += 1;
        std::cerr << "RANK: " << GlobalConfig::rank << " with counts: " << voteCount << std::endl;
        std::cerr << "Nb node: " << GlobalConfig::nb_node_min << ' ' << GlobalConfig::nb_node_max << std::endl;
        std::cerr << "Nb client: " << GlobalConfig::nb_client_min << ' ' << GlobalConfig::nb_client_max << std::endl;
        if (voteCount > (GlobalConfig::nb_node_max - GlobalConfig::nb_node_min) / 2) {
            spdlog::info("Candidate has been elected leader !");
            changeNextState(STATE::LEADER);
            return true;
        }
        return false;
    }

    void CandidateCycle::handle_message(const Rpc::RpcResponse *rpc) {
        auto message = static_cast<Rpc::Message*>(rpc->rpc.get());
        switch (message->type) {
        case Rpc::MESSAGE_TYPE::STATUS:
            std::cout << "Node | " << GlobalConfig::rank << " | Candidate";
            has_crashed(std::cout) << std::endl;
            break;
        case Rpc::MESSAGE_TYPE::CRASH:
            std::cout << "Node " << GlobalConfig::rank << " crashed" << std::endl;
            node.crash = true;
            break;
        default:
            break;
        }
    }

    void CandidateCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut) {
            spdlog::info("Candidate timed out");
        }
    }

}
