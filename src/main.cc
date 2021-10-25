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

    GlobalConfig::initConfig(rank, size);
    std::srand(std::time(nullptr) + rank);
    auto pattern = std::string("[RANK: ") + std::to_string(rank) + "] %+";
    spdlog::set_pattern(pattern);

    Node node = Node();
    if (rank == 0) {
        node.start();
//        while (true) {
//            auto rpcReponse = MPI::Recv_Rpc(MPI_ANY_SOURCE);
//            if (rpcReponse == nullptr) {
//                usleep(100 * 1000);
//                continue;
//            }
//            auto& msg = rpcReponse->rpc;
//            if (msg->Type() == Rpc::TYPE::MESSAGE) {
//                auto value = static_cast<Rpc::Message*>(msg.get());
//                spdlog::info("Received message: {}", value->message);
//            } else {
//                spdlog::warn("Forgot to handle case here !");
//            }
//        }
    } else {
        node.start();
//        auto rpc = Rpc::Message(std::string("Hello"));
//        MPI::Send_Rpc(rpc, 0);
    }
    return 0;
}
