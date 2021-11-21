#include "leaderCycle.hh"

#include "runners/node.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void LeaderCycle::pre_cycle() {
        auto rpc = Rpc::AppendEntries(node.term, GlobalConfig::rank);
        leaderId = GlobalConfig::rank;
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

    void LeaderCycle::receive_client_request()
    {
        auto start = std::chrono::steady_clock::now();

        for (auto dst = GlobalConfig::nb_client_min; dst <= GlobalConfig::nb_client_max; ++dst)
        {
            auto cur = std::chrono::steady_clock::now();
            long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
            long timeToWait = Timer() - countTime;
            //TODO: request_client is the request (message) sent by the client
            auto request_client = MPI::Recv_Rpc_Timeout(dst, timeToWait, 0, MPI_COMM_WORLD);

            if (request_client && request_client->rpc.get()->Type() == Rpc::TYPE::MESSAGE)
            {
                auto request = static_cast<Rpc::Message *>(request_client->rpc.get());
                requests_client.push_back(request);
            }
        }
    }

    void LeaderCycle::share_client_request()
    {
        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst)
        {
            if (dst != GlobalConfig::rank)
            {
                for (auto rpc: requests_client)
                    MPI::Send_Rpc(*rpc, dst);
            }
        }

        requests_client.clear();
    }

    void LeaderCycle::handle_message(const Rpc::RpcResponse *rpc) {
        auto message = static_cast<Rpc::Message*>(rpc->rpc.get());

        switch (message->type) {
        case Rpc::MESSAGE_TYPE::STATUS:
            std::cout << "Node | " << GlobalConfig::rank << " | Leader   ";
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
}
