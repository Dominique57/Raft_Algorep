#include <iostream>
#include <thread>
#include <spdlog/spdlog.h>
#include <cstdlib>

#include <config/globalConfig.hh>
#include <runners/node.hh>
#include "mpi.h"

void executeAtAexit() {
    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        std::cerr << "Must have at least two process" << std::endl;
        return 1;
    }

    std::atexit(executeAtAexit);
    if (rank == 0) {
        int len;
        char version[MPI_MAX_LIBRARY_VERSION_STRING];
        MPI_Get_library_version(version, &len);
        spdlog::info("Initialisation: I am {} out of {}, ({}, {})", rank, size, version, len);
    }

    { // Initialisation
        GlobalConfig::initConfig(rank, size);
        std::srand(std::time(nullptr) + rank);
        std::stringstream ss;
        ss << "[%^%L%$] [RANK " << GlobalConfig::rank << "] [%H:%M:%S.%f]: %v";
        spdlog::set_pattern(ss.str());
    }

    Node node = Node();
    node.start();
    return 0;
}