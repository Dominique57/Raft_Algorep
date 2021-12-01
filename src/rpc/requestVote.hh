#pragma once

#include "rpc.hh"

namespace Rpc {
    /// Rpc representing a request to vote for a given node (id).
    class RequestVote : public Rpc {
    public:
        /**
         * @brief Constructor
         * @param term              The candidate's term.
         * @param candidateId       The candidate's ID.
         */
        RequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm)
            : Rpc(TYPE::REQUEST_VOTE), term(term), candidateId(candidateId),
              lastLogIndex(lastLogIndex), lastLogTerm(lastLogTerm)
        {}

        /**
         * @brief Constructor
         * @param json              The JSON containing the term and the candidateId.
         */
        RequestVote(const json &json)
            : RequestVote(
                  json["term"].get<int>(),
                  json["candidateId"].get<int>(),
                  json["lastLogIndex"].get<int>(),
                  json["lastLogTerm"].get<int>()) {}

    protected:
        json serialize_self() const override {
            return json::object({
                {"term", term}, {"candidateId", candidateId},
                {"lastLogIndex", lastLogIndex}, {"lastLogTerm", lastLogTerm},
            });
        }

    public:
        /// Current term of the sender / responder.
        int term;
        /// Id of the candidate node for whom the request has been sent for.
        int candidateId;
        /// Index of candidate’s last log entry
        int lastLogIndex;
        /// Term of candidate’s last log entry
        int lastLogTerm;
    };

    /// Rpc response to Rpc::RequestVote
    class RequestVoteResponse : public Rpc {
    public:
        /**
         * @brief Constructor
         * @param term              The candidate's term.
         * @param voteGranted       True if the follower voted for the candidate.
         */
        RequestVoteResponse(int term, bool voteGranted)
            : Rpc(TYPE::REQUEST_VOTE_RESPONSE), term(term), voteGranted(voteGranted) {}

        /**
         * @brief Constructor
         * @param json              The JSON containing the term and if the follower voted for the candidate.
         */
        RequestVoteResponse(const json& json)
            : RequestVoteResponse(json["term"].get<int>(), json["voteGranted"].get<bool>()) {}

    protected:
        json serialize_self() const override {
            return json::object({{"term", term}, {"voteGranted", voteGranted}});
        }

    public:
        /// Current term of the sender / responder.
        int term;
        /// True if vote has been granted by sender / responder.
        bool voteGranted;
    };
}