#pragma once

#include <mpi.h>
#include <stdio.h>

class Client
{
public:
    Client(int uid)
        : uid(uid)
    {}
    void send(int dst_uid);

private:
    int uid;
};
