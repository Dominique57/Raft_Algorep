#include "node.hh"

#include <spdlog/spdlog.h>
#include <config/globalConfig.hh>
#include <rpc/requestVote.hh>

namespace Node {

    void Node::update(Cycle &cycle) {
        // Set timeout beginning
        auto start = std::chrono::steady_clock::now();

        cycle.pre_cycle();

        std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
        bool leaveCycle = false;
        bool hasTimedOut;
        do {
            auto cur = std::chrono::steady_clock::now();
            long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
            long timeToWait = cycle.Timer() - countTime;
            rpcResponse = rpcReciever.get_rpc_timeout(MPI_ANY_SOURCE, timeToWait);
            hasTimedOut = rpcResponse == nullptr;

            if (!hasTimedOut) {
                int senderId = rpcResponse->senderId;
                auto type = rpcResponse->rpc->Type();
                if (type == Rpc::TYPE::CONTROLLER_REQUEST)
                    leaveCycle = cycle.handle_controller_request(rpcResponse.get());

                else if (GlobalConfig::is_node(senderId))
                    leaveCycle = cycle.handle_node_request(std::move(rpcResponse));

                else if (GlobalConfig::is_client(senderId))
                    cycle.handle_client_request(std::move(rpcResponse));
            }
        } while (!hasTimedOut && !leaveCycle);

        cycle.post_cycle(hasTimedOut);

        if (cycle.NextState().has_value())
            state = *cycle.NextState();
    }

    void Node::start() {
        while (true) {

            this->clock.wait();

            if (state == STATE::FOLLOWER) {
                auto cycle = FollowerCycle(*this);
                update(cycle);
            } else if (state == STATE::LEADER) {
                auto cycle = LeaderCycle(*this);
                update(cycle);
            } else if (state == STATE::CANDIDATE) {
                auto cycle = CandidateCycle(*this);
                update(cycle);
            } else {
                throw std::logic_error("NODE>UPDATE> invalid state !");
            }
        }
    }

    void Node::initLeader() {
        std::fill(matchIndex.begin(), matchIndex.end(), -1);
        std::fill(nextIndex.begin(), nextIndex.end(), logs.size());
    }
}
