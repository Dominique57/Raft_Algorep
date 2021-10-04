#pragma once

#include <unistd.h>
#include <vector>
#include <mpi.h>
#include <stdio.h>

class Server
{
public:
    Server(int uid, int nb_messages)
            : uid(uid),
            leader(uid)
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

    bool received_leader(int src_uid);

    int leader;
    int uid;
private:
    std::vector<int> messages;
};

/**
 * \brief elect the uid
 * @param nb_server number of servers
 */
int get_leader_uid(int nb_server);