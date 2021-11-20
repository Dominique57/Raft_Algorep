#include "client.hh"

#include "wrappers/debug/print_log.hh"
#include "config/globalConfig.hh"
#include "wrappers/mpi_include.hh"
#include "rpc/requestLeader.hh"

namespace Client
{
    Client::Client(const int& timeout_)
        : timeout(timeout_)
    {
        leaderId = this->request_leader_id();
    }

    int Client::request_leader_id()
    {
        bool success = false;
        const int max = GlobalConfig::nb_node_max - GlobalConfig::nb_node_min;

        do {
            const int dst = (std::rand() % max) + GlobalConfig::nb_node_min;

            MPI::Send_Rpc(Rpc::RequestLeader(), dst);

            auto response = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, this->timeout, 0, MPI_COMM_WORLD);

            if (response && response->rpc.get()->Type() == Rpc::TYPE::REQUEST_LEADER_RESPONSE) {
                std::cout << "Receive response of type REQUEST_LEADER_RESPONSE" << std::endl;
                auto resp = static_cast<Rpc::RequestLeaderResponse *>(response->rpc.get());
                success = resp->success;
                if (success) {
                    std::cout << "Success response: leader is " << resp->leaderId << std::endl;
                    leaderId = resp->leaderId;
                }
            }
        } while (!success);

        Log::recieve_leader_response(leaderId);
        return leaderId;
    }

    //TODO: must take all message type
    void Client::send_message(const json &message)
    {
        std::cout << "Client: " << GlobalConfig::rank << " with leader: " << this->leaderId << std::endl;

        //auto rpc = Rpc::AppendEntries(message);
        //MPI::Send_Rpc(rpc, this->leaderId);
        //std::cout << "client " << GlobalConfig::rank << " has sent : \" "\
        //    << message << " \"" << std::endl;
    }

}
