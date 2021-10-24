#include <iostream>
#include <map>
#include <thread>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <wrappers/mpi_include.hh>

#include "mpi.h"
#include "src/runners/node.hh"

void executeAtAexit() {
    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::atexit(executeAtAexit);

    if (rank == 0) {
        int len;
        char version[MPI_MAX_LIBRARY_VERSION_STRING];
        MPI_Get_library_version(version, &len);
        spdlog::info("Hello, world, I am {} out of {}, ({}, {})", rank, size, version, len);
    }

    if (size < 2)
    {
        std::cerr << "Must have at least two process" << std::endl;
        return 1;
    }

    std::srand(std::time(nullptr));
    Node node = Node(rank);
    if (rank == 0) {
        while (true) {
            try {
                auto msg = MPI::Recv_Rpc(MPI_ANY_SOURCE);
                spdlog::info("Received message: {}", msg.message_);
            } catch (const std::logic_error &) {
                usleep(100 * 1000);
            }
        }
    } else {
        auto rpc = Rpc(std::string("Hello"));
        MPI::Send_Rpc(rpc, 0);
    }
    return 0;
}
