// Created by dominique on 10/22/21.

#include "node.hh"

#include <spdlog/spdlog.h>
#include <config/globalConfig.hh>
#include <rpc/requestVote.hh>
#include <rpc/appendEntries.hh>

namespace Node {

    // todo
    bool Node::update_always(std::unique_ptr<Rpc::RpcResponse> &rpc) {
        int sentTerm = -1;
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                sentTerm = static_cast<Rpc::RequestVote *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::APPEND_ENTRIES:
                sentTerm = static_cast<Rpc::AppendEntries *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
                sentTerm = static_cast<Rpc::RequestVoteResponse *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
                sentTerm = static_cast<Rpc::AppendEntriesResponse *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::MESSAGE:
                break;
        }
        if (sentTerm != -1 && sentTerm > term) {
            term = sentTerm;
            if (state != STATE::FOLLOWER) {
//                transition_to_follower();
                rpcReciever.reinject_rpc(std::move(rpc));
                return true;
            }
        }
        return false;
    }

    // done
    bool Node::update_follower(std::unique_ptr<Rpc::RpcResponse> &rpc, FollowerCycleState &) {
        spdlog::info("Follower: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                MPI::Send_Rpc(Rpc::RequestVoteResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::APPEND_ENTRIES:
                MPI::Send_Rpc(Rpc::AppendEntriesResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
            case Rpc::TYPE::MESSAGE:
                break;
        }
        return false;
    }

    // done
    bool Node::update_leader(std::unique_ptr<Rpc::RpcResponse> &rpc, LeaderCycleState &) {
        spdlog::info("Leader: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        if (term == 1 && std::rand() % 10 == 0) {
            spdlog::critical("Leader: going to sleep !");
            usleep(1000 * 1000);
            spdlog::critical("Leader: waking up !");
            return true;
        }
        return false;
    }

    // done
    bool Node::update_candidate(std::unique_ptr<Rpc::RpcResponse> &rpc, CandidateCycleState &cycleState) {
        spdlog::info("Candidate: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        cycleState.voteCount += 1;
        if (cycleState.voteCount > GlobalConfig::size / 2) {
//            transition_to_leader();
            return true;
        }
        return false;
    }

    // done
    void Node::pre_update(local_state_t &variant, int &timer) {
        if (state == STATE::FOLLOWER) {
            variant = FollowerCycleState();
            timer = (std::rand() % 150) + 150;
        } else if (state == STATE::LEADER) {
            variant = LeaderCycleState();
            timer = 100;// FIXME
            auto rpc = Rpc::AppendEntries(term, GlobalConfig::rank);
            for (auto dst = 0; dst < GlobalConfig::size; ++dst)
                if (dst != GlobalConfig::rank)
                    MPI::Send_Rpc(rpc, dst);
        } else if (state == STATE::CANDIDATE) {
            timer = 200;// FIXME
            votedFor = GlobalConfig::rank;
            variant = CandidateCycleState{.voteCount = 1};
            auto rpc = Rpc::RequestVote(term, GlobalConfig::rank);
            for (auto dst = 0; dst < GlobalConfig::size; ++dst)
                if (dst != GlobalConfig::rank)
                    MPI::Send_Rpc(rpc, dst);
        } else {
            throw std::logic_error("NODE>UPDATE> invalid state !");
        }
    }

    // done
    void Node::post_update(bool hasTimedOut) {
        if (state == STATE::FOLLOWER && hasTimedOut) {
            spdlog::info("Follower timed out");
//            transition_to_candidate();
        } else if (state == STATE::CANDIDATE && hasTimedOut) {
            spdlog::info("Candidate timed out");
//            transition_to_follower();
        }
    }

    void Node::update(Cycle &cycle) {
        // Set timeout beginning
        auto start = std::chrono::steady_clock::now();
//        local_state_t variant;// FIXME: Polymorphism classes?
//        int timer;            // FIXME: more elegant solution

//        pre_update(variant, timer);
        cycle.pre_cycle();

        std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
        bool leaveCycle = false;
        bool hasTimedOut = false;
        do {
            auto cur = std::chrono::steady_clock::now();
            long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
//            long timeToWait = timer - countTime;
            long timeToWait = cycle.Timer() - countTime;
            rpcResponse = rpcReciever.get_rpc_timeout(MPI_ANY_SOURCE, timeToWait);
            hasTimedOut = rpcResponse == nullptr;
            if (!hasTimedOut) {
                leaveCycle = cycle.should_stop_cycle(std::move(rpcResponse));
            }
//            if (!hasTimedOut) {
//                leaveCycle = update_always(rpcResponse);
//                if (!leaveCycle) {
////                     Received a message
//                    if (state == STATE::FOLLOWER) {
//                        leaveCycle = update_follower(rpcResponse, std::get<Node::FollowerCycleState>(variant));
//                    } else if (state == STATE::LEADER) {
//                        leaveCycle = update_leader(rpcResponse, std::get<Node::LeaderCycleState>(variant));
//                    } else if (state == STATE::CANDIDATE) {
//                        leaveCycle = update_candidate(rpcResponse, std::get<Node::CandidateCycleState>(variant));
//                    } else {
//                        throw std::logic_error("NODE>UPDATE> invalid state !");
//                    }
//                }
//            }
        } while (!hasTimedOut && !leaveCycle);

        cycle.post_cycle(hasTimedOut);
//        post_update(hasTimedOut);
        if (cycle.NextState().has_value()) {
            state = *cycle.NextState();
        }
    }

    void Node::start() {
        while (true) {
            if (state == STATE::FOLLOWER) {
                auto cycle = FollowerCycle(rpcReciever);
                update(cycle);
            } else if (state == STATE::LEADER) {
                auto cycle = LeaderCycle(rpcReciever);
                update(cycle);
            } else if (state == STATE::CANDIDATE) {
                auto cycle = CandidateCycle(rpcReciever);
                update(cycle);
            } else {
                throw std::logic_error("NODE>UPDATE> invalid state !");
            }
        }
    }

}