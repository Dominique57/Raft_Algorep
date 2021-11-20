#pragma once

#include <string>
#include <stdexcept>
#include <iostream>

namespace string {

inline int to_int(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (std::invalid_argument const&) {
        std::cerr << "Invalid argument: " << str << std::endl;
        return -1;
    }
}

}
