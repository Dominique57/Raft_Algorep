// Created by dominique on 11/27/21.

#include <utility>

#include "entry.hh"

Entry::Entry(int index, int term, std::string cmd)
    : index(index), term(term), cmd(std::move(cmd))
{}

void to_json(json &j, const Entry &entry) {
    j = json{ {"index", entry.index}, {"term", entry.term}, {"cmd", entry.cmd} };
}

void from_json(const json &j, Entry &p) {
    j.at("index").get_to(p.index);
    j.at("term").get_to(p.term);
    j.at("cmd").get_to(p.cmd);
}

std::ostream &operator<<(std::ostream &os, const Entry &entry) {
    return os << "INDEX: " << entry.index
              << " TERM: " << entry.term
              << " CMD: " << entry.cmd;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Entry> &entries) {
    for (const auto &entry : entries)
        os << entry << '\n';
    return os;
}
