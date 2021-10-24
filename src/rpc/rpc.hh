// Created by dominique on 10/24/21.

#pragma once

#include <wrappers/nlhomann_include.hh>

namespace Rpc {
    enum class TYPE {
        MESSAGE = 0,
    };
    static const char *type_names[] = {
            "MESSAGE",
    };

    inline const char *getTypeName(Rpc::TYPE type) {
        static const char *unknownName = "Unknown Type";
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(type_names) / sizeof(type_names[0]))
            return unknownName;
        return type_names[typeIndex];
    }

    class Rpc {
    public:
        Rpc(TYPE type)
                : type_(type) {}

        json serialize() const;

        TYPE Type() const { return type_; }

    protected:
        virtual json serialize_self() const = 0;

        TYPE type_;
    };
}