#include "leaderCycle.hh"

#include "runners/node.hh"
#include "rpc/appendEntries.hh"
#include "wrappers/debug/print_log.hh"

namespace Node {

    void LeaderCycle::pre_cycle() {
        node.leaderId = GlobalConfig::rank;

        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst) {
            if (dst == GlobalConfig::rank)
                continue;

            int prevLogTerm = 0;
            int prevLogIndex = node.nextIndex[dst - GlobalConfig::nb_node_min] - 1;
            if (0 <= prevLogIndex && prevLogIndex < (int) node.logs.size())
                prevLogTerm = node.logs[prevLogIndex].term;

            // FIXME: TODO: add leader commit to rpc
            auto rpc = Rpc::AppendEntries(node.term, GlobalConfig::rank, prevLogTerm, prevLogIndex, node.commitIndex);
            for (int i = prevLogIndex + 1; i < (int) node.logs.size(); i++)
                rpc.entries.push_back(node.logs[i]);
            MPI::Send_Rpc(rpc, dst);
        }
    }

    bool LeaderCycle::update_commitIndex()
    {
        assert(node.commitIndex >= (int)node.logs.size());

        int majority = 0;
        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst) {
            if (dst == GlobalConfig::rank) {
                majority += 1;
                continue;
            }
           if (node.matchIndex[dst - GlobalConfig::nb_node_min] >= node.commitIndex + 1)
               majority++;
        }


        assert(node.logs[node.commitIndex + 1].term == node.term);
        if (majority > GlobalConfig::nb_node() / 2) {
            node.commitIndex += 1;
            spdlog::info("Log index {} committed", node.commitIndex);
            return true;
        }
        return false;
    }

    bool LeaderCycle::handle_node_request(std::unique_ptr<Rpc::RpcResponse> rpc) {
        Log::recieve(STATE::LEADER, rpc->rpc->Type(), rpc->senderId);

        if (check_always_should_stop(rpc))
            return true;

        // FIXME: handle node request
        if (rpc->rpc->Type() == Rpc::TYPE::APPEND_ENTRIES_RESPONSE) {
            auto senderId = rpc->senderId;
            auto res = static_cast<Rpc::AppendEntriesResponse*>(rpc->rpc.get());
            if (res->success) {
                node.nextIndex[senderId - GlobalConfig::nb_node_min] = res->newIndex;
                node.matchIndex[senderId - GlobalConfig::nb_node_min] = res->newIndex - 1;
            } else {
                node.nextIndex[senderId - GlobalConfig::nb_node_min] -= 1;
            }
        }

        while (update_commitIndex())
            continue;

        return false;
    }

    void LeaderCycle::post_cycle(bool) {
    }


    void LeaderCycle::handle_client_request(std::unique_ptr<Rpc::RpcResponse> message) {
        if (message->rpc->Type() == Rpc::TYPE::REQUEST_LEADER)
            client_request_leader_response(std::move(message));

        else if (message->rpc->Type() == Rpc::TYPE::REQUEST_CLIENT) {
            auto rpc = static_cast<Rpc::RequestClient *>(message->rpc.get());
            node.logs.emplace_back(node.logs.size(), node.term, rpc->message);
        }
    }

    void LeaderCycle::respond_client_request(int senderId) {
        auto rpc = Rpc::RequestClientResponse(true);
        MPI::Send_Rpc(rpc, senderId);
    }
}
