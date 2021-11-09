#pragma once

#include "fwd.hh"
#include <optional>
#include <wrappers/mpi/rpcRecieverReinjecter.hh>

namespace Node {

    // Foreward declaration
    class Cycle {
    protected:
        Cycle(int timer, Node &node)
            : timer(timer), nextState(std::nullopt), node(node) {}

    public:
        /**
         * @brief Executes actions like HEARTBEAT depending on the current cycle.
         */
        virtual void pre_cycle() = 0;

        /**
         * @brief Check if we should stop the current cycle depending on the received message.
         * @param message   The received RPC message
         * @return True if we have to stop the current cycle, false otherwise.
         */
        virtual bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> message) = 0;

        /**
         * @brief Executes actions after leaving the cycle depending if we have timed out or not.
         * @param hasTimedOut   bool set to true if we timed out
         */
        virtual void post_cycle(bool hasTimedOut) = 0;

        /**
         * @brief Checks if we have to stop the current cycle depending on the received message.
         * @param message   The received RPC message
         * @return True if we have to stop the current cycle, false otherwise.
         */
        bool check_always_should_stop(std::unique_ptr<Rpc::RpcResponse> &message);

        void request_leader_response(int senderId);
        /**
         * @brief GETTER: Node::Cycle::timer
         * @return The current timer value.
         */
        int Timer() const { return timer; }

        /**
         * @brief GETTER: Node::Cycle::nextState
         * @return The possible next state for the cycle.
         */
        std::optional<STATE> NextState() const;

        int leaderId = 0;
        int timer;
    protected:
        /**
         * @brief Changes our current cycle's state to the next.
         * @param newState  The next state for our cycle.
         */
        void changeNextState(STATE newState);

        std::optional<STATE> nextState;
        Node &node;
    };

}
