#include "cycle.hh"

#include <iomanip>

#include "spdlog/spdlog.h"

#include "config/globalConfig.hh"
#include "runners/node.hh"
#include "rpc/requestLeader.hh"
#include "rpc/appendEntries.hh"
#include "rpc/controllerRequest.hh"

namespace Node {

    void Cycle::changeNextState(STATE newState) {
        assert(nextState == std::nullopt && "Changing a state that already changed !");
        nextState = newState;
    }

    std::optional<STATE> Cycle::NextState() const {
        return nextState;
    }

    bool Cycle::check_always_should_stop(std::unique_ptr<Rpc::RpcResponse> &rpc) {
        int sentTerm = -1;
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                sentTerm = static_cast<Rpc::RequestVote *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::APPEND_ENTRIES:
                sentTerm = static_cast<Rpc::AppendEntries *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
                sentTerm = static_cast<Rpc::RequestVoteResponse *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
                sentTerm = static_cast<Rpc::AppendEntriesResponse *>(rpc->rpc.get())->term;
                break;
            case Rpc::TYPE::REQUEST_LEADER:
            case Rpc::TYPE::REQUEST_LEADER_RESPONSE:
            case Rpc::TYPE::REQUEST_CLIENT:
            case Rpc::TYPE::REQUEST_CLIENT_RESPONSE:
            case Rpc::TYPE::CONTROLLER_REQUEST:
                break;
        }
        if (sentTerm != -1 && sentTerm > node.term) {
            node.term = sentTerm;
            node.votedFor = std::nullopt;
            if (node.state != STATE::FOLLOWER) {
                changeNextState(STATE::FOLLOWER);
                node.rpcReciever.reinject_rpc(std::move(rpc));
                return true;
            }
        }
        return false;
    }

    void Cycle::client_request_leader_response(std::unique_ptr<Rpc::RpcResponse> message) {
        auto rpc = Rpc::RequestLeaderResponse(node.leaderId.value_or(0), node.leaderId.has_value());
        MPI::Send_Rpc(rpc, message->senderId);
    }

    static const std::string has_crashed(const bool& crash) {
        return crash ? "crashed" : "Not crashed";
    }

    void Cycle::handle_controller_request(const Rpc::RpcResponse *rpc) {
        const auto request = static_cast<Rpc::ControllerRequest*>(rpc->rpc.get());
        switch (request->type) {
        case Rpc::CONTROLLER_REQUEST_TYPE::STATUS:
            std::cout << "Node   | " << GlobalConfig::rank
                << " | " << std::setw(9) << getStateName(node.state)
                << " | " << std::setw(11) << has_crashed(node.crash)
                << " | " << std::setw(6) << Clock::getSpeedTypeName(node.clock.speed) << " speed"
                << std::endl;
            break;

        case Rpc::CONTROLLER_REQUEST_TYPE::CRASH:
            std::cout << "Node " << GlobalConfig::rank << " crashed" << std::endl;
            node.crash = true;
            break;

        case Rpc::CONTROLLER_REQUEST_TYPE::SPEED:
            std::cout << "Node " << GlobalConfig::rank << " set speed to " << request->message << std::endl;
            node.clock.speed = Clock::getSpeedType(request->message);
            break;

        case Rpc::CONTROLLER_REQUEST_TYPE::RECOVERY:
            node.crash = false;
            // FIXME : TODO
            break;

        case Rpc::CONTROLLER_REQUEST_TYPE::PRINT_LOG:
            std::cout << "Node " << GlobalConfig::rank << " logs: \n" << node.logs;
            break;
        default:
            std::cout << "Unknown controller request" << std::endl;
            break;
        }
    }
}
