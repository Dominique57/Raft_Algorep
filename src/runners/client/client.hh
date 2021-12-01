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
        void handle_requests();
        void handle_controller_request(const Rpc::RpcResponse *rpc);

    protected:

        bool run;

        int timer;
        int leaderId;

        Rpc::RpcRecieverReinjecter rpcReciever;
        std::vector<Rpc::RequestClient> request;

        /**
         * @brief controller set request client
         * @param request   Request set by controller to be sent by client
         */
        void add_request(const std::string request_);


        Clock::Clock clock;
    };
}
