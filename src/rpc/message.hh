#pragma once

#include "rpc.hh"

namespace Rpc {

    /// Simple Rpc class that sends a string
    class Message : public Rpc {
    public:
        /**
         * @brief Constructor with string representation of the message.
         * @param message       The message value to store
         */
        Message(const std::string& message)
                : Rpc(TYPE::MESSAGE), message(std::move(message)) {}

        /**
         * @brief Constructor with char* representation of the message.
         * @param msg           The message value to store
         */
        Message(const char *msg)
                : Message(std::string(msg)) {}

        /**
         * @brief Constructor with JSON representation of the message.
         * @param json          The message value to store
         */
        Message(const json &json)
                : Message(json["value"].get<std::string>()) {}

        json serialize_self() const override {
            return json::object({{"value", message}});
        }

        /// Message: content of rpc
        std::string message;
    };
}

