#pragma once

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>

#include <wrappers/mpi_include.hh>
#include <wrappers/mpi/rpcRecieverReinjecter.hh>
#include <variant>
#include <optional>

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
            : state(STATE::FOLLOWER), rpcReciever() {}

    void transition_to_leader();

    void transition_to_follower();

    void transition_to_candidate();

    bool update_always(std::unique_ptr<Rpc::RpcResponse> &rpc);

    bool update_follower(std::unique_ptr<Rpc::RpcResponse> &rpc, FollowerCycleState &cycleState);

    bool update_leader(std::unique_ptr<Rpc::RpcResponse> &rpc, LeaderCycleState &cycleState);

    bool update_candidate(std::unique_ptr<Rpc::RpcResponse> &rpc, CandidateCycleState &cycleState);

    void pre_update(local_state_t &variant, int &timer);

    void post_update(bool hasTimedOut);

    void update();

    void start();

protected:
    STATE state;
    Rpc::RpcRecieverReinjecter rpcReciever;
    int term = 0;
    std::optional<int> votedFor = std::nullopt;
};