#include <iostream>
#include <thread>
#include <spdlog/spdlog.h>
#include <cstdlib>

#include "config/globalConfig.hh"
#include "runners/node.hh"
#include "mpi.h"
#include "runners/client/client.hh"

/**
 * Function to execute every time we stop the program, no matter what happens.
 */
void executeAtAexit() {
    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

//    if (size < 2)
//    {
//        std::cerr << "Must have at least two process" << std::endl;
//        return 1;
//    }

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

    if (rank < GlobalConfig::nb_node) {
        // Server
        auto node = Node::Node();
        node.start();
    } else {
        // Client
        auto client = Client::Client(2000);
        //usleep(10000000); //TODO: wait until leader elected done by the controller
        //client.request_leader_id();
        usleep(2000); //TODO: wait for controller demande
        std::cout << "couocu4 "<< std::endl;

        json exemple = json::object({ {"one", 1}, {"two", 2} });
        std::cout << "couocu5 "<< std::endl;
        client.send_message(exemple);
    }
    return 0;
}
