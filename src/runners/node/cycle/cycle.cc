// Created by dominique on 10/28/21.

#include "cycle.hh"
#include <runners/node.hh>

namespace Node {

    void Cycle::changeNextState(STATE newState) {
        assert(nextState == std::nullopt && "Changing a state that already changed !");
        nextState = newState;
    }

    std::optional<STATE> Cycle::NextState() const {
        return nextState;
    }
}