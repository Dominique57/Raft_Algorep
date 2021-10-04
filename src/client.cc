#include <iostream>
#include "../include/client.hh"

void Client::send(int dst_uid)
{
    MPI_Send(&uid,
              1, // TODO : calculate the right count for the message
              MPI_INT,
              dst_uid,
              0,
              MPI_COMM_WORLD);
}
