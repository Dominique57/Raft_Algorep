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
    MPI_Send(&leader,
              1,
              MPI_INT,
              dst_uid,
              0,
              MPI_COMM_WORLD);
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
    std::cout << "from "<< uid <<  " \"received\" from : "<< src_uid<< std::endl;
    if (status.MPI_ERROR == MPI_SUCCESS)
    {
        if (uid < message)
            leader = message;
    }
}

int get_leader_uid(int nb_server)
{
    return 0;

}
