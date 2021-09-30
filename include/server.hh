#pragma once

#include <unistd.h>
#include <mpi.h>
#include <stdio.h>

class Server
{
public:
    Server(int uid) : uid(uid)
    {}

    void receive(int src_uid);

private:
    int uid;
};
