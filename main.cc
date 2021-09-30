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
        auto server = Server(rank, half);
        int next = (rank + 1) % half;
        std::cerr << "Next: " << next << std::endl;
        int prev = (rank - 1 < 0)? 0 : (rank - 1) % half;
        std::cerr << "Prev: " << prev << std::endl;

        server.send_leader(next);
        server.send_leader(prev);

        server.received_leader(next);
        server.received_leader(prev);

        std::cout << "I am " << server.uid << " My leader is " << server.leader << std::endl;
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
