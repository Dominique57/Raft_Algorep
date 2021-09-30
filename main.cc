#include <mpi.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

#include "client.hh"
#include "server.hh"

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        std::cerr << "Must have at least two process" << std::endl;
        MPI_Finalize();
        return 1;
    }
    int uid_client = 0;
    int uid_server = 1;

    if (rank == uid_client)
    {
        std::cerr << "CLIENT STARTED : uid=" << uid_client << std::endl;
        auto client = Client(uid_client);
        client.send(uid_server);
    }

    if (rank == uid_server)
    {
        std::cerr << "SERVER STARTED uid=" << uid_server << std::endl;
        auto server = Server(uid_server);
        server.receive(uid_client);
    }
    MPI_Finalize();

    return 0;
}
