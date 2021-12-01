#pragma once

#include "cycle.hh"

#include "rpc/appendEntries.hh"

namespace Node {

    class FollowerCycle : public Cycle {
    public:
        FollowerCycle(Node &node)
            : Cycle((std::rand() % 150) + 150, node) {}

        void pre_cycle() override;

        bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) override;
        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;
        bool check_entry_consistency(const Rpc::AppendEntries &appendEntries);
        void handle_append_entries(std::unique_ptr<Rpc::RpcResponse>& rpc);
        void handle_request_vote(std::unique_ptr<Rpc::RpcResponse>& rpc);
        void append_entries(const Rpc::AppendEntries &appendEntries);

        void post_cycle(bool hasTimedOut) override;

    };

}
