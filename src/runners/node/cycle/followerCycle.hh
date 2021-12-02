#pragma once

#include "cycle.hh"

#include "rpc/appendEntries.hh"

namespace Node {

    /// FollowerCycle class for the nodes in the Leader election cycle.
    class FollowerCycle : public Cycle {
    public:
        /**
         * @brief Constructor of the followerCycle.
         * @param node              The node to enter the followerCycle.
         */
        FollowerCycle(Node &node)
            : Cycle((std::rand() % 150) + 150, node) {}

        void pre_cycle() override;

        bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) override;
        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;

        /**
         * @brief Check if a given entry is coherent with our log.
         * @param appendEntries     The entry to check the consistency of.
         * @return If the entry has the correct log index and term.
         */
        bool check_entry_consistency(const Rpc::AppendEntries &appendEntries);

        /**
         * @brief Check if the entry is consistent with our log and add it if it's the case.
         * @param rpc               The entry to be added.
         */
        void handle_append_entries(std::unique_ptr<Rpc::RpcResponse>& rpc);

        /**
         * @brief Handles a REQUEST_VOTE message received from a node, answers if it's vote has been granted or note.
         * @param rpc               The REQUEST_VOTE message received to be handled.
         */
        void handle_request_vote(std::unique_ptr<Rpc::RpcResponse>& rpc);

        /**
         * @brief Check and append multiple entries at once.
         * @param appendEntries     The entries to be added to the system's logs.
         */
        void append_entries(const Rpc::AppendEntries &appendEntries);

        void post_cycle(bool hasTimedOut) override;

    };

}
