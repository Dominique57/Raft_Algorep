// Created by dominique on 10/24/21.

#include "globalConfig.hh"

int GlobalConfig::rank = 0;

//TODO: add the controller rank (and so move the other ranks: servers/clients)

int GlobalConfig::nb_node = 0;
int GlobalConfig::start_node = 0;
int GlobalConfig::end_node = 0;
int GlobalConfig::nb_client = 0;
int GlobalConfig::start_client = 0;
int GlobalConfig::end_client = 0;

void GlobalConfig::initConfig(int rank_, int size) {
    rank = rank_;

    start_node = 0;
    end_node = size /2;
    nb_node = end_node - start_node;

    start_client = end_node;
    end_client = size;
    nb_client = end_client - start_client;
}

bool GlobalConfig::is_client(int id) {
    return (start_client <= id && id < end_client);
}
bool GlobalConfig::is_node(int id) {
    return (start_node <= id && id < end_node);
}
