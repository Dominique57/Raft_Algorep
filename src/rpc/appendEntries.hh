#pragma once

#include "rpc.hh"

#include <runners/node/entry.hh>
#include <utility>

namespace Rpc {

    /// Rpc representing the heartbeat of the leader and the log synchronization request
    class AppendEntries : public Rpc {
    public:
        /**
         * @brief constructor appendEntries command
         * @param term          the current term.
         * @param leaderId      the current leader's id.
         * @param prevLogTerm   the term of prevlogindex entry.
         * @param prevLogIndex  the index of the log entry immediately precceding new ones.
         * @param leaderCommit  the leader´s commitindex.
         * @param entries       The log entries to store (empty for heartbeat: may send more than one for efficiency)
         */
        AppendEntries(int term, int leaderId, int prevLogTerm, int prevLogIndex, int leaderCommit, std::vector<Entry> entries)
            : Rpc(TYPE::APPEND_ENTRIES), term(term), leaderId(leaderId), prevLogIndex(prevLogIndex),
              prevLogTerm(prevLogTerm), leaderCommit(leaderCommit), entries(std::move(entries))
        {}

        /**
         * @brief constructor appendEntries heartbeat
         * @param term          the current term.
         * @param leaderId      the current leader's id.
         * @param prevLogTerm   the term of prevlogindex entry
         * @param prevLogIndex  the index of the log entry immediately precceding new ones
         * @param leaderCommit  the leader´s commitindex
         */
        AppendEntries(int term, int leaderId, int prevLogTerm, int prevLogIndex, int leaderCommit)
            : AppendEntries(term, leaderId, prevLogTerm, prevLogIndex, leaderCommit, {})
        {}

        /**
         * @brief Constructor
         * @param json          The JSON containing the data.
         */
        AppendEntries(const json &json)
            : AppendEntries(json["term"].get<int>(),
                json["leaderId"].get<int>(),
                json["prevLogTerm"].get<int>(),
                json["prevLogIndex"].get<int>(),
                json["leaderCommit"].get<int>(),
                json["entries"].get<std::vector<Entry>>()) {}

    protected:
        /**
         * @brief Serialize the entries' data into json form.
         * @return The JSON object containing the data.
         */
        json serialize_self() const override {
            return json::object({
                {"term", term}, {"leaderId", leaderId}, {"prevLogTerm", prevLogTerm},
                {"prevLogIndex", prevLogIndex}, {"leaderCommit", leaderCommit},
                {"entries", entries}
            });
        }

    public:
        /// Current term of the sender / responder.
        int term;

        /// Current leader id, will allow follower to share that knowledge.
        int leaderId;

        /// Index of log entry immediately preceding new ones
        int prevLogIndex;

        /// Term of prevLogIndex entry
        int prevLogTerm;

        /// Leader’s commitIndex
        int leaderCommit;

        /// Entries to send
        std::vector<Entry> entries;
    };

    /// Response of sender / responder to Rpc::AppendEntries
    class AppendEntriesResponse : public Rpc {
    public:
        /**
         * @brief Constructor
         * @param term          The current term.
         * @param success       If the entry appended was successful.
         * @param newIndex      The new index after appending the entries to the system's logs.
         */
        AppendEntriesResponse(int term, bool success, int newIndex)
            : Rpc(TYPE::APPEND_ENTRIES_RESPONSE), term(term),
              success(success), newIndex(newIndex)
        {}

        /**
         * @brief Constructor
         * @param json          The JSON containing the necessary data.
         */
        AppendEntriesResponse(const json &json)
            : AppendEntriesResponse(
                  json["term"].get<int>(),
                  json["success"].get<bool>(),
                  json["newIndex"].get<int>()) {}

    protected:
        json serialize_self() const override {
            return json::object({{"term", term}, {"success", success}, {"newIndex", newIndex}});
        }

    public:
         /// Current term of the sender / responder
        int term;

         /// True if sender / responder contained entry matching prevLogIndex and prevLogTerm
        bool success;

        /// New index of the node if success, -1 otherwise
        int newIndex;
    };
}
