// Created by dominique on 10/24/21.

#include "globalConfig.hh"

int GlobalConfig::rank = 0;
int GlobalConfig::nb_node = 0;
int GlobalConfig::nb_client = 0;

void GlobalConfig::initConfig(const int& rank_, const int& size_) {
    rank = rank_;
    nb_node = size_ /2;
    nb_client = size_ - nb_node;
}
