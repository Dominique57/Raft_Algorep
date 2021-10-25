// Created by dominique on 10/24/21.

#pragma once

#include <wrappers/nlhomann_include.hh>

namespace Rpc {

    /// Rpc type (different possible object-messages)
    enum class TYPE {
        MESSAGE = 0,
    };

    /// Rpc type names (used to convert enum to text)
    static const char *type_names[] = {
        "MESSAGE",
    };

    /** Converts an Rpc::Type enum to text
     * @return static const pointer to name
     */
    inline const char *getTypeName(Rpc::TYPE type) {
        static const char *unknownName = "Unknown Type";
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(type_names) / sizeof(type_names[0]))
            return unknownName;
        return type_names[typeIndex];
    }

    /// Abstract class using template design pattern to create Model-Json objects
    class Rpc {
    public:
        /// Constructor
        Rpc(TYPE type)
                : type_(type) {}

        /** Template design pattern entry-point
         * @return json object of the current polymorphic object
         */
        json serialize() const;

        /// Getter: type_
        TYPE Type() const { return type_; }

    protected:
        /** Pure virtual function that must convert the current polymorphic class data
         * @return json of the current polymorphic class data
         */
        virtual json serialize_self() const = 0;

        /// Rpc type: used at runtime to know which subclass it is
        TYPE type_;
    };
}