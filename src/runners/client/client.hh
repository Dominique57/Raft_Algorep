#pragma once

#include "wrappers/mpi/rpcRecieverReinjecter.hh"


namespace Client {

    class Client {
    public:
        /**
         * @brief Default constructor.
         */
        Client(int timeout_) : timeout(timeout_) {}

        int request_leader_id();

    protected:
        int timeout;
        int leaderId;
        Rpc::RpcRecieverReinjecter rpcReciever;
    };
}
