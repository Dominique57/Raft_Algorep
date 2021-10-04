#include <mpi.h>
#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <functional>

#include "client.hh"
#include "server.hh"

void timer_start(std::function<void(void)> func, unsigned int interval)
{
    std::thread([func, interval]()
    {
        while (true)
        {
            auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
            func();
            std::this_thread::sleep_until(x);
        }
    }).detach();
}

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
    std::map<int, Server> servers;

    /* severs will be process with uid from [0;half[
     * again why not? ┐(‘～` )┌ ??
     */

    if (rank < half)
    {
        std::cerr << "SERVER STARTED uid=" << rank << std::endl;
        servers[rank] = Server(rank,
                             (rank + 1) % half,
                             (rank - 1 < 0) ? half - 1 : (rank - 1) % half);

        servers[rank].leader_election();

        std::cerr << "I am " << servers[rank].get_uid() << " My leader is "
                  << servers[rank].get_leader() << std::endl;

        timer_start((servers[rank].leader_election()), 1000);
        while (true);
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
