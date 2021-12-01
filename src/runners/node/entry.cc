// Created by dominique on 11/27/21.

#include <utility>

#include "entry.hh"

Entry::Entry(int index, int term, std::string cmd, int clientId)
    : index(index), term(term), cmd(std::move(cmd)), clientId(clientId)
{}

void to_json(json &j, const Entry &entry) {
    j = json{ {"index", entry.index}, {"term", entry.term}, {"cmd", entry.cmd}, {"clientId", entry.clientId}};
}

void from_json(const json &j, Entry &p) {
    j.at("index").get_to(p.index);
    j.at("term").get_to(p.term);
    j.at("cmd").get_to(p.cmd);
    j.at("clientId").get_to(p.clientId);
}

std::ostream &operator<<(std::ostream &os, const Entry &entry) {
    return os << "  INDEX: " << entry.index
              << " TERM: " << entry.term
              << " CLIENT_ID: " << entry.clientId
              << " CMD: " << entry.cmd;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Entry> &entries) {
    for (const auto &entry : entries)
        os << entry << '\n';
    return os << std::endl;
}
