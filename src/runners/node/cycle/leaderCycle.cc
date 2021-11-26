#include "leaderCycle.hh"

#include "runners/node.hh"
#include "rpc/appendEntries.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void LeaderCycle::pre_cycle() {
        auto rpc = Rpc::AppendEntries(node.term, GlobalConfig::rank);
        node.leaderId = GlobalConfig::rank;
        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool LeaderCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        Log::recieve(STATE::LEADER, rpc->rpc->Type(), rpc->senderId);

        if (check_always_should_stop(rpc))
            return true;

        // Small fault code that forces new re-election
        if (node.term == 1 && std::rand() % 10 == 0) {
            spdlog::critical("Leader: going to sleep !");
            usleep(1000 * 1000 * 2);
            spdlog::critical("Leader: waking up !");
            return true;
        }
        return false;
    }

    void LeaderCycle::post_cycle(bool) {
    }

    void LeaderCycle::handle_client_request(std::unique_ptr<Rpc::RpcResponse> message)
    {
        if (message->rpc->Type() == Rpc::TYPE::REQUEST_LEADER) {
            client_request_leader_response(std::move(message));
            return;
        }
        if (message->rpc->Type() == Rpc::TYPE::REQUEST_CLIENT)
        {
            share_client_request(std::move(message));
        }
    }

    void LeaderCycle::share_client_request(std::unique_ptr<Rpc::RpcResponse> message) {
        auto rpc = static_cast<Rpc::RequestClient *>(message->rpc.get());
        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst) {
            if (dst != GlobalConfig::rank) {
                MPI::Send_Rpc(*rpc, dst);
            }
        }
    }

}
