#include <iostream>
#include <map>
#include <thread>
#include <functional>

#include "mpi.h"
#include "src/runners/node.hh"


int main(int argc, char *argv[]) {
    int rank, size, len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_library_version(version, &len);
    printf("Hello, world, I am %d of %d, (%s, %d)\n", rank, size, version, len);
    // Main
//    init(rank, size);
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
    // Main
    MPI_Finalize();
    return 0;
}
