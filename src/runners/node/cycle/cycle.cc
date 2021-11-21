#include "cycle.hh"

#include "spdlog/spdlog.h"

#include "runners/node.hh"
#include "rpc/requestLeader.hh"
#include "rpc/appendEntries.hh"

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

    void Cycle::client_response(std::unique_ptr<Rpc::RpcResponse> message)
    {
        if (message->rpc->Type() == Rpc::TYPE::REQUEST_LEADER)
        {
            auto rpc = Rpc::RequestLeaderResponse(node.leaderId.value_or(0), node.leaderId.has_value());
            MPI::Send_Rpc(rpc, message->senderId);
        }
    }

    std::ostream& Cycle::has_crashed(std::ostream& out) const {
        const std::string s = node.crash ? "    crashed" : "Not crashed";
        return out << " | " << s;
    }
}
