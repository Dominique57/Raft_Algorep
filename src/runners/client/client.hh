#pragma once

#include "wrappers/debug/print_log.hh"
#include "wrappers/mpi/rpcRecieverReinjecter.hh"


namespace Client {

    class Client {
    public:
        /**
         * @brief Default constructor.
         */
        Client(int timeout_) : timeout(timeout_) {}

        int request_leader_id();
        void send_message(std::string message);

    protected:
        int timeout;
        int leaderId;
        Rpc::RpcRecieverReinjecter rpcReciever;
    };
}
