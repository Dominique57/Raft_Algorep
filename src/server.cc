#include <iostream>
#include "../include/server.hh"

void Server::receive(int src_uid)
{
    sleep(1);
    MPI_Status status;
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    int message;
    MPI_Recv(&message,
             count,
             MPI_INT,
             src_uid,
             0,
             MPI_COMM_WORLD,
             &status);


    if (status.MPI_ERROR == MPI_SUCCESS)
        printf("received : %d\n", message);
    else
        printf("Failed pong :(");
}

void Server::send_leader(int dst_uid)
{
    MPI_Send(&leader_,
              1,
              MPI_INT,
              dst_uid,
              0,
              MPI_COMM_WORLD);
//    std::cerr << "Server n*" << uid_ << " sent " << leader_ << " to " << dst_uid << std::endl;
}

int Server::received_leader(int src_uid)
{
//    std::cerr << uid_ << " waiting for message from " << src_uid << std::endl;
    MPI_Status status;
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    int message;
    MPI_Recv(&message,
             1,
             MPI_INT,
             src_uid,
             0,
             MPI_COMM_WORLD,
             &status);
    if (status.MPI_ERROR == MPI_SUCCESS)
    {
//        std::cerr << "Server n*" << uid_ << " received " << message << " from " << src_uid << std::endl;
        return message;
    }
//    std::cerr << "No message received" << std::endl;
    return leader_;
}


bool Server::end_condition(int src_uid)
{
    // We receive the message from our neighbor
    int message = received_leader(src_uid);

    // If we receive the same message as the leader we already have
    // Then we broadcast the leader
    // And stops the election
    if (message == leader_)
    {
        if (src_uid == prev_)
            send_leader(next_);
        if (src_uid == next_)
            send_leader(prev_);
        return true;
    }

    // If the message we received is higher than our current leader
    // Then this becomes our lead
    if (message > leader_)
        leader_ = message;
    return false;
}

void Server::leader_election()
{
    send_leader(next_);
    send_leader(prev_);

    if (end_condition(next_) || end_condition(prev_))
        return;

    leader_election();
}


int get_leader_uid(int nb_server)
{
    return 0;
}
