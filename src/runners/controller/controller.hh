#pragma once

namespace Controller
{
    /// Controllers class to interact with our distributed system.
    class Controller {

    public:
        /**
         * @brief The default constructor
         */
        Controller() = default;

        /**
         * @brief Starts the controller.
         */
        void start() const;

        /**
         * @brief Handle the commands in the file path given as parameter.
         * @param file      File path to read the commands from.
         */
        void start_command_list(const char* file) const;
    };
}
