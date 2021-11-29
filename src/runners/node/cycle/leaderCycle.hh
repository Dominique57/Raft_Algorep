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

        bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;

        void post_cycle(bool hasTimedOut) override;
        /**
         * @brief send all elements from requests_client to the other nodes
         */
        bool share_client_request(std::unique_ptr<Rpc::RpcResponse> message);
        void respond_client_request(int senderId);

    private:
        void update_commitIndex();
    };

}
