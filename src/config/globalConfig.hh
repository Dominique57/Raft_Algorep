#pragma once

/// Global processus configuration data
class GlobalConfig {
public:
    /** Initializes the global configuration
     * @param rank : GlobalConfig::rank
     * @param size : initialise the servers, clients and later controller ids
     */
    static void initConfig(int rank, int size);

    static bool is_node(int id);
    static bool is_client(int id);

    /// Current node id
    static int rank;
    /// Total number of node
    /// [0; size/2]
    static int nb_node;
    static int end_node;
    static int start_node;

    /// Total number of client
    /// [size/2; size]
    static int nb_client;
    static int end_client;
    static int start_client;
};

