#pragma once

#include "wrappers/debug/print_log.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"


namespace Client {

    class Client {
    public:
        /**
         * @brief Default constructor.
         */
        Client(const int& timeout_);

        void send_message(const json& message);
        int request_leader_id();

    protected:
        int timeout;
        int leaderId;
        Rpc::RpcRecieverReinjecter rpcReciever;
    };
}
