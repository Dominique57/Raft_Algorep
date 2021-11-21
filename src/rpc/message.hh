#pragma once

#include "rpc.hh"

namespace Rpc {

    /// Rpc message type (different possible object-messages).
    enum class MESSAGE_TYPE {
        STATUS = 0,
        CRASH,
        SLEEP,
        SPEED,
        START,
    };

    /// Rpc message type names (used to convert enum to text).
    static const char *message_type_names[] = {
        "STATUS",
        "CRASH",
        "SLEEP",
        "SPEED",
        "START",
    };

    /**
     * @brief Converts an Rpc::MESSAGE_TYPE enum to text.
     * @return static const pointer to name.
     */
    inline const char *getMessageTypeName(MESSAGE_TYPE type) {
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(message_type_names) / sizeof(message_type_names[0]))
            return "Unkown Name";

        return message_type_names[typeIndex];
    }

    inline MESSAGE_TYPE getMessageType(const std::string type) {
        for (size_t i = 0; i < sizeof(message_type_names) / sizeof(message_type_names[0]); ++i)
            if (message_type_names[i] == type)
                return static_cast<MESSAGE_TYPE>(i);
        return MESSAGE_TYPE::STATUS;
    }

    /// Simple Rpc class that sends a string
    class Message : public Rpc {
    public:
        /**
         * @brief Constructor with string representation of the message.
         * @param message       The message value to store
         */
        Message(const MESSAGE_TYPE& type, const std::string& message)
                : Rpc(TYPE::MESSAGE), type(type), message(std::move(message)) {}

        /**
         * @brief Constructor with char* representation of the message.
         * @param msg           The message value to store
         */
        Message(const MESSAGE_TYPE& type, const char *msg)
                : Message(type, std::string(msg)) {}

        /**
         * @brief Constructor with JSON representation of the message.
         * @param json          The message value to store
         */
        Message(const json &json)
                : Message(getMessageType(json["type"].get<std::string>()),
                        json["value"].get<std::string>()) {}

        json serialize_self() const override {
            return json::object({{"type", getMessageTypeName(type)}, {"value", message}});
        }

        /// Message: content of rpc
        MESSAGE_TYPE type;
        std::string message;
    };
}

