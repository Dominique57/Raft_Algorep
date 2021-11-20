#pragma once

/// Global processus configuration data
class GlobalConfig {
public:
    /** Initializes the global configuration
     * @param rank : GlobalConfig::rank
     * @param size : GlobalConfig::size
     */
    static void initConfig(const int& rank, const int& nb_node, const int& nb_client);

    static bool is_controller();
    static bool is_client();
    static bool is_node();

    /// Current node id
    static int rank;
    /// Total number of node
    static int nb_node_min;
    static int nb_node_max;
    /// Total number of client
    static int nb_client_min;
    static int nb_client_max;
};

