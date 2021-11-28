#include "followerCycle.hh"

#include "runners/node.hh"
#include "rpc/appendEntries.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void FollowerCycle::pre_cycle() {}

    bool FollowerCycle::check_entry_consistency(const Rpc::AppendEntries &appendEntries) {
        // if log doesn’t contain an entry at prevLogIndex whose term matches prevLogTerm
        if (appendEntries.prevLogIndex != -1
            && (appendEntries.prevLogIndex >= (int) node.logs.size()
            || (node.logs.at(appendEntries.prevLogIndex).term != appendEntries.prevLogTerm))) {
            return false;
        }
        return true;
    }

//    1. Reply false if term < currentTerm (§5.1)
//
//    2. Reply false if log doesn’t contain an entry at prevLogIndex
//    whose term matches prevLogTerm (§5.3)
//    3. If an existing entry conflicts with a new one (same index
//        but different terms), delete the existing entry and all that
//        follow it (§5.3)
//    4. Append any new entries not already in the log
//
//    5. If leaderCommit > commitIndex, set commitIndex =
//        min(leaderCommit, index of last new entry)

    void FollowerCycle::append_entries(const Rpc::AppendEntries &appendEntries) {
        for (unsigned i = 0; i < appendEntries.entries.size(); i++) {
            //auto idx = appendEntries.prevLogIndex
            node.logs.emplace_back(appendEntries.entries[i]);
        }
    }

    void FollowerCycle::handle_append_entries(std::unique_ptr<Rpc::RpcResponse>& rpc) {
        node.leaderId = rpc->senderId;

        const auto msg = static_cast<Rpc::AppendEntries *>(rpc->rpc.get());
        if (!check_entry_consistency(*msg)) {
            MPI::Send_Rpc(Rpc::AppendEntriesResponse(node.term, false, -1), rpc->senderId);
            return;
        }

        if (msg->entries.empty())
            return;

        append_entries(*msg);
        MPI::Send_Rpc(Rpc::AppendEntriesResponse(node.term, true, (int) node.logs.size()), rpc->senderId);
    }

    bool FollowerCycle::handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) {
        if (check_always_should_stop(rpc))
            return true;

        Log::recieve(STATE::FOLLOWER, rpc->rpc->Type(), rpc->senderId);
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                MPI::Send_Rpc(Rpc::RequestVoteResponse(0, true), rpc->senderId);
                return true;
            case Rpc::TYPE::APPEND_ENTRIES:
                handle_append_entries(rpc);
                return true;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
            case Rpc::TYPE::REQUEST_CLIENT:
            case Rpc::TYPE::REQUEST_CLIENT_RESPONSE:
            case Rpc::TYPE::REQUEST_LEADER:
            case Rpc::TYPE::REQUEST_LEADER_RESPONSE:
            case Rpc::TYPE::CONTROLLER_REQUEST:
                break;
        }
        return false;
    }

    void FollowerCycle::handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) {
        if (message->rpc->Type() == Rpc::TYPE::REQUEST_LEADER)
            client_request_leader_response(std::move(message));

        // FIXME: return va te faire foutre
    }

    void FollowerCycle::post_cycle(bool hasTimedOut) {
        if (hasTimedOut) {
            spdlog::info("Follower timed out");
            changeNextState(STATE::CANDIDATE);
        }
    }

}
