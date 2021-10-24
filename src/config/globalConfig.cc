// Created by dominique on 10/24/21.

#include "globalConfig.hh"

int GlobalConfig::rank = 0;
int GlobalConfig::size = 0;

void GlobalConfig::initConfig(int rank_, int size_) {
    rank = rank_;
    size = size_;
}
