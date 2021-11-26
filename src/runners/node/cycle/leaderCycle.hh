#pragma once
#include <vector>

#include "cycle.hh"
#include "rpc/requestClient.hh"

namespace Node {

    class LeaderCycle : public Cycle {
    public:
        LeaderCycle(Node &node)
            : Cycle(100, node) {}

        void pre_cycle() override;

        bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;

        void post_cycle(bool hasTimedOut) override;
        /**
         * @brief send all elements from requests_client to the other nodes
         */
        void share_client_request(std::unique_ptr<Rpc::RpcResponse> message);

    };

}
