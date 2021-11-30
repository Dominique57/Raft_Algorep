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

    void LeaderCycle::update_commitIndex()
    {
        // 1 for self vote
        int majority = 1;
        for (auto dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst)
        {
           if (node.matchIndex[dst] >= node.commitIndex + 1)
                majority++;
        }


        //FIXME : do we save in file?
        if (majority > GlobalConfig::nb_node() / 2
            && (node.commitIndex + 1 < (int) node.logs.size()
                    && node.logs[node.commitIndex + 1].term == node.term))
            node.commitIndex += 1;
        //std::cout << majority << " " << node.commitIndex << std::endl;
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

        update_commitIndex();

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
