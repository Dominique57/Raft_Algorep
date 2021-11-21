#include "controller.hh"

#include <iostream>

#include "config/globalConfig.hh"
#include "rpc/message.hh"
#include "utils/string.hh"
#include "wrappers/mpi_include.hh"

namespace Controller
{
    static void crash_command(const std::string& arg);
    static void sleep_command(const std::string& arg);
    static void speed_command(const std::string& arg);
    static void start_command(const std::string& arg);
    static void status_command(const std::string& arg);

    const std::pair<const char*, void (*)(const std::string&)> commands[] = {
        std::make_pair("CRASH", crash_command),
        std::make_pair("SLEEP", sleep_command),
        std::make_pair("SPEED", speed_command),
        std::make_pair("START", start_command),
        std::make_pair("STATUS", status_command),
    };

    static void parse_input_command(const std::string& line) {
        const auto idx = line.find(" ");
        const std::string command = line.substr(0, idx);
        const std::string arg = idx == std::string::npos ? "" : line.substr(idx + 1);

        for (const auto& cmd: commands) {
            if (cmd.first == command) {
                cmd.second(arg);
                return;
            }
        }

        std::cout << "Invalid command" << std::endl;
    }

    static void sleep_command(const std::string& arg) {
        if (arg.empty()) {
            std::cerr << "Missing argument for SLEEP command" << std::endl;
            return;
        }

        int dst = string::to_int(arg);
        if (dst == -1)
            return;

        if (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))
            std::cerr << "Invalid rankId" << std::endl;

        // TODO: send message to dst
    }

    static void speed_command(const std::string& arg) {
        if (arg.empty()) {
            std::cout << "Missing argument for SPEED command" << std::endl;
            return;
        }

        std::cout << "SPEED command" << std::endl;
    }

    static void crash_command(const std::string& arg) {
        if (arg.empty()) {
            std::cout << "Missing argument for CRASH command" << std::endl;
            return;
        }
        std::cout << "CRASH command" << std::endl;
    }

    static void start_command(const std::string& arg) {
        if (arg.empty()) {
            // TODO: start clients
        }
        std::cout << "START command" << std::endl;
    }

    static void status_command(const std::string& arg) {
        std::cout << "STATUS command" << std::endl;
        if (arg.empty()) {
            for (int dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst) {
                MPI::Send_Rpc(Rpc::Message(Rpc::MESSAGE_TYPE::STATUS, ""), dst);
            }
        } else {
            int dst = string::to_int(arg);
            if (dst == -1)
                return;

            if (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))
                std::cerr << "Invalid rankId" << std::endl;

            MPI::Send_Rpc(Rpc::Message(Rpc::MESSAGE_TYPE::STATUS, ""), dst);
        }
    }

    void Controller::start() const {

        std::cout << "[CONTROLLER] ";

        for (std::string line; std::getline(std::cin, line);) {

            if (!line.empty())
                parse_input_command(line);

            std::cout << "[CONTROLLER] ";
        }
    }
}
