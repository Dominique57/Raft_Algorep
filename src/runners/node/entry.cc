// Created by dominique on 11/27/21.

#include <utility>

#include "entry.hh"

Entry::Entry(int index, int term, std::string cmd)
    : index(index), term(term), cmd(std::move(cmd))
{}
