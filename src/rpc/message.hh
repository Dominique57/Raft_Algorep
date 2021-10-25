// Created by dominique on 10/24/21.

#pragma once

#include "rpc.hh"

namespace Rpc {

    /// Simple Rpc class that sends a string
    class Message : public Rpc {
    public:
        /// Constructor
        Message(std::string message)
                : Rpc(TYPE::MESSAGE), message(std::move(message)) {}

        /// Constructor
        Message(const char *msg)
                : Message(std::string(msg)) {}

        /// Constructor
        Message(const json &json)
                : Message(json["data"]["value"].get<std::string>()) {}

        json serialize_self() const override {
            return json::object({{"value", message}});
        }

        /// Message: content of rpc
        std::string message;
    };
}

