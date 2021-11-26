#include "followerCycle.hh"

#include "runners/node.hh"
#include "rpc/appendEntries.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void FollowerCycle::pre_cycle() {}

    bool FollowerCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        if (check_always_should_stop(rpc))
            return true;

        Log::recieve(STATE::FOLLOWER, rpc->rpc->Type(), rpc->senderId);
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                MPI::Send_Rpc(Rpc::RequestVoteResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::APPEND_ENTRIES:
                node.leaderId = rpc->senderId;
                MPI::Send_Rpc(Rpc::AppendEntriesResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
            case Rpc::TYPE::REQUEST_CLIENT:
            case Rpc::TYPE::REQUEST_LEADER:
            case Rpc::TYPE::REQUEST_LEADER_RESPONSE:
            case Rpc::TYPE::CONTROLLER_REQUEST:
                break;
        }
        return false;
    }
    void FollowerCycle::handle_client_request(std::unique_ptr<Rpc::RpcResponse> message)
    {
        if (message->rpc->Type() == Rpc::TYPE::REQUEST_LEADER) {
            client_request_leader_response(std::move(message));
            return;
        }
    }
    void FollowerCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut) {
            spdlog::info("Follower timed out");
            changeNextState(STATE::CANDIDATE);
        }
    }

}
