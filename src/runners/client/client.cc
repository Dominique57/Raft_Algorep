#include "client.hh"

#include "config/globalConfig.hh"
#include "wrappers/mpi_include.hh"
#include "rpc/requestLeader.hh"

namespace Client
{
    int Client::request_leader_id()
    {
        bool success = false;

        do {
        int max = GlobalConfig::size;
        int min = GlobalConfig::size / 2;
        int dst = min + ( std::rand() % ( max - min + 1 ) );

        MPI::Send_Rpc(Rpc::RequestLeader(), dst);

        auto response = static_cast<Rpc::RequestLeaderResponse *>(MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeout)->rpc.get());

        success = response->success;
        if (success)
            leaderId = response->leaderId;

        } while (!success);



    }
}