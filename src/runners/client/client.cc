#include "client.hh"

#include "config/globalConfig.hh"
#include "wrappers/mpi_include.hh"
#include "wrappers/debug/print_log.hh"
#include "rpc/requestLeader.hh"

namespace Client
{
    int Client::request_leader_id()
    {
        bool success = false;

        do {
            int max = GlobalConfig::nb_node;
            int dst = (std::rand() %  max );

            MPI::Send_Rpc(Rpc::RequestLeader(), dst);

            auto response = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, timeout, 0, MPI_COMM_WORLD);

            if (response && response->rpc.get()->Type() == Rpc::TYPE::REQUEST_LEADER_RESPONSE)
            {
                auto resp = static_cast<Rpc::RequestLeaderResponse *>(response->rpc.get());
                success = resp->success;
                if (success)
                    leaderId = resp->leaderId;
            }
        }while (!success);

        //TODO : fix compilation
        //Log::recieve_leader_response(GlobalConfig::rank, leaderId);
        return leaderId;
    }

    //TODO: must take all message type
    void Client::send_message(std::string message)
    {
        auto rpc = Rpc::Message(message);
        MPI::Send_Rpc(rpc, leaderId);
    }

}
