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
    /// [0; size/2]
    static int nb_node;
    /// Total number of client
    /// [size/2; size]
    static int nb_client;
};

