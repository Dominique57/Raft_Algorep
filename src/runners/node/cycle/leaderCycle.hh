// Created by dominique on 10/28/21.

#pragma once

#include <rpc/appendEntries.hh>

#include "fwd.hh"
#include "cycle.hh"

namespace Node {

    class LeaderCycle : public Cycle {
    public:
        LeaderCycle(Node &node)
            : Cycle(100, node) {}

        void pre_cycle() override;

        bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void post_cycle(bool hasTimedOut) override;
    };

}
