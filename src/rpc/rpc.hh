// Created by dominique on 10/24/21.

#pragma once

#include <wrappers/nlhomann_include.hh>

class Rpc {
public:
    Rpc(const std::string& message)
            : message_(message)
    {}
    Rpc(const json& json)
            : message_(json["value"].get<std::string>())
    {}

    json serialize() const;

    std::string message_;
};