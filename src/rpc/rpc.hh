#pragma once

#include <wrappers/nlhomann_include.hh>

namespace Rpc {

    /// Rpc type (different possible object-messages).
    enum class TYPE {
        MESSAGE = 0,
        REQUEST_VOTE,
        REQUEST_VOTE_RESPONSE,
        APPEND_ENTRIES,
        APPEND_ENTRIES_RESPONSE,
        REQUEST_LEADER,
        REQUEST_LEADER_RESPONSE,
    };

    /// Rpc type names (used to convert enum to text).
    static const char *type_names[] = {
        "MESSAGE",
        "REQUEST_VOTE",
        "REQUEST_VOTE_RESPONSE",
        "APPEND_ENTRIES",
        "APPEND_ENTRIES_RESPONSE",
        "REQUEST_LEADER",
        "REQUEST_LEADER_RESPONSE",
    };

    /**
     * @brief Converts an Rpc::TYPE enum to text.
     * @return static const pointer to name.
     */
    inline const char *getTypeName(Rpc::TYPE type) {
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(type_names) / sizeof(type_names[0]))
            return "unknownName";
        ///
        return type_names[typeIndex];
    }

    /// Abstract class using template design pattern to create Model-Json objects.
    class Rpc {
    public:
        /**
         * @brief Constructor
         * @param type          The type of the RPC message.
         */
        Rpc(TYPE type)
                : type_(type) {}

        /**
         * @brief Template design pattern entry-point.
         * @return json object of the current polymorphic object.
         */
        json serialize() const;

        /**
         * @brief Getter: type_
         * @return The RPC message's type.
         */
        TYPE Type() const { return type_; }

    protected:
        /**
         * @brief Pure virtual function that must convert the current polymorphic class data
         * @return json of the current polymorphic class data
         */
        virtual json serialize_self() const = 0;

        /// Rpc type: used at runtime to know which subclass it is
        TYPE type_;
    };
}
