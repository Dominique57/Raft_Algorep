#include "followerCycle.hh"

#include "runners/node.hh"
#include "rpc/appendEntries.hh"
#include "rpc/controllerRequest.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void FollowerCycle::pre_cycle() {
        if (postLeader != -1)
            leaderId = postLeader;
    }

    bool FollowerCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        if (check_always_should_stop(rpc))
            return true;

        Log::recieve(STATE::FOLLOWER, rpc->rpc->Type(), rpc->senderId);
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                leaderId = rpc->senderId;
                MPI::Send_Rpc(Rpc::RequestVoteResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::APPEND_ENTRIES:
                MPI::Send_Rpc(Rpc::AppendEntriesResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
            case Rpc::TYPE::MESSAGE:
            case Rpc::TYPE::REQUEST_LEADER:
            case Rpc::TYPE::REQUEST_LEADER_RESPONSE:
            case Rpc::TYPE::CONTROLLER_REQUEST:
                break;
        }
        return false;
    }

    void FollowerCycle::handle_controller_request(const Rpc::RpcResponse *rpc) {
        auto message = static_cast<Rpc::ControllerRequest*>(rpc->rpc.get());
        switch (message->type) {
        case Rpc::CONTROLLER_REQUEST_TYPE::STATUS:
            std::cout << "Node | " << GlobalConfig::rank << " | Follower ";
            has_crashed(std::cout) << std::endl;
            break;
        case Rpc::CONTROLLER_REQUEST_TYPE::CRASH:
            std::cout << "Node " << GlobalConfig::rank << " crashed" << std::endl;
            node.crash = true;
            break;
        default:
            break;
        }
    }

    void FollowerCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut) {
            spdlog::info("Follower timed out");
            changeNextState(STATE::CANDIDATE);
        }
    }

}
