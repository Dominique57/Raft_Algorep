#include "client.hh"

#include <iomanip>

#include "config/globalConfig.hh"
#include "rpc/requestLeader.hh"
#include "rpc/controllerRequest.hh"
#include "wrappers/debug/print_log.hh"
#include "wrappers/mpi_include.hh"

namespace Client {

    Client::Client(const int& timer_)
        : run(false),
        timer(timer_),
        request(std::nullopt),
        clock(Clock::SPEED_TYPE::HIGH)
    {
        this->request_leader_id();
    }

    void Client::set_request(const json& request_) {
        request = std::optional<Rpc::RequestClient>{request_};
    }

    void Client::start() {
        while (true) {
            this->clock.wait();

            this->handle_requests();

            if (!this->run)
                continue;

            // TODO: always request leader id ? or only when leader dead ? When dead, leaderId value ?
            this->request_leader_id();

            if (this->request)
                this->send_request();
        }
    }

    void Client::request_leader_id() {
        bool success = false;
        const int max = GlobalConfig::nb_node_max - GlobalConfig::nb_node_min + 1;

        do {
            const int dst = (std::rand() % max) + GlobalConfig::nb_node_min;

            MPI::Send_Rpc(Rpc::RequestLeader(), dst);

            auto response = MPI::Recv_Rpc_Timeout(MPI_ANY_SOURCE, this->timer, 0, MPI_COMM_WORLD);

            if (response && response->rpc.get()->Type() == Rpc::TYPE::REQUEST_LEADER_RESPONSE) {
                auto resp = static_cast<Rpc::RequestLeaderResponse *>(response->rpc.get());
                success = resp->success;
                if (success)
                    this->leaderId = resp->leaderId;
            }
        } while (!success);

        Log::recieve_leader_response(this->leaderId);
    }

    void Client::send_request() {

        bool success = false;

        do {
            MPI::Send_Rpc(this->request.value(), this->leaderId);

            auto response = MPI::Recv_Rpc_Timeout(this->leaderId, this->timer, 0, MPI_COMM_WORLD);

            if (response && response->rpc.get()->Type() == Rpc::TYPE::REQUEST_CLIENT_RESPONSE) {
                auto resp = static_cast<Rpc::RequestClientResponse *>(response->rpc.get());
                success = resp->success;
            }
        } while (!success);

      //  Log::recieve_Client_response(this->leaderId);

        std::cout << "client " << GlobalConfig::rank << " has sent : \" " << this->request.value().message << " \"" << std::endl;

        this->request = std::nullopt;
    }

    void Client::handle_requests() {
        // Set timeout beginning
        auto start = std::chrono::steady_clock::now();

        std::unique_ptr<Rpc::RpcResponse> rpcResponse = nullptr;
        bool hasTimedOut = false;
        do {
            auto cur = std::chrono::steady_clock::now();
            long countTime = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
            long timeToWait = this->timer - countTime;
            rpcResponse = rpcReciever.get_rpc_timeout(MPI_ANY_SOURCE, timeToWait);
            hasTimedOut = rpcResponse == nullptr;

            if (!hasTimedOut) {
                auto type = rpcResponse->rpc->Type();
                if (type == Rpc::TYPE::CONTROLLER_REQUEST)
                    this->handle_controller_request(rpcResponse.get());
            }
        } while (!hasTimedOut);
    }

    static const std::string has_started(const bool& start) {
        return start ? "started" : "Not started";
    }

    void Client::handle_controller_request(const Rpc::RpcResponse *rpc) {
        const auto request = static_cast<Rpc::ControllerRequest*>(rpc->rpc.get());
        switch (request->type) {
        case Rpc::CONTROLLER_REQUEST_TYPE::STATUS:
            std::cout << "Client | " << GlobalConfig::rank
                << " | " << std::setw(11) << has_started(this->run)
                << " | " << std::setw(6) << Clock::getSpeedTypeName(this->clock.speed) << " speed"
                << std::endl;
            break;
        case Rpc::CONTROLLER_REQUEST_TYPE::CRASH:
            std::cout << "Client " << GlobalConfig::rank << " crashed" << std::endl;
            this->run = false;
            break;
        case Rpc::CONTROLLER_REQUEST_TYPE::START:
            std::cout << "Client " << GlobalConfig::rank << " started" << std::endl;
            this->run = true;
            break;
        case Rpc::CONTROLLER_REQUEST_TYPE::SPEED:
            std::cout << "Client " << GlobalConfig::rank << " set speed to " << request->message << std::endl;
            clock.speed = Clock::getSpeedType(request->message);
            break;
        default:
            std::cout << "Unkown controller request" << std::endl;
            break;
        }
    }

}
