// Created by dominique on 11/27/21.

#pragma once

#include <string>
#include <sstream>
#include <wrappers/nlhomann_include.hh>

/// Struct containing the useful informations to handle a new entry.
struct Entry {
    Entry() = default;
    Entry(int index, int term, std::string cmd, int clientId);
    int index;
    int term;
    std::string cmd;
    int clientId;
};

using json = nlohmann::json;

/**
 * @brief Convert an entry and stores it in the json.
 * @param j         the json variable to stores our values in.
 * @param entry     the entry to convert into json format.
 */
void to_json(json& j, const Entry &entry);

/**
 * @brief Convert a json to an entry.
 * @param j         the json variable to parse.
 * @param p         the entry to store the values in.
 */
void from_json(const json& j, Entry& p);

/**
 * @brief Overload of the << operator to display the values of an entry on an output stream.
 * @param os        the output stream
 * @param entry     the entry to display on the stream.
 * @return The output stream with the entry's values.
 */
std::ostream& operator<<(std::ostream& os, const Entry &entry);

/**
 * @brief Overload of the << operator to display the values of multiples entries on an output stream.
 * @param os        the output stream
 * @param entries   the entries to display on the stream.
 * @return The output stream with the entries' values.
 */
std::ostream& operator<<(std::ostream& os, const std::vector<Entry> &entries);

/**
 * @brief Overload of the to_string function to convert multiples entries as a string.
 * @param entries   the entries to convert as string.
 * @return The output string with the entries' values.
 */
std::string to_string(const std::vector<Entry> &entries);