// Created by dominique on 10/28/21.

#include "leaderCycle.hh"

#include <config/globalConfig.hh>
#include <spdlog/spdlog.h>

namespace Node {

    void LeaderCycle::pre_cycle() {
        auto rpc = Rpc::AppendEntries(0, GlobalConfig::rank); // FIXME: term
        for (auto dst = 0; dst < GlobalConfig::size; ++dst)
            if (dst != GlobalConfig::rank)
                MPI::Send_Rpc(rpc, dst);
    }

    bool LeaderCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        spdlog::info("Leader: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        return false;
    }

    void LeaderCycle::post_cycle(bool) {
    }

}