#pragma once

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>

#include <wrappers/mpi_include.hh>

class Node {
public:
    enum class STATE {
        FOLLOWER = 0,
        LEADER,
        CANDIDATE,
    };

public:
    Node()
        : state(STATE::FOLLOWER)
    {}

    void start();

protected:
    STATE state;
};
