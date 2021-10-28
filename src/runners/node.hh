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

#include "node/cycle/cycle.hh"
#include "node/cycle/followerCycle.hh"
#include "node/cycle/leaderCycle.hh"
#include "node/cycle/candidateCycle.hh"

namespace Node {

    enum class STATE {
        FOLLOWER = 0,
        LEADER,
        CANDIDATE,
    };

    class Node {
    public:
        Node()
            : state(STATE::FOLLOWER), rpcReciever() {}

        void update(Cycle &cycle);

        void start();

        friend Cycle;
        friend LeaderCycle;
        friend FollowerCycle;
        friend CandidateCycle;

    protected:
        STATE state;
        Rpc::RpcRecieverReinjecter rpcReciever;
        int term = 0;
        std::optional<int> votedFor = std::nullopt;
    };
}