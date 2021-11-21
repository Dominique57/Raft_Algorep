#include "node.hh"

#include <spdlog/spdlog.h>
#include <config/globalConfig.hh>
#include <rpc/requestVote.hh>
#include <rpc/appendEntries.hh>

namespace Node {

    void Node::update(Cycle &cycle) {
        // Set timeout beginning
        auto start = std::chrono::steady_clock::now();

        cycle.pre_cycle();

        std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
        bool leaveCycle = false;
        bool hasTimedOut = false;
        do {
            auto cur = std::chrono::steady_clock::now();
            long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
            long timeToWait = cycle.Timer() - countTime;
            rpcResponse = rpcReciever.get_rpc_timeout(MPI_ANY_SOURCE, timeToWait);
            hasTimedOut = rpcResponse == nullptr;

            if (!hasTimedOut) {
                auto type = rpcResponse->rpc->Type();
                int senderId = rpcResponse->senderId;
                if (type == Rpc::TYPE::REQUEST_LEADER)
                    cycle.request_leader_response(senderId);
                else if (type == Rpc::TYPE::MESSAGE)
                    cycle.handle_message(rpcResponse.get());

                leaveCycle = cycle.should_stop_cycle(std::move(rpcResponse));
                postLeader = cycle.leaderId;
            }
        } while (!hasTimedOut && !leaveCycle);

        cycle.post_cycle(hasTimedOut);

        if (cycle.NextState().has_value()) {
            state = *cycle.NextState();
            postLeader = -1;
        }
    }

    void Node::start() {
        while (true) {
            if (state == STATE::FOLLOWER) {
                auto cycle = FollowerCycle(*this, postLeader);
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

}
