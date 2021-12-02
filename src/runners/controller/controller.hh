#pragma once

namespace Controller
{
    class Controller {

    public:
        /* Constructor */
        Controller() = default;

        /* Methods */
        void start() const;
        void start_command_list(const char* file) const;
    };
}
