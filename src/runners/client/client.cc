#include "client.hh"

#include "wrappers/debug/print_log.hh"
#include "config/globalConfig.hh"
#include "wrappers/mpi_include.hh"
#include "rpc/requestLeader.hh"

namespace Client
{
    Client::Client(const int& timeout_)
        : timeout(timeout_), clock(Clock::SPEED_TYPE::HIGH)
    {
        leaderId = this->request_leader_id();
        first_start = false;
        request = std::nullopt;
    }

    void Client::set_start()
    {
        first_start = true;
    }

    void Client::set_request(const json& request_)
    {
        request = std::optional<Rpc::RequestClient>{request_};
    }

    void Client::start()
    {
        while(true)
        {
            if (first_start)
            {
                request_leader_id();
                first_start = false;
            }
            if (request)
            {
                send_request();
                request = std::nullopt;
            }
        }
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
                auto resp = static_cast<Rpc::RequestLeaderResponse *>(response->rpc.get());
                success = resp->success;
                if (success)
                    leaderId = resp->leaderId;
            }
        } while (!success);

        Log::recieve_leader_response(leaderId);
        return leaderId;
    }

    //TODO: clean message notification
    void Client::send_request() {
        if (request)
        {
            MPI::Send_Rpc(request.value(), leaderId);
            std::cout << "client " << GlobalConfig::rank << " has sent : \" " << request.value().message << " \"" << std::endl;
        }
        else
            std::cout << "could not send request" << std::endl;
    }

}
