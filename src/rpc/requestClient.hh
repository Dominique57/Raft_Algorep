#pragma once

#include "rpc.hh"

//TODO: change the message for a json or something that corespond to client requests
namespace Rpc {

    /// Simple Rpc class that sends a string
    class RequestClient : public Rpc {
    public:
        /**
         * @brief Constructor with string representation of the message.
         * @param message       The message value to store
         */
        RequestClient(std::string message)
                : Rpc(TYPE::REQUEST_CLIENT), message(std::move(message)) {}

        /**
         * @brief Constructor with char* representation of the message.
         * @param msg           The message value to store
         */
        RequestClient(const char *msg)
                : RequestClient(std::string(msg)) {}

        /**
         * @brief Constructor with JSON representation of the message.
         * @param json          The message value to store
         */
        RequestClient(const json &json)
                : RequestClient(json["value"].get<std::string>()) {}

        json serialize_self() const override {
            return json::object({{"value", message}});
        }

        /// RequestClient: content of rpc
        std::string message;
    };
    class RequestClientResponse : public Rpc {
        public:
            /**
             * @brief Construct a REQUEST_CLIENT_RESPONSE RPC message
             * @param success       If we successfully retrieved the leader ID or not.
             */
            RequestClientResponse(bool success)
                : Rpc(TYPE::REQUEST_CLIENT_RESPONSE), success(success) {}

            /**
             * @brief Constructor
             * @param json          The JSON containing boolean true if leader responded.
             */
            RequestClientResponse(const json &json)
                : RequestClientResponse(json["success"].get<bool>()) {}

        protected:
            /**
             * @brief Serialize the entries' data into json form.
             * @return The JSON object containing the data.
             */
            json serialize_self() const override {
                return json::object({{"success", success}});
            }

        public:
            /// True if we received the leader's response
            bool success;
    };

}

