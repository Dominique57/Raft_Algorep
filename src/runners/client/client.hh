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
         * @param timeout_      Max time to wait
         */
        Client(const int& timeout_);
        void start();

        void send_request();
        int request_leader_id();

        void set_start();

        /**
         * @brief controller set request client
         # @param request_ request set by controller to be send by client
         */
        void set_request(const json& request);

    protected:
        bool first_start;

        std::optional<Rpc::RequestClient> request;

        int timeout;
        int leaderId;
        Rpc::RpcRecieverReinjecter rpcReciever;

        // TODO: use clock attribute (process speed)
        Clock::Clock clock;
    };
}
