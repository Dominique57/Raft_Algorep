#pragma once

#include <mpi.h>
#include <stdio.h>

class Client
{
public:
    Client(int uid)
        : uid(uid)
    {}
    /**
     * \brief send message to dst_uid
     * @param dst_uid The uid to send the message from
     */
    void send(int dst_uid);

private:
    int uid;
};
