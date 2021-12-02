#pragma once

#include "rpc.hh"

namespace Rpc {
    /// Rpc message type (different possible object-messages).
    enum class CONTROLLER_REQUEST_TYPE {
        STATUS = 0,
        CRASH,
        SPEED,
        START,
        RECOVERY,
        PRINT_LOG,
        ENTRY,
    };

    /// Rpc message type names (used to convert enum to text).
    static const char *controller_request_type_names[] = {
        "STATUS",
        "CRASH",
        "SPEED",
        "START",
        "RECOVERY",
        "PRINT_LOG",
        "ENTRY",
    };

    /**
     * @brief Converts an Rpc::CONTROLLER_REQUEST_TYPE enum to text.
     * @return static const pointer to name.
     */
    inline const char *getControllerRequestTypeName(const CONTROLLER_REQUEST_TYPE& type) {
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(controller_request_type_names) / sizeof(controller_request_type_names[0]))
            return "Unknown Name";

        return controller_request_type_names[typeIndex];
    }

    /**
     * @brief Get the controller's request's in string format.
     * @param type              The type of the request in string format
     * @return  The request in CONTROLLER_REQUEST_TYPE enum format.
     */
    inline CONTROLLER_REQUEST_TYPE getControllerRequestType(const std::string& type) {
        for (size_t i = 0; i < sizeof(controller_request_type_names) / sizeof(controller_request_type_names[0]); ++i)
            if (controller_request_type_names[i] == type)
                return static_cast<CONTROLLER_REQUEST_TYPE>(i);
        return CONTROLLER_REQUEST_TYPE::STATUS;
    }

    /// Rpc representing the heartbeat of the leader and the log synchronization request
    class ControllerRequest : public Rpc {
    public:
        /**
         * @brief Constructor with CONTROLLER_REQUEST_TYPE enum and message in string format.
         * @param type          The type of the request from CONTROLLER_REQUEST_TYPE
         * @param message       The message to send in string format.
         */
        ControllerRequest(const CONTROLLER_REQUEST_TYPE& type, std::string message)
                : Rpc(TYPE::CONTROLLER_REQUEST), type(type), message(std::move(message)) {}

        /**
         * @brief Overload of the constructor with char* representation of the message.
         * @param type          The type of the request from CONTROLLER_REQUEST_TYPE
         * @param msg           The message to send in a bufferof char format
         */
        ControllerRequest(const CONTROLLER_REQUEST_TYPE& type, const char *msg)
                : ControllerRequest(type, std::string(msg)) {}

        /**
         * @brief Overload of the constructor with json representation of the message.
         * @param json          The message to send containing the type and the value of it
         */
        ControllerRequest(const json &json)
                : ControllerRequest(getControllerRequestType(json["type"].get<std::string>()),
                        json["value"].get<std::string>()) {}

        /**
         * @brief Create a json containing the message and it's type
         * @return The json containing the message and the CONTROLLER_REQUEST_TYPE converted into a string
         */
        json serialize_self() const override {
            return json::object({{"type", getControllerRequestTypeName(type)}, {"value", message}});
        }

    public:
        /// The type of the controller's request
        CONTROLLER_REQUEST_TYPE type;

        /// The current message
        std::string message;
    };
}
