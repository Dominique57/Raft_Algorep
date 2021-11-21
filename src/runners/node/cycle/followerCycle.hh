#pragma once

#include "cycle.hh"

namespace Node {

    class FollowerCycle : public Cycle {
    public:
        FollowerCycle(Node &node, int postLeader)
            : Cycle((std::rand() % 150) + 150, node), postLeader(postLeader) {}

        void pre_cycle() override;

        bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void handle_controller_request(const Rpc::RpcResponse *rpc) override;

        void post_cycle(bool hasTimedOut) override;

        int postLeader;
    };

}
