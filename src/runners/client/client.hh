#pragma once

#include "utils/clock.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"
#include "rpc/requestClient.hh"

#include <optional>

namespace Client {

    class Client {
    public:
        /**
         * @brief Default constructor.
         * @param timer_      Max time to wait
         */
        Client(const int& timer_);
        void start();

        void send_request();
        void request_leader_id();

        /**
         * @brief controller set request client
         # @param request   Request set by controller to be sent by client
         */
        void set_request(const json& request);

        void handle_requests();
        void handle_controller_request(const Rpc::RpcResponse *rpc);

    protected:
        bool run;

        int timer;
        int leaderId;

        Rpc::RpcRecieverReinjecter rpcReciever;
        std::optional<Rpc::RequestClient> request;

        Clock::Clock clock;
    };
}
