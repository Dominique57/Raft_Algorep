#include "cycle.hh"
#include "runners/node.hh"
#include "spdlog/spdlog.h"
#include "rpc/requestLeader.hh"


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
            case Rpc::TYPE::MESSAGE:
            case Rpc::TYPE::REQUEST_LEADER:
            case Rpc::TYPE::REQUEST_LEADER_RESPONSE:
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

    void Cycle::request_leader_response(int senderId) {
        auto rpc_response = Rpc::RequestLeaderResponse(leaderId, true);
        MPI::Send_Rpc(rpc_response, senderId);
    }
}
