#pragma once

#include "cycle.hh"

#include <config/globalConfig.hh>
#include <rpc/requestVote.hh>

#include "fwd.hh"

namespace Node {

    class CandidateCycle : public Cycle {
    public:
        CandidateCycle(Node &node)
            : Cycle((std::rand() % 150) + 150, node),
              voteCount(0), hasVotedForMe(GlobalConfig::nb_node(), false)
        {}

        void pre_cycle() override;

        bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) override;
        void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) override;

        void post_cycle(bool hasTimedOut) override;

    protected:
        int voteCount;
        std::vector<bool> hasVotedForMe;
    };

}
