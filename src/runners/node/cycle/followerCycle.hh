#pragma once

#include <rpc/appendEntries.hh>
#include <rpc/requestVote.hh>

#include "fwd.hh"
#include "cycle.hh"

namespace Node {

    class FollowerCycle : public Cycle {
    public:
        FollowerCycle(Node &node)
            : Cycle((std::rand() % 150) + 150, node) {}

        void pre_cycle() override;

        bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void post_cycle(bool hasTimedOut) override;

    };

}
