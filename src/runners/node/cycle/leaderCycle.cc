#include "leaderCycle.hh"

#include <config/globalConfig.hh>
#include <spdlog/spdlog.h>
#include <runners/node.hh>

namespace Node {

    void LeaderCycle::pre_cycle() {
        auto rpc = Rpc::AppendEntries(node.term, GlobalConfig::rank);
        for (auto dst = 0; dst < GlobalConfig::nb_node; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool LeaderCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        spdlog::info("Leader: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        if (check_always_should_stop(rpc))
            return true;

      /*
        // Small fault code that forces new re-election
        if (node.term == 1 && std::rand() % 10 == 0) {
            spdlog::critical("Leader: going to sleep !");
            usleep(1000 * 1000);
            spdlog::critical("Leader: waking up !");
            return true;
        }
        */
        return false;
    }

    void LeaderCycle::post_cycle(bool) {
    }

}
