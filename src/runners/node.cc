#include "node.hh"

#include <iostream>
#include <fstream>

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

            // HOTFIX: Sometimes MPI_Iprobe fails and return MPI_ERROR but with incorrect code
            // We saw that rpcReciever.get_rpc_timeout (SOMEHOW) manages to avoid that bug
            auto msg = rpcReciever.get_rpc_timeout(MPI_ANY_SOURCE, 0);
            if (msg == nullptr) {
                // If no message awaiting, wait the clock time (LOW/MEDIUM/HIGH)
                this->clock.wait();
            } else {
                // else keep processing messages (because it is a priority to flush before
                // running a local event
                rpcReciever.reinject_rpc(std::move(msg));
            }

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

    void Node::save_commit(Entry &entry) {
        std::ofstream file;
        auto file_name = "node_" + std::to_string(GlobalConfig::rank) + ".log";
        file.open (file_name, std::fstream::app);
        file << entry << std::endl;
        file.close();
    }

    void Node::initLeader() {
        std::fill(matchIndex.begin(), matchIndex.end(), -1);
        std::fill(nextIndex.begin(), nextIndex.end(), logs.size());
    }
}
