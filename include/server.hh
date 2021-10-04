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

    /**
     * \brief receive message from src_uid and returns it
     * @param src_uid The uid to receive the message from
     */
    int received_leader(int src_uid);

    /**
     * \brief runs the leader election and chooses a leader for the server based on HS algorithm
     * @param none
     */
    void leader_election();

    /**
     * \brief returns this server current leader's id
     * @param none
     */
    int get_leader() {
        return leader_;
    }

    /**
     * \brief returns this server's id
     * @param none
     */
    int get_uid() {
        return uid_;
    }

    /**
     * \brief returns this server next neighbor's id
     * @param none
     */
    int get_next() {
        return next_;
    }

    /**
     * \brief returns this server previous neighbor's id
     * @param none
     */
    int get_prev() {
        return prev_;
    }

private:
    /**
     * \brief server's log (for future uses)
     */
    std::vector<int> messages;

    /**
     * \brief receives a message from src_uid, compares it with the current server's leader's ID, stops the current election if already received or continue
     * @param src_uid The uid to get the message from
     */
    bool end_condition(int src_uid);

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