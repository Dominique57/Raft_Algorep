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
