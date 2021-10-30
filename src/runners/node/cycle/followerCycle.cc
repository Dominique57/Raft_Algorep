#include "followerCycle.hh"
#include <runners/node.hh>
#include <spdlog/spdlog.h>

namespace Node {

    void FollowerCycle::pre_cycle() {
    }

    bool FollowerCycle::should_stop_cycle(std::unique_ptr<Rpc::RpcResponse> rpc) {
        spdlog::info("Follower: Received {} from {}", Rpc::getTypeName(rpc->rpc->Type()), rpc->senderId);
        if (check_always_should_stop(rpc))
            return true;

        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                MPI::Send_Rpc(Rpc::RequestVoteResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::APPEND_ENTRIES:
                MPI::Send_Rpc(Rpc::AppendEntriesResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
            case Rpc::TYPE::MESSAGE:
                break;
        }
        return false;
    }

    void FollowerCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut) {
            spdlog::info("Follower timed out");
            changeNextState(STATE::CANDIDATE);
        }
    }

}
