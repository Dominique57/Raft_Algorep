#pragma once

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>

#include <wrappers/mpi_include.hh>
#include <variant>

class Node {
public:
    enum class STATE {
        FOLLOWER = 0,
        LEADER,
        CANDIDATE,
    };

    struct FollowerCycleState {
    };
    struct LeaderCycleState {
    };
    struct CandidateCycleState {
        int voteCount = 0;
    };
    using local_state_t = std::variant<FollowerCycleState, LeaderCycleState, CandidateCycleState>;

public:
    Node()
            : state(STATE::FOLLOWER) {}

    bool update_follower(std::unique_ptr<Rpc::RpcResponse> &rpc, FollowerCycleState &cycleState);

    bool update_leader(std::unique_ptr<Rpc::RpcResponse> &rpc, LeaderCycleState &cycleState);

    bool update_candidate(std::unique_ptr<Rpc::RpcResponse> &rpc, CandidateCycleState &cycleState);

    void pre_update(local_state_t &variant, int &timer);

    void post_update(bool hasTimedOut);

    void update();

    void start();

protected:
    STATE state;
};
