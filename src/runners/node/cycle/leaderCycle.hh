#pragma once
#include <vector>

#include "cycle.hh"
#include "rpc/requestClient.hh"

namespace Node {

    /// LeaderCycle class for the nodes in the Leader election cycle.
    class LeaderCycle : public Cycle {
    public:
        /**
         * @brief Constructor of the leaderCycle.
         * @param node      The node to enter the leaderCycle.
         */
        LeaderCycle(Node &node)
            : Cycle(100, node) {}

        void pre_cycle() override;

        bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;

        void post_cycle(bool hasTimedOut) override;

        /**
         * @brief Send the updated log to a follower while updating its own.
         * @param dst       The node to send the updated log to.
         */
        void send_update_to_follower(int dst);

    private:
        /**
         * @brief Checks if the majority is good to update the commit index
         * @return If the majority has reached a consensus and updated the commit index.
         */
        bool update_commitIndex();

        /**
         * @brief Send a response to a client who sent a message.
         */
        void respond_client();
    };

}
