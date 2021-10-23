#include <iostream>
#include <map>
#include <thread>
#include <spdlog/spdlog.h>
#include <cstdlib>

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
            node.receive_message();
        }
    } else {
        for (int i = 0; i < 4; ++i) {
            node.send_message();
        }
    }
    return 0;
}
