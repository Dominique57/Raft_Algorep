#pragma once

/// Global processus configuration data
class GlobalConfig {
public:
    /** Initializes the global configuration
     * @param rank : GlobalConfig::rank
     * @param size : GlobalConfig::size
     */
    static void initConfig(int rank, int size);

    /// Current node id
    static int rank;
    /// Total number of node
    static int nb_node;
    /// Total number of client
    static int nb_client;
};

