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
         * @brief Constructor
         * @param
         * @param
         */
        ControllerRequest(const CONTROLLER_REQUEST_TYPE& type, std::string  message)
                : Rpc(TYPE::CONTROLLER_REQUEST), type(type), message(std::move(message)) {}

        /**
         * @brief Constructor with char* representation of the message.
         * @param type
         * @param msg
         */
        ControllerRequest(const CONTROLLER_REQUEST_TYPE& type, const char *msg)
                : ControllerRequest(type, std::string(msg)) {}


        ControllerRequest(const json &json)
                : ControllerRequest(getControllerRequestType(json["type"].get<std::string>()),
                        json["value"].get<std::string>()) {}

        json serialize_self() const override {
            return json::object({{"type", getControllerRequestTypeName(type)}, {"value", message}});
        }

    public:
        CONTROLLER_REQUEST_TYPE type;
        std::string message;
    };
}
