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
        // We do not follow exactly raft implementation as we remove everything
        // after prevLogIndex to sync with leader
        node.logs.resize(appendEntries.prevLogIndex + 1);

        for (const auto &entry : appendEntries.entries) {
            assert(entry.index == (int) node.logs.size() && "Attempted to add log with inconsistent index");
            node.logs.emplace_back(entry);
        }
    }

    void FollowerCycle::handle_append_entries(std::unique_ptr<Rpc::RpcResponse>& rpc) {
        const auto msg = static_cast<Rpc::AppendEntries *>(rpc->rpc.get());

        if (msg->term < node.term) {
            MPI::Send_Rpc(Rpc::AppendEntriesResponse(node.term, false, (int) node.logs.size()), rpc->senderId);
            return;
        }

        node.leaderId = rpc->senderId;

        if (!check_entry_consistency(*msg)) {
            MPI::Send_Rpc(Rpc::AppendEntriesResponse(node.term, false, (int) node.logs.size()), rpc->senderId);
            return;
        }

        append_entries(*msg);

        assert(msg->leaderCommit < (int)node.logs.size() && "Attempted to commit non-existing log !");
        for (auto index = node.commitIndex + 1; index <= msg->leaderCommit; ++index) {
            spdlog::info("Committed {} (cmd: {})", index, node.logs[index].cmd);
        }
        node.commitIndex = msg->leaderCommit;

        if (!msg->entries.empty())
            MPI::Send_Rpc(Rpc::AppendEntriesResponse(node.term, true, (int) node.logs.size()), rpc->senderId);
    }

    void FollowerCycle::handle_request_vote(std::unique_ptr<Rpc::RpcResponse>& rpc) {
        auto reqVote = static_cast<Rpc::RequestVote*>(rpc->rpc.get());

        spdlog::warn("Received reqVote (id: {}, term: {}, lastLogIndex: {}, lastLogTerm: {})", reqVote->candidateId, reqVote->term, reqVote->lastLogIndex, reqVote->lastLogTerm);
        if (reqVote->term < node.term) {
            // If older request, reply false to force candidate detect newer term and convert to follower
            MPI::Send_Rpc(Rpc::RequestVoteResponse(node.term, false), rpc->senderId);
        } else if (node.votedFor.has_value()) {
            // If follower has already voted for someone, cant revote for someone else
            MPI::Send_Rpc(Rpc::RequestVoteResponse(node.term,  node.votedFor.value() == rpc->senderId), rpc->senderId);
        } else if (node.logs.empty()
                   || node.logs.back().term < reqVote->lastLogTerm
                   || (node.logs.back().term == reqVote->lastLogTerm && (int) node.logs.size() - 1 < reqVote->lastLogIndex)) {
            // Add check that the candidate's log is at least as up-to-date as the receiver's log
            // Register vote AND reply voteGranted: success
            node.votedFor = rpc->senderId;
            MPI::Send_Rpc(Rpc::RequestVoteResponse(node.term, true), rpc->senderId);
        } else {
            MPI::Send_Rpc(Rpc::RequestVoteResponse(node.term, false), rpc->senderId);
        }
    }

    bool FollowerCycle::handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) {
        if (check_always_should_stop(rpc))
            return true;

        Log::recieve(STATE::FOLLOWER, rpc->rpc->Type(), rpc->senderId);
        switch (rpc->rpc->Type()) {
            case Rpc::TYPE::REQUEST_VOTE:
                handle_request_vote(rpc);
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
