// Created by dominique on 10/24/21.

#pragma once

#include "rpc.hh"

namespace Rpc {
    class Message : public Rpc {
    public:
        Message(std::string message)
                : Rpc(TYPE::MESSAGE), message(std::move(message)) {}

        Message(const char *msg)
                : Message(std::string(msg)) {}

        Message(const json &json)
                : Message(json["data"]["value"].get<std::string>()) {}

        json serialize_self() const override {
            return json::object({{"value", message}});
        }

        std::string message;
    };
}

