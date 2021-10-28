// Created by dominique on 10/28/21.

#pragma once

#include "fwd.hh"
#include <optional>
#include <wrappers/mpi/rpcRecieverReinjecter.hh>

namespace Node {

    // Foreward declaration
    class Cycle {
    protected:
        Cycle(int timer, Node &node)
            : timer(timer), nextState(std::nullopt), node(node) {}

    public:
        virtual void pre_cycle() = 0;

        virtual bool should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> message) = 0;

        virtual void post_cycle(bool hasTimedOut) = 0;

        bool check_always_should_stop(std::unique_ptr<Rpc::RpcResponse> &message);

        /// GETTER: Node::Cycle::timer
        int Timer() const { return timer; }

        /// GETTER: Node::Cycle::nextState
        std::optional<STATE> NextState() const;

    protected:
        void changeNextState(STATE newState);

    protected:
        int timer;
        std::optional<STATE> nextState;
        Node &node;
    };

}