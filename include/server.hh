#pragma once

#include <unistd.h>
#include <vector>
#include <mpi.h>
#include <stdio.h>

class Server
{
public:
    Server(int uid, int next, int prev)
            : uid_(uid),
            leader_(uid),
            next_(next),
            prev_(prev)
    {}

    /**
     * \brief receive message from src_uid
     * @param src_uid The uid to get the message from
     */
    void receive(int src_uid);
    /**
     * \brief send message to dst_uid
     * @param dst_uid The uid to send the message from
     */
    void send_leader(int dst_uid);

    void received_leader(int src_uid);

    void leader_election();

    int get_leader() {
        return leader_;
    }

    int get_uid() {
        return uid_;
    }

    int get_next() {
        return next_;
    }

    int get_prev() {
        return prev_;
    }

private:
    std::vector<int> messages;

    int leader_;
    int uid_;

    int prev_;
    int next_;
};

/**
 * \brief elect the uid
 * @param nb_server number of servers
 */
int get_leader_uid(int nb_server);