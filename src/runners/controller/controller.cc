#include "controller.hh"

#include <iostream>
#include <string>

#include "config/globalConfig.hh"
#include "rpc/controllerRequest.hh"
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

        int dst = String::to_int(arg);
        if (dst == -1 || (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))) {
            std::cerr << "Invalid rankId" << std::endl;
            return;
        }

        // TODO: send message to dst
    }

    static void speed_command(const std::string& arg) {
        const auto idx = arg.find(" ");
        const std::string rank_id = arg.substr(0, idx);
        const std::string speed = idx == std::string::npos ? "" : arg.substr(idx + 1);

        if (rank_id.empty() || speed.empty()) {
            std::cout << "Missing argument. Expect: SPEED [id] [speed]" << std::endl;
            return;
        }

        int dst = String::to_int(rank_id);
        if (dst == -1 || (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))) {
            std::cerr << "Invalid id. Expect id between " << GlobalConfig::nb_node_min << " and " << GlobalConfig::nb_client_max << std::endl;
            return;
        }

        if (speed != "low" && speed != "medium" && speed != "high") {
            std::cerr << "Invalid speed argument" << std::endl;
            return;
        }

        MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::SPEED, speed), dst);
    }

    static void crash_command(const std::string& arg) {
        if (arg.empty()) {
            std::cout << "Missing argument. Expect: CRASH [id]" << std::endl;
            return;
        }
        int dst = String::to_int(arg);
        if (dst == -1 || (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))) {
            std::cerr << "Invalid id. Expect id between " << GlobalConfig::nb_node_min << " and " << GlobalConfig::nb_client_max << std::endl;
            return;
        }

        MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::CRASH, ""), dst);
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

            // Send message to nodes
            for (int dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst)
                MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::STATUS, ""), dst);

            // TODO: send message to clients
        } else {
            int dst = String::to_int(arg);
            if (dst == -1 || (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))) {
                std::cerr << "Invalid id. Expect id between " << GlobalConfig::nb_node_min << " and " << GlobalConfig::nb_client_max << std::endl;
                return;
            }

            MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::STATUS, ""), dst);
        }
    }

    void Controller::start() const {
        std::string line;

        do {
            std::cout << "[CONTROLLER] ";

            if (!line.empty())
                parse_input_command(line);

        } while (std::getline(std::cin, line));
    }
}
