// Created by dominique on 11/27/21.

#pragma once

#include <string>
#include <wrappers/nlhomann_include.hh>

struct Entry {
    Entry() = default;
    Entry(int index, int term, std::string cmd);
    int index;
    int term;
    std::string cmd;
};

using json = nlohmann::json;

inline void to_json(json& j, const Entry &entry) {
    j = json{ {"index", entry.index}, {"term", entry.term}, {"cmd", entry.cmd} };
}

inline void from_json(const json& j, Entry& p) {
    j.at("index").get_to(p.index);
    j.at("term").get_to(p.term);
    j.at("cmd").get_to(p.cmd);
}