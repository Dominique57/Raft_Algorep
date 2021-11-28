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

void to_json(json& j, const Entry &entry);

void from_json(const json& j, Entry& p);

std::ostream& operator<<(std::ostream& os, const Entry &entry);

std::ostream& operator<<(std::ostream& os, const std::vector<Entry> &entries);