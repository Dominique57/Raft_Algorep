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
            int clientId = message->senderId;
            bool has_been_send  = share_client_request(std::move(message));
            if (has_been_send)
                respond_client_request(clientId);
        }
    }

    void LeaderCycle::respond_client_request(int senderId)
    {
        auto rpc = Rpc::RequestClientResponse(true);
        MPI::Send_Rpc(rpc, senderId);
    }

    bool LeaderCycle::share_client_request(std::unique_ptr<Rpc::RpcResponse> message) {
        auto rpc = static_cast<Rpc::RequestClient *>(message->rpc.get());
        node.logs.push_back(std::pair<int, json>(node.term, rpc->message));

        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst) {
            if (dst != GlobalConfig::rank) {
                MPI::Send_Rpc(*rpc, dst);
            }
        }

        //TODO wait for response client request from all nodes then return true
        return true;
    }

}
