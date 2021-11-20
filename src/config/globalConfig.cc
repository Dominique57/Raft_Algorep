// Created by dominique on 10/24/21.

#include "globalConfig.hh"

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
    nb_node_max = nb_node_min + nb_node_;

    nb_client_min = nb_node_max + 1;
    nb_client_max = nb_client_min + nb_client_;
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
