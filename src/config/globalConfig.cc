// Created by dominique on 10/24/21.

#include "globalConfig.hh"

#include <iostream>

int GlobalConfig::rank = 0;
int GlobalConfig::nb_node_min = 0;
int GlobalConfig::nb_node_max = 0;
int GlobalConfig::nb_client_min = 0;
int GlobalConfig::nb_client_max = 0;

void GlobalConfig::initConfig(const int& rank_, const int& nb_node_, const int& nb_client_) {

    // Rank: [Controller,
    // nb_node_min, ..., nb_node_max,
    // nb_client_min, ..., nb_client_max]
    rank = rank_;

    nb_node_min = 1;
    nb_node_max = nb_node_min + nb_node_ - 1;

    nb_client_min = nb_node_max + 1;
    nb_client_max = nb_client_min + nb_client_ - 1;
}

bool GlobalConfig::is_controller() {
    return rank == 0;
}

bool GlobalConfig::is_client() {
    return nb_client_min <= rank && rank <= nb_client_max;
}

bool GlobalConfig::is_node() {
    return nb_node_min <= rank && rank <= nb_node_max;
}

bool GlobalConfig::is_controller(const int& rank_) {
    return rank_ == 0;
}

bool GlobalConfig::is_client(const int& rank_) {
    return nb_client_min <= rank_ && rank_ <= nb_client_max;
}

bool GlobalConfig::is_node(const int& rank_) {
    return nb_node_min <= rank_ && rank_ <= nb_node_max;
}

int GlobalConfig::nb_node() {
    return nb_node_max - nb_node_min + 1;
}