#pragma once

#include "rpc.hh"

namespace Rpc {

    /// Rpc representing the heartbeat of the leader and the log synchronization request
    class AppendEntries : public Rpc {
    public:
        /**
         * @brief Constructor
         * @param term          The current term.
         * @param leaderId      The current LEADER's ID.
         */
        AppendEntries(int term, int leaderId)
            : Rpc(TYPE::APPEND_ENTRIES), term(term), leaderId(leaderId) {}

        /**
         * @brief Constructor
         * @param json          The JSON containing the term and the leaderId.
         */
        AppendEntries(const json &json)
            : AppendEntries(json["term"].get<int>(), json["leaderId"].get<int>()) {}

    protected:
        /**
         * @brief Serialize the entries' data into json form.
         * @return The JSON object containing the data.
         */
        json serialize_self() const override {
            return json::object({{"term", term}, {"leaderId", leaderId}});
        }

    public:
        /// Current term of the sender / responder.
        int term;


        /// Current leader id, will allow follower to share that knowledge.
        int leaderId;
    };

    /// Response of sender / responder to Rpc::AppendEntries
    class AppendEntriesResponse : public Rpc {
    public:
        /**
         * @brief Constructor
         * @param term          The current term.
         * @param success       If the entry appended was successful.
         */
        AppendEntriesResponse(int term, bool success)
            : Rpc(TYPE::APPEND_ENTRIES_RESPONSE), term(term), success(success) {}

        /**
         * @brief Constructor
         * @param json          The JSON containing the necessary data.
         */
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
