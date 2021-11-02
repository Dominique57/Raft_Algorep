#pragma once

#include "rpc.hh"

namespace Rpc {
    class RequestLeader : public Rpc {
    public:
        /**
         * @brief Construct a request to ask for the leader
         */
        RequestLeader()
            : Rpc(TYPE::REQUEST_LEADER) {}

        /**
         * @brief Construct a request to ask for the leader
         */
        RequestLeader(const json &json)
            : RequestLeader() {}

    protected:
        /**
         * @brief Serialize the entries' data into json form.
         * @return The JSON object containing the data (in this case, nothing).
         */
        json serialize_self() const override {
            return json::object();
        }
    };

    class RequestLeaderResponse : public Rpc {
    public:
        /**
         * @brief Construct a REQUEST_LEADER_RESPONSE RPC message
         * @param leaderId      The current LEADER's ID.
         * @param success       If we successfully retrieved the leader ID or not.
         */
        RequestLeaderResponse(int leaderId, bool success)
            : Rpc(TYPE::REQUEST_LEADER_RESPONSE), leaderId(leaderId), success(success) {}

        /**
         * @brief Constructor
         * @param json          The JSON containing the term and the leaderId.
         */
        RequestLeaderResponse(const json &json)
            : RequestLeaderResponse(json["leaderId"].get<int>(), json["success"].get<bool>()) {}

    protected:
        /**
         * @brief Serialize the entries' data into json form.
         * @return The JSON object containing the data.
         */
        json serialize_self() const override {
            return json::object({{"leaderId", leaderId}, {"success", success}});
        }

    public:
        /// ID the current leader
        int leaderId;

        /// True if we received the leader's ID
        bool success;
    };
}
