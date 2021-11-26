#pragma once

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>
#include <variant>
#include <optional>

#include "wrappers/mpi_include.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"

#include "node/cycle/cycle.hh"
#include "node/cycle/followerCycle.hh"
#include "node/cycle/leaderCycle.hh"
#include "node/cycle/candidateCycle.hh"

#include "utils/clock.hh"

namespace Node {

    enum class STATE {
        FOLLOWER = 0,
        LEADER,
        CANDIDATE,
    };

    /// Node state names (used to convert enum to text).
    static const char *state_names[] = {
        "Follower",
        "Leader",
        "Candidate",
    };

    /**
     * @brief Converts an Node::STATE enum to text.
     * @return static const pointer to name.
     */
    inline const char *getStateName(const STATE& state) {
        auto stateIndex = static_cast<unsigned int>(state);

        if (stateIndex >= sizeof(state_names) / sizeof(state_names[0]))
            return "Unknown name";
        ///
        return state_names[stateIndex];
    }


    class Node {
    public:
        /**
         * @brief Node constructor.
         */
        Node()
            : state(STATE::FOLLOWER), rpcReciever(), clock(Clock::SPEED_TYPE::HIGH) {}

        /**
         * @brief Updates the node depending on it's current cycle.
         * @details The cycle is in which state the node currently is: FOLLOWER,
         *          LEADER and CANDIDATE.
         *          It first calls pre_cycle, which depends on the current cycle.
         *          It sends heartbeat for the LEADER, and sends REQUEST_VOTE
         *          messages to every node.
         *          It then launches the actual cycle with a timeout and checks
         *          if we have timed out while waiting for a message.
         *          If we receives a message, we check if we need to leave the
         *          current cycle like timing out as a FOLLOWER.
         *          If we quit the current cycle, we call post_cycle, especially
         *          used for changing from FOLLOWER to CANDIDATE and changing
         *          to the next cycle.
         * @param cycle The node's current cycle, can be changed during the execution.
         */
        void update(Cycle &cycle);

        /**
         * @brief Launches the node's cycle.
         */
        void start();

        friend Cycle;
        friend LeaderCycle;
        friend FollowerCycle;
        friend CandidateCycle;

    protected:
        STATE state;
        std::vector<std::pair<int, json>> logs;
        Rpc::RpcRecieverReinjecter rpcReciever;
        int term = 0;

        std::optional<int> votedFor = std::nullopt;
        std::optional<int> leaderId = std::nullopt;

        bool crash = false;
        Clock::Clock clock;
    };
}
