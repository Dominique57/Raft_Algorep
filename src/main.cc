#include <iostream>
#include <thread>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <fstream>
#include <string>

#include "config/globalConfig.hh"
#include "mpi.h"
#include "runners/node.hh"
#include "runners/client/client.hh"
#include "runners/controller/controller.hh"

static inline void init_nb_process(int argc, char *argv[], int& nb_node, int& nb_client);

/**
 * Function to execute every time we stop the program, no matter what happens.
 */
void executeAtAexit() {
    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    int nb_node, nb_client;
    init_nb_process(argc, argv, nb_node, nb_client);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != (nb_node + nb_client + 1)) {
        std::cerr << "The number of slots must be equal to nb_node + nb_client + 1" << std::endl;
        return 1;
    }

    if (size < 2) {
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

    {   // Initialisation
        GlobalConfig::initConfig(rank, nb_node, nb_client);
        std::srand(std::time(nullptr) + rank);
        std::stringstream ss;
        ss << "[%^%L%$] [RANK " << GlobalConfig::rank << "] [%H:%M:%S.%f]: %v";
        spdlog::set_pattern(ss.str());
    }

    if (GlobalConfig::is_controller()) {
        // Controller
        auto controller = Controller::Controller();
        if (argc == 2)
            controller.start_command_list(argv[1]);

        controller.start();
    } else if (GlobalConfig::is_node()) {
        // Server
        auto node = Node::Node();
        node.start();
    } else {
        // Client
        auto client = Client::Client(2000);
        client.start();
    }
    return 0;
}

static inline void init_nb_process(int argc, char *argv[], int& nb_node, int& nb_client) {
    std::string str_nb_node;
    std::string str_nb_client;

    if (argc == 2) {
        std::ifstream ifs(argv[1]);
        if (!ifs.is_open()) {
            std::cerr << "Cannot open file: " << argv[1] << '\n'
                    << "Usage: ./algorep [ nb_node nb_client | cmd_file ]" << std::endl;
            exit(EXIT_FAILURE);
        }

        ifs >> str_nb_node;
        ifs >> str_nb_client;

        ifs.close();

    } else if (argc == 3) {
        str_nb_node = argv[1];
        str_nb_client = argv[2];

    } else {
        std::cerr << "Usage: ./algorep [ nb_node nb_client | cmd_file ]" << std::endl;
        exit(EXIT_FAILURE);
    }

    nb_node = std::atoi(str_nb_node.c_str());
    nb_client = std::atoi(str_nb_client.c_str());
}

