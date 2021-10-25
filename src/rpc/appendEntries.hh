// Created by dominique on 10/26/21.

#pragma once

#include "rpc.hh"

namespace Rpc {

    /// Rpc representing the heartbeat of the leader and the log synchronization request
    class AppendEntries : public Rpc {
    public:
        /// Constructor
        AppendEntries(int term, int leaderId)
            : Rpc(TYPE::APPEND_ENTRIES), term(term), leaderId(leaderId) {}

        /// Constructor
        AppendEntries(const json &json)
            : AppendEntries(json["term"].get<int>(), json["leaderId"].get<int>()) {}

    protected:
        json serialize_self() const override {
            return json::object({{"term", term}, {"leaderId", leaderId}});
        }

    public:
        /// Current term of the sender / responder
        int term;
        /// Current leader id, will allow follower to share that knowledge
        int leaderId;
    };

    /// Response of sender / responder to Rpc::AppendEntries
    class AppendEntriesResponse : public Rpc {
    public:
        /// Constructor
        AppendEntriesResponse(int term, bool success)
            : Rpc(TYPE::APPEND_ENTRIES_RESPONSE), term(term), success(success) {}

        /// Constructor
        AppendEntriesResponse(const json &json)
            : AppendEntriesResponse(json["term"].get<int>(), json["success"].get<bool>()) {}

    protected:
        json serialize_self() const override {
            return json::object({{"term", term}, {"success", success}});
        }

    public:
        /// Current term of the sender / responder
        int term;
        /// True if sender / responder contained entry matching prevLogIndex and prevLogTerm (!TODO!)
        bool success;
    };
}
