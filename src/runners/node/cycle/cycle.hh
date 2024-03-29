#pragma once

#include <optional>

#include "fwd.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"

namespace Node {

    /// Abstract class to derive every node's cycle from (LeaderCycle, CandidateCycle and FollowerCycle).
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
        virtual bool handle_node_request(std::unique_ptr<Rpc::RpcResponse> message) = 0;

        /**
         * @brief Executes actions after leaving the cycle depending if we have timed out or not.
         * @param hasTimedOut   bool set to true if we timed out
         */
        virtual void post_cycle(bool hasTimedOut) = 0;

        /**
         * @brief Checks if we have to stop the current cycle depending on the received message.
         * @param message   The received RPC message alaways send from a node
         * @return True if we have to stop the current cycle, false otherwise.
         */
        bool check_always_should_stop(std::unique_ptr<Rpc::RpcResponse> &message);

        /**
         * @brief Reply client request
         * @param message The received RPC message always from a client
         */
        virtual void handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) = 0;

        void client_request_leader_response(std::unique_ptr<Rpc::RpcResponse> message);

        /**
         * @brief Handle controller's request
         * @param rpc The received RPC
         */
        bool handle_controller_request(const Rpc::RpcResponse *rpc);

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


    protected:
        /**
         * @brief Changes our current cycle's state to the next.
         * @param newState  The next state for our cycle.
         */
        void changeNextState(STATE newState);

        /**
         * @brief Function to call while a process is crashed.
         */
        void crash_loop();

        /// The current timeout value.
        int timer;

        /// The next state to enter into.
        std::optional<STATE> nextState;

        /// The node which cycle we are handling.
        Node &node;
    };

}
