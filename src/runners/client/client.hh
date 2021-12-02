#pragma once

#include "utils/clock.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"
#include "rpc/requestClient.hh"

#include <optional>

namespace Client {

    /// Class to handle and use clients with the controller.
    class Client {
    public:
        /**
         * @brief Default constructor.
         * @param timer_      Max time to wait
         */
        Client(const int& timer_);

        /**
         * @brief Launches the client run the main client's loop.
         */
        [[noreturn]] void start();

        /**
         * @brief Send a request to the system.
         */
        void send_request();

        /**
         * @brief Request the leader ID to send requests to afterwards.
         */
        void request_leader_id();

        /**
         * @brief Handles receiving messages from the controller or responses from the system after sending a message.
         */
        void handle_requests();

        /**
         * @brief Handles and applies commands from the controller.
         * @param rpc       The RPC message received from the controller to be handled.
         */
        void handle_controller_request(const Rpc::RpcResponse *rpc);

    protected:

        /// If the client is currently running.
        bool run;

        /// The timer for timeout of our clients
        int timer;

        /// The requested leader ID
        int leaderId;

        /// The reinjecter containing the messages waiting to be received.
        Rpc::RpcRecieverReinjecter rpcReciever;

        /// The current client's requests
        std::vector<Rpc::RequestClient> request;

        /**
         * @brief controller set request client
         * @param request_   Request set by controller to be sent by client
         */
        void add_request(const std::string &request_);

        /// The current client's clock containing the speed and wait() function.
        Clock::Clock clock;
    };
}
