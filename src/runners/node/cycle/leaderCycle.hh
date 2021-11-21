#pragma once
#include <vector>

#include "rpc/appendEntries.hh"
#include "rpc/message.hh"

#include "fwd.hh"
#include "cycle.hh"

namespace Node {

    class LeaderCycle : public Cycle {
    public:
        LeaderCycle(Node &node)
            : Cycle(100, node) {}

        void pre_cycle() override;

        bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) override;

        void post_cycle(bool hasTimedOut) override;

        /**
         * @brief receive the request from all clients and push them to requests_client
         */
        void receive_client_request();

        /**
         * @brief send all elements from requests_client to the other nodes
         */
        void share_client_request();

    protected:
        //TODO: not sure about "void¨ it must be a message but the message
        //can be a string/char*/json
        std::vector<Rpc::Message *> requests_client;

    };

}
