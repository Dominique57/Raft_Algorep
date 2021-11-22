#pragma once

#include "utils/clock.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"

namespace Client {

    class Client {
    public:
        /**
         * @brief Default constructor.
         * @param timeout_      Max time to wait
         */
        Client(const int& timeout_);

        void send_message(const json& message);
        int request_leader_id();

    protected:
        int timeout;
        int leaderId;
        Rpc::RpcRecieverReinjecter rpcReciever;

        // TODO: use clock attribute (process speed)
        Clock::Clock clock;
    };
}
