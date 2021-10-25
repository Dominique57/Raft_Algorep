// Created by dominique on 10/26/21.

#pragma once

#include "rpc.hh"

namespace Rpc {
    /// Rpc representing a request to vote for a given node (id)
    class RequestVote : public Rpc {
    public:
        /// Constructor
        RequestVote(int term, int candidateId)
            : Rpc(TYPE::REQUEST_VOTE), term(term), candidateId(candidateId) {}

        /// Constructor
        RequestVote(const json &json)
            : RequestVote(json["term"].get<int>(), json["candidateId"].get<int>()) {}

    protected:
        json serialize_self() const override {
            return json::object({{"term", term}, {"candidateId", candidateId}});
        }

    public:
        /// Current term of the sender / responder
        int term;
        /// Id of the candidate node for whom the request has been sent for
        int candidateId;
    };

    /// Rpc response to Rpc::RequestVote
    class RequestVoteResponse : public Rpc {
    public:
        /// Constructor
        RequestVoteResponse(int term, bool voteGranted)
            : Rpc(TYPE::REQUEST_VOTE_RESPONSE), term(term), voteGranted(voteGranted) {}

        /// Constructor
        RequestVoteResponse(const json& json)
            : RequestVoteResponse(json["term"].get<int>(), json["voteGranted"].get<bool>()) {}

    protected:
        json serialize_self() const override {
            return json::object({{"term", term}, {"voteGranted", voteGranted}});
        }

    public:
        /// Current term of the sender / responder
        int term;
        /// True if vote has been granted by sender / responder
        bool voteGranted;
    };
}