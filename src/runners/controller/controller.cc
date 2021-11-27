#include "controller.hh"

#include <iostream>
#include <string>
#include <unistd.h>

#include "config/globalConfig.hh"
#include "rpc/controllerRequest.hh"
#include "utils/string.hh"
#include "wrappers/mpi_include.hh"

namespace Controller
{
    static void crash_command(const std::string& arg);
    static void help_command(const std::string& arg);
    static void speed_command(const std::string& arg);
    static void start_command(const std::string& arg);
    static void status_command(const std::string& arg);
    static void recovery_command(const std::string& arg);

    const std::pair<const char*, void (*)(const std::string&)> commands[] = {
        std::make_pair("CRASH", crash_command),
        std::make_pair("HELP", help_command),
        std::make_pair("SPEED", speed_command),
        std::make_pair("START", start_command),
        std::make_pair("STATUS", status_command),
        std::make_pair("RECOVERY", recovery_command),
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

    static void help_command(const std::string&) {
        std::cout << "Commands: " << std::endl
            << "* `HELP`: display help message" << std::endl
            << "* `STATUS [rank]`: display information for the given process or for all processes" << std::endl
            << "* `CRASH [rank]`: crash the given process" << std::endl
            << "* `SPEED [rank] [speed]`: set speed for the given process, available speed: low, medium, high" << std::endl
            << "* `START [client_rank]`: start the given client" << std::endl;
    }

    static void recovery_command(const std::string& arg) {
        if (arg.empty()) {
            std::cout << "Missing argument. Expect: RECOVERY [id]" << std::endl;
            return;
        }

        int dst = String::to_int(arg);
        if (dst == -1 || (!GlobalConfig::is_node(dst) && !GlobalConfig::is_client(dst))) {
            std::cerr << "Invalid id. Expect id between " << GlobalConfig::nb_node_min << " and " << GlobalConfig::nb_client_max << std::endl;
            return;
        }

        MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::RECOVERY, ""), dst);
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
            // Start all clients
            for (int dst = GlobalConfig::nb_client_min; dst <= GlobalConfig::nb_client_max; ++dst)
                MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::START, ""), dst);

        } else {
            int dst = String::to_int(arg);
            if (dst == -1 || !GlobalConfig::is_client(dst)) {
                std::cerr << "Invalid id. Expect id between " << GlobalConfig::nb_client_min << " and " << GlobalConfig::nb_client_max << std::endl;
                return;
            }

            MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::START, ""), dst);
        }
    }

    static void status_command(const std::string& arg) {
        if (arg.empty()) {

            // Send message to nodes
            for (int dst = GlobalConfig::nb_node_min; dst <= GlobalConfig::nb_node_max; ++dst)
                MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::STATUS, ""), dst);

            // Wait before sending message to clients
            usleep(100);

            // Send message to clients
            for (int dst = GlobalConfig::nb_client_min; dst <= GlobalConfig::nb_client_max; ++dst)
                MPI::Send_Rpc(Rpc::ControllerRequest(Rpc::CONTROLLER_REQUEST_TYPE::STATUS, ""), dst);

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

            if (!line.empty())
                parse_input_command(line);

            std::cout << "[CONTROLLER] ";

        } while (std::getline(std::cin, line));
    }
}
