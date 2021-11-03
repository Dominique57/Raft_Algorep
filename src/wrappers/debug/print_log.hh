#pragma once
#include "rpc/rpc.hh"
#include "runners/node.hh"

namespace Log {
    void recieve(Node::STATE state, Rpc::TYPE type, int senderId);
    void recieve_leader_response(int rank, int leaderId);
}
