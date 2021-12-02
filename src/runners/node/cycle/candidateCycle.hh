#pragma once

#include "cycle.hh"

#include <config/globalConfig.hh>
#include <rpc/requestVote.hh>

#include "fwd.hh"

namespace Node {

    /// CandidateCycle class for the nodes in the Leader election cycle.
    class CandidateCycle : public Cycle {
    public:
        /**
         * @brief Constructor for the candidateCycle.
         * @param node      The node to enter the candidateCycle.
         */
        CandidateCycle(Node &node)
            : Cycle((std::rand() % 150) + 150, node),
              voteCount(0), hasVotedForMe(GlobalConfig::nb_node(), false)
        {}

        void pre_cycle() override;

        bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) override;
        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;

        void post_cycle(bool hasTimedOut) override;

    protected:
        /// The number of votes recieved.
        int voteCount;

        /// To check if a node has already given it's vote to the current node.
        std::vector<bool> hasVotedForMe;
    };

}
