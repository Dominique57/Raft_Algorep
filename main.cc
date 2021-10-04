#include <mpi.h>
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

    int half = size / 2;

    /* severs will be process with uid from [0;half[
     * again why not? ┐(‘～` )┌ ??
     */

    if (rank < half)
    {
        std::cerr << "SERVER STARTED uid=" << rank << std::endl;
        auto server = Server(rank,
                             (rank + 1) % half,
                             (rank - 1 < 0) ? half - 1 : (rank - 1) % half);

        server.leader_election();

        std::cout << "I am " << server.get_uid() << " My leader is "
                  << server.get_leader() << std::endl;
    }
    /* clients will be process with uid from [half;size[
     * because why not? ┐(‘～` )┌ ??
     */
    if (rank >= half)
    {
        std::cerr << "CLIENT STARTED : uid=" << rank << std::endl;
        auto client = Client(rank);
        client.send(get_leader_uid(0));
    }


    MPI_Finalize();

    return 0;
}
