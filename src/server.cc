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


//    if (status.MPI_ERROR == MPI_SUCCESS)
//        printf("received : %d\n", message);
//    else
//        printf("Failed pong :(");
}

void Server::send_leader(int dst_uid)
{
    MPI_Send(&leader_,
              1,
              MPI_INT,
              dst_uid,
              0,
              MPI_COMM_WORLD);
    std::cerr << "Server n*" << uid_ << " sent " << leader_ << " to " << dst_uid << std::endl;
}

void Server::received_leader(int src_uid)
{
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
    std::cerr << "Server n*" << uid_ << " received " << message << " from " << src_uid << std::endl;
    if (status.MPI_ERROR == MPI_SUCCESS)
    {
        if (uid_ < message) {
            leader_ = message;
        }
    }
}

void Server::leader_election()
{
    send_leader(next_);
    send_leader(prev_);

    received_leader(next_);
    received_leader(prev_);

    if (leader_ > uid_)
        leader_election();
}

int get_leader_uid(int nb_server)
{
    return 0;
}
